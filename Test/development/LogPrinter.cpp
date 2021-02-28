#include "LogPrinter.h"
#include "gtest/gtest.h"
#include <stdio.h>
#include <string>

using ::testing::EmptyTestEventListener;
using ::testing::InitGoogleTest;
using ::testing::Test;
using ::testing::TestCase;
using ::testing::TestEventListeners;
using ::testing::TestInfo;
using ::testing::TestPartResult;
using ::testing::UnitTest;
// "\033[{FORMAT_ATTRIBUTE};{FORGROUND_COLOR};{BACKGROUND_COLOR}m{TEXT}\033[{RESET_FORMATE_ATTRIBUTE}m"

void LogPrinter::OnTestProgramEnd(const UnitTest& unit_test) {
    for (Log l : Logs) {
        std::cout << "\033[1;94;49m-<(   " << l.TestName << "   )>-\033[0m\n" <<
            "\n\t\033[4;34;49mstdout\033[0m\n" <<
            "\033[0;39;49m"<< l.STDOUT <<"\033[0m" <<
            "\t\033[4;95;49mstderr\033[0m\n" <<
            "\033[0;39;49m"<< l.STDERR <<"\033[0m"; 
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

std::string formatTitle(std::string title, std::string style) {
    int length_of_bar = (70 - title.size())/2-1;
    std::string formated_title;
    int count = 0;
    for (; count<length_of_bar; count++) {
        formated_title += "=";
    }
    formated_title.append(" ");
    return formated_title;
}
