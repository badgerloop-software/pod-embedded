#include <math.h>
#include "adc128.h"
#include "i2c.h"
#include "boardTelem.h"
#include "data.h"
#include <stdint.h>

#define SCALAR (1.6) //(0.00097656/2) 

extern i2c_settings i2c;
extern Data data;

i2c_settings u2 = {
    .fd = 0,
    .bus = 2,
    .deviceAddress = U2_ADDR,
    .openMode = O_RDWR
};

i2c_settings u4 = {
    .fd = 0,
    .bus = 2,
    .deviceAddress = U4_ADDR,
    .openMode = O_RDWR
};


static Adc u2_adc(&u2, railADC7Addr);
static Adc u4_adc(&u4, railADC7Addr);

int initBoardTelem() {
    return (u2_adc.init() && u4_adc.init());
}

void harvestBoardTelem() {
    data.boardTelem[BUS_V]      = readBusV();
    data.boardTelem[BUS_A]      = readBusA();
    data.boardTelem[V5_RAIL_V]  = read5VRailV();
    data.boardTelem[V5_RAIL_A]  = read5VRailA();
    data.boardTelem[V7_RAIL_V]  = read7VRailV();
    data.boardTelem[V7_RAIL_A]  = read7VRailA();
    data.boardTelem[THERM_1]    = readTherm1();
    data.boardTelem[THERM_2]    = readTherm2();
}

uint16_t readTelem_V_24V() {
    return adc.readChannel(CHAN_0);
}

uint16_t readTelem_I_24V() {
/*    uint16_t ret = adc.readChannel(chan);// * SCALAR;*/
    return adc.readChannel(CHAN_1);
}

uint16_t readTelem_I_5V() {
/*    AdcChan chan = CHAN_2;*/
/*    uint16_t ret = adc.readChannel(chan) * SCALAR;*/
    return adc.readChannel(CHAN_2);
    
}

uint16_t readTelem_I_12V() {
/*    AdcChan chan = CHAN_3;*/
/*    uint16_t ret = adc.readChannel(chan) * SCALAR;*/
    return adc.readChannel(CHAN_3);
}

uint16_t readTelem_V_12V() {
/*    AdcChan chan = CHAN_4;*/
/*    uint16_t ret = adc.readChannel(chan) * SCALAR;*/
    return adc.readChannel(CHAN_4);
}

uint16_t readTelem_V_5V() {
/*    AdcChan chan = CHAN_5;*/
/*    uint16_t ret = adc.readChannel(chan) * SCALAR;*/
    return adc.readChannel(CHAN_5);
}

uint16_t readTelem_Therm_3() {
/*    AdcChan chan = CHAN_6;*/
/*    uint16_t ret = adc.readChannel(chan);*/
/*    ret = 1 / ((log(ret/4700) / 3500) + 0.04);*/
    return adc.readChannel(CHAN_6);
}

uint16_t readTelem_Therm_2() {
/*    AdcChan chan = CHAN_7;*/
/*    uint16_t ret = adc.readChannel(chan);*/
/*    ret = 1 / ((log(ret/4700) / 3500) + 0.04);*/
    return adc.readChannel(CHAN_7);
}
