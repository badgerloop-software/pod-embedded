/**
 * Author: 
*/

#include "gtest/gtest.h"

#include "../../embedded/peripherals/include/imu.h"
#include "data.h"


// a fixture class to setup
class ImuTest : public ::testing::Test {
    protected: 
    void SetUp() override {
        initData();
        SetupIMU();
    }


};

// testing to see if fixture works, beginning
TEST_F(ImuTest, Zeros){

}