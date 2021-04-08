#include <math.h>
#include "adc128.h"
#include "i2c.h"
#include "boardTelem.h"
#include "data.h"
#include <stdint.h>

extern i2c_settings i2c;

static Adc u2_adc(&u2, U2_ADDR);
static Adc u4_adc(&u4, U4_ADDR);

int initBoardVoltage() {
    return (u2_adc.init() && u4_adc.init());
}

uint16_t readTelem_V_24V() {
    return u2_adc.readChannel(CHAN_0);
}

uint16_t readTelem_I_24V() {
/*    uint16_t ret = adc.readChannel(chan);// * SCALAR;*/
    return u2_adc.readChannel(CHAN_1);
}

uint16_t readTelem_I_5V() {
/*    AdcChan chan = CHAN_2;*/
/*    uint16_t ret = adc.readChannel(chan) * SCALAR;*/
    return u2_adc.readChannel(CHAN_2);
    
}

uint16_t readTelem_I_12V() {
/*    AdcChan chan = CHAN_3;*/
/*    uint16_t ret = adc.readChannel(chan) * SCALAR;*/
    return u2_adc.readChannel(CHAN_3);
}

uint16_t readTelem_V_12V() {
/*    AdcChan chan = CHAN_4;*/
/*    uint16_t ret = adc.readChannel(chan) * SCALAR;*/
    return u2_adc.readChannel(CHAN_4);
}

uint16_t readTelem_V_5V() {
/*    AdcChan chan = CHAN_5;*/
/*    uint16_t ret = adc.readChannel(chan) * SCALAR;*/
    return u2_adc.readChannel(CHAN_5);
}

uint16_t readTelem_Therm_3() {
/*    AdcChan chan = CHAN_6;*/
/*    uint16_t ret = adc.readChannel(chan);*/
/*    ret = 1 / ((log(ret/4700) / 3500) + 0.04);*/
    return u2_adc.readChannel(CHAN_6);
}

uint16_t readTelem_Therm_2() {
/*    AdcChan chan = CHAN_7;*/
/*    uint16_t ret = adc.readChannel(chan);*/
/*    ret = 1 / ((log(ret/4700) / 3500) + 0.04);*/
    return u2_adc.readChannel(CHAN_7);
}
