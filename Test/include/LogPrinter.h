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
    struct Log {
        std::string TestName;
        std::string STDOUT;
        std::string STDERR;
    };
    std::vector<Log> Logs;
    public:
    virtual void OnTestProgramEnd(const UnitTest& unit_test);
    virtual void OnTestStart(const testing::TestInfo& test_info);
    virtual void OnTestEnd(const testing::TestInfo& test_info);
};

