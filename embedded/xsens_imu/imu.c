#include <stdio.h>
#include <stdlib.h>
#include <inttypes.h>
#include <pthread.h>
#include <unistd.h>
#include "imu.h"
#include "lib/i2c.h"

//Global Variables
IMU_data *data;
i2c_settings *i2c;
pthread_t IMUThread;

void SetupIMU(){
	i2c = malloc(sizeof(i2c_settings));
	i2c->bus = 2;
	i2c->deviceAddress = I2C_ADDRESS;
	i2c->openMode = O_RDWR;
	
	data = malloc(sizeof(IMU_data));
	
	if (i2c_begin(i2c) == -1) {
		fprintf(stderr, "Could not open i2c bus.\n");
		return;
	}
	
	if (pthread_create(&IMUThread, NULL, IMULoop, NULL))
	{
		fprintf(stderr, "Error creating IMU thread\n");
	}
	
}

void IMULoop(void *some_void_ptr){
	
	while(1){
			write_byte_i2c(i2c, STATUS_REG);
			unsigned char res1[4];
			read_i2c(i2c, res1, 4);
			
			//Get buffer sizes
			//int notifSize = res1[0] | res1[1] << 8;
			
			int mSize = res1[2] | res1[3] << 8;
			
			//Notification buffer, ignoring for now
			/*
			write_byte_i2c(i2c, 0x05);
			unsigned char res2[notifSize];
			read_i2c(i2c, res2, notifSize);*/
			
			// Get Data
			write_byte_i2c(i2c, DATA_REG);
			unsigned char dataBuffer[mSize];
			read_i2c(i2c, dataBuffer, mSize);
			
			for(int i = 0; i < mSize; i++){
				//printf("0x%02x ", dataBuffer[i]);
				
				// 0x40 and 0x10 identifiers for deltaV data
				if(dataBuffer[i] == 0x40 && dataBuffer[i+1] == 0x10 && dataBuffer[i+2] == 0x0C){
					
					//Load data into 32-bit unsigned integers
					uint32_t tempx = (dataBuffer[i+3] << 24) | (dataBuffer[i+4] << 16) | (dataBuffer[i+5] << 8) | dataBuffer[i+6];
					uint32_t tempy = (dataBuffer[i+7] << 24) | (dataBuffer[i+8] << 16) | (dataBuffer[i+9] << 8) | dataBuffer[i+10];
					uint32_t tempz = (dataBuffer[i+11] << 24) | (dataBuffer[i+12] << 16) | (dataBuffer[i+13] << 8) | dataBuffer[i+14];
					
					//Convert into float values
					data->dVx = *((float*) &tempx);
					data->dVy = *((float*) &tempy);
					data->dVz = *((float*) &tempz);
					
					//Time increment is 10ms, convert deltaV into accel
					data->accelX = data->dVx / 0.01;
					data->accelY = data->dVy / 0.01;
					data->accelZ = data->dVz / 0.01;
					
				}
				
				
				
			}
			
			//IMU Time Increment (10 ms)
			usleep(10000);
		}
		free(i2c);
		free(data);
	
}

float getDeltaVX(){
	return data->dVx;
}

float getDeltaVY(){
	return data->dVy;
}

float getDeltaVZ(){
	return data->dVz;
}

float getAccelX(){
	return data->accelX;
}

float getAccelY(){
	return data->accelY;
}

float getAccelZ(){
	return data->accelZ;
}