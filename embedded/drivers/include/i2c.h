/*
* i2c.h
* Author: Rohan Daruwala
* Based on work by: gijs
*/

#ifndef I2C_H
#define I2C_H

#include <fcntl.h>
#include "i2c-dev.h"
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/ioctl.h>

typedef struct {
	int fd;
	int bus;
	int deviceAddress;
	int openMode;
} i2c_settings;




int i2c_begin(i2c_settings *i2c);
int write_byte_i2c(i2c_settings *i2c, unsigned char reg);
int write_data_i2c(i2c_settings *i2c, unsigned char reg, char value);
int read_i2c(i2c_settings *i2c, unsigned char *readBuffer, int bufferSize);

#endif 