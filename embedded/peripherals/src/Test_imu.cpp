/**
 * Author: 
*/

#include <gtest/gtest.h>

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <imu.h>
#include <data.h>


class ImuTest : public ::testing::Test {
    protected: void SetUp() override {
        SetupIMU(); // setupIMU from imu.h
    }
};



TEST(imuTest, VerifyWorkingGTest){

}