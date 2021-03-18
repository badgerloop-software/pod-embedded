#ifndef __BBGPIO_H__
#define __BBGPIO_H__

#include <stdbool.h>

#define RISING_EDGE  "rising"
#define FALLING_EDGE "falling"
#define BOTH_EDGE	 "both"

#define OUT_DIR		 "out"
#define IN_DIR		 "in"

int bbGetAbsPinNum(unsigned int bank, unsigned int num);

int bbGpioExport(unsigned int gpio);

int bbGpioSetEdge(unsigned int gpio, char * edge);

int bbGpioSetDir(unsigned int gpio, char * dir);

int bbGpioSetValue(unsigned int gpio, bool val);

char * bbGpioGetEdge(unsigned int gpio);

char * bbGpioGetDir(unsigned int gpio);

bool bbGpioGetValue(unsigned int gpio);

int bbGpioFdOpen(unsigned int gpio);

int bbGpioFdClose(int fd);

#endif /* __BBGPIO_H__ */
