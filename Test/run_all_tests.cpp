#include "gtest_globals.h"
#include <gtest/gtest.h>

char executable_path[128];

// Run all GTest Tests
int main(int argc, char** argv)
{
    // testing::internal::CaptureStderr();
    // Set executable path
    strcpy(executable_path, argv[0]);

    testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
    // std::string output = testing::internal::GetCapturedStderr();

    // printf("========\n");
    // printf("%s", output);
    // return 0;
}