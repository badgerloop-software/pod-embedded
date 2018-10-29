#include <linux/can.h>
#include <net/if.h>
#include <sys/ioctl.h>
#include "can.h"

struct sockaddr_can addr;
struct ifreq ifreq;  // Used to look at flags on the network interface

int init_can_connection(struct socket *s) {
    *s = socket(PF_CAN, SOCK_RAW, CAN_RAW);
    if (strcpy(ifr.ifr_name, "can0")) {
        printf("Failed to copy bus name into network interface\n\r");
        exit(1);
    }
    if (ioctl(*s, SIOCGIFINDEX, &ifr) == -1) {
        printf("Failed to find bus\n\r");
        exit(1);
    }

    addr.can_family = AF_CAN;
    addr.can_ifindex = ifr.ifr_ifindex;
}

int main() {
    
}
