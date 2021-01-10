// Author: Marco
// just to learn
#include <gtest/gtest.h>
#include <hv_iox.h>

TEST(HV_IOX, VerifyHVIox) {
    HVIox hv_iox;
    hv_iox.init(true);
    EXPECT_EQ(hv_iox.setupIox(), 0);
    #ifdef NOI2C
        EXPECT_EQ(hv_iox.setMCULatch(true), 0);
        EXPECT_EQ(hv_iox.isHVIndicatorEnabled(), 1);
    #endif
    hv_iox.setMCUHVEnabled(false);
    EXPECT_NE(hv_iox.isHVIndicatorEnabled(), -1);
}

TEST(HV_IOX, HVIOX_MCU_LATCH) {
    HVIox hv_iox;
    hv_iox.init(true);
    EXPECT_EQ(hv_iox.setMCULatch(true), 1);
    EXPECT_EQ(hv_iox.setMCULatch(false), 1);
}