#include <gtest/gtest.h>
#include <string>
#include <stdio.h>

// Include Software Parameters
extern "C"{
    #include "load_software_parameters.h"
}
#include "software_parameters.h"
#include "gtest_globals.h"

TEST(LoadSoftwareParameters, VerifyNominalParameters){
    int status = loadParameters(executable_path, ACTIVE_RUN_PROFILE);
    
    // Verify successful status
    EXPECT_EQ(status, 0);

    // Verify expected values
    EXPECT_EQ(PUMPDOWN_TIMEOUT, 300000000);
    EXPECT_EQ(MAXIMUM_RUN_TIME, 30000000);
    EXPECT_EQ(RUN_RETRO_COUNT, 3);
    EXPECT_EQ(BRAKING_CHECK_PRESSURE_TIMEOUT, 5000000);
    EXPECT_EQ(BRAKING_CHECK_RMS_TIMEOUT, 10000000);
    EXPECT_EQ(BRAKING_TRANSITION_STOPPED_TIMEOUT, 15000000);
    EXPECT_EQ(CRAWL_RETRO_COUNT, 2);
    EXPECT_EQ(MAXIMUM_CRAWL_TIME, 5000000);
}