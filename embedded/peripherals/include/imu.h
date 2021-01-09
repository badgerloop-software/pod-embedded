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


class IMU
{

	IMU(void);
	void SetupIMU();
	static void *IMULoop(void *arg);

	void getPosData(float *fData);
	float getPosX();
	float getPosY();
	float getPosZ();

	void setPosX(float val);
	void setPosY(float val);
	void setPosZ(float val);

	void getDeltaVData(float *fData);
	float getDeltaVX();
	float getDeltaVY();
	float getDeltaVZ();

	float getVelX();
	float getVelY();
	float getVelZ();

	void getAccelData(float *fData);
	float getAccelX();
	float getAccelY();
	float getAccelZ();
};

class IMUData // Classes should follow UpperCamelCase
{
	public:
		//Delta velocity
		float dVx, dVy, dVz;
		// Vel
		float velX, velY, velZ;
		//Acceleration (Based off of dV)
		float accelX, accelY, accelZ;
		float posX, posY, posZ;
		// Mutex
		sem_t mutex;
};

#endif

