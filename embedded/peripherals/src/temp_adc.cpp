#include <math.h>
#include "adc128.h"
#include "i2c.h"
#include "temp_adc.h"
#include "data.h"
#include <stdint.h>

int initTempSensors() {
    return (u2_adc.init() && u4_adc.init());
}

uint16_t readTelem_Therm_0() {
    return u4_adc.readChannel(CHAN_0);
}

uint16_t readTelem_Therm_1() {
    return u4_adc.readChannel(CHAN_1);
}

uint16_t readTelem_Therm_2() {
    return u2_adc.readChannel(CHAN_7);
}

uint16_t readTelem_Therm_3() {
    return u2_adc.readChannel(CHAN_6);
}

uint16_t readTelem_Therm_4() {
    return u4_adc.readChannel(CHAN_2);
}

uint16_t readTelem_Therm_5() {
    return u4_adc.readChannel(CHAN_3);
}

uint16_t readADC_Extra0() {
    return u4_adc.readChannel(CHAN_4);
}

uint16_t readADC_Extra1() {
    return u4_adc.readChannel(CHAN_5);
}

uint16_t readADC_Extra2() {
    return u4_adc.readChannel(CHAN_6);
}




uint16_t* debugTemps(void) {
    uint16_t temps[6];

    temps[0] = readTelem_Therm_0();
    temps[1] = readTelem_Therm_1();
    temps[2] = readTelem_Therm_2();
    temps[3] = readTelem_Therm_3();
    temps[4] = readTelem_Therm_4();
    temps[5] = readTelem_Therm_5();

    return temps;
}
