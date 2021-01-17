#ifndef _RMS__H__
#define _RMS__H__

#include <stdbool.h>
#include <stdint.h>

#define RMS_EEPROM_SEND_ID 0xC1
#define RMS_HB_ID 0xC0
#define RMS_CLR_FAULTS_ID 0xC1
#define RMS_INV_DIS_ID 0xC0
#define RMS_INV_EN_ID 0xC0
#define RMS_INV_FW_10_ID 0xC0
#define RMS_INV_FW_20_ID 0xC0
#define RMS_INV_FW_30_ID 0xC0
#define RMS_CMD_0_NM_ID 0xC0
#define RMS_INV_DISCHARGE_ID 0xC0

#define WR_SUCCESS_BIT 2
#define NO_FILTER 0

int rmsEnHeartbeat();
int rmsClrFaults();
int rmsInvDis();
int rmsInvEn();
int rmsInvForward20();
int rmsInvForward30();
int rmsCmdNoTorque();
int rmsDischarge();
int rmsIdleHb();
int rmsSendHbMsg(uint16_t torque);
int rmsWriteEeprom(uint16_t addr, uint16_t val);
int rmsReadEeprom(uint16_t addr);
int rmsCmdResponseParse(uint8_t* rmsData, uint16_t filter, bool write);
int rmsInvEnNoTorque();

/* Handles parsing of all recieved CAN messages */
int rms_parser(uint32_t id, uint8_t* data, uint32_t filter);

#endif
