/*
* imu.cpp
* Author: Rohan Daruwala
*/

#include <cstdio>
#include <cstdlib>
#include <cinttypes>
#include <pthread.h>
#include <unistd.h>

#include "imu.h"

extern "C" {
    #include "i2c.h"
}


#define _POSIX_C_SOURCE 199309L // for CLOCK_MONOTONIC
#include <ctime>

static inline uint64_t convertTouS(timespec *currTime) {
    return (uint64_t)((currTime->tv_sec * 1000000) + (currTime->tv_nsec / 1000));
}

static inline uint64_t getuSTimestamp() {
    timespec _temp;
    clock_gettime(CLOCK_MONOTONIC, &_temp);
    uint64_t _tempTs = convertTouS(&_temp);
    return _tempTs;
}

// static variables
    static IMUData * imudata;
    static i2c_settings * i2c;
    static pthread_t IMUThread;  

// It's best practice in embedded software to keep the constructor and setup methods seperate

// constructor allocates space on the heap for the static variables 
IMU::IMU() {
    i2c = (i2c_settings *) malloc(sizeof(i2c_settings));
    imudata = (IMUData *) malloc(sizeof(IMUData));
}

// setup IMU
void IMU::SetupIMU() {
    i2c->bus = 2;
    i2c->deviceAddress = I2C_ADDRESS;
    i2c->openMode = O_RDWR;

    if (i2c_begin(i2c) == -1){
        fprintf(stderr, "Could not open i2c bus.\n");
        return;
    }
        if (pthread_create(&IMUThread, NULL, (void* (*) (void*))&IMU::IMULoop, NULL)){
            fprintf(stderr, "Error creating IMU thread\n");
        }

    //Set all IMU structure values to zero
    imudata->posX = 0;
    imudata->posY = 0;
    imudata->posZ = 0;

    imudata->dVx = 0;
    imudata->dVy = 0;
    imudata->dVz = 0;

    imudata->velX = 0;
    imudata->velY = 0;
    imudata->velZ = 0;

    imudata->accelX = 0;
    imudata->accelY = 0;
    imudata->accelZ = 0;

    sem_init(&imudata->mutex, 0, 1);
}

// getters for the IMU data
// DeltaV data
void IMU::getDeltaVData(float *fData){
    sem_wait(&imudata->mutex);
    fData[0] = imudata->dVx;
    fData[1] = imudata->dVy;
    fData[2] = imudata->dVz;
    sem_post(&imudata->mutex);
}

float IMU::getDeltaVX(){
    sem_wait(&imudata->mutex);
    float ans = imudata->dVx;
    sem_post(&imudata->mutex);
    return ans;
}

float IMU::getDeltaVY(){
    sem_wait(&imudata->mutex);
    float ans = imudata->dVy;
    sem_post(&imudata->mutex);
    return ans;
}

float IMU::getDeltaVZ(){
    sem_wait(&imudata->mutex);
    float ans = imudata->dVz;
    sem_post(&imudata->mutex);
    return ans;
}

// Velocity data
float IMU::getVelX(){
    sem_wait(&imudata->mutex);
    float ans = imudata->velX;
    sem_post(&imudata->mutex);
    return ans;
}

float IMU::getVelY(){
    sem_wait(&imudata->mutex);
    float ans = imudata->velY;
    sem_post(&imudata->mutex);
    return ans;
}

float IMU::getVelZ(){
    sem_wait(&imudata->mutex);
    float ans = imudata->velZ;
    sem_post(&imudata->mutex);
    return ans;
}

// Accel data
void IMU::getAccelData(float *fData){
    sem_wait(&imudata->mutex);
    fData[0] = imudata->accelX;
    fData[1] = imudata->accelY;
    fData[2] = imudata->accelZ;
    sem_post(&imudata->mutex);
}

float IMU::getAccelX(){
    sem_wait(&imudata->mutex);
    float ans = imudata->accelX;
    sem_post(&imudata->mutex);
    return ans;
}

