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
IMU_data * data;
i2c_settings * i2c;
pthread_t IMUThread;

void SetupIMU(){
	sem_init(&data->mutex, 0, 1);
	sem_init(&data->mutex, 0, 1);
	i2c = malloc(sizeof(i2c_settings));
	i2c->bus = 2;
	i2c->deviceAddress = I2C_ADDRESS;
	i2c->openMode = O_RDWR;

	data = malloc(sizeof(IMU_data));

	if (i2c_begin(i2c) == -1){
		fprintf(stderr, "Could not open i2c bus.\n");
		return;
	}

	if (pthread_create(&IMUThread, NULL, IMULoop, NULL)){
		fprintf(stderr, "Error creating IMU thread\n");
	}

	//Set all IMU structure values to zero
	data->dVx = 0;
	data->dVy = 0;
	data->dVz = 0;

	data->accelX = 0;
	data->accelY = 0;
	data->accelZ = 0;

	data->q0 = 0;
	data->q1 = 0;
	data->q2 = 0;
	data->q3 = 0;
}

void *IMULoop(){
	
	unsigned char res1[4];
	uint32_t tempx, tempy, tempz;
	uint32_t tempq0, tempq1, tempq2, tempq3;

	while (1){
		
		// Information on data registers can be found @ https://www.xsens.com/download/pdf/documentation/mti-1/mti-1-series_datasheet.pdf

		//Get message length size
		write_byte_i2c(i2c, STATUS_REG);
		read_i2c(i2c, res1, 4);
		uint16_t messageSize = res1[2] | res1[3] << 8;


		// Get Data
		write_byte_i2c(i2c, DATA_REG);
		unsigned char dataBuffer[messageSize];
		read_i2c(i2c, dataBuffer, messageSize);

		for (int i = 0; i < messageSize; i++){
			//Check delta velocity
			if (i + 14 < messageSize &&dataBuffer[i] == 0x40 &&dataBuffer[i + 1] == 0x10 &&dataBuffer[i + 2] == 0x0C){

				//Load data into 32-bit unsigned integers
				tempx = (dataBuffer[i + 3] << 24) | (dataBuffer[i + 4] << 16) | (dataBuffer[i + 5] << 8) | dataBuffer[i + 6];
				tempy = (dataBuffer[i + 7] << 24) | (dataBuffer[i + 8] << 16) | (dataBuffer[i + 9] << 8) | dataBuffer[i + 10];
				tempz = (dataBuffer[i + 11] << 24) | (dataBuffer[i + 12] << 16) | (dataBuffer[i + 13] << 8) | dataBuffer[i + 14];

				sem_wait(&data->mutex);
				//Convert into float values
				data->dVx = * ((float *) &tempx);
				data->dVy = * ((float *) &tempy);
				data->dVz = * ((float *) &tempz);

				//Time increment is 10ms, convert deltaV into accel
				data->accelX = data->dVx / 0.01;
				data->accelY = data->dVy / 0.01;
				data->accelZ = data->dVz / 0.01;
				sem_post(&data->mutex);

			}

			//Check orientation
			if (i + 18 < messageSize &&dataBuffer[i] == 0x80 &&dataBuffer[i + 1] == 0x30 &&dataBuffer[i + 2] == 0x10){
				tempq0 = (dataBuffer[i + 3] << 24) | (dataBuffer[i + 4] << 16) | (dataBuffer[i + 5] << 8) | dataBuffer[i + 6];
				tempq1 = (dataBuffer[i + 7] << 24) | (dataBuffer[i + 8] << 16) | (dataBuffer[i + 9] << 8) | dataBuffer[i + 10];
				tempq2 = (dataBuffer[i + 11] << 24) | (dataBuffer[i + 12] << 16) | (dataBuffer[i + 13] << 8) | dataBuffer[i + 14];
				tempq3 = (dataBuffer[i + 15] << 24) | (dataBuffer[i + 16] << 16) | (dataBuffer[i + 17] << 8) | dataBuffer[i + 18];

				sem_wait(&data->mutex);
				data->q0 = * ((float *) &tempq0);
				data->q1 = * ((float *) &tempq1);
				data->q2 = * ((float *) &tempq2);
				data->q3 = * ((float *) &tempq3);
				sem_post(&data->mutex);

			}

		}

		//IMU Time Increment (10 ms)
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

void getDeltaOrientationData(float *fData){
	sem_wait(&data->mutex);
	fData[0] = data->q0;
	fData[1] = data->q1;
	fData[2] = data->q2;
	fData[3] = data->q3;
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

float getOrientationDeltaQ0(){
	sem_wait(&data->mutex);
	float ans = data->q0;
	sem_post(&data->mutex);
	return ans;
}

float getOrientationDeltaQ1(){
	sem_wait(&data->mutex);
	float ans = data->q1;
	sem_post(&data->mutex);
	return ans;
}

float getOrientationDeltaQ2(){
	sem_wait(&data->mutex);
	float ans = data->q2;
	sem_post(&data->mutex);
	return ans;
}

float getOrientationDeltaQ3(){
	sem_wait(&data->mutex);
	float ans = data->q3;
	sem_post(&data->mutex);
	return ans;
}