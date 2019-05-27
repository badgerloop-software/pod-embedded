#ifndef __MOTOR_H__
#define __MOTOR_H__

#include <stdint.h>
#include <stdbool.h>

int initMotor(void);

void joinMotorHbThread(void);

void setTorque(uint16_t val);

uint16_t getTorque(void);

void setMotorIsOn(bool val);

bool getMotorIsOn(void);

int startMotor(void);

int stopMotor(void);

void idleMotor(void);

#endif
