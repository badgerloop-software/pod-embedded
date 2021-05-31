// Author: Marco
#include <gtest/gtest.h>
#include <iox_morty.h>
#include <iox_rick.h>

TEST(IOX_RICK, VerifyRickIox)
{
    iox_rick Rick;
    
#ifdef NOI2C
    EXPECT_EQ(Rick.init(true), 1);
    EXPECT_EQ(Rick.setupIox(), 1);


    EXPECT_NE(Rick.setMCULatch(false), 1);
#else
    EXPECT_EQ(Rick.init(true), 0);
    EXPECT_EQ(Rick.setupIox(), 0);

    EXPECT_EQ(Rick.setMCULatch(false), 0);
    EXPECT_EQ(Rick.setMCULatch(true), 1);
#endif
}
