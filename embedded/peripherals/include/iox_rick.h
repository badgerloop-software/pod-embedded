#ifndef __IOX_RICK_H__
#define __IOX_RICK_H__

#include <stdbool.h>

extern "C" {
#include <data.h>
#include <i2c.h>
#include <mcp23017.h>
#include <proc_iox.h>
}

#define MCU_LATCH MCP_GPIOB_0
#define MSTR_SW_FDBK MCP_GPIOB_1
#define E_STOP_FDBK MCP_GPIOB_2
#define LMT_SW_FDBK MCP_GPIOB_3
#define BMS_STAT_FDBK MCP_GPIOB_4
#define IMD_STAT_FDBK MCP_GPIOB_5
#define INRT_STAT_FDBK MCP_GPIOB_6
#define MCU_STAT_FDBK MCP_GPIOB_7

#define MCU_HV_EN MCP_GPIOA_7
#define BMS_MPI1 MCP_GPIOA_6
#define LIM_MCU3 MCP_GPIOA_5
#define LIM_MCU2 MCP_GPIOA_4
#define LIM_MCU1 MCP_GPIOA_3
#define LIM_MCU0 MCP_GPIOA_2
#define CHARGE_LV MCP_GPIOA_1


class iox_rick {
private:
    i2c_settings iox;
public:
    iox_rick(void);

    int init(bool);

    int setupIox(void);

    i2c_settings getiox_rickDev(void);

    int setMCULatch(bool val);

    int getMSTRSw(void);

    int getEStop(void);

    int getLMTSw(void);

    int getBMSStat(void);

    int getIMDStat(void);

    int getIRTStat(void);

    int getMCUStat(void);

    int setMCUHvEn(bool val);

    int setBMS_MPI1(bool val);

    int getLIM_MCU3(void);

    int getLIM_MCU2(void);

    int getLIM_MCU1(void);

    int getLIM_MCU0(void);

    int ChargeLV(int val);
};

extern iox_rick ioxRick;
#endif

