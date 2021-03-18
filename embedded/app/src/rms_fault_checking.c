/***
 *  Filename: rms_fault_checking.c
 *
 *  Summary: Checks to make sure recorded HV values are within limits
 *	at all points during the different states. Information about specific 
 *	values can be found in the "states.h" file
 *
 */

#include <stdio.h>
#include <stdbool.h>
#include <math.h>

#include "data.h"
#include "states.h"

#include "rms_fault_checking.h"

extern data_t *data;

bool checkPrerunRMS(void){
	if(getRmsIgbtTemp() < MIN_IGBT_TEMP || getRmsIgbtTemp() > MAX_IGBT_TEMP_PRERUN){
		printf("IGBT Prerun Temp Failure: %i\n", getRmsIgbtTemp());
		return false;
	}
    
    if(getRmsDcBusVoltage() < DC_BUS_VOLTAGE_MIN || getRmsDcBusVoltage() > DC_BUS_VOLTAGE_MAX){

		printf("DC Bus Voltage Failure: %i\n", getRmsDcBusVoltage());
		return false;
	}

	// IDLE
	if(getDataState() == 1){
		if(getRmsDcBusCurrent() < DC_BUS_CURRENT_MIN || getRmsDcBusCurrent() > DC_BUS_CURRENT_MAX_IDLE){
			printf("DC Bus Current Idle Failure: %i\n", getRmsDcBusCurrent());
			return false;
		}
	if(getRmsGateDriverBoardTemp() < MIN_GATE_TEMP || getRmsGateDriverBoardTemp() > MAX_GATE_TEMP_PRERUN){
			printf("Gate Driver Temp Failure: %i\n", getRmsGateDriverBoardTemp());
			return false;
		}
		if(getRmsControlBoardTemp() < MIN_CONTROL_TEMP || getRmsControlBoardTemp() > MAX_CONTROL_TEMP_IDLE){
			printf("Control Temp Failure: %i\n", getRmsControlBoardTemp());
			return false;
		}
	}
	// READY PUMPDOWN SPECIFIC
	else if(getDataState() == 2){
	if(getRmsGateDriverBoardTemp() < MIN_GATE_TEMP || getRmsGateDriverBoardTemp() > MAX_GATE_TEMP_PRERUN){
			printf("Gate Driver Temp Failure: %i\n", getRmsGateDriverBoardTemp());
			return false;
		}
		if(getRmsControlBoardTemp() < MIN_CONTROL_TEMP || getRmsControlBoardTemp() > MAX_CONTROL_TEMP_PUMP){ //changes on spreadsheet
			printf("Control Temp Failure: %i\n", getRmsControlBoardTemp());
			return false;
		}
	}
	// PUMPDOWN SPECIFIC
	else if(getDataState() == 3){
	if(getRmsGateDriverBoardTemp() < MIN_GATE_TEMP || getRmsGateDriverBoardTemp() > MAX_GATE_TEMP_PRERUN){
			printf("Gate Driver Temp Failure: %i\n", getRmsGateDriverBoardTemp());
			return false;
		}
		if(getRmsControlBoardTemp() < MIN_CONTROL_TEMP || getRmsControlBoardTemp() > MAX_CONTROL_TEMP_PUMP){
			printf("Control Temp Failure: %i\n", getRmsControlBoardTemp());
			return false;
		}
	}
	// PRE-PROPULSE SPECIFIC
	else if(getDataState() == 4){
	if(getRmsGateDriverBoardTemp() < MIN_GATE_TEMP || getRmsGateDriverBoardTemp() > MAX_GATE_TEMP_RUN){ //changes on spreadsheet
			printf("Gate Driver Temp Failure: %i\n", getRmsGateDriverBoardTemp());
			return false;
		}
		if(getRmsControlBoardTemp() < MIN_CONTROL_TEMP || getRmsControlBoardTemp() > MAX_CONTROL_TEMP_PUMP){
			printf("Control Temp Failure: %i\n", getRmsControlBoardTemp());
			return false;
		}
	}
	
	return true;
}

