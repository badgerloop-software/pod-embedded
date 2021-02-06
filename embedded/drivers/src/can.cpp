#include <linux/can.h>
#include <net/if.h>
#include <sys/ioctl.h>
#include <sys/types.h>
#include "can.h"
#include <unistd.h>
//#include <linux/interrupt.h>
#include <signal.h>
#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h> 



CAN::CAN():new_val({{0, 10000},
                    {0, 10000}})
{
NEW_CAN_MESSAGE = false;
};


static void can_rx_irq(){
    can.NEW_CAN_MESSAGE = true;
}

static int init_can_connection(int *s) {
    *s = socket(PF_CAN, SOCK_RAW, CAN_RAW);
    strcpy(can.ifr.ifr_name, CAN_INTF);
        //printf("Failed to copy bus name into network interface\n\r");
        //return 1;
    
    if (ioctl(*s, SIOCGIFINDEX, &can) == -1) {
        printf("Failed to find bus\n\r");
        return 1;
    }

    can.addr.can_family = AF_CAN;
    can.addr.can_ifindex = can.ifr.ifr_ifindex;

    bind(*s, (struct sockaddr *)&can.addr, sizeof(can.addr));
    return 0;
}

static int init_can_timer(const struct itimerval *updated, struct itimerval *old) {
    signal(SIGALRM, (__sighandler_t)can_rx_irq);
    setitimer(ITIMER_REAL, updated, old);
    return 0;
}

int CAN::canRead(struct can_frame *recvd_msg) {
    int nBytes = recv(can_sock, recvd_msg, sizeof(struct can_frame), MSG_DONTWAIT);
    /* This is actually ok if it fails here, it just means no new info */
    if (nBytes < 0) {
        return 1;
    }
    return 0;
}


int CAN::canSend(uint32_t id, uint8_t *data, uint8_t size) {
    struct can_frame tx_msg;

    tx_msg.can_dlc = size;
    tx_msg.can_id = id;     // Should actually be 11 bits max
    int i;
    for(i = 0; i < size; i++) {
        tx_msg.data[i] = data[i];
    }
    send(can_sock, &tx_msg, sizeof(struct can_frame), MSG_DONTWAIT);

    return 0;   // Not much we do with error codes here
}


int CAN::initCan() {
    if (init_can_connection(&can_sock)) {
        fprintf(stderr, "Failed to init\n\r");
        return 1;
    }
    if (init_can_timer(&new_val, NULL)) {
        fprintf(stderr, "Failed to set read timer\n\r");
        return 1;
    }
    return 0;
}
