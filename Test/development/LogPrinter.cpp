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

    void LogPrinter::OnTestStart(const testing::TestInfo& test_info)
    {
        printf("*** Test %s.%s starting.\n",
            test_info.test_case_name(), test_info.name());
        testing::internal::CaptureStdout();
        testing::internal::CaptureStderr();
    }

    // Called after a failed assertion or a SUCCESS().
    void LogPrinter::OnTestPartResult(const testing::TestPartResult& test_part_result)
    {
        printf("%s in %s:%d\n%s\n",
            test_part_result.failed() ? "*** Failure" : "Success",
            test_part_result.file_name(),
            test_part_result.line_number(),
            test_part_result.summary());
    }

    // Called after a test ends.
    void LogPrinter::OnTestEnd(const testing::TestInfo& test_info)
    {
        std::string stdout_output = testing::internal::GetCapturedStdout();
        std::string stderr_output = testing::internal::GetCapturedStderr();
        std::cout << "STDOUT: {" << stdout_output << "}" << std::endl;
        std::cout << "STDERR: {" << stderr_output << "}" << std::endl;
        printf("*** Test %s.%s ending.\n",
            test_info.test_case_name(), test_info.name());
    }
/**
 * UNCOMMENT THIS FOR A DEMO
 *
 * RUN:
 * g++ -Wall -g LogPrinter.cpp -lgtest -lgtest_main -lpthread
 * ./a.out
 */

// TEST(DevelopmentTest, Test_stdout)
// {
//    for(int i=0; i < 5; i++) {
//        fprintf(stdout, "Printing to stdout\n");
//    }
// }
//
// TEST(DevelopmentTest, Test_stderr) {
//     for(int i=0; i < 5; i++) {
//         fprintf(stderr, "Printing to stderr\n");
//     }
// }
// }
//
// int main(int argc, char **argv) {
//     InitGoogleTest(&argc, argv);
//
//     UnitTest& unit_test = *UnitTest::GetInstance();
//     TestEventListeners& listeners = unit_test.listeners();
//     delete listeners.Release(listeners.default_result_printer());
//     listeners.Append(new LogPrinter);
//
//     int ret_val = RUN_ALL_TESTS();
//     return ret_val;
// }
