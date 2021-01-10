// Author: Marco
// just to learn
#include <gtest/gtest.h>
#include <hv_iox.h>

TEST(ComponentTests, VerifyHVIox) {
    hv_iox.init(true);
    EXPECT_EQ(hv_iox.setupIox(), 0);
}