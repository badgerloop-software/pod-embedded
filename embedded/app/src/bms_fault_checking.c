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
	if(data->bms->highTemp > MAX_BATT_TEMP_PRERUN){
		printf("Temp too high: %i\n", data->bms->highTemp);
		return false;
	}
	if(data->bms->packCurrent > MAX_BATT_CURRENT_STILL){
		printf("Pack Current too high: %f\n", data->bms->packCurrent);
		return false;
	}
	if(data->bms->cellMaxVoltage > MAX_CELL_VOLTAGE || data->bms->cellMinVoltage < MIN_CELL_VOLTAGE){
		printf("Cell Voltage Error: %f, %f\n", data->bms->cellMinVoltage, data->bms->cellMaxVoltage);
		return false;
	}
	if(data->bms->packVoltage > MAX_PACK_VOLTAGE || data->bms->packVoltage < MIN_PACK_VOLTAGE_PRERUN){
		printf("Pack Voltage Error: %f\n", data->bms->packVoltage);
		return false;
	}
	if(data->bms->Soc < MIN_SOC_PRERUN){
		printf("SOC is less than expected: %i\n", data->bms->Soc);
		return false;
	}
	
	return true;
}

bool checkRunBattery(void){
	if(data->bms->highTemp > MAX_BATT_TEMP_RUN){
		printf("Temp too high: %i\n", data->bms->highTemp);
		return false;
	}
	if(data->bms->packCurrent > MAX_BATT_CURRENT_MOVING){
		printf("Pack Current too high: %f\n", data->bms->packCurrent);
		return false;
	}
	if(data->bms->cellMaxVoltage > MAX_CELL_VOLTAGE || data->bms->cellMinVoltage < MIN_CELL_VOLTAGE){
		printf("Cell Voltage Error: %f, %f\n", data->bms->cellMinVoltage, data->bms->cellMaxVoltage);
		return false;
	}
	if(data->bms->packVoltage > MAX_PACK_VOLTAGE || data->bms->packVoltage < MIN_PACK_VOLTAGE_RUN){
		printf("Pack Voltage Error: %f\n", data->bms->packVoltage);
		return false;
	}
	if(data->bms->Soc < MIN_SOC_RUN){
		printf("SOC is less than expected: %i", data->bms->Soc);
		return false;
	}
	
	return true;
}

bool checkBrakingBattery(void){
	if(data->bms->highTemp > MAX_BATT_TEMP_RUN){
		printf("Temp too high: %i\n", data->bms->highTemp);
		return false;
	}
	if(data->bms->packCurrent > MAX_BATT_CURRENT_STILL){
		printf("Pack Current too high: %f\n", data->bms->packCurrent);
		return false;
	}
	if(data->bms->cellMaxVoltage > MAX_CELL_VOLTAGE || data->bms->cellMinVoltage < MIN_CELL_VOLTAGE){
		printf("Cell Voltage Error: %f, %f\n", data->bms->cellMinVoltage, data->bms->cellMaxVoltage);
		return false;
	}
	if(data->bms->packVoltage > MAX_PACK_VOLTAGE || data->bms->packVoltage < MIN_PACK_VOLTAGE_RUN){
		printf("Pack Voltage Error: %f\n", data->bms->packVoltage);
		return false;
	}
	if(data->bms->Soc < MIN_SOC_RUN){
		printf("SOC is less than expected: %i", data->bms->Soc);
		return false;
	}
	
	return true;
}

bool checkStoppedBattery(void){
	if(data->bms->highTemp > MAX_BATT_TEMP_RUN){
		printf("Temp too high: %i\n", data->bms->highTemp);
		return false;
	}
	if(data->bms->packCurrent > MAX_BATT_CURRENT_STILL){
		printf("Pack Current too high: %f\n", data->bms->packCurrent);
		return false;
	}
	if(data->bms->cellMaxVoltage > MAX_CELL_VOLTAGE || data->bms->cellMinVoltage < MIN_CELL_VOLTAGE){
		printf("Cell Voltage Error: %f, %f\n", data->bms->cellMinVoltage, data->bms->cellMaxVoltage);
		return false;
	}
	if(data->bms->packVoltage > MAX_PACK_VOLTAGE || data->bms->packVoltage < MIN_PACK_VOLTAGE_RUN){
		printf("Pack Voltage Error: %f\n", data->bms->packVoltage);
		return false;
	}
	if(data->bms->Soc < MIN_SOC_RUN){
		printf("SOC is less than expected: %i", data->bms->Soc);
		return false;
	}
	
	return true;
}

bool checkCrawlBattery(void){
	if(data->bms->highTemp > MAX_BATT_TEMP_RUN){
		printf("Temp too high: %i\n", data->bms->highTemp);
		return false;
	}
	if(data->bms->packCurrent > MAX_BATT_CURRENT_MOVING){
		printf("Pack Current too high: %f\n", data->bms->packCurrent);
		return false;
	}
	if(data->bms->cellMaxVoltage > MAX_CELL_VOLTAGE || data->bms->cellMinVoltage < MIN_CELL_VOLTAGE){
		printf("Cell Voltage Error: %f, %f\n", data->bms->cellMinVoltage, data->bms->cellMaxVoltage);
		return false;
	}
	if(data->bms->packVoltage > MAX_PACK_VOLTAGE || data->bms->packVoltage < MIN_PACK_VOLTAGE_POSTRUN){
		printf("Pack Voltage Error: %f\n", data->bms->packVoltage);
		return false;
	}
	if(data->bms->Soc < MIN_SOC_POSTRUN){
		printf("SOC is less than expected: %i", data->bms->Soc);
		return false;
	}
	
	return true;
}

bool checkPostrunBattery(void){
	if(data->bms->highTemp > MAX_BATT_TEMP_RUN){
		printf("Temp too high: %i\n", data->bms->highTemp);
		return false;
	}
	if(data->bms->packCurrent > MAX_BATT_CURRENT_STILL){
		printf("Pack Current too high: %f\n", data->bms->packCurrent);
		return false;
	}
	if(data->bms->cellMaxVoltage > MAX_CELL_VOLTAGE || data->bms->cellMinVoltage < MIN_CELL_VOLTAGE){
		printf("Cell Voltage Error: %f, %f\n", data->bms->cellMinVoltage, data->bms->cellMaxVoltage);
		return false;
	}
	if(data->bms->packVoltage > MAX_PACK_VOLTAGE || data->bms->packVoltage < MIN_PACK_VOLTAGE_POSTRUN){
		printf("Pack Voltage Error: %f\n", data->bms->packVoltage);
		return false;
	}
	if(data->bms->Soc < MIN_SOC_POSTRUN){
		printf("SOC is less than expected: %i", data->bms->Soc);
		return false;
	}
	
	return true;
}
