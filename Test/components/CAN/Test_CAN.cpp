// Author: Marco
#include <gtest/gtest.h>
#include "can.h"

TEST(CAN, CAN_INIT) {
    CAN can;
    EXPECT_EQ(can.initCan(), 0);
}
