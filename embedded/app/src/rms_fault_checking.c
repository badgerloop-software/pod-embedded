#include <stdio.h>
#include <stdbool.h>
#include <math.h>

#include "data.h"
#include "states.h"

#include "rms_fault_checking.h"

extern data_t *data;

bool checkPrerunRMS(void){
	if(data->rms->igbtTemp < MIN_IGBT_TEMP || data->rms->igbtTemp > MAX_IGBT_TEMP_PRERUN){
		printf("IGBT Prerun Temp Failure: %i\n", data->rms->igbtTemp);
		return false;
	}
	if(data->rms->motorTemp < MIN_MOTOR_TEMP || data->rms->motorTemp > MAX_MOTOR_TEMP_PRERUN){
		printf("Motor Temp Failure: %i\n", data->rms->motorTemp);
		return false;
	}
	if(data->rms->phaseACurrent < PHASE_A_MIN || data->rms->phaseACurrent > PHASE_A_MAX_PRE){
		printf("Phase A Current Failure: %i\n", data->rms->phaseACurrent);
		return false;
	}
	if(data->rms->dcBusVoltage < DC_BUS_VOLTAGE_MIN || data->rms->dcBusVoltage > DC_BUS_VOLTAGE_MAX){
		printf("DC Bus Voltage Failure: %i\n", data->rms->dcBusVoltage);
		return false;
	}
	if(data->rms->lvVoltage < LV_VOLTAGE_MIN || data->rms->lvVoltage > LV_VOLTAGE_MAX){
		printf("LV Voltage Failure: %i\n", data->rms->lvVoltage);
		return false;
	}
	if(data->rms->commandedTorque < CMD_TORQUE_MIN || data->rms->commandedTorque > CMD_TORQUE_MAX_PRE){
		printf("CMD Torque Failure: %i\n", data->rms->commandedTorque);
		return false;
	}
	if(data->rms->actualTorque < ACTUAL_TORQUE_MIN || data->rms->actualTorque > ACTUAL_TORQUE_MAX_PRE){
		printf("Actual Torque Failure: %i\n", data->rms->actualTorque);
		return false;
	}
	
	// IDLE
	if(data->state == 1){
		if(data->rms->dcBusCurrent < DC_BUS_CURRENT_MIN || data->rms->dcBusCurrent > DC_BUS_CURRENT_MAX_IDLE){
			printf("DC Bus Current Idle Failure: %i\n", data->rms->dcBusCurrent);
			return false;
		}
		if(data->rms->motorSpeed < MIN_RPM_IDLE || data->rms->motorSpeed > MAX_RPM_IDLE){
			printf("Motor speed Failure: %i\n", data->rms->motorSpeed);
			return false;
		}
		if(data->rms->gateDriverBoardTemp < MIN_GATE_TEMP || data->rms->gateDriverBoardTemp > MAX_GATE_TEMP_PRERUN){
			printf("Gate Driver Temp Failure: %i\n", data->rms->gateDriverBoardTemp);
			return false;
		}
		if(data->rms->controlBoardTemp < MIN_CONTROL_TEMP || data->rms->controlBoardTemp > MAX_CONTROL_TEMP_IDLE){
			printf("Control Temp Failure: %i\n", data->rms->controlBoardTemp);
			return false;
		}
	}
	// READY PUMPDOWN SPECIFIC
	else if(data->state == 2){
		if(data->rms->dcBusCurrent < DC_BUS_CURRENT_MIN || data->rms->dcBusCurrent > DC_BUS_CURRENT_MAX_PUMPDOWN){
			printf("DC Bus Current Pumpdown Failure: %i\n", data->rms->dcBusCurrent);
			return false;
		}
		if(data->rms->motorSpeed < MIN_RPM_IDLE || data->rms->motorSpeed > MAX_RPM_IDLE){
			printf("Motor speed Failure: %i\n", data->rms->motorSpeed);
			return false;
		}
		if(data->rms->gateDriverBoardTemp < MIN_GATE_TEMP || data->rms->gateDriverBoardTemp > MAX_GATE_TEMP_PRERUN){
			printf("Gate Driver Temp Failure: %i\n", data->rms->gateDriverBoardTemp);
			return false;
		}
		if(data->rms->controlBoardTemp < MIN_CONTROL_TEMP || data->rms->controlBoardTemp > MAX_CONTROL_TEMP_PUMP){ //changes on spreadsheet
			printf("Control Temp Failure: %i\n", data->rms->controlBoardTemp);
			return false;
		}
	}
	// PUMPDOWN SPECIFIC
	else if(data->state == 3){
		if(data->rms->dcBusCurrent < DC_BUS_CURRENT_MIN || data->rms->dcBusCurrent > DC_BUS_CURRENT_MAX_PUMPDOWN){
			printf("DC Bus Current Pumpdown Failure: %i\n", data->rms->dcBusCurrent);
			return false;
		}
		if(data->rms->motorSpeed < MIN_RPM_PUMPDOWN || data->rms->motorSpeed > MAX_RPM_PUMPDOWN){ //changes on spreadsheet
			printf("Motor speed Failure: %i\n", data->rms->motorSpeed);
			return false;
		}
		if(data->rms->gateDriverBoardTemp < MIN_GATE_TEMP || data->rms->gateDriverBoardTemp > MAX_GATE_TEMP_PRERUN){
			printf("Gate Driver Temp Failure: %i\n", data->rms->gateDriverBoardTemp);
			return false;
		}
		if(data->rms->controlBoardTemp < MIN_CONTROL_TEMP || data->rms->controlBoardTemp > MAX_CONTROL_TEMP_PUMP){
			printf("Control Temp Failure: %i\n", data->rms->controlBoardTemp);
			return false;
		}
	}
	// PRE-PROPULSE SPECIFIC
	else if(data->state == 4){
		if(data->rms->dcBusCurrent < DC_BUS_CURRENT_MIN || data->rms->dcBusCurrent > DC_BUS_CURRENT_MAX_PUMPDOWN){
			printf("DC Bus Current Pumpdown Failure: %i\n", data->rms->dcBusCurrent);
			return false;
		}
		if(data->rms->motorSpeed < MIN_RPM_PUMPDOWN || data->rms->motorSpeed > MAX_RPM_PUMPDOWN){
			printf("Motor speed Failure: %i\n", data->rms->motorSpeed);
			return false;
		}
		if(data->rms->gateDriverBoardTemp < MIN_GATE_TEMP || data->rms->gateDriverBoardTemp > MAX_GATE_TEMP_RUN){ //changes on spreadsheet
			printf("Gate Driver Temp Failure: %i\n", data->rms->gateDriverBoardTemp);
			return false;
		}
		if(data->rms->controlBoardTemp < MIN_CONTROL_TEMP || data->rms->controlBoardTemp > MAX_CONTROL_TEMP_PUMP){
			printf("Control Temp Failure: %i\n", data->rms->controlBoardTemp);
			return false;
		}
	}
	
	return true;
}

