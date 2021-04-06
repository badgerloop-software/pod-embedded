#ifndef __BOARD_TELEM_H__
#define __BOARD_TELEM_H__

#include <stdint.h>

int initBoardVoltage();
uint16_t readTelem_Therm_0();
uint16_t readTelem_Therm_1();
uint16_t readTelem_Therm_4();
uint16_t readTelem_Therm_5();
uint16_t readADC_Extra0();
uint16_t readADC_Extra1();
uint16_t readADC_Extra2();
uint16_t readTelem_I_28V();
#endif
