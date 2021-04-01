/* Inspired by work found here
 * here://developer.ridgerun.com/wiki/index.php?title=Gpio-int-test.c
 */

#include <bbgpio.h>
#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#define BUFF_SIZE 256
#define SYSFS_GPIO "/sys/class/gpio"
#define NUM_BANKS 3

/* For converting when a pin is listed as GPIOX_Y,
 *   where X is the bank, and Y is the num
 */
int bbGetAbsPinNum(unsigned int bank, unsigned int num)
{
    if (bank > NUM_BANKS) {
        fprintf(stderr, "Improper input, valid banks are in the range of 0-3");
        return -1;
    }

    return bank * 32 + num;
}

int bbGpioExport(unsigned int gpio)
{
    int fd = open(SYSFS_GPIO "/export", O_WRONLY);
    int len = 0;
    char buff[BUFF_SIZE];

    if (fd < 0) {
        fprintf(stderr, "Failed to export pin %d", gpio);
        return -1;
    }

    len = snprintf(buff, sizeof(buff), "%d", gpio);
    write(fd, buff, len);
    close(fd);
    return 0;
}

int bbGpioSetEdge(unsigned int gpio, char* edge)
{
    int fd;
    char buff[BUFF_SIZE];

    snprintf(buff, sizeof(buff), SYSFS_GPIO "/gpio%d/edge", gpio);

    fd = open(buff, O_WRONLY);

    if (fd < 0) {
        fprintf(stderr, "Failed to set edge on pin %d", gpio);
        return -1;
    }

    write(fd, edge, strlen(edge) + 1);
    close(fd);
    return 0;
}

int bbGpioSetDir(unsigned int gpio, char* dir)
{
    int fd;
    char buff[BUFF_SIZE];

    snprintf(buff, sizeof(buff), SYSFS_GPIO "/gpio%d/direction", gpio);

    fd = open(buff, O_WRONLY);

    if (fd < 0) {
        fprintf(stderr, "Failed to set direction on pin %d", gpio);
        return -1;
    }

    write(fd, dir, strlen(dir) + 1);
    close(fd);
    return 0;
}

int bbGpioSetValue(unsigned int gpio, bool val)
{
    int fd;
    char buff[BUFF_SIZE];

    snprintf(buff, sizeof(buff), SYSFS_GPIO "/gpio%d/value", gpio);

    fd = open(buff, O_WRONLY);

    if (fd < 0) {
        fprintf(stderr, "Failed to set value on pin %d\n", gpio);
        return -1;
    }
    if (val)
        write(fd, "1", 2);
    else
        write(fd, "0", 2);
    close(fd);
    return 0;
}

/* Figure out the edge by checking the first character */
char* bbGpioGetEdge(unsigned int gpio)
{
    int fd;
    char buff[BUFF_SIZE];
    char firstCharOfEdge;

    snprintf(buff, sizeof(buff), SYSFS_GPIO "/gpio%d/edge", gpio);

    fd = open(buff, O_RDONLY);

    if (fd < 0) {
        fprintf(stderr, "Failed to get edge on pin %d\n", gpio);
        return "error";
    }

    read(fd, &firstCharOfEdge, 1);
    close(fd);

    if (firstCharOfEdge == RISING_EDGE[0]) {
        return RISING_EDGE;
    } else if (firstCharOfEdge == FALLING_EDGE[0]) {
        return FALLING_EDGE;
    } else if (firstCharOfEdge == BOTH_EDGE[0]) {
        return BOTH_EDGE;
    } else {
        fprintf(stderr, "Unexpected edge value, returning error...\n");
        return "error";
    }
}

/* Gets the direction of that pin, cheats a little bit though by
 * operating on the assumption that it can only be one of three values
 * and thus we know that the first character for each is unique, and we only
 * read that */
char* bbGpioGetDir(unsigned int gpio)
{
    int fd;
    char buff[BUFF_SIZE];
    char firstCharOfDir; /* Extra explicit because of this hacky method */

    snprintf(buff, sizeof(buff), SYSFS_GPIO "/gpio%d/direction", gpio);

    fd = open(buff, O_RDONLY);

    if (fd < 0) {
        fprintf(stderr, "Failed to get direction on pin %d\n", gpio);
        return "error";
    }
    read(fd, &firstCharOfDir, 1);
    close(fd);

    if (firstCharOfDir == OUT_DIR[0]) {
        return OUT_DIR;
    } else if (firstCharOfDir == IN_DIR[0]) {
        return IN_DIR;
    } else {
        fprintf(stderr, "Unexpected direction, returning error\n");
        return "error";
    }
}

bool bbGpioGetValue(unsigned int gpio)
{
    int fd;
    char buff[BUFF_SIZE];
    char val;

    snprintf(buff, sizeof(buff), SYSFS_GPIO "/gpio%d/value", gpio);

    fd = open(buff, O_RDONLY);

    if (fd < 0) {
        fprintf(stderr, "Failed to get value on pin %d\n", gpio);
        return -1;
    }
    read(fd, &val, 1);
    close(fd);

    return val == '1';
}

int bbGpioFdOpen(unsigned int gpio)
{
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

int bbGpioFdClose(int fd)
{
    return close(fd);
}
