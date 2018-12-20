#include "can.h"
#include <stdio.h>

int main() {
    init_can();
    struct can_frame can_mesg;
    uint8_t data[4] = {0xDE, 0xAD, 0xBE, 0xEF};
    while (1) {
/*        if (NEW_CAN_MESSAGE) {
            read_can_message(&can_mesg);
            printf("ID: %#X || ", can_mesg.can_id);
            printf("Data: [%#X.%#X.%#X.%#X.%#X.%#X.%#X.%#X]\n\r", can_mesg.data[0], can_mesg.data[1], can_mesg.data[2], can_mesg.data[3], can_mesg.data[4], can_mesg.data[5], can_mesg.data[6], can_mesg.data[7]);
            NEW_CAN_MESSAGE = false;
        }*/
        send_can_msg(0x123, data, 4);
        usleep(500000);
    }
    return 0;
}
