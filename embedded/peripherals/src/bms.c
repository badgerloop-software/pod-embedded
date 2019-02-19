#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "bms.h"
#include "can.h"

// extern data_t data;

void bmsInit(void) {
	bms = malloc(sizeof(Bms));  // FIXME: data.bms = malloc(sizeof(bms_t));
	printf("initialize_bms\r\n");
	bms->packCurrent = 0;
	bms->packVoltage = 0;
	bms->packDCL = 0;
	bms->packCCL = 0;
	bms->packResistance = 0;
	bms->packHealth = 0;
	bms->packOpenVoltage = 0;
	bms->packCycles = 0;
	bms->packAh = 0;
	bms->inputVoltage = 0;
	bms->Soc = 0;
	bms->relayStatus = 0;
	bms->highTemp = 0;
	bms->lowTemp = 0;
	bms->cellMaxVoltage = 0;
	bms->cellMinVoltage = 0;
	bms->maxCells = 0;
	bms->numCells = 0;
	bmsClearFaults();
}

int bmsClearFaults(void){

	uint16_t can_id = 0x7e3;
	uint8_t TxData[8];  // TODO: find out why this is diff
	uint8_t length = 16/*8*/;    // TODO p2: than this

	TxData[0] = 0x01;
	TxData[1] = 0x04;
	TxData[2] = 0x00;
	TxData[3] = 0x00;
	TxData[4] = 0x00;
	TxData[5] = 0x01;
	TxData[6] = 0x00;
	TxData[7] = 0x00;

	sendCanMsg(can_id, TxData, length);

	return 0;
}

/**
  * Receives a CAN Message and updates global BMS_Data struct
  */
