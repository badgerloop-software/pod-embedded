#include <iox_rick.h>
#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>

extern "C" {
#include <data.h>
#include <i2c.h>
#include <mcp23017.h>
#include <proc_iox.h>
}

#define HV_IO_ADDR 0x24

#ifdef NOI2C
#define VI2C
#endif

static i2c_settings iox;
static int setupIox(void);

/***
 *  initiox_rick - Meant to be overly pedantic
 *      even if the values arent changing,
 *      just to make extremely clear the initial
 *      state of everything.
 *
 */
iox_rick::iox_rick()
{
}

int iox_rick::init(bool hardStart)
{
    if (setupMCP(&iox, HV_IO_ADDR) != 0)
        return -1;
    else if (hardStart) {
        if (clearSettingsMCP(&iox) != 0)
            return -1;
        else if (this->setupIox() != 0)
            return -1;
    }
    return 0;
}

int iox_rick::setupIox()
{
    // static int setupIox() moved into constructor:
    setDir(&iox, MCU_LATCH, MCP_DIR_IN);
    setDir(&iox, MSTR_SW_FDBK, MCP_DIR_OUT);
    setDir(&iox, E_STOP_FDBK, MCP_DIR_IN);
    setDir(&iox, LMT_SW_FDBK, MCP_DIR_IN);
    setDir(&iox, BMS_STAT_FDBK, MCP_DIR_IN);
    setDir(&iox, IMD_STAT_FDBK, MCP_DIR_IN);
    setDir(&iox, INRT_STAT_FDBK, MCP_DIR_IN);
    setDir(&iox, MCU_STAT_FDBK, MCP_DIR_IN);
    setDir(&iox, MCU_HV_EN, MCP_DIR_IN);
    setDir(&iox, BMS_MPI1, MCP_DIR_IN);
    setDir(&iox, LIM_MCU3, MCP_DIR_IN);
    setDir(&iox, LIM_MCU2, MCP_DIR_IN);
    setDir(&iox, LIM_MCU1, MCP_DIR_IN);
    setDir(&iox, LIM_MCU0, MCP_DIR_IN);
    setDir(&iox, Charge_LV, MCP_DIR_IN);

    return 0;
}

i2c_settings iox_rick::getiox_rickDev()
{
    return iox;
}

int iox_rick::setMCULatch(bool val)
{
#ifdef NOI2C
    return 0;
#endif
    return setState(&iox, MCU_LATCH, val);
}

int iox_rick::getMSTRSw()
{
#ifdef NOI2C
    return 1;
#endif
    return getState(&iox, MSTR_SW_FDBK);
}

int iox_rick::getEStop()
{
#ifdef NOI2C
    return 1;
#endif
    return getState(&iox, E_STOP_FDBK);
}

int iox_rick::getLMTSw()
{
#ifdef NOI2C
    return 1;
#endif
    return getState(&iox, LMT_SW_FDBK);
}

int iox_rick::getBMSStat()
{
#ifdef NOI2C
    return 1;
#endif
    return getState(&iox, BMS_STAT_FDBK);
}

int iox_rick::getIMBStat()
{
#ifdef NOI2C
    return 0;
#endif
    return getState(&iox, IMD_STAT_FDBK);
}

int iox_rick::isMCUHVEnabled()
{
#ifdef NOI2C
    return 1;
#endif
    setDir(&iox, MCU_HV_EN, MCP_DIR_IN);
    return getState(&iox, MCU_HV_EN);
}

int iox_rick::emergencyDisableMCU()
{
#ifdef NOI2C
    return 1;
#endif
    setDir(&iox, MCU_HV_EN, MCP_DIR_OUT);
    return setState(&iox, MCU_HV_EN, true);
}

int iox_rick::getPsStatus()
{
#ifdef NOI2C
    return 1;
#endif
    return getState(&iox, PS_FDBK);
}

int iox_rick::getBMSStatus()
{
#ifdef NOI2C
    return 1;
#endif
    return getState(&iox, BMS_STAT_FDBK);
}

int iox_rick::isEStopOn()
{
#ifdef NOI2C
    return 0;
#endif
    return getState(&iox, E_STOP_FDBK);
}

int iox_rick::getMasterSwFeedback()
{
#ifdef NOI2C
    return 1;
#endif
    return getState(&iox, MSTR_SW_FDBK);
}

int iox_rick::setMCUHVEnabled(int val)
{
#ifdef NOI2C
    return 0;
#endif
    setDir(&iox, MCU_HV_EN, MCP_DIR_OUT);
    return setState(&iox, MCU_HV_EN, val);
}