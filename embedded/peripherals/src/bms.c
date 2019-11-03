#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "bms.h"
#include "can.h"
#include "data.h"

extern data_t *data;
float cells[72] = {0};

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

	canSend(can_id, TxData, length);

	return 0;
}
/**
  * Receives a CAN Message and updates global BMS_Data struct
  */
int bmsParseMsg(uint32_t id, uint8_t *msg) {
#if 0 /*DEBUG_BMS*/
	printf("Recieved bms message\n");
    printf("ID: 0x%3lx\r\n", (long unsigned int) id);
    printf("Data: %d, %d, %d, %d, %d, %d, %d\n", msg[0], msg[1], msg[2], 
	    msg[3], msg[4], msg[5], msg[6]);
#endif
    bms_t *bms = data->bms;
	switch(id) {
		case 0x6B0:		
            bms->packCurrent = (msg[1] | msg[0] << 8)/10;
		printf("0x6B0 Pack Current 1\ 0: %f\n", bms->packCurrent);
		//int64_t tryTwo = (((int16_t) msg[1]) |((int16_t) msg[0]) << 8)/10;
		//printf("0x6B0 Pack Current hepl: %lld\n", tryTwo);

			bms->packVoltage = (msg[3] | msg[2] << 8)/10;
			bms->Soc = msg[4]/2;
			bms->relayStatus = msg[6] | msg[5] << 8;
            bms->cellMaxVoltage = ((msg[5] << 8)| msg[6]) /10000.0;
#ifdef DEBUG_BMS
			printf("V: %f\r\n", bms->packVoltage);
			printf("A: %f\r\n", bms->packCurrent);
			printf("Soc: %d\r\n", bms->Soc);
			printf("Relay: %d\r\n", bms->relayStatus);
#endif
            break;
		case 0x6B1:
			bms->packDCL = msg[1] | msg[0] << 8;
			bms->highTemp = msg[4];
/*			bms->cellMinVoltage = (msg[7] | (msg[6] << 8)) / 10000.0;*/
#ifdef DEBUG_BMS
            printf("DCL: %d\r\n", bms->packDCL);
			printf("High T: %d\r\n", bms->highTemp);
			printf("Low T: %d\r\n", bms->lowTemp);
#endif
			break;
		case 0x653:
/*			printf("ID: 0x%3lx\r\n", (long unsigned int) id);*/
			bms->relayStatus = msg[1] | msg[0] << 8;
			bms->relayStatus = msg[0];
			bms->inputVoltage = (msg[2] | (msg[3] << 8))/10;
#ifdef DEBUG_BMS
            printf("Relay status %d\r\n", bms->relayStatus);
			printf("Input Source Supply Voltage: %f\r\n", bms->inputVoltage);
#endif
            break;
		case 0x652:

			bms->packCCL = msg[0] | (msg[1] << 8);
			bms->packDCL = msg[2] | (msg[3] << 8);
/*			bms->cellMaxVoltage = msg[4] | (msg[5] << 8);*/
/*			bms->cellMaxVoltage /= 10000;*/
/*			bms->cellMinVoltage = msg[6] | (msg[7] << 8);*/
/*			bms->cellMinVoltage /= 10000;*/
#ifdef DEBUG_BMS
            printf("DCL %d\r\n", bms->packDCL);
			printf("Cell Min V:  %d, Cell Max V: %d\r\n", bms->cellMinVoltage, bms->cellMaxVoltage);
#endif
            break;
		case 0x651:
/*			bms->cellMaxVoltage = msg[2] | (msg[3] << 8);*/
/*			bms->cellMinVoltage = msg[0] | (msg[1] << 8);*/
/*			bms->cellAvgVoltage = msg[5] | (msg[4] << 8);*/
/*			bms->cellAvgVoltage /= 1000;*/
/*			bms->cellMaxVoltage /= 1000;*/
/*			bms->cellMinVoltage /= 1000;*/
			bms->maxCells = msg[6];
			bms->numCells = msg[7];
#ifdef DEBUG_BMS
			printf("Num Cells %d\r\n", bms->numCells);
			printf("Cell Avg V:  %d\r\n", bms->cellAvgVoltage);
			printf("Cell Min V:  %d, Cell Max V: %d\r\n", bms->cellMinVoltage, bms->cellMaxVoltage);
#endif
            break;
		case 0x650:

			bms->Soc = msg[0];
			bms->Soc /= 2;
			bms->packResistance = msg[1] | (msg[2] << 8);
			bms->packHealth = msg[3];
			bms->packOpenVoltage = (msg[4] | (msg[5] << 8))/10;
			bms->packCycles = msg[6] | (msg[7] << 8);
#ifdef DEBUG_BMS
			printf("SOC %d\r\n", bms->Soc);
			printf("Pack Resistance %d\r\n", bms->packResistance);
			printf("Pack Health %d\r\n", bms->packHealth);
			printf("Pack Open Voltage  %f\r\n", bms->packOpenVoltage);
            printf("Pack Cycles  %d\r\n", bms->packCycles);
#endif
            break;
		case 0x150:
		//	printf("");
		//	float twoTrye = (msg[0] | (msg[1] << 8))/10;
		//	printf("0x150 Pack Current 0/1: %f\n", twoTrye);

		//	float twoTry2 = (msg[1] | (msg[0] << 8))/10;
                       // printf("0x150 Pack Current 1/ 0: %f\n", twoTry2); 


/*			bms->avgTemp = msg[0];*/
		//	bms->packCurrent /= 10;
/*            bms->cellMinVoltage = ((msg[3] << 8) | msg[4]) / 10000.0;*/
/*			bms->packVoltage = msg[2] | (msg[3] << 8);*/
/*			bms->packVoltage /= 10;*/
			bms->packAh = msg[4] | (msg[5] << 8);
			bms->highTemp = msg[6];
			bms->lowTemp = msg[7];
#ifdef DEBUG_BMS
			printf("Pack Current %f\r\n", bms->packCurrent);
			printf("Pack Voltage  %f\r\n", bms->packVoltage);
			printf("Pack Amp Hours  %d\r\n", bms->packAh);
			printf("High Temp  %d\r\n", bms->highTemp);
            printf("Low Temp  %d\r\n", bms->lowTemp);
#endif
            break;
        case 0x6b2:
            data->bms->cellMinVoltage = ((msg[0] << 8) | msg[1]) / 10000.0;
/*            data->bms->cellMaxVoltage = ((msg[2] << 8) | msg[3]) / 10000;*/
            bms->avgTemp = msg[2];
            bms->imdStatus = msg[3];
            break;
		case 0x80:
			break;
        case 0x36:
            if (msg[0] >= 0 && msg[0] < 72)
                cells[msg[0]] = (msg[2] | (msg[1] << 8)) / 10000.0;
            break; 
        
        default:
			return 0;
        
    }
	return 1;
}

void bmsDump () {
    bms_t *bms = data->bms;
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
    printf("\tAvg Temp          = %d\n", bms->avgTemp);
    printf("\tCell Max Voltage  = %f\n", bms->cellMaxVoltage);
    printf("\tCell Min Voltage  = %f\n", bms->cellMinVoltage);
    printf("\tMax Cells         = %u\n", bms->maxCells);
    printf("\tNumber of Cells   = %u\n", bms->numCells);
    printf("---END BMS---\n");
}
float *getCellArray();
float *getCellArray() {
    return cells;
}

void dumpCells() {
    printf("BATTERY CELLS: \n");
    for (int i = 0; i < 72; i++) {
        printf("CELL: %d : %f V\n", i , cells[i]);
    }
}