int bmsParseMsg(uint32_t id, uint8_t *data) {
	//printf("Recieved bms message\r\n");
	switch(id) {
		case 0x6B0:
			//printf("ID: 0x%3lx\r\n", id);
			//printf("Data: %d, %d, %d, %d, %d, %d, %d, %d\r\n", data[0],
			bms->packCurrent = data[1] | data[0] << 8;
			bms->packCurrent /= 10;
			bms->packVoltage = data[3] | data[2] << 8;
			bms->packVoltage /= 10;
			bms->Soc = data[4]/2;
			bms->relayStatus = data[6] | data[5] << 8;

			//printf("V: %f\r\n", bms->packVoltage);
			//printf("A: %f\r\n", bms->packCurrent);
			//printf("Soc: %d\r\n", bms->Soc);
			//printf("Relay: %d\r\n", bms->relayStatus);
			break;
		case 0x6B1:
			bms->packDCL = data[1] | data[0] << 8;
			bms->highTemp = data[4];
			bms->lowTemp = data[5];
			//printf("DCL: %d\r\n", bms->packDCL);
			//printf("High T: %d\r\n", bms->highTemp);
			//printf("Low T: %d\r\n", bms->lowTemp);

			break;
		case 0x653:
			//printf("ID: 0x%3lx\r\n", id);
			//bms->relayStatus = data[1] | data[0] << 8;
			bms->relayStatus = data[0];
			bms->inputVoltage = data[2] | (data[3] << 8);
			bms->inputVoltage /= 10;
			//printf("Relay status %d\r\n", bms->relayStatus);
			//printf("Input Source Supply Voltage: %f\r\n", bms->inputVoltage);
			break;
		case 0x652:

			bms->packCCL = data[0] | (data[1] << 8);
			bms->packDCL = data[2] | (data[3] << 8);
			//bms->cellMaxVoltage = data[4] | (data[5] << 8);
			//bms->cellMaxVoltage /= 10000;
			//bms->cellMinVoltage = data[6] | (data[7] << 8);
			//bms->cellMinVoltage /= 10000;
			//printf("DCL %d\r\n", bms->packDCL);
			//printf("Cell Min V:  %d, Cell Max V: %d\r\n", bms->cellMinVoltage, bms->cellMaxVoltage);
			break;
		case 0x651:
				
			bms->cellMaxVoltage = data[2] | (data[3] << 8);
			bms->cellMinVoltage = data[0] | (data[1] << 8);
			bms->cellAvgVoltage = data[5] | (data[4] << 8);
			//bms->cellAvgVoltage /= 1000;
			//bms->cellMaxVoltage /= 1000;
			//bms->cellMinVoltage /= 1000;
			//bms->maxCells = data[6];
			bms->numCells = data[7];

			//printf("Num Cells %d\r\n", bms->numCells);
			//printf("Cell Avg V:  %d\r\n", bms->cellAvgVoltage);
			//printf("Cell Min V:  %d, Cell Max V: %d\r\n", bms->cellMinVoltage, bms->cellMaxVoltage);
			break;
		case 0x650:

			bms->Soc = data[0];
			bms->Soc /= 2;
			bms->packResistance = data[1] | (data[2] << 8);
			bms->packHealth = data[3];
			bms->packOpenVoltage = data[4] | (data[5] << 8);
			//bms->packOpenVoltage /= 10;
			bms->packCycles = data[6] | (data[7] << 8);

			//printf("SOC %d\r\n", bms->Soc);
			//printf("Pack Resistance %d\r\n", bms->packResistance);
			//printf("Pack Health %d\r\n", bms->packHealth);
			//printf("Pack Open Voltage  %f\r\n", bms->packOpenVoltage);
			//printf("Pack Cycles  %d\r\n", bms->packCycles);
			break;
		case 0x150:

			bms->packCurrent = data[0] | (data[1] << 8);
			bms->packCurrent /= 10;
			bms->packVoltage = data[2] | (data[3] << 8);
			bms->packVoltage /= 10;
			bms->packAh = data[4] | (data[5] << 8);
			bms->highTemp = data[6];
			bms->lowTemp = data[7];

			//printf("Pack Current %f\r\n", bms->packCurrent);
			//printf("Pack Voltage  %f\r\n", bms->packVoltage);
			//printf("Pack Amp Hours  %d\r\n", bms->packAh);
			//printf("High Temp  %d\r\n", bms->highTemp);
			//printf("Low Temp  %d\r\n", bms->lowTemp);
			break;
		case 0x80:
			break;
		default:
			return 0;
			//printf("BMS CAN command not found 0x%08lx\r\n", id);

	}
	return 0;
}

void bmsDump (void) {
    bms_t *bms = data.bms;
    printf("---BMS DATA---\n");
    printf("\tPack Current      = %f\n", bms->packCurrent);
    printf("\tPack Voltage      = %f\n", bms->packVoltage);
    printf("\tPack DCL          = %u\n", bms->packDCL);
    printf("\tPack CCL          = %i\n", bms->packCCL);
    printf("\tPack Resistance   = %u\n", bms->packResistance);
    printf("\tPack Health       = %u\n", bms->packHealth);
    printf("\tPack Open Voltage = %f\n", bms->packOpenVoltage);
    printf("\tPack Cycles       = %u\n", bms->packCycles);
    printf("\tPack Amp Hours    = %u\n", bms->packAh);
    printf("\tInput Voltage     = %f\n", bms->inputVoltage);
    printf("\tSOC               = %u\n", bms->Soc);
    printf("\tRelay Status      = %u\n", bms->relayStatus);
    printf("\tHigh Temp         = %u\n", bms->highTemp);
    printf("\tLow Temp          = %u\n", bms->lowTemp);
    printf("\tCell Max Voltage  = %u\n", bms->cellMaxVoltage);
    printf("\tCell Min Voltage  = %u\n", bms->cellMinVoltage);
    printf("\tMax Cells         = %u\n", bms->maxCells);
    printf("\tNumber of Cells   = %u\n", bms->numCells);
    printf("---END BMS---\n");
}
