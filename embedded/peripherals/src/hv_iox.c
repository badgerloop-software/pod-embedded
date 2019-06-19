#include <stdio.h>
#include <stdbool.h>
#include <stdint.h>
#include <data.h>
#include <i2c.h>
#include <mcp23017.h>
#include <proc_iox.h>
#include <hv_iox.h>

#define HV_IO_ADDR   	0x24

#define HV_IND_EN       MCP_GPIOB_0
#define MCU_LATCH       MCP_GPIOB_1
#define BMS_MULTI_IN    MCP_GPIOB_2

#define IMD_STAT_FDBK   MCP_GPIOA_7
#define INRT_STAT_FDBK  MCP_GPIOA_6
#define HV_EN_FDBK      MCP_GPIOA_5
#define MCU_HV_EN       MCP_GPIOA_4
#define PS_FDBK         MCP_GPIOA_3
#define BMS_STAT_FDBK   MCP_GPIOA_2
#define E_STOP_FDBK     MCP_GPIOA_1
#define MSTR_SW_FDBK    MCP_GPIOA_0


static i2c_settings iox;
static int setupIox(void);

int initHVIox() {
    setupMCP(&iox, HV_IO_ADDR);
    clearSettingsMCP(&iox);
    setupIox();
    return 0;
}

i2c_settings getHVIoxDev() {
	return iox;
}

/***
 *  initHVIox - Meant to be overly pedantic
 *      even if the values arent changing,
 *      just to make extremely clear the initial
 *      state of everything.
 *
 */
static int setupIox() {
    setDir(&iox, HV_IND_EN, MCP_DIR_IN);
    setDir(&iox, MCU_LATCH, MCP_DIR_OUT);
    setDir(&iox, BMS_MULTI_IN, MCP_DIR_IN);
    setDir(&iox, IMD_STAT_FDBK, MCP_DIR_IN);
    setDir(&iox, INRT_STAT_FDBK, MCP_DIR_IN);
    setDir(&iox, HV_EN_FDBK, MCP_DIR_IN);
    setDir(&iox, MCU_HV_EN, MCP_DIR_IN);
    setDir(&iox, PS_FDBK, MCP_DIR_IN);
    setDir(&iox, BMS_STAT_FDBK, MCP_DIR_IN);
    setDir(&iox, E_STOP_FDBK, MCP_DIR_IN);
    setDir(&iox, MSTR_SW_FDBK, MCP_DIR_IN);
    return 0; /* TODO Add error checking */
}

int isHVIndicatorEnabled() {
    return getState(&iox, HV_IND_EN);
}

int setMCULatch(bool val) {
    return setState(&iox, MCU_LATCH, val);
}

int getBMSMultiIn() {
    return getState(&iox, BMS_MULTI_IN);
}

int getIMDStatus() {
    return getState(&iox, IMD_STAT_FDBK);
}

int getINRTStatus() {
    return getState(&iox, INRT_STAT_FDBK);
}

int isHVEnabled() {
    return getState(&iox, HV_EN_FDBK);
}

int isMCUHVEnabled() {
    setDir(&iox, MCU_HV_EN, MCP_DIR_IN);
    return getState(&iox, MCU_HV_EN);
}

int emergencyDisableMCU() {
    setDir(&iox, MCU_HV_EN, MCP_DIR_OUT);
    return setState(&iox, MCU_HV_EN, true);
}

int getPsStatus() {
    return getState(&iox, PS_FDBK);
}

int getBMSStatus() {
    return getState(&iox, BMS_STAT_FDBK);
}

int isEStopOn() {
    return getState(&iox, E_STOP_FDBK);
}

int getMasterSwFeedback() {
    return getState(&iox, MSTR_SW_FDBK);
}
