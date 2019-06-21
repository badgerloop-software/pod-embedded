#include <stdio.h>
#include <stdint.h>
#include <stdbool.h>
#include <data.h>
#include <i2c.h>
#include <mcp23017.h>
#include <proc_iox.h>
#include <lv_iox.h>

#define LV_IO_ADDR   0x21

static i2c_settings iox;
static int setupIox();


int initLVIox(bool hardStart) {
    if (setupMCP(&iox, LV_IO_ADDR) != 0) return -1;
    if (hardStart) {
        if (clearSettingsMCP(&iox) != 0) return -1;
        if (setupIox() != 0) return -1;
    }
    return 0;
}

/***
 *  initLVIox - Meant to be overly pedantic
 *      even if the values arent changing,
 *      just to make extremely clear the initial
 *      state of everything.
 *
 */
static int setupIox() {
    /* Limit switch inputs */
    int ret = 0;
    ret += setDir(&iox, MCP_GPIOA_0, MCP_DIR_IN);
    ret += setDir(&iox, MCP_GPIOA_1, MCP_DIR_IN);
    ret += setDir(&iox, MCP_GPIOA_2, MCP_DIR_IN);
    ret += setDir(&iox, MCP_GPIOA_3, MCP_DIR_IN);
    
    /* Solenoid outputs */
    ret += setDir(&iox, MCP_GPIOB_0, MCP_DIR_OUT);
    ret += setDir(&iox, MCP_GPIOB_1, MCP_DIR_OUT);
    ret += setDir(&iox, MCP_GPIOB_2, MCP_DIR_OUT);
    ret += setDir(&iox, MCP_GPIOB_3, MCP_DIR_OUT);
    ret += setDir(&iox, MCP_GPIOB_4, MCP_DIR_OUT);
    ret += setDir(&iox, MCP_GPIOB_5, MCP_DIR_OUT);
    ret += setDir(&iox, MCP_GPIOB_6, MCP_DIR_OUT);
    ret += setDir(&iox, MCP_GPIOB_7, MCP_DIR_OUT);
    ret += setState(&iox, MCP_GPIOB_0, 0); 
    ret += setState(&iox, MCP_GPIOB_1, 0); 
    ret += setState(&iox, MCP_GPIOB_2, 0); 
    ret += setState(&iox, MCP_GPIOB_3, 0); 
    ret += setState(&iox, MCP_GPIOB_4, 0); 
    ret += setState(&iox, MCP_GPIOB_5, 0); 
    ret += setState(&iox, MCP_GPIOB_6, 0); 
    ret += setState(&iox, MCP_GPIOB_7, 0); 
    if (ret != 0) fprintf(stderr, "Error initializing LV IOX\n");
    return ret; /* TODO Add error checking */
}

int limSwitchGet(int limSwitch) {
    if (limSwitch != LIM_SWITCH_0 &&
            limSwitch != LIM_SWITCH_1 &&
            limSwitch != LIM_SWITCH_2 &&
            limSwitch != LIM_SWITCH_3
       ) {
        fprintf(stderr, "Invalid Limit Switch\n");
        return -1;
    }
    return getState(&iox, limSwitch);
}

int solenoidSet(int solenoid, bool val) { 
    if (solenoid != SOLENOID_0 && solenoid != SOLENOID_1
            && solenoid != SOLENOID_2 && solenoid != SOLENOID_3
            && solenoid != SOLENOID_4 && solenoid != SOLENOID_5
            && solenoid != SOLENOID_6 && solenoid != SOLENOID_7
       ) {
        fprintf(stderr, "Invalid solenoid\n");
        return -1;
    }
    return setState(&iox, solenoid, val);
}

