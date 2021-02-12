/**
 * Author: Rohan
*/

#include <gtest/gtest.h>

TEST(ExampleTest, VerifyWorkingGTest)
{
    EXPECT_NE(1, 0);
    EXPECT_GE(1, 1);
    EXPECT_GE(1, 0);
    EXPECT_LE(1, 1);
    EXPECT_LE(0, 1);

    ASSERT_EQ(0, 0);
}

