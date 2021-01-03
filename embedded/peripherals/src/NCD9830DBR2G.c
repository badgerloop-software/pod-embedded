/**
 *  Driver for an 8 bit, 8 channel ADC controlled over i2c
 */

#include "NCD9830DBR2G.h"
#include "i2c.h"
#include <stdint.h>
#include <stdio.h>

i2c_settings adc0 = {
    .fd = 0,
    .bus = 2,
    .deviceAddress = NCD9830_ADR0,
    .openMode = O_RDWR
};

/*i2c_settings adc1 = {*/
/*    .fd              =   0,*/
/*    .bus             =   2,*/
/*    .deviceAddress   =   NCD9830_ADR1,*/
/*    .openMode        =   O_RDWR*/
/*};*/

i2c_settings* adcs[2];

static int selectChannel(uint8_t devNum, uint8_t channel);
static int readChannel(uint8_t devNum, uint8_t channel, uint8_t* data);

int readPressureSensor(int sensor, uint8_t channel, uint8_t* data)
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

    uint8_t cmdByte = 0;
    cmdByte = SD_BIT | CHANNEL(channel) | PD_BITS;

    if (write_byte_i2c(adcs[devNum], cmdByte) != 0) {
        fprintf(stderr, "Failed to write channel select byte: %#x\n", cmdByte);
        return -1;
    }

    return 0;
}

static int readChannel(uint8_t devNum, uint8_t channel, uint8_t* data)
{
    selectChannel(devNum, channel);
    if (read_i2c((i2c_settings*)adcs[devNum], data, 1) != 0) {
        fprintf(stderr, "Failed to read data from ADC %d.\n", devNum);
        return -1;
    }
    return 0;
}

int initPressureSensors()
{
    adcs[0] = &adc0;

    if (i2c_begin(&adc0) != 0) {
        fprintf(stderr, "Failed to open i2c bus for pressure sensor 1.\n");
        return -1;
    }

    /*	adcs[1] = &adc1; 
    NOT ON THE BUS
    if (i2c_begin(presSens1) != 0) {
        fprintf(stderr, "Failed to open i2c bus for pressure sensor 2.\n");
	}
*/
    return 0;
}
