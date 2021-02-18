#include <stdio.h>
#include "gtest/gtest.h"

using ::testing::EmptyTestEventListener;
using ::testing::InitGoogleTest;
using ::testing::Test;
using ::testing::TestCase;
using ::testing::TestEventListeners;
using ::testing::TestInfo;
using ::testing::TestPartResult;
using ::testing::UnitTest;
namespace {
class LogPrinter : public EmptyTestEventListener {
    virtual void OnTestStart(const testing::TestInfo& test_info);
    // Called after a failed assertion or a SUCCESS().
    virtual void OnTestPartResult(const testing::TestPartResult& test_part_result);
    // Called after a test ends.
    virtual void OnTestEnd(const testing::TestInfo& test_info);
};
}
