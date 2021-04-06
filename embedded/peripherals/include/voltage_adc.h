#ifndef __BOARD_TELEM_H__
#define __BOARD_TELEM_H__

#include <stdint.h>

int initBoardVoltage();
uint16_t readTelem_V_24V();
uint16_t readBusA();
uint16_t read5VRailV();
uint16_t read5VRailA();
uint16_t read7VRailA();
uint16_t read7VRailV();
uint16_t readTherm1();
uint16_t readTherm2();
#endif
