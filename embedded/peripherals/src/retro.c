#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <bbgpio.h>
#include <poll.h>

#define TIMEOUT 1000	/* 1 second */

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
}


int waitForStrip() {
	int gpio = 66;
	struct pollfd fds[2];
	int nfds = 2;
	int ret, len, gpioFd;
	
	gpioFd = bbGpioFdOpen(gpio);

	while (1) {
		memset((void *)fdset, 0, sizeof(fdset));
		fdset[0].fd = STDIN_FILENO;
		fdset[0].events = POLLIN;

		fdset[1].fd = gpioFd;
		fdset[1].events = POLLPRI;

		ret = poll(fds, nfds, TIMEOUT);

		if (ret < 0) {
			printf("\npoll() failed!\n");
		}
		
		if (ret == 0) {
			printf(".");
		}

		if (fds[1].revents & POLLPRI) {
			lseek(fdset[1].fd, 0, SEEK_SET);
			len = read(fdset[1].fd, buf, BUFF_LEN);
			printf("\npoll() GPIO %d int occurred\n", gpio);
		}

		if (fds[0].revents & POLLIN) {
			printf("\nblam");
		}
		fflush(stdout);
	}

}
