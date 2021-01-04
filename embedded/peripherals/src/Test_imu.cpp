/**
 * Author: 
*/

#include <gtest/gtest.h>

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <imu.h>
#include <data.h>

TEST(imuTest, VerifyWorkingGTest){
    initData();
    SetupIMU();
    float deltaVelocityData[3];
			getDeltaVData(deltaVelocityData);
			
			float accelData[3];
			getAccelData(accelData);
			
			printf("Delta X: %f, Delta Y: %f, Delta Z: %f\n", deltaVelocityData[0], deltaVelocityData[1], deltaVelocityData[2]);
			printf("Accel X: %f, Accel Y: %f, Accel Z: %f\n", accelData[0], accelData[1], accelData[2]);

}