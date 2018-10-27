#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include "imu.h"

int main() {
	SetupIMU();
	
	int exampleSwitcher = 0;
	while(1){
		
		
		if(exampleSwitcher){
			// How to get data values using individual value pull functions
			printf("Delta X: %f, Delta Y: %f, Delta Z: %f\n", getDeltaVX() , getDeltaVY(), getDeltaVZ());
			printf("Accel X: %f, Accel Y: %f, Accel Z: %f\n", getAccelX(), getAccelY(), getAccelZ());
			printf("Quat q0: %f, Quat q1: %f, Quat q2: %f, Quat q3: %f\n", getOrientationQ0(), getOrientationQ1(), getOrientationQ2(), getOrientationQ3());
		}
		else{
			// How to get all data values at a time 
			float deltaVelocityData[3];
			getDeltaVData(deltaVelocityData);
			
			float accelData[3];
			getAccelData(accelData);
			
			float deltaOrientationData[4];
			getDeltaOrientationData(deltaOrientationData);
			
			printf("Delta X: %f, Delta Y: %f, Delta Z: %f\n", deltaVelocityData[0], deltaVelocityData[1], deltaVelocityData[2]);
			printf("Accel X: %f, Accel Y: %f, Accel Z: %f\n", accelData[0], accelData[1], accelData[2]);
			printf("Quat q0: %f, Quat q1: %f, Quat q2: %f, Quat q3: %f\n", deltaOrientationData[0], deltaOrientationData[1], deltaOrientationData[2], deltaOrientationData[3]);
		}
		
		exampleSwitcher = !exampleSwitcher;
		printf("\n");
		usleep(100000);
	}
}