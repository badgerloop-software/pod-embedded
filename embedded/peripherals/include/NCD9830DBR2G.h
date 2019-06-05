#ifndef __NCD98_H__
#define __NCD98_H__

#include "badgerloop.h"
#include <stdint.h>
#define ADC_0  0x0
#define ADC_1  0x1
#define NCD9830_ADR0 0x48   /* This is the base */
#define NCD9830_ADR1 0x49   /* Could be wrong */

/* The i2c message builder for this device */
#define SD_BIT       0x1 << 7
#define PD_BITS      0x1 << 2
#define CHANNEL(num) num << 4

#define CHANNEL_0    0x0
#define CHANNEL_1    0x4
#define CHANNEL_2    0x1
#define CHANNEL_3    0x5
#define CHANNEL_4    0x2
#define CHANNEL_5    0x6
#define CHANNEL_6    0x3
#define CHANNEL_7    0x7

#define PS_TANK    CHANNEL_0
#define PS_LINE    CHANNEL_1
#define PS_ACTUATE CHANNEL_2

#define BS_TANK    CHANNEL_3
#define BS_LINE    CHANNEL_4
#define BS_ACTUATE CHANNEL_5

#define PRES_VESL   CHANNEL_6
STATUS readPressureSensor(int sensor, uint8_t channel, uint8_t *data);
STATUS initPressureSensors(void);


#endif
