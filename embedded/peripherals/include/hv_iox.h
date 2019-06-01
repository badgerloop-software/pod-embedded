#ifndef __HV_IOX_H__
#define __HV_IOX_H__

#include <mcp23017.h>
#include <i2c.h>

int initHVIox(void);

i2c_settings getHVIoxDev(void);

int isHVIndicatorEnabled(void);

int setMCULatch(bool val);

int getBMSMultiIn(void);

int getIMDStatus(void);

int getINRTStatus(void);

int isHVEnabled(void);

int isMCUHVEnabled(void);

int getPsStatus(void);

int getBMSStatus(void);

int isEStopOn(void);

int getMasterSwFeedback(void);

#endif
