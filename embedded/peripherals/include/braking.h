
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

int initComms(void);

int brakePrimaryActuate(void);

int brakeSecondaryActuate(void);

int brakePrimaryUnactuate(void);

int brakeSecondaryActuate(void);

int brakeSecondaryUnactuate(void);

int brake(void);

int unbrake(void);
