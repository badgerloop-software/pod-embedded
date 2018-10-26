#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include "imu.h"

int main(int argc, char *argv[]) {
	SetupIMU();
	while(1){
		printf("Delta X: %f, Delta Y: %f, Delta Z: %f\n", getDeltaVX(), getDeltaVY(), getDeltaVZ());
		printf("Accel X: %f, Accel Y: %f, Accel Z: %f\n", getAccelX(), getAccelY(), getAccelZ());
		printf("Quat q0: %f, Quat q1: %f, Quat q2: %f, Quat q3: %f\n", getOrientationQ0(), getOrientationQ1(), getOrientationQ2(), getOrientationQ3());
		printf("\n");
		usleep(100000);
	}
}