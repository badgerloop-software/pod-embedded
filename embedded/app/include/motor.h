#ifndef __MOTOR_H__
#define __MOTOR_H__

#include <stdint.h>
#include <stdbool.h>

#define CRAWL_TORQUE 1 /* This is random right now, set at SF */
#define FULL_SPEED_AHEAD 1 /* Steamfitters should refine this */

int initMotor(void);

void joinMotorHbThread(void);

void setTorque(uint16_t val);

uint16_t getTorque(void);

void setMotorIsOn(bool val);
void setMotorCrawl(void);
bool getMotorIsOn(void);

int startMotor(void);

int stopMotor(void);

void idleMotor(void);

void setMotorEn();
void clrMotorEn();
void SetupMotor();


#endif
