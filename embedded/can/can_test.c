#include "can.h"
#include <stdio.h>

int main() {
    init_can();
    struct can_frame can_mesg;
    while (1) {
        if (NEW_CAN_MESSAGE) {
            read_can_message(&can_mesg);
            printf("CAN data: [%#X , %#X , %#X , %#X , %#X , %#X , %#X , %#X]\n\r", can_mesg.data[0], can_mesg.data[1], can_mesg.data[2], can_mesg.data[3], can_mesg.data[4], can_mesg.data[5], can_mesg.data[6], can_mesg.data[7]);
            NEW_CAN_MESSAGE = false;
        }
        usleep(100000);
    }
    return 0;
}
