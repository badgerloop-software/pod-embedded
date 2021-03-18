#include <data.h>
#include <unistd.h>
#include <stdint.h>
#include <stdio.h>
#include <stdbool.h>
#include <connStat.h>

#define HB_DELAY 2000000
#define SLP      100000

static bool udpStat;
static bool tcpStat;
static bool tcpStatHV;

bool checkUDPStat() {
    return udpStat;
}

bool checkTCPStat() {
    return tcpStat;
}

bool checkTCPStatHV() {
    return tcpStatHV;
}

void *connStatUDPLoop(void *timestamp) {
    uint64_t *lastPacket = (uint64_t *) timestamp;

    while(1)
    {
    if ((getuSTimestamp() - *lastPacket) > HB_DELAY) {
        udpStat = false;
    } else {
        udpStat = true;
    }
    usleep(SLP);
    }
}

void *connStatTCPLoop(void *timestamp) {
    uint64_t *lastPacket = (uint64_t *) timestamp;

    while (1) {
        if ((getuSTimestamp() - *lastPacket) > HB_DELAY) {
            tcpStat = false;
        } else {
            tcpStat = true;
        }
        usleep(SLP);
    }
}

void *connStatTCPLoopHV(void *timestamp) {
    uint64_t *lastPacket = (uint64_t *) timestamp;

    while (1) {
        if ((getuSTimestamp() - *lastPacket) > HB_DELAY) {
            tcpStatHV = false;
        } else {
            tcpStatHV = true;
        }
        usleep(SLP);
    }
}
