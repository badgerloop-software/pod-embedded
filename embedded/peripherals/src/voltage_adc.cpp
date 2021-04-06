#include <math.h>
#include "adc128.h"
#include "i2c.h"
#include "voltage_adc.h"
#include "data.h"
#include <stdint.h>

int initVoltageSensors() {
    return (u2_adc.init() && u4_adc.init());
}

uint16_t readTelem_V_24V() {
    return u2_adc.readChannel(CHAN_0);
}

uint16_t readTelem_I_24V() {
    return u2_adc.readChannel(CHAN_1);
}

uint16_t readTelem_I_5V() {
    return u2_adc.readChannel(CHAN_2);
}

uint16_t readTelem_I_12V() {
    return u2_adc.readChannel(CHAN_3);
}

uint16_t readTelem_V_12V() {
    return u2_adc.readChannel(CHAN_4);
}

uint16_t readTelem_V_5V() {
    return u2_adc.readChannel(CHAN_5);
}

uint16_t readTelem_Therm_3() {
    return u2_adc.readChannel(CHAN_6);
}

uint16_t readTelem_Therm_2() {
    return u2_adc.readChannel(CHAN_7);
}
