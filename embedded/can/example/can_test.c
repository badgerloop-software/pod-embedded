#include "../src/can.h"
#include <stdio.h>

#define NUM_BYTES 4

void rx_test(struct can_frame *can_mesg) {
    if (NEW_CAN_MESSAGE) {
        read_can_message(can_mesg);    // Checks for a CAN message
        printf("ID: %#X || ", can_mesg->can_id);
        printf("Data: [%#X.%#X.%#X.%#X.%#X.%#X.%#X.%#X]\n\r", can_mesg->data[0], can_mesg->data[1], can_mesg->data[2], can_mesg->data[3], can_mesg->data[4], can_mesg->data[5], can_mesg->data[6], can_mesg->data[7]);
        NEW_CAN_MESSAGE = false;
    }
}

void tx_test(uint32_t can_id, uint8_t *data, const int num_bytes) {
    send_can_msg(can_id, data, num_bytes);
}

int main() {
    init_can();
    struct can_frame can_mesg;
    uint8_t data[NUM_BYTES] = {0xDE, 0xAD, 0xBE, 0xEF};
    uint32_t can_id = 0x123;    // Actually must be < 12 bits, but format is 32
    while (1) {
        rx_test(&can_mesg);
        tx_test(can_id, data, NUM_BYTES);
        usleep(500000); // Able to edit freq of control here
    }
    return 0;
}


