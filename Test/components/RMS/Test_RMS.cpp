// Author: Marco
#include <gtest/gtest.h>
#include "rms.h"

TEST(RMS, RMS_CANSEND) {
    RMS rms;
    EXPECT_EQ(rms.rmsEnHeartbeat(), 0);
    EXPECT_EQ(rms.rmsClrFaults(), 0);
    EXPECT_EQ(rms.rmsInvDis(), 0);
    EXPECT_EQ(rms.rmsInvEn(), 0);
    EXPECT_EQ(rms.rmsInvEnNoTorque(), 0);
    EXPECT_EQ(rms.rmsInvForward20(), 0);
    EXPECT_EQ(rms.rmsInvForward30(), 0);
    EXPECT_EQ(rms.rmsCmdNoTorque(), 0);
    EXPECT_EQ(rms.rmsDischarge(), 0);
    EXPECT_EQ(rms.rmsIdleHb(), 0);
    EXPECT_EQ(rms.rmsWriteEeprom(), 0);
    EXPECT_EQ(rms.rmsReadEeprom(), 0);
    EXPECT_EQ(rms.rmsCmdNoTorque(), 0);
}
