/* Inspired by work found here
 * here://developer.ridgerun.com/wiki/index.php?title=Gpio-int-test.c
 */


#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>

#define BUFF_SIZE 256
#define SYSFS_GPIO "/sys/class/gpio"


int bbGpioFdOpen(int gpio) {
	int fd;

	/* add a validate function */
	char buf[BUFF_SIZE];
	snprintf(buf, sizeof(buf), SYSFS_GPIO "/gpio%d/value", gpio);

	fd = open(buf, O_RDONLY | O_NONBLOCK);
	if (fd < 0) {
		fprintf(stderr, "GPIO might not exist! Couldn't be opened\n");
	}
	return fd;
}

int bbGpioFdClose(int fd) {
	return close(fd);
}
