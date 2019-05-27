#include <stdio.h>
#include <stdlib.h>
#include "can.h"
#include <stdint.h>
#include "rms.h"
#include "data.h"

/* Uncomment define for additional prints in the parser */
/*#define DEBUG_RMS*/

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

int rmsIdleHb() {
    uint8_t payload[] = {0x0, 0x0, 0x0, 0x0, 0x1, 0x0, 0x0, 0x0};
    return canSend(RMS_HB_ID, payload, 8);
}

int rmsWriteEeprom(uint16_t addr, uint16_t val) {
    uint8_t payload[] = {addr & 0xff, (addr >> 8), 0x1, 0x0,
        val & 0xff, (val >> 8), 0x0, 0x0};
    return canSend(RMS_EEPROM_SEND_ID, payload, 8);
}

int rmsReadEeprom(uint16_t addr) {
    uint8_t payload[] = {addr & 0xff, (addr >> 8), 0x0, 0x0,
        0x0, 0x0, 0x0, 0x0};
    return canSend(RMS_EEPROM_SEND_ID, payload, 8);
}

static uint16_t convRmsDataFormat(uint8_t byte1, uint8_t byte2) {
    return byte1 | (byte2 << 8);
}

int rmsCmdResponseParse(uint8_t *rmsData, uint16_t filter, bool write) {
    /* Eeprom message */
 #ifdef DEBUG_RMS
    int i = 0;
    printf("Message: 0xC2:");
    for (i = 0; i < 8; i++) printf(" %#X", rmsData[i]);
    printf("\n");
#endif
    uint16_t paramAddr = convRmsDataFormat(rmsData[0], rmsData[1]);
    if (paramAddr == 0) {
        fprintf(stderr, "RMS did not recognize that message\n");
        return -1;
    }
    if (paramAddr != filter) return -1;

    if (write && !rmsData[WR_SUCCESS_BIT]) {
        fprintf(stderr, "Write failed\n");
        return -1;
    } else if (write) {
        return 0;
    }

    return convRmsDataFormat(rmsData[4], rmsData[5]);
}
int rmsSendHbMsg(uint16_t torque) {
    uint8_t payload[] = {torque & 0xff, torque >> 8, 0x0, 0x0, 
        0x1, 0x1, 0x0, 0x0};
    
    return canSend(RMS_HB_ID, payload, 8);
}


/* RMS CAN Parser Function
 *      Based on the CAN ID passed, parsing out the data bytes into
 *      their respective values in the RMS data struct
 */
int rms_parser(uint32_t id, uint8_t *rmsData, uint32_t filter){
    if (filter != 0 && filter != id) {
        return 1;
    }
	switch(id){
        case (0xc2):
           case (0xa0):
			data->rms->igbtTemp = (rmsData[0] | (rmsData[1] <<8)) / 10; //Deg C
			data->rms->gateDriverBoardTemp = (rmsData[6] | (rmsData[7] << 8)) / 10; //Deg C
#ifdef DEBUG_RMS
            printf("IGBT: %d\r\n", data->rms->igbtTemp);
			printf("Gate Driver Board Temp: %d\r\n", data->rms->gateDriverBoardTemp);
#endif
            break;
		case (0xa1):
			data->rms->controlBoardTemp = (rmsData[0] | (rmsData[1] << 8)) / 10; // Deg C
#ifdef DEBUG_RMS
            printf("Control Board Temp: %d\r\n", data->rms->controlBoardTemp);
#endif
            break;
		case (0xa2):
			data->rms->motorTemp = (rmsData[4] | (rmsData[5] << 8)) / 10; //Deg C
#ifdef DEBUG_RMS
            printf("Motor Temp: %d\r\n", data->rms->motorTemp);
#endif
            break;
		case (0xa3):
			break;
		case (0xa4):
			break;
		case (0xa5):
			data->rms->motorSpeed = (rmsData[2] | (rmsData[3] << 8)); // RPM
			data->rms->electricalFreq = (rmsData[4] | (rmsData[5] << 8 )) / 10; //electrical frequency Hz
#ifdef DEBUG_RMS
            printf("Motor Speed: %d\r\n", data->rms->motorSpeed);
			printf("Elect. Freq: %d\r\n", data->rms->electricalFreq);
#endif
            break;
		case (0xa6):
			data->rms->phaseACurrent = (rmsData[0] | (rmsData[1] << 8)) / 10; // Phase A current
			data->rms->dcBusCurrent = (rmsData[6] | (rmsData[7] << 8)) / 10; //DC Bus current
#ifdef DEBUG_RMS
            printf("Phase A Current: %d\r\n", data->rms->phaseACurrent);
			printf("DC Bus Current: %d\r\n", data->rms->dcBusCurrent);
            printf("Phase B Current: %d\r\n", data->rms->phaseBCurrent); //FIXME This isnt actually being read in?
#endif
            break;
		case (0xa7):
			data->rms->dcBusVoltage = (rmsData[0] | (rmsData[1] << 8))/10; //DC Bus voltage
			data->rms->outputVoltageLn = (rmsData[2] | (rmsData[3] << 8)) / 10; //Voltage line to netural 
#ifdef DEBUG_RMS
            printf("DC Bus Voltage: %d\r\n", data->rms->dcBusVoltage);
			printf("Output Voltage line: %d\r\n", data->rms->outputVoltageLn);
#endif
            break;
		case (0xa8):
			break;
		case (0xa9):
			data->rms->lvVoltage = (rmsData[6] | (rmsData[7] << 8)) / 100;
#ifdef DEBUG_RMS
            printf("LV Voltage: %d\r\n", data->rms->lvVoltage);
#endif
            break;

		case (0xaa):
			data->rms->canCode1 = (rmsData[3] << 24) | (rmsData[2] << 16) | (rmsData[1] << 8) | rmsData[0];
			data->rms->canCode2 = (rmsData[7] << 24) | (rmsData[6] << 16) | (rmsData[5] << 8) | rmsData[4];
#ifdef DEBUG_RMS
            printf("CAN Code 1: %lld\r\n", (long long int) data->rms->canCode1);
			printf("CAN Code 2: %lld\r\n", (long long int) data->rms->canCode2);
#endif
            break;
		case (0xab):
			data->rms->faultCode1 = (rmsData[3] << 24) | (rmsData[2] << 16) | (rmsData[1] << 8) | rmsData[0];
			data->rms->faultCode2 = (rmsData[7] << 24) | (rmsData[6] << 16) | (rmsData[5] << 8) | rmsData[4];
#ifdef DEBUG_RMS
            printf("Fault Code 1: %lld\r\n", (long long int) data->rms->faultCode1);
			printf("Fault Code 2: %lld\r\n", (long long int) data->rms->faultCode2);
#endif
            break;
		case (0xac):
			data->rms->commandedTorque = (rmsData[0] | (rmsData[1] << 8)) /10;
			data->rms->actualTorque = (rmsData[2] | (rmsData[3] << 8)) / 10;
#ifdef DEBUG_RMS
            printf("Commanded Torque: %d\r\n", data->rms->commandedTorque);
			printf("Actual Torque: %d\r\n", data->rms->actualTorque);
#endif
            break;
		case (0xad):
			break;
		case (0xae):
			break;
		case (0xaf):
			break;
		default:
			return 1;
	}
	return 0;
}
