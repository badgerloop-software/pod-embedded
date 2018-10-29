#include <linux/can.h>
#include <net/if.h>
#include <sys/ioctl.h>
#include "can.h"

struct sockaddr_can addr;
struct ifreq ifreq;     // Used to look at flags on the network interface

int init_can_connection(struct socket *s) {
    *s = socket(PF_CAN, SOCK_RAW, CAN_RAW);
    strcpy(ifr.ifr_name, "can0");
    
}

int main() {
    
}
