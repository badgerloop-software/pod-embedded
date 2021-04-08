#include <temp_adc.h>
#include <voltage_adc.h>
#include "stdio.h"

extern "C" {
    #include <NCD9830DBR2G.h>
}

int main(int argc, char **argv) {
    printf("Initting Sensor\n");
    initPressureSensors();
    uint16_t data;
    printf("Reading data\n");
    int res = readPressureSensor(0, 0, &data);

    printf("Got %x\n", data);

    for(int i=0; i < 8; i++) {
        uint16_t buff;
        int res = readPressureSensor(0, i, &buff);
        printf("Channel %d: %x\n", i, buff);
    }

    return 0;
}