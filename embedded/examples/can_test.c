#include "can.h"
#include "rms.h"
#include <stdio.h>
#include "data.h"
#include "bms.h"
#define NUM_BYTES 4

extern data_t *data;

void rx_test(struct can_frame *can_mesg) {
        if(!readCanMsg(can_mesg)){ // Checks for a CAN message
            printf("ID: %#X || ", (unsigned int) can_mesg->can_id);
            printf("Data: [%#X.%#X.%#X.%#X.%#X.%#X.%#X.%#X]\n\r", can_mesg->data[0], can_mesg->data[1], can_mesg->data[2], can_mesg->data[3], can_mesg->data[4], can_mesg->data[5], can_mesg->data[6], can_mesg->data[7]);
            
            if(rms_parser(can_mesg->can_id, can_mesg->data)){
                printf("RMS Data parsed successfully\n");
            }
        
            if(bmsParseMsg(can_mesg->can_id, can_mesg->data)){
                printf("BMS Data parsed successfully\n");
                bmsDump();
            
        }
        NEW_CAN_MESSAGE = false;
    }
}

void tx_test(uint32_t can_id, uint8_t *data, const int num_bytes) {
    sendCanMsg(can_id, data, num_bytes);
}



int main() {
    initCan();
    rms_init();
    bmsInit();
    
    struct can_frame can_mesg;
    //uint8_t data[NUM_BYTES] = {0xDE, 0xAD, 0xBE, 0xEF};
    //uint32_t can_id = 0x123;    // Actually must be < 12 bits, but format is 32
    while (1) {
        rx_test(&can_mesg);
 //       tx_test(can_id, data, NUM_BYTES);
  //      usleep(500000); // Able to edit freq of control here
    }
    return 0;
}