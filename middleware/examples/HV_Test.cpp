// Includes
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <time.h>

#include "HVTelemetry_Loop.h"
#include "HVTCPSocket.h"
#include "HV_Telem_Recv.h"
#include "data.h"

extern "C" 
{
    #include "can.h"
}

data_t *data;

int init() {
	// 1. init all drivers
    
    init_can();
	
    // Init Data struct
	if ((data = (data_t *) malloc(sizeof(data_t)))		                 	 == NULL) { return 1; }
	if ((data->pressure = (pressure_t *) malloc(sizeof(pressure_t)))         == NULL) { return 1; }
	if ((data->motion = (motion_t *)malloc(sizeof(motion_t)))                == NULL) { return 1; }
	if ((data->bms = (bms_t *) malloc(sizeof(bms_t)))     	          		 == NULL) { return 1; }
	if ((data->rms = (rms_t *) malloc(sizeof(rms_t)))      		       	     == NULL) { return 1; }
	if ((data->flags = (flags_t *) malloc(sizeof(flags_t)))                  == NULL) { return 1; }
	if ((data->timers = (timers_t *) malloc(sizeof(timers_t)))               == NULL) { return 1; }
	
	// Init pressure values to 0
	data->pressure->ps1 = 0;
	data->pressure->ps2 = 0;
	data->pressure->ps3 = 0;
	data->pressure->ps4 = 0;
	data->pressure->sec_ps1 = 0;
	data->pressure->sec_ps2 = 0;
	data->pressure->sec_ps3 = 0;
	data->pressure->sec_ps4 = 0;
	data->pressure->pv = 0;
	
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
	
	// Init initial timer
	data->timers->startTime = time(NULL);
	
	data->state = 0;
	
    SetupHVTelemetry((char *) "192.168.1.112", 33333);
	SetupHVTCPServer();
	SetupHVTelemRecv();	
    return 0;	
}

int main() {
	/* Create the big data structures to house pod data */
	
	if (init() == 1) {
		printf("Error in initialization! Exiting...\r\n");
		exit(1);
	}

	while(1) {
        usleep(100000);
		// Control loop
	}
    return 0;
}
