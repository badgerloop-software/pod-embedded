#include <linux/can.h>
#include <net/if.h>
#include <sys/ioctl.h>
#include "can.h"
#include <unistd.h>
#include <linux/interrupt.h>


struct sockaddr_can addr;
struct ifreq ifreq;  // Used to look at flags on the network interface

int init_can_connection(struct socket *s) {
    *s = socket(PF_CAN, SOCK_RAW, CAN_RAW);
    if (strcpy(ifr.ifr_name, "can0")) {
        printf("Failed to copy bus name into network interface\n\r");
        return 1;
    }
    if (ioctl(*s, SIOCGIFINDEX, &ifr) == -1) {
        printf("Failed to find bus\n\r");
        return 1;
    }

    addr.can_family = AF_CAN;
    addr.can_ifindex = ifr.ifr_ifindex;
    
    return 0;
}

static int can_rx_irq(){

}

int read_can_message(struct socket *s) {
    struct can_frame recvd_msg;
    uint8_t can_data[8];
    int nBytes = read(*s, &rcvd_msg, sizeof(struct can_frame));

    if (nBytes < 0) {
        printf("Failed to receive data\n\r");
        return 1;
    }
    can_data = recvd_msg.data;
    request_irq
    
    
    return 0;
}

int main() {
    struct socket can_sock;
    
    init_can_connection(&can_sock);
    while (1) {
        if 
        
        usleep(100000);  // Try to maintain 10 Hz for now

    }
}
