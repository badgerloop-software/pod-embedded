#include "i2c.h"
#include "mcp23017.h"
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define LIST 0
#define GET 1
#define SET 2

extern const int NUM_PINS;

void printUsage(void)
{
    printf("USAGE: bloopGpio <CMD> <DEV NUM> <PIN> <VAL>\n");
    printf("\t<CMD>     : \n\t\tlist\n\t\tset\n\t\tget\n");
    printf("\t<DEV NUM> : \n\t\tGPIO Device number, try list to check\n");
    printf("\t<PIN>     : \n\t\tThe pin on the device (0-7 on bank A, 8-15 on bank B)\n");
    printf("\t<VAL>     : \n\t\tUsed with the set command, sets a pin to val, should be 0 or 1\n");
}

void printPin(i2c_settings* dev, uint8_t pin)
{
    int val = getState(dev, pin);
    int dir = getDir(dev, pin);
    if (val == -1 || dir == -1) {
        fprintf(stderr, "Error, exiting\n");
        exit(-1);
    }
    char* bank;
    if (pin < 8) {
        bank = "A";
    } else {
        pin -= 8;
        bank = "B";
    }
    printf("Bank %s, Pin %d\n\tstate = %d\n\tdir = %d\n", bank, pin, val, dir);
}

int parseCommand(char* cmd, int argc)
{
    if (strcmp(cmd, "list") == 0) {
        return LIST;
    } else if (strcmp(cmd, "get") == 0) {
        if (argc != 4 && argc != 3) {
            fprintf(stderr, "Invalid number of arguements for a get command\n");
            printUsage();
            exit(-1);
        }
        return GET;
    } else if (strcmp(cmd, "set") == 0) {
        if (argc != 5) {
            fprintf(stderr, "Invalid number of arguements for a set command\n");
            printUsage();
            exit(-1);
        }
        return SET;
    } else {
        return -1;
    }
}

void printAllDevs(void)
{
    int addr = 0x20;
    FILE* fp = fopen("/dev/null", "w");
    stderr = fp; // So that we dont get killed by I2C errors
    i2c_settings dev;
    for (addr = 0x20; addr < 0x28; addr++) {
        if (setupMCP(&dev, (uint8_t)addr) == 0) {
            // TODO: Add all kinds of helpful info here (states);
            printf("DEV @ %d\n", addr);
        }
    }
}

void printAllPins(i2c_settings* dev)
{
    int pin = 0;
    for (pin = 0; pin < NUM_PINS; pin++) {
        printPin(dev, pin);
    }
}

int main(int argc, char* argv[])
{
    if (argc > 5 || argc <= 1) {
        printUsage();
        exit(-1);
    }

    int cmd = parseCommand(argv[1], argc);
    if (cmd == -1) {
        fprintf(stderr, "Unrecognized command\n");
        printUsage();
        exit(-1);
    }

    if (cmd == LIST) {
        printAllDevs();
        return 0;
    }

    int addr = atoi(argv[2]);

    if (addr < 0x20 || addr > 0x27) {
        fprintf(stderr, "Invalid address for MCP device, run bloopGpio list\n");
    }

    i2c_settings dev;
    if (setupMCP(&dev, addr) != 0) {
        fprintf(stderr, "Error setting up device, exiting\n");
        exit(-1);
    }

    if (cmd == GET) {
        if (argc < 4) {
            printAllPins(&dev);
        } else {
            int pin = atoi(argv[3]);
            printPin(&dev, pin);
        }
    } else if (cmd == SET) {
        int pin = atoi(argv[3]);
        int val = atoi(argv[4]);
        int ret = setState(&dev, pin, val);
        if (ret == -1) {
            fprintf(stderr, "Error, exiting\n");
            exit(-1);
        }
        printf("Success\n");
    }
    return 0;
}
