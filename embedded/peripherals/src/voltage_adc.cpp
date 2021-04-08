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

uint16_t readTelem_V_12V() {
    return u2_adc.readChannel(CHAN_4);
}

uint16_t readTelem_I_12V() {
    return u2_adc.readChannel(CHAN_3);
}

uint16_t readTelem_V_5V() {
    return u2_adc.readChannel(CHAN_5);
}

uint16_t readTelem_I_5V() {
    return u2_adc.readChannel(CHAN_2);
}

uint16_t readTelem_I_28V() {
    return u4_adc.readChannel(CHAN_7);
}

uint16_t* harvestBoardData() {
    uint16_t readings[7];

    readings[0] = readTelem_I_24V();
    readings[1] = readTelem_V_24V();
    readings[2] = readTelem_I_12V();
    readings[3] = readTelem_V_12V();
    readings[4] = readTelem_I_5V();
    readings[5] = readTelem_V_5V();
    readings[6] = readTelem_I_28V();

    return readings;

}