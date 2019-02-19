// Includes
#include "state_machine.h"
#include "HVTelemetry_Loop.h"
#include "HVTCPSocket.h"
#include "data.h"
#include <stdio.h>
#include <stdlib.h>
data_t *data;

extern void SetupHVTelemetry(char *ip, int port);
extern void SetupHVTCPServer(void);

int init() {
	// 1. init all drivers
	
	// 2. init all peripherals
	
	if ((data = malloc(sizeof(data_t)))		            == NULL) { STATE_ERROR(); }
	if ((data->pressure = malloc(sizeof(pressure_t)))   == NULL) { STATE_ERROR(); }
	if ((data->motion = malloc(sizeof(motion_t)))       == NULL) { STATE_ERROR(); }
	if ((data->bms = malloc(sizeof(bms_t)))     		== NULL) { STATE_ERROR(); }
	if ((data->rms = malloc(sizeof(rms_t)))      		== NULL) { STATE_ERROR(); }
	if ((data->flags = malloc(sizeof(flags_t)))         == NULL) { STATE_ERROR(); }
	
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
	data->motion->retro_count = 0;
	
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
	data->rms->igbt_temp = 0;
	data->rms->gate_driver_board_temp = 0;
	data->rms->control_board_temp = 0;
	data->rms->motor_temp = 0;
	data->rms->motor_speed = 0;
	data->rms->phase_a_current = 0;
	data->rms->phase_b_current = 0;
	data->rms->phase_c_current = 0;
	data->rms->dc_bus_voltage = 0;
	//data->rms->output_voltage_peak = 0;
	data->rms->lv_voltage = 0;
	data->rms->can_code_1 = 0;
	data->rms->can_code_2 = 0;
	data->rms->fault_code_1 = 0;
	data->rms->fault_code_2 = 0;
	data->rms->commanded_torque = 0;
	data->rms->actual_torque = 0;
	data->rms->relay_state = 0;
	data->rms->electrical_freq = 0;
	data->rms->dc_bus_current = 0;
	data->rms->output_voltage_ln = 0;
	
	// Init flags values to 0
	data->flags->readyPump = 0;
	data->flags->pumpDown = 0;
	data->flags->readyCommand = 0;
	data->flags->propulse = 0;
	data->flags->emergencyBreak = 0;
	
	
//	SetupHVTelemetry("192.168.1.112", 33333);
//	SetupHVTCPServer();	
    return 0;	
}

int main() {
	/* Create the big data structures to house pod data */
	
	if (init() == 1) {
		printf("Error in initialization! Exiting...\r\n");
		exit(1);
	}
	printf("Now this is pod racing\n");
	while(1) {
	    break;
		// Control Loop!		

	}
    return 0;
}
