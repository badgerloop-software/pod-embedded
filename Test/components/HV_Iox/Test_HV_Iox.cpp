// Author: Marco
#include <gtest/gtest.h>
#include <hv_iox.h>

TEST(HV_IOX, VerifyHVIox) {
    HVIox hv_iox;
    hv_iox.init(true);
    EXPECT_EQ(hv_iox.setupIox(), 0);
#ifdef NOI2C
    EXPECT_EQ(hv_iox.isHVIndicatorEnabled(), 1);
#else
    hv_iox.setMCUHVEnabled(false);
    EXPECT_NE(hv_iox.isHVIndicatorEnabled(), -1);
#endif
}

TEST(HV_IOX, HVIOX_MCU_LATCH) {
    HVIox hv_iox;
    hv_iox.init(true);
#ifdef NOI2C
    EXPECT_EQ(hv_iox.setMCULatch(true), 0);
    EXPECT_EQ(hv_iox.setMCULatch(false), 0);
#else
    EXPECT_EQ(hv_iox.setMCULatch(true), 1);
    EXPECT_EQ(hv_iox.setMCULatch(false), 1);
#endif
}

TEST(HV_IOX, HVIOX_STATES) {
    HVIox hv_iox;
    hv_iox.init(true);
#ifdef NOI2C
    EXPECT_EQ(hv_iox.getBMSMultiIn(), 1);
    EXPECT_EQ(hv_iox.getIMDStatus(), 1);
    EXPECT_EQ(hv_iox.getINRTStatus(), 1);
    EXPECT_EQ(hv_iox.isHVEnabled(), 0);
#else
    EXPECT_NE(hv_iox.getBMSMultiIn(), -1);
    EXPECT_EQ(hv_iox.getIMDStatus(), true);
    EXPECT_NE(hv_iox.getINRTStatus(), -1);
    EXPECT_NE(hv_iox.isHVEnabled(), -1);
#endif
}

TEST(HV_IOX, HVIOX_MCUHV) {
    HVIox hv_iox;
    hv_iox.init(true);
#ifdef NOI2C
    EXPECT_EQ(hv_iox.setMCUHVEnabled(true), 1);
    EXPECT_EQ(hv_iox.isMCUHVEnabled(), 1);
    EXPECT_EQ(hv_iox.emergencyDisableMCU(), 1);
#else
    EXPECT_NE(hv_iox.setMCUHVEnabled(true), -1);
    EXPECT_NE(hv_iox.isMCUHVEnabled(), -1);
    EXPECT_NE(hv_iox.emergencyDisableMCU(), -1);
#endif
}

