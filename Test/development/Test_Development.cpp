#include "gtest/gtest.h"

// Basic tests to use when developing the logging infastructure
TEST(DevelopmentTest, Test_stdout)
{
    for (int i = 0; i < 5; i++) {
        fprintf(stdout, "Printing to stdout\n");
    }
}

TEST(DevelopmentTest, Test_stderr)
{
    for (int i = 0; i < 5; i++) {
        fprintf(stderr, "Printing to stderr\n");
    }
}

// int main(int argc, char **argv) {
//     InitGoogleTest(&argc, argv);
//
//     UnitTest& unit_test = *UnitTest::GetInstance();
//     TestEventListeners& listeners = unit_test.listeners();
//     delete listeners.Release(listeners.default_result_printer());
//     listeners.Append(new LogPrinter);
//
//     int ret_val = RUN_ALL_TESTS();
//     return ret_val;
// }