bool checkRunRMS(void){
	if(data->rms->igbtTemp < MIN_IGBT_TEMP || data->rms->igbtTemp > MAX_IGBT_TEMP_RUN){
		printf("IGBT Prerun Temp Failure: %i\n", data->rms->igbtTemp);
		return false;
	}
	if(data->rms->controlBoardTemp < MIN_CONTROL_TEMP || data->rms->controlBoardTemp > MAX_CONTROL_TEMP_RUN){
		printf("Control Temp Failure: %i\n", data->rms->controlBoardTemp);
		return false;
	}
	if(data->rms->motorTemp < MIN_MOTOR_TEMP || data->rms->motorTemp > MAX_MOTOR_TEMP_RUN){
		printf("Motor Temp Failure: %i\n", data->rms->motorTemp);
		return false;
	}
	if(data->rms->phaseACurrent < PHASE_A_MIN || data->rms->phaseACurrent > PHASE_A_MAX_PROPULSION){
		printf("Phase A Current Failure: %i\n", data->rms->phaseACurrent);
		return false;
	}
	if(data->rms->dcBusVoltage < DC_BUS_VOLTAGE_MIN || data->rms->dcBusVoltage > DC_BUS_VOLTAGE_MAX){
		printf("DC Bus Voltage Failure: %i\n", data->rms->dcBusVoltage);
		return false;
	}
	if(data->rms->lvVoltage < LV_VOLTAGE_MIN || data->rms->lvVoltage > LV_VOLTAGE_MAX){
		printf("LV Voltage Failure: %i\n", data->rms->lvVoltage);
		return false;
	}
	if(data->rms->commandedTorque < CMD_TORQUE_MIN || data->rms->commandedTorque > CMD_TORQUE_MAX_RUN){
		printf("CMD Torque Failure: %i\n", data->rms->commandedTorque);
		return false;
	}
	if(data->rms->actualTorque < ACTUAL_TORQUE_MIN || data->rms->actualTorque > ACTUAL_TORQUE_MAX_RUN){
		printf("Actual Torque Failure: %i\n", data->rms->actualTorque);
		return false;
	}
	if(data->rms->dcBusCurrent < DC_BUS_CURRENT_MIN || data->rms->dcBusCurrent > DC_BUS_CURRENT_MAX_PROP){
		printf("DC Bus Current Pumpdown Failure: %i\n", data->rms->dcBusCurrent);
		return false;
	}
	if(data->rms->motorSpeed < MIN_RPM_IDLE || data->rms->motorSpeed > MAX_RPM_PROPULSION){
		printf("Motor speed Failure: %i\n", data->rms->motorSpeed);
		return false;
	}
	if(data->rms->gateDriverBoardTemp < MIN_GATE_TEMP || data->rms->gateDriverBoardTemp > MAX_GATE_TEMP_RUN){
		printf("Gate Driver Temp Failure: %i\n", data->rms->gateDriverBoardTemp);
		return false;
	}
	
	return true;
}

