#include <stdio.h>
#include <stdlib.h>
#include "can.h"
#include "rms.h"

extern "C"
{
#include "data.h"
#include "bms.h"
}

#define NUM_BYTES 4
#define TEST_LEN 10
RMS rms;
CAN can;
int msgRecv = 0;

void rx_test(struct can_frame *can_mesg)
{
    if (!can.canRead(can_mesg))
    { // Checks for a CAN message
        printf("ID: %#X || ", (unsigned int)can_mesg->can_id);
        printf("Data: [%#X.%#X.%#X.%#X.%#X.%#X.%#X.%#X]\n\r", can_mesg->data[0], can_mesg->data[1], can_mesg->data[2], can_mesg->data[3], can_mesg->data[4], can_mesg->data[5], can_mesg->data[6], can_mesg->data[7]);
        bool validRMSMesg = false;
        if (rms.rms_parser(can_mesg->can_id, can_mesg->data, NO_FILTER))
        {
            printf("RMS Data parsed successfully\n");
            validRMSMesg = true;
            msgRecv = 1;
        }

        if (!validRMSMesg && bmsParseMsg(can_mesg->can_id, can_mesg->data))
        {
            printf("BMS Data parsed successfully\n");
            bmsDump();
            msgRecv = 1;
        }

        can.NEW_CAN_MESSAGE = false;
    }
}

void txTest()
{
    rms.rmsEnHeartbeat();
    rms.rmsClrFaults();
    rms.rmsInvDis();
    rms.rmsInvEn();
    rms.rmsInvForward20();
    rms.rmsInvForward30();
    rms.rmsCmdNoTorque();
    rms.rmsDischarge();
}

int main(int argc, char *argv[])
{
    initData();
    can.initCan();
    struct can_frame can_mesg;
    txTest(); // test RMS Commands
    printf("---Begin %d second CAN test---\n", TEST_LEN);
    unsigned long currTime = (unsigned long)time(NULL);
    int timeout = TEST_LEN + currTime;
    while (currTime < timeout)
    {
        rx_test(&can_mesg);
        //      usleep(500000); // Able to edit freq of control here
        currTime = (unsigned long)time(NULL);
    }
    printf("---End CAN Test---\n");
    if (msgRecv != 1)
        exit(-1);
    return 0;
}
