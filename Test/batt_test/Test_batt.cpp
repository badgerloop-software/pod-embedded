    /**
 * Author: Wilson Guo
*/

#include "../../embedded/peripherals/src/batt.c"
#include "../../embedded/peripherals/src/batt.cpp"
#include <gtest/gtest.h>

TEST(BattTest, VoltageScaling){
    EXPECT_EQ(voltScale(10), VOLT_SCALEOrig(10));
    EXPECT_EQ(voltScale(0), VOLT_SCALEOrig(0));
    EXPECT_EQ(voltScale(255), VOLT_SCALEOrig(255));

}

TEST(BattTest, CurrentScaling){
    EXPECT_EQ(voltScale(10), VOLT_SCALEOrig(10));
    EXPECT_EQ(voltScale(0), VOLT_SCALEOrig(0));
    EXPECT_EQ(voltScale(255), VOLT_SCALEOrig(255));
}

TEST(BattTest, getVoltage){
    ASSERT_FLOAT_EQ(getLVBattVoltage(), getLVBattVoltageOrig());
    ASSERT_FLOAT_EQ(getLVBattVoltage(), getLVBattVoltageOrig());
}

TEST(BattTest, getCurrent){
    ASSERT_FLOAT_EQ(getLVCurrent(), getLVCurrentOrig());
    ASSERT_FLOAT_EQ(getLVCurrent(), getLVCurrentOrig());
}