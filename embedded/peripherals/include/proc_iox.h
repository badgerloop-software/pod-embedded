#ifndef __PROC_IOX_H__
#define __PROC_IOX_H__

#include <stdbool.h>
#include <mcp23017.h>

#define DEBUG_LED_0 MCP_GPIOB_4
#define DEBUG_LED_1 MCP_GPIOB_5
#define DEBUG_LED_2 MCP_GPIOB_6
#define DEBUG_LED_3 MCP_GPIOB_7

#define SW_DEBUG_PIN_0 MCP_GPIOB_0
#define SW_DEBUG_PIN_1 MCP_GPIOB_1

int initProcIox(void);

int procIoxLedSet(int led, bool val);

#endif
