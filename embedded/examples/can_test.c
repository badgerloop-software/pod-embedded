#include <stdio.h>
#include <stdlib.h>

#include "can.h"
#include "rms.h"
#include "data.h"
#include "bms.h"
#define NUM_BYTES 4

data_t *data;

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
    
    if ((data = (data_t *) malloc(sizeof(data_t)))		                 == NULL) { return 1; }
    if ((data->pressure = (pressure_t *) malloc(sizeof(pressure_t)))     == NULL) { return 1; }
    if ((data->motion = (motion_t *)malloc(sizeof(motion_t)))            == NULL) { return 1; }
    if ((data->bms = (bms_t *) malloc(sizeof(bms_t)))     	          	 == NULL) { return 1; }
    if ((data->rms = (rms_t *) malloc(sizeof(rms_t)))      		         == NULL) { return 1; }
    if ((data->flags = (flags_t *) malloc(sizeof(flags_t)))              == NULL) { return 1; }
    
    // Init pressure values to 0
    data->pressure->ps1 = 0;
    data->pressure->ps2 = 0;
    data->pressure->ps3 = 0;
    data->pressure->ps4 = 0;
    data->pressure->sec_ps1 = 0;
    data->pressure->sec_ps2 = 0;
    data->pressure->sec_ps3 = 0;
    data->pressure->sec_ps4 = 0;
    
    // Init motion values to 0
    data->motion->pos = 0;
    data->motion->vel = 0;
    data->motion->accel = 0;
    data->motion->retroCount = 0;
    
    // Init bms values to 0
    data->bms->packCurrent = 0;
    data->bms->packVoltage = 0;
    data->bms->packDCL = 0;
    data->bms->packCCL = 0;
    data->bms->packResistance = 0;
    data->bms->packHealth = 0;
    data->bms->packOpenVoltage = 0;
    data->bms->packCycles = 0;
    data->bms->packAh = 0;
    data->bms->inputVoltage = 0;
    data->bms->Soc = 0;
    data->bms->relayStatus = 0;
    data->bms->highTemp = 0;
    data->bms->lowTemp = 0;
    data->bms->cellMaxVoltage = 0;
    data->bms->cellMinVoltage = 0;
    data->bms->cellAvgVoltage = 0;
    data->bms->maxCells = 0;
    data->bms->numCells = 0;
    
    // Init rms values to 0
    data->rms->igbtTemp = 0;
    data->rms->gateDriverBoardTemp = 0;
    data->rms->controlBoardTemp = 0;
    data->rms->motorTemp = 0;
    data->rms->motorSpeed = 0;
    data->rms->phaseACurrent = 0;
    data->rms->phaseBCurrent = 0;
    data->rms->phaseCCurrent = 0;
    data->rms->dcBusVoltage = 0;
    //data->rms->output_voltage_peak = 0;
    data->rms->lvVoltage = 0;
    data->rms->canCode1 = 0;
    data->rms->canCode2 = 0;
    data->rms->faultCode1 = 0;
    data->rms->faultCode2 = 0;
    data->rms->commandedTorque = 0;
    data->rms->actualTorque = 0;
    data->rms->relayState = 0;
    data->rms->electricalFreq = 0;
    data->rms->dcBusCurrent = 0;
    data->rms->outputVoltageLn = 0;
    
    // Init flags values to 0
    data->flags->readyPump = 0;
    data->flags->pumpDown = 0;
    data->flags->readyCommand = 0;
    data->flags->propulse = 0;
    data->flags->emergencyBrake = 0;
    
    data->state = 0;

    
    
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