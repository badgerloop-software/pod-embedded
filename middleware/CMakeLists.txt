project(middlewarelib VERSION 1.0
                      DESCRIPTION "middleware"
                      LANGUAGES CXX)

add_library(middleware src/connStat.c
                       src/HVTCPSocket.cpp
                       src/LVTCPSocket.cpp
                       src/DataDump.cpp
                       libs/PracticalSocket/PracticalSocket.cpp
                       src/TelemetryLoop.cpp)

# Run Autocoder
add_dependencies(middleware autocoder)

target_include_directories(middleware PUBLIC include libs)

target_link_libraries(middleware PRIVATE data peripherals app drivers)

export(TARGETS middleware FILE MiddlewareConfig.cmake)

#examples executables
add_executable(LV_TCP_test examples/LV_TCP_test.cpp src/LVTCPSocket.cpp)
add_executable(TelemetryLoopTest examples/TelemetryLoopTest.cpp src/TelemetryLoop.cpp ../embedded/app/src/init.c src/DataDump.cpp include/DataDump.h)

# Set test definitions
target_compile_definitions(TelemetryLoopTest PUBLIC TELEMETRY_LOOP_TEST)

#link local includes
target_include_directories(LV_TCP_test PUBLIC include libs)
target_include_directories(TelemetryLoopTest PUBLIC include libs)

#link required libraries
target_link_libraries(LV_TCP_test PRIVATE data peripherals)
target_link_libraries(TelemetryLoopTest PRIVATE data peripherals)

#put examples into out/tests
set_target_properties(LV_TCP_test
                      PROPERTIES
                      RUNTIME_OUTPUT_DIRECTORY "${PROJECT_SOURCE_DIR}/out/tests")
set_target_properties(TelemetryLoopTest
        PROPERTIES
        RUNTIME_OUTPUT_DIRECTORY "${PROJECT_SOURCE_DIR}/out/tests")
