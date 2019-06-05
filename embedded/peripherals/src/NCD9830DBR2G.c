/**
 *  Driver for an 8 bit, 8 channel ADC controlled over i2c
 */

#include <badgerloop.h>
#include <stdio.h>
#include <stdint.h>
#include "NCD9830DBR2G.h"
#include "i2c.h"

i2c_settings adc0    = {
    .fd              =   0,
    .bus             =   2,
    .deviceAddress   =   NCD9830_ADR0,
    .openMode        =   O_RDWR
};

i2c_settings adc1 = {
    .fd              =   0,
    .bus             =   2,
    .deviceAddress   =   NCD9830_ADR1,
    .openMode        =   O_RDWR
};

i2c_settings *adcs[2];

static STATUS selectChannel(uint8_t devNum, uint8_t channel);
static STATUS readChannel(uint8_t devNum, uint8_t channel, uint8_t *data);

STATUS readPressureSensor(int sensor, uint8_t channel, uint8_t *data) {
    if (readChannel(sensor, channel, data) == ERROR) {
        fprintf(stderr, "Error reading the pressure sensor: %d\n", sensor);
        return ERROR;
    }
    return OK;
}


static STATUS selectChannel(uint8_t devNum, uint8_t channel) {
    if (channel > 7) {
        fprintf(stderr, "Invalid channel, must be 0-7.\n");
        return ERROR;
    }

    uint8_t cmdByte = 0;
    cmdByte = SD_BIT | CHANNEL(channel) | PD_BITS;

    if (write_byte_i2c(adcs[devNum], cmdByte) == ERROR) {
        fprintf(stderr, "Failed to write channel select byte: %#x\n", cmdByte);
        return ERROR;
    }

    return OK;
}

static STATUS readChannel(uint8_t devNum, uint8_t channel, uint8_t *data) {
   selectChannel(devNum, channel);
   if (read_i2c((i2c_settings *)adcs[devNum], data, 1) == ERROR) {
       fprintf(stderr, "Failed to read data from ADC %d.\n", devNum);
       return ERROR;
   }
   return OK;
}

STATUS initPressureSensors() {
	adcs[0] = &adc0;
/*	adcs[1] = &adc1; */

    if (i2c_begin(&adc0) == ERROR) {
        fprintf(stderr, "Failed to open i2c bus for pressure sensor 1.\n");
        return ERROR;
    }
/* NOT ON THE BUS
    if (i2c_begin(presSens1) == ERROR) {
        fprintf(stderr, "Failed to open i2c bus for pressure sensor 2.\n");
	}
*/
	return OK;
}
