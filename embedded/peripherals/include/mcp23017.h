#ifndef __MCP23017_H__
#define __MCP23017_H__

#include "i2c.h"
#include <stdbool.h>
#include <stdint.h>

#define MCP_GPIOA_0 0
#define MCP_GPIOA_1 1
#define MCP_GPIOA_2 2
#define MCP_GPIOA_3 3
#define MCP_GPIOA_4 4
#define MCP_GPIOA_5 5
#define MCP_GPIOA_6 6
#define MCP_GPIOA_7 7

#define MCP_GPIOB_0 8
#define MCP_GPIOB_1 9
#define MCP_GPIOB_2 10
#define MCP_GPIOB_3 11
#define MCP_GPIOB_4 12
#define MCP_GPIOB_5 13
#define MCP_GPIOB_6 14
#define MCP_GPIOB_7 15

#define MCP_DIR_IN 1
#define MCP_DIR_OUT 0

static const int NUM_PINS = 16;
static const char IODIR = 0x00;
static const char GPIO = 0x12;

static const char IODIRA = 0x00;
static const char IODIRB = 0x01;
static const char GPIOA = 0x12;
static const char GPIOB = 0x13;

int setupMCP(i2c_settings* i2c, char mcpAddress);

int clearSettingsMCP(i2c_settings* i2c);

int getState(i2c_settings* i2c, uint8_t pin);

int setState(i2c_settings* i2c, uint8_t pin, bool val);

int getDir(i2c_settings* i2c, uint8_t pin);

int setDir(i2c_settings* i2c, uint8_t pin, bool val);

#endif
