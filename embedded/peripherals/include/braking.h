#include <NCD9830DBR2G.h>

#define PS_TANK     CHANNEL_0
#define PS_LINE     CHANNEL_1
#define PS_ACTUATE  CHANNEL_2

#define BS_TANK     CHANNEL_3
#define BS_LINE     CHANNEL_4
#define BS_ACTUATE  CHANNEL_5

#define PRES_VESL   CHANNEL_6

double readPrimaryTank(void);

double readPrimaryLine(void);

double readPrimaryActuator(void);

double readSecTank(void);

double readSecLine(void);

double readSecActuator(void);

void *pressureMonitor(void);

int initPressureMonitor(void);

void showPressures(void);
