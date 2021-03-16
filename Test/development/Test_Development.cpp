#include "gtest/gtest.h"

// Basic tests to use when developing the logging infastructure
TEST(DevelopmentTest, Test_stdout)
{
    EXPECT_EQ(1,1);
    for (int i = 0; i < 5; i++) {
        fprintf(stdout, "Printing to stdout\n");
    }
}

TEST(DevelopmentTest, Test_stderr)
{
    EXPECT_EQ(1,1);
    for (int i = 0; i < 5; i++) {
        fprintf(stderr, "Printing to stderr\n");
    }
}
