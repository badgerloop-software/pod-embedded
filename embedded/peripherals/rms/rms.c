#include <stdio.h>
#include <stdlib.h>
#include "rms.h"

Rms *rms;

void rms_init() {
	rms = malloc(sizeof(Rms));
	rms->igbt_temp = 0;
	rms->gate_driver_board_temp = 0;
	rms->control_board_temp = 0;
	rms->motor_temp = 0;
	rms->motor_speed = 0;
	rms->phase_a_current = 0;
	rms->phase_b_current = 0;
	rms->phase_c_current = 0;	
	rms->dc_bus_voltage=0;
	//rms->output_voltage_peak = 0;
	rms->lv_voltage = 0;
	rms->can_code_1 = 0;
	rms->can_code_2 = 0;
	rms->fault_code_1 = 0;
	rms->fault_code_2 = 0;
	rms->commanded_torque = 0;
	rms->actual_torque = 0;
	rms->relay_state = 0;	
	rms->electrical_freq = 0;
	rms->dc_bus_current = 0;
	rms->output_voltage_ln = 0;
}

int rms_parser(uint32_t id, uint8_t *data){
	switch(id){
		case (0xa0):
			rms->igbt_temp = (data[0] | (data[1] <<8)) / 10; //Deg C
			rms->gate_driver_board_temp = (data[6] | (data[7] << 8)) / 10; //Deg C
			//printf("IGBT: %d\r\n", rms->igbt_temp);
			//printf("Gate Driver Board Temp: %d\r\n", rms->gate_driver_board_temp);
			break;
		case (0xa1):
			rms->control_board_temp = (data[0] | (data[1] << 8)) / 10; // Deg C
			//printf("Control Board Temp: %d\r\n", rms->control_board_temp);
			break;
		case (0xa2):
			rms->motor_temp = (data[4] | (data[5] << 8)) / 10; //Deg C
			//printf("Motor Temp: %d\r\n", rms->motor_temp);
			break;
		case (0xa3):
			break;
		case (0xa4): 
			break;
		case (0xa5):
			rms->motor_speed = (data[2] | (data[3] << 8)); // RPM
			rms->electrical_freq = (data[4] | (data[5] << 8 )) / 10; //electrical frequency Hz
			//printf("Motor Speed: %d\r\n", rms->motor_speed);
			//printf("Elect. Freq: %d\r\n", rms->electrical_freq);
			break;
		case (0xa6):
			rms->phase_a_current = (data[0] | (data[1] << 8)) / 10; // Phase A current
			rms->dc_bus_current = (data[6] | (data[7] << 8)) / 10; //DC Bus current
			//printf("Phase A Current: %d\r\n", rms->phase_a_current);
			//printf("Phase B Current: %d\r\n", rms->phase_b_current);
			break;
		case (0xa7):
			rms->dc_bus_voltage = (data[0] | (data[1] << 8))/10; //DC Bus voltage
			rms->output_voltage_ln = (data[2] | (data[3] << 8)) / 10; //Voltage line to netural 
			//printf("DC Bus Voltage: %d\r\n", rms->dc_bus_voltage);
			//printf("Output Voltage line: %d\r\n", rms->output_voltage_ln);
			break;
		case (0xa8):
			break;
		case (0xa9):
			rms->lv_voltage = (data[6] | (data[7] << 8)) / 100;
			//printf("LV Voltage: %d\r\n", rms->lv_voltage);
			break;

		case (0xaa):
			rms->can_code_1 = (data[3] << 24) | (data[2] << 16) | (data[1] << 8) | data[0];
			rms->can_code_2 = (data[7] << 24) | (data[6] << 16) | (data[5] << 8) | data[4];
			//printf("CAN Code 1: %lld\r\n", rms->can_code_1);
			//printf("CAN Code 2: %lld\r\n", rms->can_code_2);
			break;
		case (0xab):
			rms->fault_code_1 = (data[3] << 24) | (data[2] << 16) | (data[1] << 8) | data[0];
			rms->fault_code_2 = (data[7] << 24) | (data[6] << 16) | (data[5] << 8) | data[4];
			//printf("Fault Code 1: %lld\r\n", rms->fault_code_1);
			//printf("Fault Code 2: %lld\r\n", rms->fault_code_2);
			break;
		case (0xac):
			rms->commanded_torque = (data[0] | (data[1] << 8)) /10;
			rms->actual_torque = (data[2] | (data[3] << 8)) / 10;
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
