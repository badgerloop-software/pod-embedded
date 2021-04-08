/*
* i2c.c
* Author: Rohan Daruwala
* Based on work by: gijs
*/

#include "i2c.h"

int i2c_begin(i2c_settings* i2c)
{
    char filename[20];
    sprintf(filename, "/dev/i2c-%d", i2c->bus);
    i2c->fd = open(filename, i2c->openMode);
    if (i2c->fd < 0) {
        fprintf(stderr, "Error - Could not open file\n");
        // Commenting out the returns allows the tests to proceed on my laptop where I do not have an I2C driver
        return -1; // Uncomment this when doing Bone Dev - EU
    }

    if (ioctl(i2c->fd, I2C_SLAVE, i2c->deviceAddress) < 0) {
        fprintf(stderr, "Error - Could not set I2C Address\n");
        return -1; // Uncomment this when doing Bone Dev - EU
    }
    return 0;
}

int write_byte_i2c(i2c_settings* i2c, unsigned char* reg)
{
    printf("[I2C] Writing %x\n", reg);
    int response = i2c_smbus_write_byte(i2c->fd, reg);
    if (response < 0) {
        fprintf(stderr, "I2C write byte error\n");
        return 1;
    }
    return 0;
}

int write_data_i2c(i2c_settings* i2c, unsigned char* reg, char value)
{
    unsigned char buf[2];
    buf[0] = reg;
    buf[1] = value;
    printf("[I2C] Writing %x, %x to %x\n", buf[0], buf[1], i2c->deviceAddress);
    if (write(i2c->fd, buf, 2) != 2) {
        fprintf(stderr, "I2C write data error\n");
        return 1;
    }
    return 0;
}

int read_i2c(i2c_settings* i2c, unsigned char* readBuffer, int bufferSize)
{
    if (read(i2c->fd, readBuffer, bufferSize) != bufferSize) {
        fprintf(stderr, "I2C data read error\n");
        return 1;
    }
    // fprintf(stdout, "[I2C] Read %x\n", readBuffer);
    return 0;
}
