#include <data.h>
#include <unistd.h>
#include <stdint.h>
#include <stdio.h>
#include <connStat.h>

#define HB_DELAY 500000
#define SLP      100000






void *connStatLoop(void *timestamp) {
    uint64_t *lastPacket = (uint64_t *) timestamp;

    while (1) {
        if ((*lastPacket - getuSTimestamp()) > HB_DELAY) {
            data->flags->isConnected = false;
        } else {
            data->flags->isConnected = true;
        }
        usleep(SLP);
    }
}
