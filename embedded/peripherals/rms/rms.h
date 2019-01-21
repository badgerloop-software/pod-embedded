#ifndef _RMS__H__
#define _RMS__H__

#include <stdint.h>

typedef struct {
	uint16_t igbt_temp;
	uint16_t gate_driver_board_temp;
	uint16_t control_board_temp;
	uint16_t motor_temp;
	uint16_t motor_speed;
	uint16_t phase_a_current;
	uint16_t phase_b_current;
	uint16_t phase_c_current;
	uint16_t dc_bus_voltage;
	//uint16_t output_voltage_peak;
	uint16_t lv_voltage;
	uint64_t can_code_1;
	uint64_t can_code_2;
	uint64_t fault_code_1;
	uint64_t fault_code_2;
	uint16_t commanded_torque;
	uint16_t actual_torque;
	uint16_t relay_state;
	uint16_t electrical_freq;
	uint16_t dc_bus_current;
	uint16_t output_voltage_ln;

} Rms;

extern Rms *rms;
void rms_init();
int rms_parser(uint32_t id, uint8_t *data);

#endif 
