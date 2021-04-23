// #include <math.h>
// #include "adc128.h"
// #include "i2c.h"
// #include "temp_adc.h"
// #include "data.h"
// #include <stdint.h>
// #include "NCD9830DBR2G.h"

// // int initTempSensors() {
// //     return (u2_adc.init() && u4_adc.init());
// // }

// uint16_t readTelem_Therm_0() {
//     uint16_t data;
//     readADCLine(0, 0, &data);
//     return data;
// }

// uint16_t readTelem_Therm_1() {
//     uint16_t data;
//     readADCLine(0, 1, &data);
//     return data;
// }

// uint16_t readTelem_Therm_2() {
//     uint16_t data;
//     readADCLine(1, 7, &data);
//     return data;
// }

// uint16_t readTelem_Therm_3() {
//     uint16_t data;
//     readADCLine(1, 6, &data);
//     return data;
// }

// uint16_t readTelem_Therm_4() {
//     uint16_t data;
//     readADCLine(0, 2, &data);
//     return data;
// }

// uint16_t readTelem_Therm_5() {
//     uint16_t data;
//     readADCLine(0, 5, &data);
//     return data;
// }

// uint16_t readADC_Extra0() {
//     uint16_t data;
//     readADCLine(0, 4, &data);
//     return data;
// }

// uint16_t readADC_Extra1() {
//     uint16_t data;
//     readADCLine(0, 5, &data);
//     return data;
// }

// uint16_t readADC_Extra2() {
//     uint16_t data;
//     readADCLine(0, 6, &data);
//     return data;
// }




// uint16_t* harvestTemps(void) {
//     uint16_t temps[6];

//     temps[0] = readTelem_Therm_0();
//     temps[1] = readTelem_Therm_1();
//     temps[2] = readTelem_Therm_2();
//     temps[3] = readTelem_Therm_3();
//     temps[4] = readTelem_Therm_4();
//     temps[5] = readTelem_Therm_5();

//     return temps;
// }
