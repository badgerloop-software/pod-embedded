#include <badgerloop.h>
#include <NCD9830DBR2G.h>
#include <braking.h>
#include <data.h>
#include <stdio.h>

#define VOLTAGE_SCALING 9.765625
#define CURRENT_SCALING 0.06103515625


double readPrimaryTank() {
    uint8_t data[2];
    if (readPressureSensor(ADC_1, PS_TANK, data) != OK)
        return -1;
        
    return (data[0] * VOLTAGE_SCALING);
}

double readPrimaryLine() {
    uint8_t data[2];
    if (readPressureSensor(ADC_1, PS_LINE, data) != OK)
        return -1;
    
    return (data[0] * CURRENT_SCALING);
}

int readPrimaryActuator() {
    uint8_t data[2];
    if (readPressureSensor(ADC_1, PS_ACTUATE, data) != OK) {
        return -1;
    }
    return (data[0] * CURRENT_SCALING);
}

int readSecTank() {
    return -1;
}

int readSecLine() {
    return -1;
}

int readSecActuate() {
    return -1;
}

/* Damn I dont know how to spell vessel */
int readPressureVessel() {
    return 0;
}
