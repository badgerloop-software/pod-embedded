#include <NCD9830DBR2G.h>
#include "buart.h"
#include <semaphore.h>
#define PS_TANK CHANNEL_0
#define PS_LINE CHANNEL_1
#define PS_ACTUATE CHANNEL_2

#define PRES_VESL CHANNEL_4

#define BS_TANK CHANNEL_5
#define BS_LINE CHANNEL_7
#define BS_ACTUATE CHANNEL_6

extern sem_t bigSem;

double readAmbientPressure(void);

double readPrimaryTank(void);

double readPrimaryLine(void);

double readPrimaryActuator(void);

double readSecTank(void);

double readSecLine(void);

double readSecActuator(void);

void* pressureMonitor(void*);

int initPressureMonitor(void);

void showPressures(void);

int joinPressureMonitor(void);

int brakePrimaryActuate(void);

int brakeSecondaryActuate(void);

int brakePrimaryUnactuate(void);

int brakeSecondaryActuate(void);

int brakeSecondaryUnactuate(void);

int brake(void);

int unbrake(void);

int initComms(void);
