/**
 *  Driver for an 8 bit, 8 channel ADC controlled over i2c
 */

#include <badgerloop.h>
#include <stdio.h>
#include "NCD9830DBR2G.h"


i2c_settings presSens1 = {
    fd              =   0,
    bus             =   2,
    deviceAddress   =   NCD9830_ADR1,
    openMode        =   O_RDWR 
};

i2c_settings presSens2 = {
    fd              =   0,
    bus             =   2,
    deviceAddress   =   NCD9830_ADR2,
    openMode        =   O_RDWR
};

i2c_settings presSens[2] = {presSens1, presSens2};


STATUS readPressureSensor(int sensor, uint8_t *data) {
    /* Need to establish a mapping from the sensors to the channels */
    /* Not sure what that is now, but when its known this will be filled in */
    if (readChannel(/*MAPPED ADC# and channel*/,,data) == ERROR) {
        fprintf("Error reading the pressure sensor: %d\n", sensor);
        return ERROR;
    }
    return OK;
}


static STATUS selectChannel(uint8_t devNum, uint8_t channel) {
    if (channel > 7) {
        fprintf("Invalid channel, must be 0-7.\n");
        return ERROR;
    }
    
    uint8_t cmdByte = 0;
    uint8_t recv;
    cmdByte = SD_BIT | CHANNEL(channel) | PD_BITS;
    
    if (write_byte_i2c(presSens[devNum], cmdByte) == ERROR) {
        fprintf(stderr, "Failed to write channel select byte: %#x\n", cmdByte);
        return ERROR;
    }

    return OK;
}

static STATUS readChannel(uint8_t devNum, uint8_t channel, uint8_t *data) {
   selectChannel(devNum, channel);
   if (read_i2c(presSens[devNum], data, 1) == ERROR) {
       fprintf(stderr, "Failed to read data from ADC %d.\n", devNum);
       return ERROR;
   }
   return OK;
}

STATUS initPressureSensors() {
    
    if (i2c_begin(presSens1) == ERROR) {
        fprintf(stderr, "Failed to open i2c bus for pressure sensor 1.\n");
        return ERROR;
    }

    if (i2c_begin(presSens2) == ERROR) {
        fprintf(stderr, "Failed to open i2c bus for pressure sensor 2.\n");
    }
}
