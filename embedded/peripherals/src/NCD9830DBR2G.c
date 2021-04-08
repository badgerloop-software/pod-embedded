/**
 *  Driver for an 8 bit, 8 channel ADC controlled over i2c
 */

#include "NCD9830DBR2G.h"
#include "i2c.h"
#include <stdint.h>
#include <stdio.h>

i2c_settings adc0 = { // U4
    .fd = 0,
    .bus = 2,
    .deviceAddress = 0x1d,
    .openMode = O_RDWR
};

i2c_settings adc1 = { // U2
    .fd              =   0,
    .bus             =   2,
    .deviceAddress   =   0x2d,
    .openMode        =   O_RDWR
};

i2c_settings* adcs[2];

static int selectChannel(uint8_t devNum, uint8_t channel);
static int readChannel(uint8_t devNum, uint8_t channel, uint16_t* data);

int readADCLine(int sensor, uint8_t channel, uint16_t* data)
{
    if (readChannel(sensor, channel, data) == -1) {
        fprintf(stderr, "-1 reading the pressure sensor: %d\n", sensor);
        return -1;
    }
    return 0;
}

static int selectChannel(uint8_t devNum, uint8_t channel)
{
    if (channel > 7) {
        fprintf(stderr, "Invalid channel, must be 0-7.\n");
        return -1;
    }

    // uint8_t cmdByte = 0;
    // cmdByte = (unsigned char*)(0x20 + channel);
    char cmdByte = 0x20;
    printf("Selecting Channel 0x%x\n", cmdByte);
    printf("Trying address %x\n",adcs[devNum]->deviceAddress);
    if (write_byte_i2c(adcs[devNum], (unsigned char*)cmdByte) != 0) {
        fprintf(stderr, "Failed to write channel select byte: %x\n", cmdByte);
        return -1;
    }

    return 0;
}

static int readChannel(uint8_t devNum, uint8_t channel, uint16_t* data)
{
    if (selectChannel(devNum, channel) == -1) {
        fprintf(stderr, "Failed to select channel %d on adc %d\n", channel, devNum);
        return -1;
    }
    if (read_i2c((i2c_settings*)adcs[devNum], data, 2) != 0) {
        fprintf(stderr, "Failed to read data from ADC %d.\n", devNum);
        return -1;
    }
    return 0;
}

int initADC128(i2c_settings adc) {
    printf("ADV CONFIG REG\n");
    if (i2c_begin(&adc) != 0) {
        fprintf(stderr, "Failed to open i2c bus for pressure sensor 1.\n");
        return -1;
    }
    char reg[1] = {0x0B};
    if(write_byte_i2c(&adc, reg) != 0) {
        fprintf(stderr, "Couldn't write to %x\n", reg[0]);
        return -1;
    }
    char data[1];

    if (read_i2c(&adc, data, 1) != 0) {
        fprintf(stderr, "Couldn't read from I2C\n");
        return -1;
    }

    data[0] &= ~(0x07);
    data[0] |= 0x03;

    char cmd[2];

    cmd[0] = reg[0];
    cmd[1] = data[0];

    if (write_byte_i2c(&adc, cmd) != 0) {
        fprintf(stderr, "Failed to write to %x", cmd);
        return -1;
    }

    reg[0] = 0x07;
    printf("CONV RATE REG\n");
    if(write_byte_i2c(&adc, reg) != 0) {
        fprintf(stderr, "Failed to write to %x", reg);
        return -1;
    }

    if (read_i2c(&adc, data, 1) != 0) {
        fprintf(stderr, "Failed to read from I2C");
        return -1;
    }

    data[0] &= ~(0x01);
    data[0] |= 0x01;

    cmd[0] = reg[0];
    cmd[1] = data[0];

    if (write_byte_i2c(&adc, cmd) != 0) {
        fprintf(stderr, "Failed to write to %x\n", cmd);
        return -1;
    }

    usleep(10000);

    reg[0] = 0x00;
    if (write_byte_i2c(&adc, reg) != 0) {
        fprintf(stderr, "Failed to write to %x", reg);
        return -1;
    }

    if (read_i2c(&adc, data, 1) != 0) {
        fprintf(stderr, "Failed to read from I2C\n");
        return -1;
    }

    usleep(10000);

    data[0] &- ~(0x09);
    data[0] |= 0x01;

    cmd[0] = reg[0];
    cmd[1] = data[0];

    if (write_byte_i2c(&adc, cmd) != 0) {
        fprintf(stderr, "Failed to write to %x", cmd);
        return -1;
    }
    
    usleep(10000);

    if (write_byte_i2c(&adc, reg) != 0) {
        fprintf(stderr, "failed to write to %x\n", reg);
        return -1;
    }

    if (read_i2c(&adc, data, 1) != 0) {
        fprintf(stderr, "Failed to read from I2C\n");
        return -1;
    }

    return 0;
}

int initADCs()
{
    if(initADC128(adc0) != 0) {
        fprintf(stderr, "Failed to init ADC0\n");
        return -1;
    }
    printf("Sucessfully initted ADC0\n\n");
    adcs[0] = &adc0;
    // if(initADC128(adc1) != 0) {
    //     fprintf(stderr, "Failed to init ADC1\n");
    // }
    return 0;
}
