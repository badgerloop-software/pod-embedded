#ifndef GTEST_GLOBALS_H
#define GTEST_GLOBALS_H

/**
 * Executable path - Pulled from argv[0] and is used for parameter loading in
 * run_profiles/t_load_software_parameters.cpp
 * 
 * Instantiated in run_all_tests.cpp
*/
extern char executable_path[128];

#endif