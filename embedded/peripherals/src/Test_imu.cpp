/**
 * Author: 
*/

#include <gtest/gtest.h>

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <imu.h>
#include <data.h>
#include <i2c.h>

// a fixture class to setup
class ImuTest : public ::testing::Test {
    protected: void SetUp() override {
        SetupIMU();
        initData();
        getDeltaVData(dvd);
    }
    float dvd[3];


};

// testing to see if fixture works, beginning
TEST_F(ImuTest, Zeros){
    EXPECT_EQ(getDeltaVX(), 0);
}