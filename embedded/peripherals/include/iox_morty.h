#ifndef __IOX_MORTY_H__
#define __IOX_MORTY_H__

#include <stdbool.h>

extern "C" {
#include <data.h>
#include <i2c.h>
#include <mcp23017.h>
#include <proc_iox.h>
}

#define BRK1_PWR_HRN MCP_GPIOB_0
#define BRK1_UART_HRN MCP_GPIOB_1
#define MCU_PUMP_PWR MCP_GPIOB_2
#define SPLY_EN_12V MCP_GPIOB_3
#define PRECHG_HRN MCP_GPIOB_4
#define RTC_RST_N MCP_GPIOB_5
#define KHZ_32 MCP_GPIOB_6
#define INT_SQW MCP_GPIOB_7

#define BRK0_UART_HRN MCP_GPIOA_7
#define BRK0_PWR_HRN MCP_GPIOA_6
#define HIGH_CONTACTOR_AUX MCP_GPIOA_5
#define LOW_CONTACTOR_AUX MCP_GPIOA_4
#define BATT1_DISABLE MCP_GPIOA_3
#define BATT0_DISABLE MCP_GPIOA_2
#define BATT2_DISABLE MCP_GPIOA_1
#define BATT3_DISABLE MCP_GPIOA_0


class iox_morty {
private:
    i2c_settings iox;
public:
    iox_morty(void);

    int init(bool);

    int setupIox(void);

    i2c_settings getiox_mortyDev(void);

    int getBrk1PWR(void);

    int getBrk1UART(void);

    int setMCUPumpPWR(int val);

    int setSplyEn12V(bool val);

    int getPreChg(void);

    int setRTC_RST_N(int val);

    int get32KHZ(void);

    int getIntSQW(void);

    int getBrk0PWR(void);

    int getBrk0UART(void);

    int getHighContactorAux (void);

    int getLowContactorAux (void);

    int setBatt0Disable (bool val);

    int setBatt1Disable (bool val);

    int setBatt2Disable (bool val);

    int setBatt3Disable (bool val);
};

extern iox_morty ioxMorty;
#endif


