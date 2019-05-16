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

#define TIMEOUT 10	/* 1 second, bump higher in production */
#define BUF_LEN 256
#define SAFETY_CONSTANT 2

static pthread_t retroThreads[3];
static bool shouldQuit = false;

static int onTapeStrip (int retroNum);
static void * waitForStrip (void * retroNum);
static int getPin(int retroNum);
static inline uint64_t getuSTimestamp();
static inline uint64_t convertTouS(struct timespec *currTime);

extern data_t *data;
static struct timespec *retroTimers[3];

/***
  * initRetros - configures the GPIO pins for the retro reflective sensors
  *  and kicks off a thread for each
  *
 ***/
int initRetros() {
	int i = 0;
	retroTimers[RETRO_1] = &(data->timers->lastRetro1);
	retroTimers[RETRO_2] = &(data->timers->lastRetro2);
	retroTimers[RETRO_3] = &(data->timers->lastRetro3);

	for (i = 0; i < NUM_RETROS; i++) {
		if (bbGpioExport(getPin(i)) != 0) return -1;
		if (bbGpioSetDir(getPin(i), IN_DIR) != 0) return -1;
		if (bbGpioSetEdge(getPin(i), RISING_EDGE) != 0) return -1;
		if (pthread_create(&retroThreads[i], NULL, waitForStrip, (void *) i) != 0)
			return -1;
	}
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

static inline uint64_t convertTouS(struct timespec *currTime) {
	return (uint64_t)((currTime->tv_sec * 1000000) + (currTime->tv_nsec / 1000));
}

/* Returns delay in uS */
static uint64_t getDelay() {
	return 1000000 * 1;//(SAFETY_CONSTANT * (WIDTH_TAPE_STRIP / data->motion->vel));
}

uint64_t masterDelay = 0;
/* Not voting yet! */
static int onTapeStrip(int retroNum) {
	struct timespec stamp;
	clock_gettime(CLOCK_MONOTONIC, &stamp);
	uint64_t currTime = convertTouS(&stamp);
	uint64_t retroTime = convertTouS(retroTimers[retroNum]); // we will cache this;
	if (masterDelay == 0) masterDelay = getDelay();
	/* No rollovers here ;) */
	if (currTime < retroTime) {
		*retroTimers[retroNum] = stamp;
		data->motion->retroCount++;
		return 0;
	}
	/* Check if it has delayed long enough (in uS) to accept another strip */
	if ((currTime - retroTime) > masterDelay) {
		*retroTimers[retroNum] = stamp;
		data->motion->retroCount++;
	}
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
void *waitForStrip(void *num) {
	int retroNum = (int) num;
	int gpioFd = bbGpioFdOpen(getPin(retroNum));
	struct pollfd fds[1];
	int nfds = 1;
	int ret, len;
	char buf[BUF_LEN];
	while (1) {
		if (shouldQuit) break;
		memset((void *)fds, 0, sizeof(fds));
		fds[0].fd = gpioFd;
		fds[0].events = POLLPRI;

		ret = poll(fds, nfds, TIMEOUT);

		if (ret < 0) {
			printf("\npoll() failed!\n");
		}

		if (ret == 0) {
			/* If nothing is detected */
			printf("Retro Count = %d\n", data->motion->retroCount);
		}

		if (fds[0].revents & POLLPRI) {
			lseek(fds[0].fd, 0, SEEK_SET);
			len = read(fds[0].fd, buf, BUF_LEN);
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
