#include <iox_morty.h>
#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>

extern "C" {
#include <data.h>
#include <i2c.h>
#include <mcp23017.h>
#include <proc_iox.h>
}

#define IOX_MORTY_ADDR 0x21

#ifdef NOI2C
#define VI2C
#endif

static i2c_settings iox;
static int setupIox(void);

/***
 *  initiox_morty - Meant to be overly pedantic
 *      even if the values arent changing,
 *      just to make extremely clear the initial
 *      state of everything.
 *
 */
iox_morty::iox_morty()
{
}

int iox_morty::init(bool hardStart)
{
    if (setupMCP(&iox, IOX_MORTY_ADDR) != 0)
        return -1;
    else if (hardStart) {
        if (clearSettingsMCP(&iox) != 0)
            return -1;
        else if (this->setupIox() != 0)
            return -1;
    }
    return 0;
}

int iox_morty::setupIox()
{
    // static int setupIox() moved into constructor:
    setDir(&iox, BRK1_PWR_HRN, MCP_DIR_IN);
    setDir(&iox, BRK1_UART_HRN, MCP_DIR_IN);
    setDir(&iox, MCU_PUMP_PWR, MCP_DIR_OUT);
    setDir(&iox, SPLY_EN_12V, MCP_DIR_OUT);
    setDir(&iox, PRECHG_HRN, MCP_DIR_IN);
    setDir(&iox, RTC_RST_N, MCP_DIR_OUT);
    setDir(&iox, KHZ_32, MCP_DIR_IN);
    setDir(&iox, INT_SQW, MCP_DIR_IN);
    setDir(&iox, BRK0_UART_HRN, MCP_DIR_IN);
    setDir(&iox, BRK0_PWR_HRN, MCP_DIR_IN);
    setDir(&iox, HIGH_CONTACTOR_AUX, MCP_DIR_IN);
    setDir(&iox, LOW_CONTACTOR_AUX, MCP_DIR_IN);
    setDir(&iox, BATT1_DISABLE, MCP_DIR_OUT);
    setDir(&iox, BATT0_DISABLE, MCP_DIR_OUT);
    setDir(&iox, BATT2_DISABLE, MCP_DIR_OUT);
    setDir(&iox, BATT3_DISABLE, MCP_DIR_OUT);

    return 0;
}

i2c_settings iox_morty::getiox_mortyDev()
{
    return iox;
}

int iox_morty::getBrk1PWR()
{
#ifdef NOI2C
    return 1;
#endif
    return getState(&iox, BRK1_PWR_HRN);
}

int iox_morty::getBrk1UART()
{
#ifdef NOI2C
    return 1;
#endif
    return getState(&iox, BRK1_UART_HRN);
}

int iox_morty::setMCUPumpPWR(int val)
{
#ifdef NOI2C
    return 1;
#endif
    return setState(&iox, MCU_PUMP_PWR, val);
}

int iox_morty::setSplyEn12V(bool val)
{
#ifdef NOI2C
    return 1;
#endif
    return setState(&iox, SPLY_EN_12V, val);
}

int iox_morty::getPreChg()
{
#ifdef NOI2C
    return 1;
#endif
    return getState(&iox, PRECHG_HRN);
}

int iox_morty::setRTC_RST_N(int val)
{
#ifdef NOI2C
    return 1;
#endif
    return setState(&iox, RTC_RST_N, val);
}

int iox_morty::get32KHZ()
{
#ifdef NOI2C
    return 1;
#endif
    return getState(&iox, KHZ_32);
}

int iox_morty::getIntSQW()
{
#ifdef NOI2C
    return 1;
#endif
    return getState(&iox, INT_SQW);
}

int iox_morty::getBrk0PWR()
{
#ifdef NOI2C
    return 1;
#endif
    return getState(&iox, BRK0_PWR_HRN);
}

int iox_morty::getBrk0UART()
{
#ifdef NOI2C
    return 1;
#endif
    return getState(&iox, BRK0_UART_HRN);
}

int iox_morty::getHighContactorAux()
{
#ifdef NOI2C
    return 1;
#endif
    return getState(&iox, HIGH_CONTACTOR_AUX);
}

int iox_morty::getLowContactorAux()
{
#ifdef NOI2C
    return 1;
#endif
    return getState(&iox, LOW_CONTACTOR_AUX);
}

int iox_morty::setBatt0Disable(bool val)
{
#ifdef NOI2C
    return 1;
#endif
    return setState(&iox, BATT0_DISABLE, val);
}

int iox_morty::setBatt1Disable(bool val)
{
#ifdef NOI2C
    return 1;
#endif
    return setState(&iox, BATT1_DISABLE, val);
}

int iox_morty::setBatt2Disable(bool val)
{
#ifdef NOI2C
    return 1;
#endif
    return setState(&iox, BATT2_DISABLE, val);
}

int iox_morty::setBatt3Disable(bool val)
{
#ifdef NOI2C
    return 1;
#endif
    return setState(&iox, BATT3_DISABLE, val);
}