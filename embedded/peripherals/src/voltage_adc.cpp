// #include <math.h>
// #include "adc128.h"
// #include "i2c.h"
// #include "voltage_adc.h"
// #include "data.h"
// #include <stdint.h>
// #include "NCD9830DBR2G.h"

// int initVoltageSensors() {
//     return (u2_adc.init() && u4_adc.init());
// }

// uint16_t readTelem_V_24V() {
//     uint16_t data;
//     readADCLine(1, 0, &data);
//     return data;
// }

// uint16_t readTelem_I_24V() {
//     uint16_t data;
//     readADCLine(1, 1, &data);
//     return data;
// }

// uint16_t readTelem_V_12V() {
//     uint16_t data;
//     readADCLine(1, 4, &data);
//     return data;
// }

// uint16_t readTelem_I_12V() {
//     uint16_t data;
//     readADCLine(1, 3, &data);
//     return data;
// }

// uint16_t readTelem_V_5V() {
//     uint16_t data;
//     readADCLine(1, 5, &data);
//     return data;
// }

// uint16_t readTelem_I_5V() {
//     uint16_t data;
//     readADCLine(1, 2, &data);
//     return data;
// }

// uint16_t readTelem_I_28V() {
//     uint16_t data;
//     readADCLine(0, 7, &data);
//     return data;
// }

// uint16_t* harvestBoardData() {
//     uint16_t readings[7];

//     readings[0] = readTelem_I_24V();
//     readings[1] = readTelem_V_24V();
//     readings[2] = readTelem_I_12V();
//     readings[3] = readTelem_V_12V();
//     readings[4] = readTelem_I_5V();
//     readings[5] = readTelem_V_5V();
//     readings[6] = readTelem_I_28V();

//     return readings;

// }