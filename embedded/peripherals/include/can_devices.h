#ifndef CANDEVICES_H
#define CANDEVICES_H

#include <semaphore.h>
#include "can.h"

extern sem_t canSem;

void SetupCANDevices();
void *CANLoop(void *arg);
void rx_recv(struct can_frame *can_mesg);

#endif
