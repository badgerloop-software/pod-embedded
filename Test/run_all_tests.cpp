#include "LogPrinter.h"
#include "gtest_globals.h"
#include <gtest/gtest.h>

char executable_path[128];

// Run all GTest Tests
int main(int argc, char** argv)
{
    // Set executable path
    strcpy(executable_path, argv[0]);

    testing::InitGoogleTest(&argc, argv);
    testing::UnitTest& unit_test = *testing::UnitTest::GetInstance();
    testing::TestEventListeners& listeners = unit_test.listeners();
    delete listeners.Release(listeners.default_result_printer());
    listeners.Append(new LogPrinter);
    return RUN_ALL_TESTS();
}
