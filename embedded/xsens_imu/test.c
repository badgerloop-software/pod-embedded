#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include "imu.h"

int main(int argc, char *argv[]) {
	SetupIMU();
	while(1){
		printf("Delta X: %f, Delta Y: %f, Delta Z: %f\n", getDeltaVX(), getDeltaVY(), getDeltaVZ());
		printf("Accel X: %f, Accel Y: %f, Accel Z: %f\n", getAccelX(), getAccelY(), getAccelZ());
		usleep(100000);
	}
}