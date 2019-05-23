#include <stdio.h>
#include <stdint.h>
#include <rms.h>
#include <data.h>
#include <can.h>
#include <errno.h>
#include <stdlib.h>
#include <pthread.h>
#include <string.h>


void *listenForResponse(void *addr);

const char *RMS_CAN_CODES_URL =
    "https://app.box.com/s/4fb49r9p6lzfz4uwcb5izkxpcwh768vc";

void printUsage() {
    printf("Usage: ./rmsProg [w|r] [parameter ID] [value]\n\n");
    printf("Help:\n\tSelect either (w)rite or (r)ead ");
    printf("then enter the parameter address found here: \n\t\t%s)\n", RMS_CAN_CODES_URL);
    printf("\n\tIf you are writing, add a value, otherwise ignore the last parameter\n");
}


uint16_t safeConvert(char *input) {
    errno = 0;
    uint16_t ret = strtol(input, NULL, 0);
    if (errno != 0 && ret == 0) {
        fprintf(stderr, "Conversion error, exiting...\n");
        exit(1);
    }
    return ret;
}

int confirmInput(uint16_t addr, int val) {
    printf("You are about to write (in decimal) value: %d to address: %d\n", val, addr);
    printf("Is this what you intended? (y/n)\n");
    char c = getchar();
    return c == 'y';
}

static bool writeEeprom = false;

void *listenForResponse(void *arg) {
    uint16_t addr = *((uint16_t*) arg);
    uint16_t val;
    while(1) {
        struct can_frame canMesg;
        if (!canRead(&canMesg)) {
            if (canMesg.can_id == 0xC2) {
                if ((val = rmsCmdResponseParse(canMesg.data, addr, writeEeprom)) != -1) {
                    if (writeEeprom) {
                        printf("Success writing\n");
                    } else {
                        printf("Parameter at address: %d == %#X\n", addr, val);
                    }
                    return NULL;
                }
            }
            NEW_CAN_MESSAGE = false;
        }
        usleep(1000);
    }
    return NULL;
}

int main(int argc, char *argv[]) {
    if (argc < 3) {
        printUsage();
        return 0;
    }
    initCan();
    initData();

    if (!strcmp(argv[1], "w")) {
        writeEeprom = true;
    } else if (!strcmp(argv[1], "r")) {
        writeEeprom = false;
    } else {
        printf("Please either try to (w)rite or (r)ead\n");
        printUsage();
    }

    uint16_t addr = safeConvert(argv[2]);
    pthread_t canRxThread;
    pthread_create(&canRxThread, NULL, &listenForResponse, (void *)&addr);
    if (writeEeprom) {
        if (argc < 4) {
            printf("Please enter a parameter address and a value\n");
            printUsage();
        }
        uint16_t val = safeConvert(argv[3]);

        if (confirmInput(addr, (int)val))
            rmsWriteEeprom(addr, val);
        else {
            printf("Aborting...\n");
            exit(1);
        }
    } else {
        printf("Reading address: %d\n", addr);
        rmsReadEeprom(addr);
    }
    pthread_join(canRxThread, NULL);
}
