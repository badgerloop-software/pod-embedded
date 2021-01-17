/*
* imu.h
* Author: Rohan Daruwala
*/

#ifndef IMU_H
#define IMU_H

#define I2C_ADDRESS 0x1D
#define STATUS_REG 0x04
#define DATA_REG 0x06

#include <semaphore.h>

typedef struct {
    //Delta velocity
    float dVx;
    float dVy;
    float dVz;

    // Vel
    float velX;
    float velY;
    float velZ;

    //Acceleration (Based off of dV)
    float accelX;
    float accelY;
    float accelZ;

    float posX;
    float posY;
    float posZ;

    // Mutex
    sem_t mutex;

} IMU_data;

void SetupIMU();
void* IMULoop(void* arg);

void getPosData(float* fData);
float getPosX();
float getPosY();
float getPosZ();

void setPosX(float val);
void setPosY(float val);
void setPosZ(float val);

void getDeltaVData(float* fData);
float getDeltaVX();
float getDeltaVY();
float getDeltaVZ();

float getVelX();
float getVelY();
float getVelZ();

void getAccelData(float* fData);
float getAccelX();
float getAccelY();
float getAccelZ();

#endif
