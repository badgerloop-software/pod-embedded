/***
 *  Filename: bms_fault_checking.c
 *
 *  Summary: Checks to make sure recorded battery values are within limits
 *	at all points during the different states. Information about specific 
 *	values can be found in the "states.h" file
 *
 */

#include <stdio.h>
#include <stdbool.h>
#include <math.h>

#include "data.h"
#include "states.h"

#include "bms_fault_checking.h"

extern data_t *data;

bool checkPrerunBattery(void){
	if(getBmsHighTemp() > MAX_BATT_TEMP_PRERUN){
		printf("Temp too high: %i\n", getBmsHighTemp());
		return false;
	}
	if(getBmsPackCurrent() > MAX_BATT_CURRENT_STILL){
		printf("Pack Current too high: %f\n", getBmsPackCurrent());
		return false;
	}
	if(getBmsCellMaxVoltage() > MAX_CELL_VOLTAGE || getBmsCellMinVoltage() < MIN_CELL_VOLTAGE){
		printf("Cell Voltage Error: %f, %f\n", getBmsCellMinVoltage(), getBmsCellMaxVoltage());
		return false;
	}
	if(getBmsPackVoltage() > MAX_PACK_VOLTAGE || getBmsPackVoltage() < MIN_PACK_VOLTAGE_PRERUN){
		printf("Pack Voltage Error: %f\n", getBmsPackVoltage());
		return false;
	}
	if(getBmsSoc() < MIN_SOC_PRERUN){
		printf("SOC is less than expected: %i\n", getBmsSoc());
		return false;
	}
	
	return true;
}

bool checkRunBattery(void){
	if(getBmsHighTemp() > MAX_BATT_TEMP_RUN){
		printf("Temp too high: %i\n", getBmsHighTemp());
		return false;
	}
	if(getBmsPackCurrent() > MAX_BATT_CURRENT_MOVING){
		printf("Pack Current too high: %f\n", getBmsPackCurrent());
		return false;
	}
	if(getBmsCellMaxVoltage() > MAX_CELL_VOLTAGE || getBmsCellMinVoltage() < MIN_CELL_VOLTAGE){
		printf("Cell Voltage Error: %f, %f\n", getBmsCellMinVoltage(), getBmsCellMaxVoltage());
		return false;
	}
	if(getBmsPackVoltage() > MAX_PACK_VOLTAGE || getBmsPackVoltage() < MIN_PACK_VOLTAGE_RUN){
		printf("Pack Voltage Error: %f\n", getBmsPackVoltage());
		return false;
	}
	if(getBmsSoc() < MIN_SOC_RUN){
		printf("SOC is less than expected: %i", getBmsSoc());
		return false;
	}
	
	return true;
}

bool checkBrakingBattery(void){
	if(getBmsHighTemp() > MAX_BATT_TEMP_RUN){
		printf("Temp too high: %i\n", getBmsHighTemp());
		return false;
	}
	if(getBmsPackCurrent() > MAX_BATT_CURRENT_STILL){
		printf("Pack Current too high: %f\n", getBmsPackCurrent());
		return false;
	}
	if(getBmsCellMaxVoltage() > MAX_CELL_VOLTAGE || getBmsCellMinVoltage() < MIN_CELL_VOLTAGE){
		printf("Cell Voltage Error: %f, %f\n", getBmsCellMinVoltage(), getBmsCellMaxVoltage());
		return false;
	}
	if(getBmsPackVoltage() > MAX_PACK_VOLTAGE || getBmsPackVoltage() < MIN_PACK_VOLTAGE_RUN){
		printf("Pack Voltage Error: %f\n", getBmsPackVoltage());
		return false;
	}
	if(getBmsSoc() < MIN_SOC_RUN){
		printf("SOC is less than expected: %i", getBmsSoc());
		return false;
	}
	
	return true;
}

bool checkStoppedBattery(void){
	if(getBmsHighTemp() > MAX_BATT_TEMP_RUN){
		printf("Temp too high: %i\n", getBmsHighTemp());
		return false;
	}
	if(getBmsPackCurrent() > MAX_BATT_CURRENT_STILL){
		printf("Pack Current too high: %f\n", getBmsPackCurrent());
		return false;
	}
	if(getBmsCellMaxVoltage() > MAX_CELL_VOLTAGE || getBmsCellMinVoltage() < MIN_CELL_VOLTAGE){
		printf("Cell Voltage Error: %f, %f\n", getBmsCellMinVoltage(), getBmsCellMaxVoltage());
		return false;
	}
	if(getBmsPackVoltage() > MAX_PACK_VOLTAGE || getBmsPackVoltage() < MIN_PACK_VOLTAGE_RUN){
		printf("Pack Voltage Error: %f\n", getBmsPackVoltage());
		return false;
	}
	if(getBmsSoc() < MIN_SOC_RUN){
		printf("SOC is less than expected: %i", getBmsSoc());
		return false;
	}
	
	return true;
}

bool checkCrawlBattery(void){
	if(getBmsHighTemp() > MAX_BATT_TEMP_RUN){
		printf("Temp too high: %i\n", getBmsHighTemp());
		return false;
	}
	if(getBmsPackCurrent() > MAX_BATT_CURRENT_MOVING){
		printf("Pack Current too high: %f\n", getBmsPackCurrent());
		return false;
	}
	if(getBmsCellMaxVoltage() > MAX_CELL_VOLTAGE || getBmsCellMinVoltage() < MIN_CELL_VOLTAGE){
		printf("Cell Voltage Error: %f, %f\n", getBmsCellMinVoltage(), getBmsCellMaxVoltage());
		return false;
	}
	if(getBmsPackVoltage() > MAX_PACK_VOLTAGE || getBmsPackVoltage() < MIN_PACK_VOLTAGE_POSTRUN){
		printf("Pack Voltage Error: %f\n", getBmsPackVoltage());
		return false;
	}
	if(getBmsSoc() < MIN_SOC_POSTRUN){
		printf("SOC is less than expected: %i", getBmsSoc());
		return false;
	}
	
	return true;
}

bool checkPostrunBattery(void){
	if(getBmsHighTemp() > MAX_BATT_TEMP_RUN){
		printf("Temp too high: %i\n", getBmsHighTemp());
		return false;
	}
	if(getBmsPackCurrent() > MAX_BATT_CURRENT_STILL){
		printf("Pack Current too high: %f\n", getBmsPackCurrent());
		return false;
	}
	if(getBmsCellMaxVoltage() > MAX_CELL_VOLTAGE || getBmsCellMinVoltage() < MIN_CELL_VOLTAGE){
		printf("Cell Voltage Error: %f, %f\n", getBmsCellMinVoltage(), getBmsCellMaxVoltage());
		return false;
	}
	if(getBmsPackVoltage() > MAX_PACK_VOLTAGE || getBmsPackVoltage() < MIN_PACK_VOLTAGE_POSTRUN){
		printf("Pack Voltage Error: %f\n", getBmsPackVoltage());
		return false;
	}
	if(getBmsSoc() < MIN_SOC_POSTRUN){
		printf("SOC is less than expected: %i", getBmsSoc());
		return false;
	}
	
	return true;
}
