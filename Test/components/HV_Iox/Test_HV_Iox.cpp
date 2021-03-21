// Author: Marco
#include <gtest/gtest.h>
#include <hv_iox.h>

TEST(HV_IOX, VerifyHVIox)
{
    HVIox hv_iox;
    hv_iox.init(true);
    EXPECT_EQ(hv_iox.setupIox(), 0);
// The expected output of the functions is generic
// when not connected to the I2C bus
#ifdef NOI2C
    // expected to equal 1
    EXPECT_EQ(hv_iox.isHVIndicatorEnabled(), 1);
// The expected output of the functions is
// != -1 because the function returns
// the getState method when connected
// to the I2C bus
#else
    // expected to not equal -1
    hv_iox.setMCUHVEnabled(false);
    EXPECT_NE(hv_iox.isHVIndicatorEnabled(), -1);
#endif
}

TEST(HV_IOX, HVIOX_MCU_LATCH)
{
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

TEST(HV_IOX, HVIOX_STATES)
{
    HVIox hv_iox;
    hv_iox.init(true);
#ifdef NOI2C
    EXPECT_EQ(hv_iox.getBMSMultiIn(), 1);
    EXPECT_EQ(hv_iox.getIMDStatus(), 1);
    EXPECT_EQ(hv_iox.getINRTStatus(), 1);
    EXPECT_EQ(hv_iox.isHVEnabled(), 0);
    EXPECT_EQ(hv_iox.getPsStatus(), 1);
    EXPECT_EQ(hv_iox.getBMSStatus(), 1);
    EXPECT_EQ(hv_iox.isEStopOn(), 0);
    EXPECT_EQ(hv_iox.getMasterSwFeedback(), 1);
#else
    EXPECT_NE(hv_iox.getBMSMultiIn(), -1);
    EXPECT_EQ(hv_iox.getIMDStatus(), true);
    EXPECT_NE(hv_iox.getINRTStatus(), -1);
    EXPECT_NE(hv_iox.isHVEnabled(), -1);
    EXPECT_NE(hv_iox.getPsStatus(), -1);
    EXPECT_NE(hv_iox.getBMSStatus(), -1);
    EXPECT_NE(hv_iox.isEStopOn(), -1);
    EXPECT_NE(hv_iox.getMasterSwFeedback(), -1);
#endif
}

TEST(HV_IOX, HVIOX_MCUHV)
{
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
