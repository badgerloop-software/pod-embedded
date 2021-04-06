#include <temp_adc.h>
#include <voltage_adc.h>
#include <stdio.h>

extern "C" {
    #include <data.h>
}

int main() {
    initBoardVoltage();

    uint16_t res = readTelem_V_12V();
    printf("%d", res);
}