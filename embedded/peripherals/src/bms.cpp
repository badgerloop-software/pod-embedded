#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "can.h"

extern "C" {
#include "data.h"
#include "bms.h"

}

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

	can.canSend(can_id, TxData, length);

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
	switch(id) {
		case 0x6B0:		
            setBmsPackCurrent((msg[1] | msg[0] << 8)/10);
			setBmsPackVoltage((msg[3] | msg[2] << 8)/10);
			setBmsSoc(msg[4]/2);
			setBmsRelayStatus(msg[6] | msg[5] << 8);
            setBmsCellMaxVoltage(((msg[5] << 8)| msg[6]) /10000.0);
#ifdef DEBUG_BMS
			printf("V: %f\r\n", getBmsPackVoltage());
			printf("A: %f\r\n", getBmsPackCurrent());
			printf("Soc: %d\r\n", getBmsSoc());
			printf("Relay: %d\r\n", getBmsRelayStatus());
#endif
            break;
		case 0x6B1:
			setBmsPackDCL(msg[1] | msg[0] << 8);
			setBmsHighTemp(msg[4]);
/*			setBmsCellMinVoltage((msg[7] | (msg[6] << 8)) / 10000.0;)*/
#ifdef DEBUG_BMS
            printf("DCL: %d\r\n", setBmspackDCL);
			printf("High T: %d\r\n", setBmshighTemp);
			printf("Low T: %d\r\n", setBmslowTemp);
#endif
			break;
		case 0x653:
/*			printf("ID: 0x%3lx\r\n", (long unsigned int) id);*/
			setBmsRelayStatus(msg[1] | msg[0] << 8);
			setBmsRelayStatus(msg[0]);
			setBmsInputVoltage((msg[2] | (msg[3] << 8))/10);
#ifdef DEBUG_BMS
            printf("Relay status %d\r\n", setBmsrelayStatus);
			printf("Input Source Supply Voltage: %f\r\n", setBmsinputVoltage);
#endif
            break;
		case 0x652:

			setBmsPackCCL(msg[0] | (msg[1] << 8));
			setBmsPackDCL(msg[2] | (msg[3] << 8));
/*			setBmsCellMaxVoltage(msg[4] | (msg[5] << 8));*/
/*			setBmsCellMaxVoltage(getBmsCellMaxVoltage() / 10000);*/
/*			setBmsCellMinVoltage(msg[6] | (msg[7] << 8));*/
/*			setBmsCellMinVoltage(getBmsCellMinVoltage() / 10000);*/
#ifdef DEBUG_BMS
            printf("DCL %d\r\n", setBmspackDCL);
			printf("Cell Min V:  %d, Cell Max V: %d\r\n", setBmscellMinVoltage, setBmscellMaxVoltage);
#endif
            break;
		case 0x651:
/*			setBmsCellMaxVoltage(msg[2] | (msg[3] << 8));*/
/*			setBmsCellMinVoltage(msg[0] | (msg[1] << 8));*/
/*			setBmsCellAvgVoltage(msg[5] | (msg[4] << 8));*/
/*			setBmsCellAvgVoltage(getBmsCellAvgVoltage() / 1000);*/
/*          setBmsCellMaxVoltage(getBmsCellMaxVoltage() / 1000);*/
/*          setBmsCellMinVoltage(getBmsCellMinVoltage() / 1000);*/
/*			setBmsCellMaxVoltage /= 1000;*/
/*			setBmsCellMinVoltage /= 1000;*/
			setBmsMaxCells(msg[6]);
			setBmsNumCells(msg[7]);
#ifdef DEBUG_BMS
			printf("Num Cells %d\r\n", getBmsNumCells());
			printf("Cell Avg V:  %d\r\n", getBmsCellAvgVoltage());
			printf("Cell Min V:  %d, Cell Max V: %d\r\n", getBmsCellMinVoltage(), getBmsCellMaxVoltage());
#endif
            break;
		case 0x650:

			setBmsSoc(msg[0]);
			setBmsSoc(getBmsSoc() / 2);
			setBmsPackResistance(msg[1] | (msg[2] << 8));
			setBmsPackHealth(msg[3]);
			setBmsPackOpenVoltage((msg[4] | (msg[5] << 8))/10);
			setBmsPackCycles(msg[6] | (msg[7] << 8));
#ifdef DEBUG_BMS
			printf("SOC %d\r\n", getBmsSoc());
			printf("Pack Resistance %d\r\n", getBmsPackResistance());
			printf("Pack Health %d\r\n", getBmsPackHealth());
			printf("Pack Open Voltage  %f\r\n", getBmsPackOpenVoltage());
            printf("Pack Cycles  %d\r\n", getBmsPackCycles());
#endif
            break;
		case 0x150:
			setBmsPackAh(msg[4] | (msg[5] << 8));
			setBmsHighTemp(msg[6]);
			setBmsLowTemp(msg[7]);
#ifdef DEBUG_BMS
			printf("Pack Current %f\r\n", getBmsPackCurrent());
			printf("Pack Voltage  %f\r\n", getBmsPackVoltage());
			printf("Pack Amp Hours  %d\r\n", getBmsPackAh());
			printf("High Temp  %d\r\n", getBmsHighTemp());
            printf("Low Temp  %d\r\n", getBmsLowTemp());
#endif
            break;
        case 0x6b2:
            setBmsCellMinVoltage(((msg[0] << 8) | msg[1]) / 10000.0);
/*            setBmsCellMaxVoltage(((msg[2] << 8) | msg[3]) / 10000)*/
            setBmsAvgTemp(msg[2]);
            setBmsImdStatus(msg[3]);
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
    printf("---BMS DATA---\n");
    printf("\tPack Current      = %f\n", getBmsPackCurrent());
    printf("\tPack Voltage      = %f\n", getBmsPackVoltage());
    printf("\tPack DCL          = %u\n", getBmsPackDCL());
    printf("\tPack CCL          = %i\n", getBmsPackCCL());
    printf("\tPack Resistance   = %u\n", getBmsPackResistance());
    printf("\tPack Health       = %u\n", getBmsPackHealth());
    printf("\tPack Open Voltage = %f\n", getBmsPackOpenVoltage());
    printf("\tPack Cycles       = %u\n", getBmsPackCycles());
    printf("\tPack Amp Hours    = %u\n", getBmsPackAh());
    printf("\tInput Voltage     = %f\n", getBmsInputVoltage());
    printf("\tSOC               = %u\n", getBmsSoc());
    printf("\tRelay Status      = %u\n", getBmsRelayStatus());
    printf("\tHigh Temp         = %u\n", getBmsHighTemp());
    printf("\tLow Temp          = %u\n", getBmsLowTemp());
    printf("\tAvg Temp          = %d\n", getBmsAvgTemp());
    printf("\tCell Max Voltage  = %f\n", getBmsCellMaxVoltage());
    printf("\tCell Min Voltage  = %f\n", getBmsCellMinVoltage());
    printf("\tMax Cells         = %u\n", getBmsMaxCells());
    printf("\tNumber of Cells   = %u\n", getBmsNumCells());
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
