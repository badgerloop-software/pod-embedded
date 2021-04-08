#ifndef __BOARD_TELEM_H__
#define __BOARD_TELEM_H__

#include <stdint.h>

int initVoltageSensors(void);

uint16_t readTelem_V_24V(void);
uint16_t readTelem_I_24V(void);

uint16_t readTelem_V_12V(void);
uint16_t readTelem_I_12V(void);

uint16_t readTelem_V_5V(void);
uint16_t readTelem_I_5V(void);

uint16_t readTelem_I_28V(void);

uint16_t* harvestBoardData(void);

#endif
