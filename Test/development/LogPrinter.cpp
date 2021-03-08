#include "LogPrinter.h"
#include "gtest/gtest.h"
#include <stdio.h>
#include <string>
// terminal fancy stuff
#include <sys/ioctl.h>
#include <unistd.h>

using ::testing::EmptyTestEventListener;
using ::testing::InitGoogleTest;
using ::testing::Test;
using ::testing::TestCase;
using ::testing::TestEventListeners;
using ::testing::TestInfo;
using ::testing::TestPartResult;
using ::testing::UnitTest;

void linePrinter(char line, std::string lStyle, std::string title, std::string rStyle) {
	struct winsize size;
	ioctl(STDOUT_FILENO, TIOCGWINSZ, &size);
	std::string space = title.length() > 0 ? "   " : "";
	for (int i = 0; i<(size.ws_col - title.length())/2 - space.length(); i++) {
		std::cout << lStyle << line << rStyle;
	}
	std::cout << space << lStyle << title << rStyle << space;
	for (int i = (size.ws_col - title.length())/2 + space.length() + title.length(); i<size.ws_col; i++) {
		std::cout << lStyle << line << rStyle;
	}
	std::cout << std::endl;
}

void LogPrinter::OnTestProgramEnd(const UnitTest& unit_test) {
	std::string failstr = "FAILURES:";
	linePrinter('=', "\033[0;39;49m", failstr, "\033[0m");

    for (Log l : Logs) {
	std::cout << std::endl;
	linePrinter('-', "\033[1;33;49m", l.TestName, "\033[0m");
        std::cout << 
            "\033[0;31;49m"<< l.STDERR <<"\033[0m" <<
            "\033[0;33;49m"<< l.STDOUT <<"\033[0m";
    	linePrinter('-',"\033[1;33;49m","","\033[0m");
    }
    fflush(stdout);
}
void LogPrinter::OnTestStart(const testing::TestInfo& test_info)
{
    testing::internal::CaptureStdout();
    testing::internal::CaptureStderr();
}

void LogPrinter::OnTestEnd(const testing::TestInfo& test_info)
{
    std::string stdout_output = testing::internal::GetCapturedStdout();
    std::string stderr_output = testing::internal::GetCapturedStderr();
    LogPrinter::Log log = {test_info.name(), stdout_output, stderr_output};
    if (test_info.result()->Failed()) {
        Logs.push_back(log);
    }
}

