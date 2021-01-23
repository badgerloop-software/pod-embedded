#ifndef _RMS__H__
#define _RMS__H__

#include <stdint.h>
#include <stdbool.h>

#define RMS_EEPROM_SEND_ID      0xC1
#define RMS_HB_ID               0xC0
#define RMS_CLR_FAULTS_ID       0xC1
#define RMS_INV_DIS_ID          0xC0
#define RMS_INV_EN_ID           0xC0
#define RMS_INV_FW_10_ID        0xC0
#define RMS_INV_FW_20_ID        0xC0
#define RMS_INV_FW_30_ID        0xC0
#define RMS_CMD_0_NM_ID         0xC0
#define RMS_INV_DISCHARGE_ID    0xC0

#define WR_SUCCESS_BIT          2
#define NO_FILTER               0

class RMS {
    public:
        RMS(void);
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
        int rmsWriteEeprom(uint8_t addr, uint8_t val);
        int rmsReadEeprom(uint8_t addr);
        int rmsCmdResponseParse(uint8_t *rmsData, uint16_t filter, bool write);
        int rmsInvEnNoTorque();

        /* Handles parsing of all recieved CAN messages */
        int rms_parser(uint32_t id, uint8_t *data, uint32_t filter);
};
extern RMS rms;
#endif

