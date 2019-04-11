#include <stdio.h>
#include <stdlib.h>
#include "rms.h"
#include "data.h"

extern data_t *data;

/* RMS Init Function:
 *      Creates RMS data struct
 *      Sets each value = 0
 */
void rms_init() {
	data->rms->igbtTemp = 0;
	data->rms->gateDriverBoardTemp = 0;
	data->rms->controlBoardTemp = 0;
	data->rms->motorTemp = 0;
	data->rms->motorSpeed = 0;
	data->rms->phaseACurrent = 0;
	data->rms->phaseBCurrent = 0;
	data->rms->phaseCCurrent = 0;	
	data->rms->dcBusVoltage=0;
	//rms->output_voltage_peak = 0;
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
}

/* RMS CAN Parser Function
 *      Based on the CAN ID passed, parsing out the data bytes into
 *      their respective values in the RMS data struct
 */
int rms_parser(uint32_t id, uint8_t *data){
	switch(id){
		case (0xa0):
			data->rms->igbtTemp = (data[0] | (data[1] <<8)) / 10; //Deg C
			data->rms->gateDriverBoardTemp = (data[6] | (data[7] << 8)) / 10; //Deg C
			//printf("IGBT: %d\r\n", rms->igbt_temp);
			//printf("Gate Driver Board Temp: %d\r\n", rms->gate_driver_board_temp);
			break;
		case (0xa1):
			rms->controlBoardTemp = (data[0] | (data[1] << 8)) / 10; // Deg C
			//printf("Control Board Temp: %d\r\n", rms->control_board_temp);
			break;
		case (0xa2):
			data->rms->motorTemp = (data[4] | (data[5] << 8)) / 10; //Deg C
			//printf("Motor Temp: %d\r\n", rms->motor_temp);
			break;
		case (0xa3):
			break;
		case (0xa4): 
			break;
		case (0xa5):
			data->rms->motorSpeed = (data[2] | (data[3] << 8)); // RPM
			data->rms->electricalFreq = (data[4] | (data[5] << 8 )) / 10; //electrical frequency Hz
			//printf("Motor Speed: %d\r\n", rms->motor_speed);
			//printf("Elect. Freq: %d\r\n", rms->electrical_freq);
			break;
		case (0xa6):
			data->rms->phaseACurrent = (data[0] | (data[1] << 8)) / 10; // Phase A current
			data->rms->dcBusCurrent = (data[6] | (data[7] << 8)) / 10; //DC Bus current
			//printf("Phase A Current: %d\r\n", rms->phase_a_current);
			//printf("Phase B Current: %d\r\n", rms->phase_b_current);
			break;
		case (0xa7):
			data->rms->dcBusVoltage = (data[0] | (data[1] << 8))/10; //DC Bus voltage
			data->rms->outputVoltageLn = (data[2] | (data[3] << 8)) / 10; //Voltage line to netural 
			//printf("DC Bus Voltage: %d\r\n", rms->dc_bus_voltage);
			//printf("Output Voltage line: %d\r\n", rms->output_voltage_ln);
			break;
		case (0xa8):
			break;
		case (0xa9):
			data->rms->lvVoltage = (data[6] | (data[7] << 8)) / 100;
			//printf("LV Voltage: %d\r\n", rms->lv_voltage);
			break;

		case (0xaa):
			data->rms->canCode1 = (data[3] << 24) | (data[2] << 16) | (data[1] << 8) | data[0];
			data->rms->canCode2 = (data[7] << 24) | (data[6] << 16) | (data[5] << 8) | data[4];
			//printf("CAN Code 1: %lld\r\n", rms->can_code_1);
			//printf("CAN Code 2: %lld\r\n", rms->can_code_2);
			break;
		case (0xab):
			data->rms->faultCode1 = (data[3] << 24) | (data[2] << 16) | (data[1] << 8) | data[0];
			data->rms->faultCode2 = (data[7] << 24) | (data[6] << 16) | (data[5] << 8) | data[4];
			//printf("Fault Code 1: %lld\r\n", rms->fault_code_1);
			//printf("Fault Code 2: %lld\r\n", rms->fault_code_2);
			break;
		case (0xac):
			data->rms->commandedTorque = (data[0] | (data[1] << 8)) /10;
			data->rms->actualTorque = (data[2] | (data[3] << 8)) / 10;
			//printf("Commanded Torque: %d\r\n", rms->commanded_torque);
			//printf("Actual Torque: %d\r\n", rms->actual_torque);
			break;
		case (0xad):
			break;
		case (0xae):
			break;
		case (0xaf): 
			break;
		default: 
			//printf("Unknown RMS CAN ID: %lx\r\n", id);
			return 0;
	}
	return 1;

}
