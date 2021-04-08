#include <temp_adc.h>
#include <voltage_adc.h>
#include "stdio.h"

extern "C" {
    #include <NCD9830DBR2G.h>
}

int main(int argc, char **argv) {
    printf("Initting Sensor\n");
    initADCs();
    uint16_t data;
    printf("Reading data\n");
    int res = readADCLine(0, 0, &data);

    printf("Got %x\n", data);

    for(int i=0; i < 8; i++) {
        uint16_t buff;
        int res = readADCLine(0, i, &buff);
        printf("Channel %d: %x\n", i, buff);
    }

    // printf("\n\n\nWrapper Classes\n\n\n");

    // uint16_t* telem = harvestBoardData();

    // for(int i=0; i < 7; i++) {
    //     printf("Reading Channel %d: %d\n", i, telem[i]);
    // }

    // uint16_t* temps = harvestTemps();

    //     for(int i=0; i < 7; i++) {
    //     printf("Reading Channel %d: %d\n", i, temps[i]);
    // }

    return 0;
}