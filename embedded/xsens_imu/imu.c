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
			unsigned char res3[mSize];
			read_i2c(i2c, res3, mSize);
			
			for(int i = 0; i < mSize; i++){
				//printf("0x%02x ", res3[i]);
				
				// 0x40 and 0x10 identifiers for deltaV data
				if(res3[i] == 0x40 && res3[i+1] == 0x10){
					//i+2 is length, which is 12 (so far, todo is to make it check)
					//i+3, i+4, i+5, i+6 is one
					// i+7, i+8, i+9 i+10 is two
					// i+11, i+12, i+13, i+14 is three
					
					//Load data into 32-bit unsigned integers
					uint32_t tempx = (res3[i+3] << 24) | (res3[i+4] << 16) | (res3[i+5] << 8) | res3[i+6];
					uint32_t tempy = (res3[i+7] << 24) | (res3[i+8] << 16) | (res3[i+9] << 8) | res3[i+10];
					uint32_t tempz = (res3[i+11] << 24) | (res3[i+12] << 16) | (res3[i+13] << 8) | res3[i+14];
					
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