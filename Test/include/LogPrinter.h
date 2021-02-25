#include "gtest/gtest.h"
#include <stdio.h>

using ::testing::EmptyTestEventListener;
using ::testing::InitGoogleTest;
using ::testing::Test;
using ::testing::TestCase;
using ::testing::TestEventListeners;
using ::testing::TestInfo;
using ::testing::TestPartResult;
using ::testing::UnitTest;
class LogPrinter : public EmptyTestEventListener {
    public:
    virtual void OnTestProgramEnd(const UnitTest& unit_test);
    virtual void OnTestStart(const testing::TestInfo& test_info);
    // Called after a failed assertion or a SUCCESS().
    virtual void OnTestPartResult(const testing::TestPartResult& test_part_result);
    // Called after a test ends.
    virtual void OnTestEnd(const testing::TestInfo& test_info);
};