bool checkRunRMS(void){
	if(getRmsIgbtTemp() < MIN_IGBT_TEMP || getRmsIgbtTemp() > MAX_IGBT_TEMP_RUN){
		printf("IGBT Prerun Temp Failure: %i\n", getRmsIgbtTemp());
		return false;
	}
	if(getRmsControlBoardTemp() < MIN_CONTROL_TEMP || getRmsControlBoardTemp() > MAX_CONTROL_TEMP_RUN){
		printf("Control Temp Failure: %i\n", getRmsControlBoardTemp());
		return false;
	}
if(getRmsDcBusVoltage() < DC_BUS_VOLTAGE_MIN || getRmsDcBusVoltage() > DC_BUS_VOLTAGE_MAX){
		printf("DC Bus Voltage Failure: %i\n", getRmsDcBusVoltage());
		return false;
	}
if(getRmsGateDriverBoardTemp() < MIN_GATE_TEMP || getRmsGateDriverBoardTemp() > MAX_GATE_TEMP_RUN){
		printf("Gate Driver Temp Failure: %i\n", getRmsGateDriverBoardTemp());
		return false;
	}
	
	return true;
}

bool checkBrakingRMS(void){
	if(getRmsIgbtTemp() < MIN_IGBT_TEMP || getRmsIgbtTemp() > MAX_IGBT_TEMP_RUN){
		printf("IGBT Prerun Temp Failure: %i\n", getRmsIgbtTemp());
		return false;
	}
	if(getRmsControlBoardTemp() < MIN_CONTROL_TEMP || getRmsControlBoardTemp() > MAX_CONTROL_TEMP_RUN){
		printf("Control Temp Failure: %i\n", getRmsControlBoardTemp());
		return false;
	}
if(getRmsGateDriverBoardTemp() < MIN_GATE_TEMP || getRmsGateDriverBoardTemp() > MAX_GATE_TEMP_RUN){
		printf("Gate Driver Temp Failure: %i\n", getRmsGateDriverBoardTemp());
		return false;
	}
	
	return true;
}

bool checkStoppedRMS(void){
	if(getRmsIgbtTemp() < MIN_IGBT_TEMP || getRmsIgbtTemp() > MAX_IGBT_TEMP_POSTRUN){
		printf("IGBT Prerun Temp Failure: %i\n", getRmsIgbtTemp());
		return false;
	}
	if(getRmsControlBoardTemp() < MIN_CONTROL_TEMP || getRmsControlBoardTemp() > MAX_CONTROL_TEMP_RUN){
		printf("Control Temp Failure: %i\n", getRmsControlBoardTemp());
		return false;
	}
if(getRmsGateDriverBoardTemp() < MIN_GATE_TEMP || getRmsGateDriverBoardTemp() > MAX_GATE_TEMP_RUN){
		printf("Gate Driver Temp Failure: %i\n", getRmsGateDriverBoardTemp());
		return false;
	}
	
	return true;
}

bool checkCrawlRMS(void){
	if(getRmsIgbtTemp() < MIN_IGBT_TEMP || getRmsIgbtTemp() > MAX_IGBT_TEMP_POSTRUN){
		printf("IGBT Prerun Temp Failure: %i\n", getRmsIgbtTemp());
		return false;
	}
	if(getRmsControlBoardTemp() < MIN_CONTROL_TEMP || getRmsControlBoardTemp() > MAX_CONTROL_TEMP_RUN){
		printf("Control Temp Failure: %i\n", getRmsControlBoardTemp());
		return false;
	}
if(getRmsDcBusCurrent() < DC_BUS_CURRENT_MIN || getRmsDcBusCurrent() > DC_BUS_CURRENT_MAX_CRAWL){
		printf("DC Bus Current Pumpdown Failure: %i\n", getRmsDcBusCurrent());
		return false;
	}
if(getRmsGateDriverBoardTemp() < MIN_GATE_TEMP || getRmsGateDriverBoardTemp() > MAX_GATE_TEMP_RUN){
		printf("Gate Driver Temp Failure: %i\n", getRmsGateDriverBoardTemp());
		return false;
	}
	
	return true;
}

bool checkPostRMS(void){
	if(getRmsIgbtTemp() < MIN_IGBT_TEMP || getRmsIgbtTemp() > MAX_IGBT_TEMP_POSTRUN){
		printf("IGBT Prerun Temp Failure: %i\n", getRmsIgbtTemp());
		return false;
	}
	if(getRmsControlBoardTemp() < MIN_CONTROL_TEMP || getRmsControlBoardTemp() > MAX_CONTROL_TEMP_RUN){
		printf("Control Temp Failure: %i\n", getRmsControlBoardTemp());
		return false;
	}
if(getRmsGateDriverBoardTemp() < MIN_GATE_TEMP || getRmsGateDriverBoardTemp() > MAX_GATE_TEMP_POSTRUN){
		printf("Gate Driver Temp Failure: %i\n", getRmsGateDriverBoardTemp());
		return false;
	}
	
	return true;
}

