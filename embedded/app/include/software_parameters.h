/**
 * Header file for software-loaded parameters. Include this file in all locations where 
 * said parameters are required. All parameters are declared as extern to prevent multiple-linkage
 * 
 * Future task could allow these software parameters to be hot-updated via a dashboard command. Volatile naming
 * ensures parameters are re-read from memory each time in order to ensure live updates aren't accidentally optimized
 * out
 * 
 * REMINDER: PLEASE VALIDATE ALL RUN_PROFILES USING utils/validate_run_profile.py. The order in which variables are declared here
 *           must match the order in which variables are declared in the run_profile files.
 * 
 * NOTE: Currently only supports int32_t parameters
 */

#ifndef SOFTWARE_PARAMETERS_H
#define SOFTWARE_PARAMETERS_H

#include <stdint.h>
#include <string.h>

/**
 * Variable name for the run profile to load. In production it is suggested to leave this variable unchanged and instead rename the
 * profile text file itself. Variable is modifiable in order to allow GTest to switch loaded profiles.
 * 
 * Default value is "run_profiles/active_run_profile.txt"
 *
 * @location load_run_profile.cpp
 */
extern char* ACTIVE_RUN_PROFILE;

/** SOFTWARE PARAMETER LIST BEGINS BELOW **/

/**
 * Maximum duration for pumpdown state, FSM transitions to fault if exceeded. Units us.
 *
 * @location states.c
 */
extern volatile int32_t PUMPDOWN_TIMEOUT;

/**
 * Maximum duration for propulsion state, FSM transitions to braking if exceeded. Units us.
 *
 * @location states.c
 */
extern volatile int32_t MAXIMUM_RUN_TIME;

/**
 * Number of retro reflector strips to pass before transitioning into braking
 *
 * @location states.c
 */
extern volatile int32_t RUN_RETRO_COUNT;

/**
 * Amount of time after initiaitng braking to begin checking for off-nominal pressure. Units us. 
 *
 * @location states.c
 */
extern volatile int32_t BRAKING_CHECK_PRESSURE_TIMEOUT;

/**
 * Amount of time after initiaitng braking to begin checking for off-nominal RMS. Units us. 
 *
 * @location states.c
 */
extern volatile int32_t BRAKING_CHECK_RMS_TIMEOUT;

/**
 * Amount of time after initiaitng braking to transition to the stopped state. Units us. 
 *
 * @location states.c
 */
extern volatile int32_t BRAKING_TRANSITION_STOPPED_TIMEOUT;

/**
 * Number of retro reflector strips to pass during crawl before transitioning.
 *
 * @location states.c
 */
extern volatile int32_t CRAWL_RETRO_COUNT;

/**
 * Maximum duration for crawl state, FSM transitions to post-run if exceeded. Units us.
 *
 * @location states.c
 */
extern volatile int32_t MAXIMUM_CRAWL_TIME;

/**
 * List of all software parameter pointers. Order must match 
 */
static volatile int32_t* PARAMETER_ARR[] = {
    &PUMPDOWN_TIMEOUT,
    &MAXIMUM_RUN_TIME,
    &RUN_RETRO_COUNT,
    &BRAKING_CHECK_PRESSURE_TIMEOUT,
    &BRAKING_CHECK_RMS_TIMEOUT,
    &BRAKING_TRANSITION_STOPPED_TIMEOUT,
    &CRAWL_RETRO_COUNT,
    &MAXIMUM_CRAWL_TIME
};

/**
 * Size of paramater pointer array
 */
static int32_t PARAMETER_ARR_COUNT = sizeof(PARAMETER_ARR) / sizeof(int32_t*);

#endif