static bool checkBrakingRMS(void){
	if(data->rms->igbtTemp < MIN_IGBT_TEMP || data->rms->igbtTemp > MAX_IGBT_TEMP_RUN){
		printf("IGBT Prerun Temp Failure: %i\n", data->rms->igbtTemp);
		return false;
	}
	if(data->rms->controlBoardTemp < MIN_CONTROL_TEMP || data->rms->controlBoardTemp > MAX_CONTROL_TEMP_RUN){
		printf("Control Temp Failure: %i\n", data->rms->controlBoardTemp);
		return false;
	}
	if(data->rms->motorTemp < MIN_MOTOR_TEMP || data->rms->motorTemp > MAX_MOTOR_TEMP_RUN){
		printf("Motor Temp Failure: %i\n", data->rms->motorTemp);
		return false;
	}
	if(data->rms->phaseACurrent < PHASE_A_MIN || data->rms->phaseACurrent > PHASE_A_MAX_POST){
		printf("Phase A Current Failure: %i\n", data->rms->phaseACurrent);
		return false;
	}
	if(data->rms->dcBusVoltage < DC_BUS_VOLTAGE_MIN || data->rms->dcBusVoltage > DC_BUS_CURRENT_MAX_BRAKING){
		printf("DC Bus Voltage Failure: %i\n", data->rms->dcBusVoltage);
		return false;
	}
	if(data->rms->lvVoltage < LV_VOLTAGE_MIN || data->rms->lvVoltage > LV_VOLTAGE_MAX){
		printf("LV Voltage Failure: %i\n", data->rms->lvVoltage);
		return false;
	}
	if(data->rms->commandedTorque < CMD_TORQUE_MIN || data->rms->commandedTorque > CMD_TORQUE_MAX_POST){
		printf("CMD Torque Failure: %i\n", data->rms->commandedTorque);
		return false;
	}
	if(data->rms->actualTorque < ACTUAL_TORQUE_MIN_BRAKING || data->rms->actualTorque > ACTUAL_TORQUE_MAX_BRAKING){
		printf("Actual Torque Failure: %i\n", data->rms->actualTorque);
		return false;
	}
	if(data->rms->dcBusCurrent < DC_BUS_CURRENT_MIN || data->rms->dcBusCurrent > DC_BUS_CURRENT_MAX_PROP){
		printf("DC Bus Current Pumpdown Failure: %i\n", data->rms->dcBusCurrent);
		return false;
	}
	if(data->rms->motorSpeed < MIN_RPM_IDLE || data->rms->motorSpeed > MAX_RPM_PROPULSION){
		printf("Motor speed Failure: %i\n", data->rms->motorSpeed);
		return false;
	}
	if(data->rms->gateDriverBoardTemp < MIN_GATE_TEMP || data->rms->gateDriverBoardTemp > MAX_GATE_TEMP_RUN){
		printf("Gate Driver Temp Failure: %i\n", data->rms->gateDriverBoardTemp);
		return false;
	}
	
	return true;
}