/***
 *  Filename: load_software_parameters.c
 *
 *  Summary: Declare software parameters and load from file
 *
 */

#include <stdint.h>
#include <stdio.h>
#include "load_software_parameters.h"
#include "software_parameters.h"


/** 
 * Instantiate all parameters in the global namespace in order to be compatable with C/C++
 * Instantiate integers to default values of 0 to be safe
 */

// Assumes that executable is being run from the out/ directory. If the final executable will live in a different location,
// please rename this path accordingly. Uses run_profiles/active_run_profile.txt by default
char* ACTIVE_RUN_PROFILE                                = "../run_profiles/active_run_profile.txt";

volatile int32_t PUMPDOWN_TIMEOUT                       = 0;
volatile int32_t MAXIMUM_RUN_TIME                       = 0;
volatile int32_t RUN_RETRO_COUNT                        = 0;
volatile int32_t BRAKING_CHECK_PRESSURE_TIMEOUT         = 0;
volatile int32_t BRAKING_CHECK_RMS_TIMEOUT              = 0;
volatile int32_t BRAKING_TRANSITION_STOPPED_TIMEOUT     = 0;
volatile int32_t CRAWL_RETRO_COUNT                      = 0;
volatile int32_t MAXIMUM_CRAWL_TIME                     = 0;

/**
 * Helper function to determine if a char array contains only whitespace (blank line)
 * 
 * Returns 0 if valid line, 1 if whitespace
*/
int is_empty(const char *s) {
  while (*s != '\0') {
    if (!isspace((unsigned char)*s))
      return 0;
    s++;
  }
  return 1;
}

int loadParameters(char* filepath){
    // Success status to return true by default
    int success_status = 0;
    FILE *fp;

    // Open file
    fp = fopen(filepath, "r");

    // Verify non-null file
    if(fp != NULL){
        // Read file
        size_t len = 0;
        ssize_t read;
        char* line = NULL;

        // Parameter Counter
        int parameterCounter = 0;

        // Iterate over each line
        while ((read = getline(&line, &len, fp)) != -1){
            // Skip comments prefaced with # or are all whitespace
            if(line[0] != '#' && !is_empty(line)){
                // Line buffers
                char buffer[64];
                char parameter[64];

                // Parse out parameter as char arrays
                int scanRet = sscanf(line, "%[^':']:%s", buffer, parameter);

                // If scan fails, return false
                if(scanRet != 2){
                    success_status = 1;
                }
                else{
                    // Insert parameter, convert to int
                    *(PARAMETER_ARR[parameterCounter]) = atoi(parameter);

                    // Increment Parameter Counter
                    parameterCounter++;
                }
            }
         }

    }
    else{ // File ptr is null, return false
        success_status = 1;
    }

    return success_status;

}

