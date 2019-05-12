#ifndef STATES_H
#define STATES_H


/* Pressure sensor acceptable limits (in PSI) */
#define PS1_BOTTOM_LIMIT        1000
#define PS1_TOP_LIMIT           3000
#define PS2_BOTTOM_LIMIT        150
#define PS2_TOP_LIMIT           275
#define PS3_BOTTOM_LIMIT        1000
#define PS3_TOP_LIMIT           3000
#define PS4_BOTTOM_LIMIT        0
#define PS4_TOP_LIMIT           20

#define PS1_BOTTOM_LIMIT_CRAWLPOSTRUN 0
#define PS2_BOTTOM_LIMIT_CRAWLPOSTRUN 0
#define PS3_BOTTOM_LIMIT_CRAWLPOSTRUN 0

/* Battery Acceptable Limits */
#define MAX_BATT_TEMP_PRERUN		33	/* Degrees Celcius */
#define MAX_BATT_TEMP_RUN			48	/* Degrees Celcius */

#define MAX_BATT_CURRENT_STILL		0
#define MAX_BATT_CURRENT_MOVING		265

/* In mV */
#define MAX_CELL_VOLTAGE        	4200    
#define MIN_CELL_VOLTAGE       		3000

#define MAX_PACK_VOLTAGE			302.5
#define MIN_PACK_VOLTAGE_PRERUN		266.4	
#define MIN_PACK_VOLTAGE_RUN		235
#define MIN_PACK_VOLTAGE_POSTRUN	224

#define MIN_SOC_PRERUN 				90
#define MIN_SOC_RUN 				75
#define MIN_SOC_POSTRUN				60

/* RMS Acceptable Limits */
#define MIN_IGBT_TEMP				10
#define MAX_IGBT_TEMP_PRERUN		50
#define MAX_IGBT_TEMP_RUN			100
#define MAX_IGBT_TEMP_POSTRUN		100

#define MIN_GATE_TEMP				10
#define MAX_GATE_TEMP_PRERUN		50
#define MAX_GATE_TEMP_RUN			100
#define MAX_GATE_TEMP_POSTRUN		100

#define MIN_CONTROL_TEMP			10
#define MAX_CONTROL_TEMP_IDLE		50
#define MAX_CONTROL_TEMP_PUMP		88
#define MAX_CONTROL_TEMP_RUN		88
#define MAX_CONTROL_TEMP_POSTRUN	88

#define MIN_MOTOR_TEMP				10
#define MAX_MOTOR_TEMP_PRERUN		50
#define MAX_MOTOR_TEMP_RUN			200
#define MAX_MOTOR_TEMP_SAFEAPPROACH	130

#define MIN_RPM_IDLE				-350
#define MIN_RPM_PUMPDOWN			-30
#define MIN_RPM_PROPULSION			-30
#define MIN_RPM_POST				-350

#define MAX_RPM_IDLE				350
#define MAX_RPM_PUMPDOWN			30
#define MAX_RPM_PROPULSION			10000
#define MAX_RPM_POST				30
#define MAX_RPM_CRAWL				350

#define PHASE_A_MIN					0
#define PHASE_A_MAX_PRE				0
#define PHASE_A_MAX_PROPULSION		452
#define PHASE_A_MAX_POST			0
#define PHASE_A_MAX_CRAWL			200

#define DC_BUS_VOLTAGE_MIN			0
#define DC_BUS_VOLTAGE_MAX			360

#define DC_BUS_CURRENT_MIN			0
#define DC_BUS_CURRENT_MAX_IDLE		319
#define DC_BUS_CURRENT_MAX_PUMPDOWN	1
#define DC_BUS_CURRENT_MAX_PROP		319
#define DC_BUS_CURRENT_MAX_BRAKING	0.01
#define DC_BUS_CURRENT_MAX_POST		0.01
#define DC_BUS_CURRENT_MAX_CRAWL	200

#define LV_VOLTAGE_MIN				10
#define LV_VOLTAGE_MAX				14

#define CMD_TORQUE_MIN				0
#define CMD_TORQUE_MAX_PRE			0
#define CMD_TORQUE_MAX_RUN			90
#define CMD_TORQUE_MAX_POST			0
#define CMD_TORQUE_MAX_CRAWL		40

#define ACTUAL_TORQUE_MIN			0
#define ACTUAL_TORQUE_MIN_BRAKING	-100
#define ACTUAL_TORQUE_MAX_PRE		20
#define ACTUAL_TORQUE_MAX_RUN		90
#define ACTUAL_TORQUE_MAX_BRAKING	0
#define ACTUAL_TORQUE_MAX_STOPPED	1
#define ACTUAL_TORQUE_MAX_CRAWL		40
#define ACTUAL_TORQUE_MAX_POST		20

#define MAX_STOPPED_ACCEL		0.3 /* Limit for qualifying the pod as stopped in m/s */



#define TUBE_LENGTH             3800    /*in ft //FIXME I could be wrong on this on haha */

#define MAX_RUN_TIME            30  /* in seconds, TODO get the real number */



#endif