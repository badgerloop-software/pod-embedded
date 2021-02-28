#include "LogPrinter.h"
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

void LogPrinter::OnTestProgramEnd(const UnitTest& unit_test) {
    fprintf(stdout, "UNIT TEST: %s\n", unit_test.Passed() ? "PASSED" : "FAILED");
    for (Log l : Logs) {
        std::cout << "***" << l.TestName << "\n---STDOUT---\n" << l.STDOUT <<
            "\n---STDERR---\n" << l.STDERR << std::endl;
    }
    fflush(stdout);
}
void LogPrinter::OnTestStart(const testing::TestInfo& test_info)
{
    testing::internal::CaptureStdout();
    testing::internal::CaptureStderr();
}

// Called after a test ends.
void LogPrinter::OnTestEnd(const testing::TestInfo& test_info)
{
    std::string stdout_output = testing::internal::GetCapturedStdout();
    std::string stderr_output = testing::internal::GetCapturedStderr();
    LogPrinter::Log log = {test_info.name(), stdout_output, stderr_output};
    if (test_info.result()->Failed()) {
        Logs.push_back(log);
    }
}
