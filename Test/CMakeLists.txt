project(testlib VERSION 1.0
                DESCRIPTION "Tests"
                LANGUAGES CXX)

# Define sources
set(dependencies example/Test_GTest.cpp)
set(dependencies components/HV_Iox/Test_HV_Iox.cpp)
set(dependencies example/Test_GTest.cpp
                 run_profiles/Test_load_software_parameters.cpp)

set(libraries app
              middleware 
              data 
              peripherals 
              drivers
              Threads::Threads) #so pthreads work
                
# Configure tests library                
add_library(tests ${dependencies})
target_link_libraries(tests PRIVATE ${libraries})
target_include_directories(tests PUBLIC include)

# Run Autocoder
add_dependencies(tests autocoder)

export(TARGETS tests FILE TestsConfig.cmake)
                

# Locate Requirements
set(GTest_ROOT /usr/src/googletest/googletest)
set(GTEST_INCLUDE_DIRS ${GTest_ROOT}/include/gtest)
set(GTEST_LIBRARIES ${GTest_ROOT}/libgtest.a)
add_subdirectory(${GTest_ROOT}
    "${CMAKE_CURRENT_BINARY_DIR}/googletest" EXCLUDE_FROM_ALL) 

# find_package(GTest REQUIRED)
find_package(Threads REQUIRED)
include_directories(${GTEST_INCLUDE_DIRS})


# Run Autocoder
add_dependencies(tests autocoder)


# Add executable
add_executable(run_all_tests run_all_tests.cpp ${dependencies})

message(STATUS "***********IMPORTANT RIGHT HERE***************"${GTEST_LIBRARIES})
target_link_libraries(run_all_tests ${GTEST_LIBRARIES} pthread ${libraries})
target_include_directories(app PUBLIC include)



