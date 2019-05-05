#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <fcntl.h>
#include <errno.h>
#include <pthread.h>
#include <bbgpio.h>
#include <poll.h>

#define TIMEOUT 1000	/* 1 second */
#define BUF_LEN 256
static pthread_t thread;
static int onTapeStrip (void);
static int waitForStrip (void);


int initRetro() {
	waitForStrip();
	//int ret = pthread_create( &thread, NULL, waitForStrip, NULL);
	return 0;
}

int onTapeStrip() {
	printf("TAPE STRIP DETECTED\n");
	return 0;
}


int waitForStrip() {
	int gpio = 66;
	struct pollfd fds[2];
	int nfds = 2;
	int ret, len, gpioFd;
	gpioFd = bbGpioFdOpen(gpio);
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
			printf("\npoll() GPIO %d int occurred\n", gpio);
		}

		if (fds[0].revents & POLLIN) {
			printf("\nblam");
		}
		fflush(stdout);
	}
	return 0;
}
