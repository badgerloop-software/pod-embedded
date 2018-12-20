#ifndef __CAN_H__
#define __CAN_H__

#include <linux/can.h>
#include <net/if.h>
#include <sys/ioctl.h>
#include <sys/time.h>
#include <signal.h>
#include <unistd.h>
#include <stdbool.h>


#define CAN_INTF "vcan0"

extern volatile bool NEW_CAN_MESSAGE;

bool init_can();

int read_can_message(struct can_frame *can_mesg);
/* Potential ideas for a future API */
// bool start_can_read();

// bool stop_can_read();

#endif
