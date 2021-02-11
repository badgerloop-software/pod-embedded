#include <iostream>
#include "NCD9830DBR2G.h"
#include <cstdint>

//Fix this Later
int const VOLTAGE_SENSOR = CHANNEL_0;
int const CURR_SENSOR  =  CHANNEL_7;

double voltScale(double rawVolt);
double currentScale(double rawCurr);
double getLVBattVoltage();
double getLVCurrent();

double voltScale(double rawVolt) {
    return (rawVolt/255.0) * 5.0 * 2.942;
}

double currentScale(double rawCurr){
    return ((((rawCurr/255.0) * 5.0) -2.5 / 2.5) * 50);
}


double getLVBattVoltage() {
    uint8_t data[2] {0};
    readPressureSensor(ADC_0, VOLTAGE_SENSOR, data);
    return voltScale(data[0]);
}

double getLVCurrent() {
    uint8_t data[2] {0};
    readPressureSensor(ADC_0, CURR_SENSOR, data);
    return currentScale(data[0]);
}
