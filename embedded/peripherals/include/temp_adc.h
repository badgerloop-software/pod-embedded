#ifndef __BOARD_TEMP_H__
#define __BOARD_TEMP_H__

#include <stdint.h>

int initTempSensors();
uint16_t readTelem_Therm_0();
uint16_t readTelem_Therm_1();
uint16_t readTelem_Therm_4();
uint16_t readTelem_Therm_5();
uint16_t readADC_Extra0();
uint16_t readADC_Extra1();
uint16_t readADC_Extra2();
uint16_t readTelem_I_28V();

uint16_t* debugTemps(void);
#endif