float IMU::getAccelY(){
    sem_wait(&imudata->mutex);
    float ans = imudata->accelY;
    sem_post(&imudata->mutex);
    return ans;
}

float IMU::getAccelZ(){
    sem_wait(&imudata->mutex);
    float ans = imudata->accelZ;
    sem_post(&imudata->mutex);
    return ans;
}

// position data
void IMU::getPosData(float *fData) {
    sem_wait(&imudata->mutex);
    fData[0] = imudata->posX;
    fData[1] = imudata->posY;
    fData[2] = imudata->posZ;
    sem_post(&imudata->mutex);
}

float IMU::getPosX() {
    sem_wait(&imudata->mutex);
    float ret = imudata->posX;
    sem_post(&imudata->mutex);
    return ret;
}

float IMU::getPosY() {
    sem_wait(&imudata->mutex);
    float ret = imudata->posY;
    sem_post(&imudata->mutex);
    return ret;
}   

float IMU::getPosZ() {
    sem_wait(&imudata->mutex);
    float ret = imudata->posZ;
    sem_post(&imudata->mutex);
    return ret;
}

// setters for the IMU data
void IMU::setPosX(float val) {
    sem_wait(&imudata->mutex);
    imudata->posX = val;
    sem_post(&imudata->mutex);
}

void IMU::setPosY(float val) {
    sem_wait(&imudata->mutex);
    imudata->posY = val;
    sem_post(&imudata->mutex);
}

void IMU::setPosZ(float val) {
    sem_wait(&imudata->mutex);
    imudata->posZ = val;
    sem_post(&imudata->mutex);
}

// The IMULoop
    void IMULoop(void *arg){
    (void) arg;

    unsigned char res1[4];
    uint32_t tempx, tempy, tempz;
    int i;

    while (1){
        i = 0;
        // Get message length size
        write_byte_i2c(i2c, STATUS_REG);
        read_i2c(i2c, res1, 4);
        uint16_t messageSize = res1[2] | res1[3] << 8;


        // Get Data
        write_byte_i2c(i2c, DATA_REG);
        unsigned char imudataBuffer[messageSize];
        read_i2c(i2c, imudataBuffer, messageSize);
        i = 0;

        while(i < messageSize){
            if (i + 14 < messageSize && imudataBuffer[i] == 0x40 && imudataBuffer[i + 1] == 0x30 && imudataBuffer[i + 2] == 0x0C) {
                tempx = (imudataBuffer[i + 3] << 24) | (imudataBuffer[i + 4] << 16) | (imudataBuffer[i + 5] << 8) | imudataBuffer[i + 6];
                tempy = (imudataBuffer[i + 7] << 24) | (imudataBuffer[i + 8] << 16) | (imudataBuffer[i + 9] << 8) | imudataBuffer[i + 10];
                tempz = (imudataBuffer[i + 11] << 24) | (imudataBuffer[i + 12] << 16) | (imudataBuffer[i + 13] << 8) | imudataBuffer[i + 14];
                sem_wait(&imudata->mutex);

                imudata->accelX = * ((float *) &tempx);
                imudata->accelY = * ((float *) &tempy);
                imudata->accelZ = * ((float *) &tempz);

                imudata->velX = imudata->velX + (imudata->accelX * .025);
                imudata->velY = imudata->velY + (imudata->accelY * .025);
                imudata->velZ = imudata->velZ + (imudata->accelZ * .025);

                sem_post(&imudata->mutex);
            }

            i++;
        }

        //IMU Time Increment (10 ms)
        sem_wait(&imudata->mutex);
        imudata->posX += imudata->velX * .025;
        imudata->posY += imudata->velY * .025;
        imudata->posZ += imudata->velZ * .025;
        sem_post(&imudata->mutex);
        //prev = curr;
        usleep(10000);

    }
    free(i2c);
    free(imudata);

};
