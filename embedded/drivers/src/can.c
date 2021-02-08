#include "can.h"
#include <linux/can.h>
#include <net/if.h>
#include <sys/ioctl.h>
#include <sys/types.h>
#include <unistd.h>
//#include <linux/interrupt.h>
#include <signal.h>
#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define NOCAN

static struct sockaddr_can addr;
static struct ifreq ifr; // Used to look at flags on the network interface

volatile bool NEW_CAN_MESSAGE = false;

static int can_sock;
static const struct itimerval new_val = {
    { 0, 10000 },
    { 0, 10000 }
};

void can_rx_irq()
{
    NEW_CAN_MESSAGE = true;
}

static int init_can_connection(int* s)
{
    #ifdef NOCAN
    return 0;
    #endif
    *s = socket(PF_CAN, SOCK_RAW, CAN_RAW);
    strcpy(ifr.ifr_name, CAN_INTF);
    //printf("Failed to copy bus name into network interface\n\r");
    //return 1;
    
    if (ioctl(*s, SIOCGIFINDEX, &ifr) == -1) {
        printf("Failed to find bus\n\r");
        return 1;
    }



    addr.can_family = AF_CAN;
    addr.can_ifindex = ifr.ifr_ifindex;

    bind(*s, (struct sockaddr*)&addr, sizeof(addr));
    return 0;
}

static int init_can_timer(const struct itimerval* new, struct itimerval* old)
{
    struct sigaction* action = malloc(sizeof(struct sigaction));
    void (*canirq)(void) = &can_rx_irq;
    action->sa_handler = (void*)(canirq);
    sigaction(SIGALRM, action, NULL);
    setitimer(ITIMER_REAL, new, old);
    return 0;
}

inline int canRead(struct can_frame* recvd_msg)
{
    #ifdef NOCAN
    return 0;
    #endif
    int nBytes = recv(can_sock, recvd_msg, sizeof(struct can_frame), MSG_DONTWAIT);
    /* This is actually ok if it fails here, it just means no new info */
    if (nBytes < 0) {
        return 1;
    }
    return 0;
}

inline int canSend(uint32_t id, uint8_t* data, uint8_t size)
{
    #ifdef NOCAN
    return 0;
    #endif
    struct can_frame tx_msg;

    tx_msg.can_dlc = size;
    tx_msg.can_id = id; // Should actually be 11 bits max
    int i;
    for (i = 0; i < size; i++) {
        tx_msg.data[i] = data[i];
    }
    send(can_sock, &tx_msg, sizeof(struct can_frame), MSG_DONTWAIT);

    return 0; // Not much we do with error codes here
}

int initCan()
{
    #ifdef NOCAN
    return 0;
    #endif
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
