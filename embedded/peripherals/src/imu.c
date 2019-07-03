/*
* imu.c
* Author: Rohan Daruwala
*/

#include <stdio.h>
#include <stdlib.h>
#include <inttypes.h>
#include <pthread.h>
#include <unistd.h>
#include "imu.h"
#include "i2c.h"

//Global Variables
static IMU_data * data;
static i2c_settings * i2c;
static pthread_t IMUThread;

void SetupIMU(){
	i2c = (i2c_settings *) malloc(sizeof(i2c_settings));
	i2c->bus = 2;
	i2c->deviceAddress = I2C_ADDRESS;
	i2c->openMode = O_RDWR;

	data = (IMU_data *) malloc(sizeof(IMU_data));

	if (i2c_begin(i2c) == -1){
		fprintf(stderr, "Could not open i2c bus.\n");
		return;
	}

	if (pthread_create(&IMUThread, NULL, IMULoop, NULL)){
		fprintf(stderr, "Error creating IMU thread\n");
	}
    
    data->posX = 0;
    data->posY = 0;
    data->posZ = 0;

	//Set all IMU structure values to zero
	data->dVx = 0;
	data->dVy = 0;
	data->dVz = 0;
    
    data->velX = 0;
    data->velY = 0;
    data->velZ = 0;
    
	data->accelX = 0;
	data->accelY = 0;
	data->accelZ = 0;
	
	sem_init(&data->mutex, 0, 1);
}

void *IMULoop(void *arg){
	(void) arg;
	
	unsigned char res1[4];
	uint32_t tempx, tempy, tempz;
	int i;
    
	while (1){
		
		i = 0;
		// Information on data registers can be found @ https://www.xsens.com/download/pdf/documentation/mti-1/mti-1-series_datasheet.pdf

		//Get message length size
		write_byte_i2c(i2c, STATUS_REG);
		read_i2c(i2c, res1, 4);
		uint16_t messageSize = res1[2] | res1[3] << 8;


		// Get Data
		write_byte_i2c(i2c, DATA_REG);
		unsigned char dataBuffer[messageSize];
		read_i2c(i2c, dataBuffer, messageSize);
        printf("message size\n");
        printf("message:\n");
        for (i = 0; i < messageSize; i++) {
            printf("%#x ", dataBuffer[i]);
        }
        printf("\n");
        i = 0;
		while(i < messageSize){
			//Check delta velocity
            
	//		if (i + 14 < messageSize && dataBuffer[i] == 0x40 && dataBuffer[i + 1] == 0x10 && dataBuffer[i + 2] == 0x0C){
//
//				//Load data into 32-bit unsigned integers
//				tempx = (dataBuffer[i + 3] << 24) | (dataBuffer[i + 4] << 16) | (dataBuffer[i + 5] << 8) | dataBuffer[i + 6];
//				tempy = (dataBuffer[i + 7] << 24) | (dataBuffer[i + 8] << 16) | (dataBuffer[i + 9] << 8) | dataBuffer[i + 10];
//				tempz = (dataBuffer[i + 11] << 24) | (dataBuffer[i + 12] << 16) | (dataBuffer[i + 13] << 8) | dataBuffer[i + 14];
//
//				sem_wait(&data->mutex);
//				//Convert into float values
//				data->dVx = * ((float *) &tempx);
//				data->dVy = * ((float *) &tempy);
//				data->dVz = * ((float *) &tempz);

				//Time increment is 10ms, convert deltaV into accel
/*				data->accelX = data->dVx / 0.01;*/
/*				data->accelY = data->dVy / 0.01;*/
/*				data->accelZ = data->dVz / 0.01;*/
//				sem_post(&data->mutex);

//			}

            if (i + 14 < messageSize && dataBuffer[i] == 0x40 && dataBuffer[i + 1] == 0x30 && dataBuffer[i + 2] == 0x0C) {
                tempx = (dataBuffer[i + 3] << 24) | (dataBuffer[i + 4] << 16) | (dataBuffer[i + 5] << 8) | dataBuffer[i + 6];
			    tempy = (dataBuffer[i + 7] << 24) | (dataBuffer[i + 8] << 16) | (dataBuffer[i + 9] << 8) | dataBuffer[i + 10];
				tempz = (dataBuffer[i + 11] << 24) | (dataBuffer[i + 12] << 16) | (dataBuffer[i + 13] << 8) | dataBuffer[i + 14];
                sem_wait(&data->mutex);
                
                data->accelX = * ((float *) &tempx);
                data->accelY = * ((float *) &tempy);
                data->accelZ = * ((float *) &tempz);
                
                data->velX = data->velX + (data->accelX * 0.01);
                data->velY = data->velY + (data->accelY * 0.01);
                data->velZ = data->velZ + (data->accelZ * 0.01);
                
                
                data->posX += (data->velX * 0.01);
                
                
                sem_post(&data->mutex);
            }

			i++;
		}

		//IMU Time Increment (10 ms)
        sem_wait(&data->mutex);
        data->posX += data->velX * 0.01;
        data->posY += data->velY * 0.01;
		data->posZ += data->velZ * 0.01;
        sem_post(&data->mutex);
        usleep(10000);
	}
	free(i2c);
	free(data);

}

void getDeltaVData(float *fData){
	sem_wait(&data->mutex);
	fData[0] = data->dVx;
	fData[1] = data->dVy;
	fData[2] = data->dVz;
	sem_post(&data->mutex);
}

void getAccelData(float *fData){
	sem_wait(&data->mutex);
	fData[0] = data->accelX;
	fData[1] = data->accelY;
	fData[2] = data->accelZ;
	sem_post(&data->mutex);
}

void getPosData(float *fData) {
	sem_wait(&data->mutex);
    fData[0] = data->posX;
    fData[1] = data->posY;
    fData[2] = data->posZ;
	sem_post(&data->mutex);
}

float getPosX() {
	sem_wait(&data->mutex);
	float ret = data->posX;
    sem_post(&data->mutex);
    return ret;
}

void setPosX(float val) {
    sem_wait(&data->mutex);
    data->posX = val;
    sem_post(&data->mutex);
}

float getPosY() {
	sem_wait(&data->mutex);
    float ret = data->posY;
	sem_post(&data->mutex);
    return ret;
}

void setPosY(float val) {
    sem_wait(&data->mutex);
    data->posY = val;
    sem_post(&data->mutex);
}

float getPosZ() {
    sem_wait(&data->mutex);
    float ret = data->posZ;
    sem_post(&data->mutex);
    return ret;
}

void setPosZ(float val) {
    sem_wait(&data->mutex);
    data->posZ = val;
    sem_post(&data->mutex);
}

float getDeltaVX(){
	sem_wait(&data->mutex);
	float ans = data->dVx;
	sem_post(&data->mutex);
	return ans;
}

float getDeltaVY(){
	sem_wait(&data->mutex);
	float ans = data->dVy;
	sem_post(&data->mutex);
	return ans;
}

float getDeltaVZ(){
	sem_wait(&data->mutex);
	float ans = data->dVz;
	sem_post(&data->mutex);
	return ans;
}

float getAccelX(){
	sem_wait(&data->mutex);
	float ans = data->accelX;
	sem_post(&data->mutex);
	return ans;
}

float getAccelY(){
	sem_wait(&data->mutex);
	float ans = data->accelY;
	sem_post(&data->mutex);
	return ans;
}

float getAccelZ(){
	sem_wait(&data->mutex);
	float ans = data->accelZ;
	sem_post(&data->mutex);
	return ans;
}
