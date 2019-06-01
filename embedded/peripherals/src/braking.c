#include <badgerloop.h>
#include <NCD9830DBR2G.h>
#include <braking.h>
#include <data.h>
#include <stdio.h>

#define VOLTAGE_SCALING(x) ( ((((x / 256) * 5) - 0.5) / 4) * 2000)
#define CURRENT_SCALING(x) ( ((((x / 256) * 5) - 0.6) / 2.4) * 500)

//Voltage
double readPrimaryTank() {
    uint8_t data[2];
    if (readPressureSensor(ADC_1, PS_TANK, data) != OK)
        return -1;

    return (VOLTAGE_SCALING(data[0]));
}

//Current
double readPrimaryLine() {
    uint8_t data[2];
    if (readPressureSensor(ADC_1, PS_LINE, data) != OK)
        return -1;
//    return data[0];
    return ( CURRENT_SCALING(data[0]) );
}

//Current
double readPrimaryActuator() {
    uint8_t data[2];
    if (readPressureSensor(ADC_1, PS_ACTUATE, data) != OK) {
        return -1;
    }
    return ( CURRENT_SCALING(data[0]) );
}

//Voltage
double readSecTank() {
    return -1;
}

//Current
double readSecLine() {
    return -1;
}

//Current
double readSecActuate() {
    return -1;
}

/* Damn I dont know how to spell vessel */
double readPressureVessel() {
    return 0;
}
