#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <fcntl.h>
#include <errno.h>
#include <stdbool.h>
#include <pthread.h>
#include <bbgpio.h>
#include <retro.h>
#include <poll.h>
#include <time.h>
#include <data.h>

#ifdef DEBUG_RETRO
#define DBG_RETRO_PRINTF(args...) printf(args)
#else
#define DBG_RETRO_PRINTF(args...)
#endif

#define SPURIOUS_BLOCK_TIMEOUT 500000
#define TIMEOUT 100	/* 1 second, bump higher in production */
#define BUF_LEN 256
#define SAFETY_CONSTANT 2
#define SEC_TO_USEC     1000000
#define VOTE_BUFFER     200000
#define VOTE_RESET_TIME 3000000  /* Hard coded for max speed, approx algo should be (DIST_BTWN_STRIPS / (2 * SPEED_FTPS)) */
#define CONST_TERM      SAFETY_CONSTANT * SEC_TO_USEC

static pthread_t retroThreads[3];
static pthread_t spuriousThread;
static bool shouldQuit = false;
static bool blockInts  = true;

static int onTapeStrip (int retroNum);
static void * waitForStrip (void * retroNum);
static int getPin(int retroNum);
static void * blockSpuriousInts(uint64_t timeout);


/***
  * initRetros - configures the GPIO pins for the retro reflective sensors
  *  and kicks off a thread for each
  *
 ***/
int initRetros() {
	int i = 0;

	for (i = 0; i < NUM_RETROS; i++) {
		if (bbGpioExport(getPin(i)) != 0) return -1;
		if (bbGpioSetDir(getPin(i), IN_DIR) != 0) return -1;
		if (bbGpioSetEdge(getPin(i), RISING_EDGE) != 0) return -1;
		if (pthread_create(&retroThreads[i], NULL, waitForStrip, (void *) i) != 0)
			return -1;
	}
    if (pthread_create(&spuriousThread, NULL, blockSpuriousInts, SPURIOUS_BLOCK_TIMEOUT) != 0)
        return -1;
	return 0;
}

/***
 * joinRetroThreads - signals to all threads that they should stop
 *  and then blocks until they do.
 *
***/
int joinRetroThreads() {
	int i = 0;
	shouldQuit = true;
	for (i = 0; i < NUM_RETROS; i++) {
		pthread_join(retroThreads[i], NULL);
	}
	return 0;
}

/* Returns delay in uS */
static inline uint64_t getDelay() {
    return (CONST_TERM * WIDTH_TAPE_STRIP) / ( .1 + data->motion->vel);
}

/* voteOnCandidate - the candidate is the most recent retro detected and the
 * vote is to determine if the count should be changed. 
 *
 *  The basic methodology is as follows:
 *      1. Get timestamps for all three retros
 *      2. Check against both other retros to see if any timestamps are close
 *          enough together that they occurred for the same strip
 *      3. Make sure the last passing vote hasn't occurred within a set delay
 *      (so we don't double count a strip)
 *      4. If conditions 2 and 3 are true, then we have travelled 100 ft, and
 *      return true to signal an increment.
 * */
static int voteOnCandidate(int retroNum) {
    /* Grab the other two retro timestamps to simplify comparisons */
    uint64_t otherRetro1 = data->timers->lastRetros[(retroNum + 1) % NUM_RETROS];
    uint64_t otherRetro2 = data->timers->lastRetros[(retroNum + 2) % NUM_RETROS];
    uint64_t candidate   = data->timers->lastRetros[retroNum];

    return (((candidate - otherRetro1) <= VOTE_BUFFER || 
        (candidate - otherRetro2) <= VOTE_BUFFER) && candidate > (data->timers->lastRetro + VOTE_RESET_TIME));
}

/* Not voting yet! */
static int onTapeStrip(int retroNum) {
    DBG_RETRO_PRINTF("Tape strip detected on retro %d\n", retroNum);
	uint64_t currTime = getuSTimestamp();
    uint64_t delay = getDelay();
    DBG_RETRO_PRINTF("Current delay: %llu\n", delay);


	/* Check if it has delayed long enough (in uS) to accept another strip */
	if (((currTime - data->timers->lastRetros[retroNum]) > delay) ||
			(currTime < data->timers->lastRetros[retroNum])) {
		data->timers->lastRetros[retroNum] = currTime;
        if (voteOnCandidate(retroNum)) {
            DBG_RETRO_PRINTF("Vote pass: incrementing count\n");
            data->timers->oldRetro = data->timers->lastRetro;
            data->timers->lastRetro = currTime;
            data->motion->retroCount++;
        }
        DBG_RETRO_PRINTF("New count: %d\n", data->motion->retroCount);
    }


    DBG_RETRO_PRINTF("Last Retro %d: %llu\n", 0, data->timers->lastRetros[0]);
    DBG_RETRO_PRINTF("Last Retro %d: %llu\n", 1, data->timers->lastRetros[1]);
    DBG_RETRO_PRINTF("Last Retro %d: %llu\n", 2, data->timers->lastRetros[2]);

	return 0;
}

/***
  * waitForStrip - Run by each retro sensor thread, it polls the file
  *  that holds the value of the retro. If it changes then it will call onTapeStrip
  *  otherwise it repeats.
  *
  * input:
  *		void *num - an identifier for which retro is running the thread
  ***/
static void *waitForStrip(void *num) {
	int retroNum = (int) num;
	int gpioFd = bbGpioFdOpen(getPin(retroNum));
	struct pollfd fds[1];
	int nfds = 1;
	int ret;
	char buf[BUF_LEN];
	
	while (1) {
		if (shouldQuit) break;
		memset((void *)fds, 0, sizeof(fds));
		fds[0].fd = gpioFd;
		fds[0].events = POLLPRI;

		ret = poll(fds, nfds, TIMEOUT);

		if (ret < 0) {
			fprintf(stderr,"\npoll() failed!\n");
		}

		if (ret == 0) {
			/* If nothing is detected */
		}

		if (fds[0].revents & POLLPRI) {
			lseek(fds[0].fd, 0, SEEK_SET);
			read(fds[0].fd, buf, BUF_LEN);
            /* Not sure why yet, but when we enable BBB GPIO Ints, it always immediately
               takes an interrupt. This is annoying, and so eventually we might figure out why,
               but until then we are just going to block all of them for a fixed interval
               to get the interrupts out of the system. */
            if (blockInts)
                continue;
			onTapeStrip(retroNum);
		}
	}
	return NULL;
}

static int getPin(int retroNum) {
	switch(retroNum) {
		case RETRO_1:
			return RETRO_1_PIN;
		case RETRO_2:
			return RETRO_2_PIN;
		case RETRO_3:
			return RETRO_3_PIN;
		default:
			fprintf(stderr, "Invalid retro number, error...\n");
			return -1;
	}
	return -1;	/* Will never get here */
}

static void *blockSpuriousInts(uint64_t timeout) {
	blockInts = true;
    usleep(timeout);
    blockInts = false;
    return NULL;
}
