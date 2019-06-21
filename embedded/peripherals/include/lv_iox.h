#ifndef __LV_IOX_H__
#define __LV_IOX_H__

#include <stdbool.h>
#include <mcp23017.h>

#define SOLENOID_0  MCP_GPIOB_0
#define SOLENOID_1  MCP_GPIOB_1
#define SOLENOID_2  MCP_GPIOB_2
#define SOLENOID_3  MCP_GPIOB_3
#define SOLENOID_4  MCP_GPIOB_4
#define SOLENOID_5  MCP_GPIOB_5
#define SOLENOID_6  MCP_GPIOB_6
#define SOLENOID_7  MCP_GPIOB_7

#define LIM_SWITCH_0 MCP_GPIOA_0
#define LIM_SWITCH_1 MCP_GPIOA_1
#define LIM_SWITCH_2 MCP_GPIOA_2
#define LIM_SWITCH_3 MCP_GPIOA_3

int initLVIox(bool hardStart);

int limSwitchGet(int limSwitch);

int solenoidSet(int solenoid, bool val);

#endif
