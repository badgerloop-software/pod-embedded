#ifndef STATES_H
#define STATES_H

/* Motion */

#define RETRO_COUNT_MAX 15


/* Pressure sensor acceptable limits (in PSI) */
#define PS1_BOTTOM_LIMIT_IDLE           -50
#define PS1_BOTTOM_LIMIT_PRE        	-50
#define PS1_BOTTOM_LIMIT_CRAWLPOST  	300
#define PS1_TOP_LIMIT_PRE          		1300
#define PS1_TOP_LIMIT_CRAWLPOST     	1000
#define PS1_TOP_LIMIT_IDLE           	1300

#define PS2_BOTTOM_LIMIT_IDLE        	-15
#define PS2_BOTTOM_LIMIT_PRE        	-5
#define PS2_BOTTOM_LIMIT_CRAWLPOST  	-15
#define PS2_TOP_LIMIT_IDLE        	    150
#define PS2_TOP_LIMIT_PRE           	300
#define PS2_TOP_LIMIT_CRAWLPOST     	160


#define PS3_BOTTOM_LIMIT_IDLE        	-10
#define PS3_BOTTOM_LIMIT_PRE        	-15
#define PS3_BOTTOM_LIMIT_BRAKING		-10
#define PS3_BOTTOM_LIMIT_CRAWLPOST		-15
#define PS3_TOP_LIMIT_IDLE        	    15
#define PS3_TOP_LIMIT_PRE           	150
#define PS3_TOP_LIMIT_BRAKING			150
#define PS3_TOP_LIMIT_CRAWLPOST			150

#define SEC_PS1_BOTTOM_LIMIT_IDLE       -50
#define SEC_PS1_BOTTOM_LIMIT_PRE		-300
#define SEC_PS1_BOTTOM_LIMIT_CRAWLPOST	300
#define SEC_PS1_TOP_LIMIT_PRE			1400
#define SEC_PS1_TOP_LIMIT_CRAWLPOST		1400
#define SEC_PS1_TOP_LIMIT_IDLE          1400


#define SEC_PS2_BOTTOM_LIMIT_IDLE       -15
#define SEC_PS2_BOTTOM_LIMIT_PRE	-10
#define SEC_PS2_BOTTOM_LIMIT_CRAWLPOST	0
#define SEC_PS2_TOP_LIMIT_PRE			150
#define SEC_PS2_TOP_LIMIT_CRAWLPOST		150
#define SEC_PS2_TOP_LIMIT_IDLE          150

#define SEC_PS3_BOTTOM_LIMIT_IDLE       -15
#define SEC_PS3_BOTTOM_LIMIT			-15
#define SEC_PS3_TOP_LIMIT				150
#define SEC_PS3_TOP_LIMIT_IDLE          150

#define PV_TOP_LIMIT                20
#define PV_BOTTOM_LIMIT             10


/* Battery Acceptable Limits */
#define MAX_BATT_TEMP_PRERUN		33	/* Degrees Celcius */
#define MAX_BATT_TEMP_RUN			48	/* Degrees Celcius */

#define MAX_BATT_CURRENT_STILL		5
#define MAX_BATT_CURRENT_MOVING		265

/* In mV */
#define MAX_CELL_VOLTAGE        	4.200    
#define MIN_CELL_VOLTAGE       		3.000

#define MAX_PACK_VOLTAGE			302.5
#define MIN_PACK_VOLTAGE_PRERUN		266.4	
#define MIN_PACK_VOLTAGE_RUN		235
#define MIN_PACK_VOLTAGE_POSTRUN	224

#define MIN_SOC_PRERUN 				20
#define MIN_SOC_RUN 				10
#define MIN_SOC_POSTRUN				10

/* RMS Acceptable Limits FIXME is this too hot?*/
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

#define MIN_RPM_IDLE				-1000
#define MIN_RPM_PUMPDOWN			-1000
#define MIN_RPM_PROPULSION			-1000
#define MIN_RPM_POST				-1000

#define MAX_RPM_IDLE				350
#define MAX_RPM_PUMPDOWN			30
#define MAX_RPM_PROPULSION			10000
#define MAX_RPM_POST				30
#define MAX_RPM_CRAWL				350

#define PHASE_A_MIN					-480
#define PHASE_A_MAX_PRE				10
#define PHASE_A_MAX_PROPULSION		480
#define PHASE_A_MAX_POST			5
#define PHASE_A_MAX_CRAWL			480

#define DC_BUS_VOLTAGE_MIN			-5
#define DC_BUS_VOLTAGE_MAX			360

#define DC_BUS_CURRENT_MIN			-5
#define DC_BUS_CURRENT_MAX_IDLE		10
#define DC_BUS_CURRENT_MAX_PUMPDOWN	319
#define DC_BUS_CURRENT_MAX_PROP		319
#define DC_BUS_CURRENT_MAX_BRAKING	319
#define DC_BUS_CURRENT_MAX_POST		319
#define DC_BUS_CURRENT_MAX_CRAWL	200

#define LV_VOLTAGE_MIN				8
#define LV_VOLTAGE_MAX				14

#define CMD_TORQUE_MIN				0
#define CMD_TORQUE_MAX_PRE			5
#define CMD_TORQUE_MAX_RUN			90
#define CMD_TORQUE_MAX_POST			0
#define CMD_TORQUE_MAX_CRAWL		40

#define ACTUAL_TORQUE_MIN			-10
#define ACTUAL_TORQUE_MIN_BRAKING	-100
#define ACTUAL_TORQUE_MAX_PRE		20
#define ACTUAL_TORQUE_MAX_RUN		90
#define ACTUAL_TORQUE_MAX_BRAKING	10
#define ACTUAL_TORQUE_MAX_STOPPED	10
#define ACTUAL_TORQUE_MAX_CRAWL		40
#define ACTUAL_TORQUE_MAX_POST		20

#define MAX_STOPPED_VEL				0.3 /* Limit for qualifying the pod as stopped in m/s */



#define TUBE_LENGTH             1158    /*in m //FIXME I could be wrong on this on haha */
#define MIN_DISTANCE_TO_END		30		/*in m //FIXME Confirm how far from the end of the tube we need to stop */

#define MAX_RUN_TIME            30000000  /* in microseconds, TODO get the real number */
#define MAX_CRAWL_TIME          30000000  /* in microseconds, TODO get the real number */



#endif
