#include <linux/can.h>
#include <net/if.h>
#include <sys/ioctl.h>
#include "can.h"
#include <unistd.h>
//#include <linux/interrupt.h>
#include <signal.h>
#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h> 

#define CAN_INTF "vcan0"

struct sockaddr_can addr;
struct ifreq ifr;  // Used to look at flags on the network interface

volatile bool NEW_CAN_MESSAGE = false;

static int can_sock;
static const struct itimerval new_val = {
    {0, 10000}, 
    {0, 10000}
};



void can_rx_irq(){   
    NEW_CAN_MESSAGE = true;
}

int init_can_connection(int *s) {
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

    bind(*s, (struct sockaddr *)&addr, sizeof(addr));
    return 0;
}

int init_can_timer(const struct itimerval *new, struct itimerval *old) {
    struct sigaction *action = malloc(sizeof(struct sigaction));
    void (*canirq)(void) = &can_rx_irq;
    action->sa_handler = (void *)(canirq);
    sigaction(SIGALRM, action, NULL);
    setitimer(ITIMER_REAL, new, old);
    return 0;
}

inline int read_can_message(struct can_frame *recvd_msg) {
    int nBytes = recv(can_sock, recvd_msg, sizeof(struct can_frame), MSG_DONTWAIT);
    if (nBytes < 0) {
        return -1;
    }
    return 0;
}


bool init_can() {
    init_can_connection(&can_sock);
    init_can_timer(&new_val, NULL);
    return true;
}
