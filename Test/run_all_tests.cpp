#include <gtest/gtest.h>

extern "C" {
#include <data.h>
}

// Run all GTest Tests
int main(int argc, char **argv) {
    initData();
    testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}