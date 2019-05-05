#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <fcntl.h>
#include <errno.h>
#include <pthread.h>
#include <bbgpio.h>
#include <retro.h>
#include <poll.h>

#define TIMEOUT 1000	/* 1 second */
#define BUF_LEN 256

static pthread_t thread;
static int onTapeStrip (void);
static int waitForStrip (int gpioFd);


int initRetro() {
	bbGpioExport(RETRO_PIN);
	bbGpioSetDir(RETRO_PIN, IN_DIR);
	bbGpioSetEdge(RETRO_PIN, FALLING_EDGE);
	int fd = bbGpioFdOpen(RETRO_PIN);
	waitForStrip(fd);
	//int ret = pthread_create( &thread, NULL, waitForStrip, fd);
	return 0;
}

int onTapeStrip() {
	printf("\nTAPE STRIP DETECTED\n");
	return 0;
}


int waitForStrip(int gpioFd) {
	struct pollfd fds[2];
	int nfds = 2;
	int ret, len;
	char buf[BUF_LEN];

	while (1) {
		memset((void *)fds, 0, sizeof(fds));
		fds[0].fd = STDIN_FILENO;
		fds[0].events = POLLIN;

		fds[1].fd = gpioFd;
		fds[1].events = POLLPRI;

		ret = poll(fds, nfds, TIMEOUT);

		if (ret < 0) {
			printf("\npoll() failed!\n");
		}

		if (ret == 0) {
			printf(".");
		}

		if (fds[1].revents & POLLPRI) {
			lseek(fds[1].fd, 0, SEEK_SET);
			len = read(fds[1].fd, buf, BUF_LEN);
			onTapeStrip();
		}

		if (fds[0].revents & POLLIN) {
			printf("\nblam");
		}
		fflush(stdout);
	}
	return 0;
}
