#include <linux/can.h>
#include <net/if.h>
#include <sys/ioctl.h>
#include "can.h"
#include <unistd.h>
//#include <linux/interrupt.h>
#include <sys/time.h>
#include <signal.h>
#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h> 

#define CAN_INTF "vcan0"

struct sockaddr_can addr;
struct ifreq ifr;  // Used to look at flags on the network interface
volatile bool CAN_MESSAGE = false;
volatile uint32_t CURR_CAN_ID = 0x00;
int can_sock;

int read_can_message();

void can_rx_irq(){   
    printf("fizz\n\r");
    printf("CAN RET CODE: %d\n", read_can_message());
    CAN_MESSAGE = true;
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


int read_can_message() {
    struct can_frame recvd_msg;
    uint8_t can_data[8];
    int nBytes = recv(can_sock, &recvd_msg, sizeof(struct can_frame), MSG_DONTWAIT);
    //int nBytes = read(can_sock, &recvd_msg, sizeof(struct can_frame));
    fprintf(stderr, "num bytes: %d", nBytes);
    if (nBytes < 0) {
        printf("Failed to receive data\n\r");
        return 1;
    }

    CURR_CAN_ID = recvd_msg.can_id;
    can_data[0] = recvd_msg.data[0];
    
    return 0;
}


int main() {
    const struct itimerval new_val = {
        {0, 10000}, {0, 10000}
    };
   // struct itimerval old_val = {
   //     {0, 200000}, {0, 200000}
   // };

    init_can_connection(&can_sock);
    init_can_timer(&new_val, NULL);
    
    while (1) {
        if (CAN_MESSAGE) {
            printf("CAN ID: %#X \n\r", CURR_CAN_ID);
            CAN_MESSAGE = false;
        }
        
        usleep(100000);  // Try to maintain 10 Hz for now

    }
}
