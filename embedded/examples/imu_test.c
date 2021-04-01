#include "imu.h"
#include <data.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

int main()
{
    initData();
    SetupIMU();

    int exampleSwitcher = 0;
    while (1) {

        if (exampleSwitcher) {
            // How to get data values using individual value pull functions
            printf("Delta X: %f, Delta Y: %f, Delta Z: %f\n", getDeltaVX(), getDeltaVY(), getDeltaVZ());
            printf("Accel X: %f, Accel Y: %f, Accel Z: %f\n", getAccelX(), getAccelY(), getAccelZ());
        } else {
            // How to get all data values at a time
            float deltaVelocityData[3];
            getDeltaVData(deltaVelocityData);

            float accelData[3];
            getAccelData(accelData);

            printf("Delta X: %f, Delta Y: %f, Delta Z: %f\n", deltaVelocityData[0], deltaVelocityData[1], deltaVelocityData[2]);
            printf("Accel X: %f, Accel Y: %f, Accel Z: %f\n", accelData[0], accelData[1], accelData[2]);
        }

        exampleSwitcher = !exampleSwitcher;
        printf("\n");
        usleep(100000);
    }
}
