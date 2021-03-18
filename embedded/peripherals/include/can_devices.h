#ifndef CANDEVICES_H
#define CANDEVICES_H

#include <semaphore.h>

extern sem_t canSem;

void SetupCANDevices();
void *CANLoop(void *arg);

#endif
