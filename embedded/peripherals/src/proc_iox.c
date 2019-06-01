#include <stdio.h>
#include <stdint.h>
#include <data.h>
#include <i2c.h>
#include <mcp23017.h>
#include <proc_iox.h>

#define PROC_IO_ADDR 0x20

static i2c_settings iox;
static int setupIox();


int initProcIox() {
    setupMCP(&iox, PROC_IO_ADDR);
    clearSettingsMCP(&iox);
    setupIox();
    return 0;
}

i2c_settings getProcIoxDev() {
	return iox;
}

static int setupIox() {
    /* Debug pins, set B1 high for optional testing */
    setDir(&iox, MCP_GPIOB_0, MCP_DIR_OUT);
    setDir(&iox, MCP_GPIOB_1, MCP_DIR_OUT);
    setState(&iox, MCP_GPIOB_0, 0);
    setState(&iox, MCP_GPIOB_1, 1);

    /* Debug LEDs */
    setDir(&iox, MCP_GPIOB_4, MCP_DIR_OUT);
    setDir(&iox, MCP_GPIOB_5, MCP_DIR_OUT);
    setDir(&iox, MCP_GPIOB_6, MCP_DIR_OUT);
    setDir(&iox, MCP_GPIOB_7, MCP_DIR_OUT);
    setState(&iox, MCP_GPIOB_4, 0);
    setState(&iox, MCP_GPIOB_5, 0);
    setState(&iox, MCP_GPIOB_6, 0);
    setState(&iox, MCP_GPIOB_7, 0);
    
    
    /* Random GPIO, may or may not be connected */
    /* Also dont know if in/out so we'll leave them as
     * in for now */
    setDir(&iox, MCP_GPIOA_0, MCP_DIR_IN);
    setDir(&iox, MCP_GPIOA_1, MCP_DIR_IN);
    setDir(&iox, MCP_GPIOA_2, MCP_DIR_IN);
    setDir(&iox, MCP_GPIOA_3, MCP_DIR_IN);
    setDir(&iox, MCP_GPIOA_4, MCP_DIR_IN);
    setDir(&iox, MCP_GPIOA_5, MCP_DIR_IN);
    setDir(&iox, MCP_GPIOA_6, MCP_DIR_IN);
    setDir(&iox, MCP_GPIOA_7, MCP_DIR_IN);

    return 0;   /* TODO add error checking */
}

int procIoxLedSet(int led, bool val) {
    if (led != DEBUG_LED_0 && led != DEBUG_LED_1
            && led != DEBUG_LED_2 && led != DEBUG_LED_3) {
        fprintf(stderr, "Invalid debug LED\n");
        return -1;
    }
    return setState(&iox, led, val);
}

