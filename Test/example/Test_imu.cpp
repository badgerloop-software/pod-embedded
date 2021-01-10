/**
 * Author: 
*/

#include "gtest/gtest.h"

#include "imu.h"
#include "data.h"


// a fixture class to setup
class ImuTest : public ::testing::Test {
    protected: 
    void SetUp() override {
        // initData();
        imu.SetupIMU();
    }
    IMU imu;
    float deltaVelocityData[3];
    float accelData[3];

};

// testing to see if fixture works, beginning
TEST_F(ImuTest, Zeros){
    imu.getDeltaVData(deltaVelocityData);
    EXPECT_EQ(deltaVelocityData[0], 0);
    EXPECT_EQ(deltaVelocityData[1], 0);
    EXPECT_EQ(deltaVelocityData[2], 0);
}