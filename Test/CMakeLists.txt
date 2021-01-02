project(testlib VERSION 1.0
                DESCRIPTION "Tests"
                LANGUAGES CXX)

# Define sources
set(dependencies example/t_test.cpp
                 run_profiles/t_load_software_parameters.cpp)

set(libraries app
              middleware 
              data 
              peripherals 
              drivers
              Threads::Threads) #so pthreads work
                
# Configure tests library                
add_library(tests ${dependencies})
target_link_libraries(tests PRIVATE ${libraries})

# Run Autocoder
add_dependencies(tests autocoder)

export(TARGETS tests FILE TestsConfig.cmake)
                

# Locate Requirements
find_package(GTest REQUIRED)
find_package(Threads REQUIRED)
include_directories(${GTEST_INCLUDE_DIRS})


# Run Autocoder
add_dependencies(tests autocoder)


# Add executable
add_executable(run_all_tests run_all_tests.cpp ${dependencies})

target_link_libraries(run_all_tests ${GTEST_LIBRARIES} pthread ${libraries})
target_include_directories(app PUBLIC include)



