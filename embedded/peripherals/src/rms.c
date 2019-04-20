#include <stdio.h>
#include <stdlib.h>
#include "can.h"
#include <stdint.h>
#include "rms.h"
#include "data.h"

extern data_t *data;

/* The following send functions are a series of cryptic steps
 * that just magically make the RMS (Motor Controller) work. 
 * If you question them too hard you may burst into flames (along with the motor
 * controller, so dont do it). The important part is that they 
 * are sent at the proper time and in the listed order. Timing information
 * will be coming a little later */

/* TODO figure out if heartbeat values may just be torque */
/* 1 */
int rmsEnHeartbeat() {
    uint8_t payload[] = {0x92, 0x0, 0x1, 0x0, 0x1, 0x0, 0x0, 0x0};
    return canSend(RMS_HB_ID, payload, 8);
}

/* 2 */
int rmsClrFaults() {
    uint8_t payload[] = {0x14, 0x0, 0x1, 0x0, 0x0, 0x0, 0x0, 0x0};
    return canSend(RMS_CLR_FAULTS_ID, payload, 8);
}

/* 3 */
int rmsInvDis() {
    uint8_t payload[] = {0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0};
    return canSend(RMS_INV_DIS_ID, payload, 8);
}

/* 4 */
int rmsInvEn() {
    uint8_t payload[] = {0x64, 0x0, 0x0, 0x0, 0x1, 0x1, 0x0, 0x0};
    return canSend(RMS_INV_EN_ID, payload, 8);
}

/* 5 */
int rmsInvForward20() {
    uint8_t payload[] = {0xC8, 0x0, 0x0, 0x0, 0x1, 0x1, 0x0, 0x0};
    return canSend(RMS_INV_FW_20_ID, payload, 8);
}

/* 6 */
int rmsInvForward30() {
    uint8_t payload[] = {0x2C, 0x1, 0x0, 0x0, 0x1, 0x1, 0x0, 0x0};
    return canSend(RMS_INV_FW_30_ID, payload, 8);
}

/* 7 */
int rmsCmdNoTorque() {
    uint8_t payload[] = {0x0, 0x0, 0x0, 0x0, 0x1, 0x0, 0x0, 0x0};
    return canSend(RMS_CMD_0_NM_ID, payload, 8);
}

/* 8 */
int rmsDischarge() {
    uint8_t payload[] = {0x0, 0x0, 0x0, 0x0, 0x1, 0x2, 0x0, 0x0};
    return canSend(RMS_INV_DISCHARGE_ID, payload, 8);
}

/* RMS CAN Parser Function
 *      Based on the CAN ID passed, parsing out the data bytes into
 *      their respective values in the RMS data struct
 */
int rms_parser(uint32_t id, uint8_t *rmsData){
	switch(id){
		case (0xa0):
			data->rms->igbtTemp = (rmsData[0] | (rmsData[1] <<8)) / 10; //Deg C
			data->rms->gateDriverBoardTemp = (rmsData[6] | (rmsData[7] << 8)) / 10; //Deg C
			printf("IGBT: %d\r\n", data->rms->igbtTemp);
			printf("Gate Driver Board Temp: %d\r\n", data->rms->gateDriverBoardTemp);
			break;
		case (0xa1):
			data->rms->controlBoardTemp = (rmsData[0] | (rmsData[1] << 8)) / 10; // Deg C
			printf("Control Board Temp: %d\r\n", data->rms->controlBoardTemp);
			break;
		case (0xa2):
			data->rms->motorTemp = (rmsData[4] | (rmsData[5] << 8)) / 10; //Deg C
			printf("Motor Temp: %d\r\n", data->rms->motorTemp);
			break;
		case (0xa3):
			break;
		case (0xa4): 
			break;
		case (0xa5):
			data->rms->motorSpeed = (rmsData[2] | (rmsData[3] << 8)); // RPM
			data->rms->electricalFreq = (rmsData[4] | (rmsData[5] << 8 )) / 10; //electrical frequency Hz
			printf("Motor Speed: %d\r\n", data->rms->motorSpeed);
			printf("Elect. Freq: %d\r\n", data->rms->electricalFreq);
			break;
		case (0xa6):
			data->rms->phaseACurrent = (rmsData[0] | (rmsData[1] << 8)) / 10; // Phase A current
			data->rms->dcBusCurrent = (rmsData[6] | (rmsData[7] << 8)) / 10; //DC Bus current
			printf("Phase A Current: %d\r\n", data->rms->phaseACurrent);
			printf("DC Bus Current: %d\r\n", data->rms->dcBusCurrent);
            printf("Phase B Current: %d\r\n", data->rms->phaseBCurrent); //FIXME This isnt actually being read in?
			break;
		case (0xa7):
			data->rms->dcBusVoltage = (rmsData[0] | (rmsData[1] << 8))/10; //DC Bus voltage
			data->rms->outputVoltageLn = (rmsData[2] | (rmsData[3] << 8)) / 10; //Voltage line to netural 
			printf("DC Bus Voltage: %d\r\n", data->rms->dcBusVoltage);
			printf("Output Voltage line: %d\r\n", data->rms->outputVoltageLn);
			break;
		case (0xa8):
			break;
		case (0xa9):
			data->rms->lvVoltage = (rmsData[6] | (rmsData[7] << 8)) / 100;
			printf("LV Voltage: %d\r\n", data->rms->lvVoltage);
			break;

		case (0xaa):
			data->rms->canCode1 = (rmsData[3] << 24) | (rmsData[2] << 16) | (rmsData[1] << 8) | rmsData[0];
			data->rms->canCode2 = (rmsData[7] << 24) | (rmsData[6] << 16) | (rmsData[5] << 8) | rmsData[4];
			printf("CAN Code 1: %lld\r\n", (long long int) data->rms->canCode1);
			printf("CAN Code 2: %lld\r\n", (long long int) data->rms->canCode2);
			break;
		case (0xab):
			data->rms->faultCode1 = (rmsData[3] << 24) | (rmsData[2] << 16) | (rmsData[1] << 8) | rmsData[0];
			data->rms->faultCode2 = (rmsData[7] << 24) | (rmsData[6] << 16) | (rmsData[5] << 8) | rmsData[4];
			printf("Fault Code 1: %lld\r\n", (long long int) data->rms->faultCode1);
			printf("Fault Code 2: %lld\r\n", (long long int) data->rms->faultCode2);
			break;
		case (0xac):
			data->rms->commandedTorque = (rmsData[0] | (rmsData[1] << 8)) /10;
			data->rms->actualTorque = (rmsData[2] | (rmsData[3] << 8)) / 10;
			printf("Commanded Torque: %d\r\n", data->rms->commandedTorque);
			printf("Actual Torque: %d\r\n", data->rms->actualTorque);
			break;
		case (0xad):
			break;
		case (0xae):
			break;
		case (0xaf): 
			break;
		default: 
			printf("Unknown RMS CAN ID: %lx\r\n", (long unsigned int) id);
			return 0;
	}
	return 1;

}
