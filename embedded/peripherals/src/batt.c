#include <stdint.h>
#include "NCD9830DBR2G.h"
#include <stdio.h>

#define VOLTAGE_SENSOR CHANNEL_0
#define CURR_SENSOR CHANNEL_7

#define VOLT_SCALEOrig(x) (((((double)x) / 255.0) * 5.0000 * 2.942))
#define CURR_SCALEOrig(x) ((((((double)x) / 255.0) * 5.0000) - 2.5 / 2.5) * 50)
double getLVBattVoltageOrig();
double getLVCurrentOrig();

double getLVBattVoltageOrig()
{
    uint8_t data[2] = { 0 };
    readPressureSensor(ADC_0, VOLTAGE_SENSOR, data);
    return VOLT_SCALE((double)data[0]);
}

double getLVCurrentOrig()
{
    uint8_t data[2] = { 0 };
    readPressureSensor(ADC_0, CURR_SENSOR, data);

    return CURR_SCALE(data[0]);
}
