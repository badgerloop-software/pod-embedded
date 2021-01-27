#include <gtest/gtest.h>
#include "can.h"
#include "rms.h"

extern "C" {
#include <data.h>
}
CAN can;
RMS rms;
// Run all GTest Tests
int main(int argc, char **argv) {
    initData();
    testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}
