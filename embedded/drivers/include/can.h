#ifndef __CAN_H__
#ifdef __cplusplus
#define __CAN_H__

#include <linux/can.h>
#include <sys/socket.h>
#include <net/if.h>
#include <sys/ioctl.h>
#include <sys/time.h>
#include <signal.h>
#include <unistd.h>
#include <stdbool.h>
#include <stdint.h>

#ifdef USE_VCAN
 #define CAN_INTF "vcan0"
#else
 #define CAN_INTF "can0"
#endif
class CAN {
    public:
        struct sockaddr_can addr;
        struct ifreq ifr;
        volatile bool NEW_CAN_MESSAGE;
        int can_sock;
        const struct itimerval new_val;
        CAN(void);
        int initCan();
        void can_rx_irq();
        static void static_can_rx_irq();
        int canRead(struct can_frame *can_mesg);
        int canSend(uint32_t id, uint8_t *data, uint8_t size);
        static CAN instance;
};

/* Potential ideas for a future API */
// bool start_can_read();

// bool stop_can_read();

extern CAN can;
#endif
#endif
