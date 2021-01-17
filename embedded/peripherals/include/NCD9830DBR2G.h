#ifndef __NCD98_H__
#define __NCD98_H__

#include <stdint.h>
#define ADC_0 0x0
#define ADC_1 0x1
#define NCD9830_ADR0 0x48 /* This is the base */
#define NCD9830_ADR1 0x49 /* Could be wrong */

/* The i2c message builder for this device */
#define SD_BIT 0x1 << 7
#define PD_BITS 0x1 << 2
#define CHANNEL(num) num << 4

#define CHANNEL_0 0x0
#define CHANNEL_1 0x4
#define CHANNEL_2 0x1
#define CHANNEL_3 0x5
#define CHANNEL_4 0x2
#define CHANNEL_5 0x6
#define CHANNEL_6 0x3
#define CHANNEL_7 0x7

int readPressureSensor(int sensor, uint8_t channel, uint8_t* data);
int initPressureSensors(void);

#endif
