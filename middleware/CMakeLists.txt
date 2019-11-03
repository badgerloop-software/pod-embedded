project(middlewarelib VERSION 1.0
                      DESCRIPTION "middleware"
                      LANGUAGES CXX)

add_library(middleware src/connStat.c
                       src/HVTCPSocket.cpp
                       src/data_dump.cpp
                       src/LVTCPSocket.cpp
                       libs/PracticalSocket/PracticalSocket.cpp
                       src/TelemetryLoop.cpp src/TCPServer.cpp include/TCPServer.h)

# Run Autocoder
add_dependencies(middleware autocoder)

target_include_directories(middleware PUBLIC include libs)

target_link_libraries(middleware PRIVATE data peripherals app drivers)

export(TARGETS middleware FILE MiddlewareConfig.cmake)

#examples executables
add_executable(LV_TCP_test examples/LV_TCP_test.cpp src/LVTCPSocket.cpp)
add_executable(TelemetryLoopTest examples/TelemetryLoopTest.cpp src/TelemetryLoop.cpp ../embedded/app/src/init.c)
add_executable(TCPServerTest examples/TCPServerTest.cpp src/TCPServer.cpp)

# Set test definitions
target_compile_definitions(TelemetryLoopTest PUBLIC TELEMETRY_LOOP_TEST)
target_compile_definitions(TCPServerTest PUBLIC TCPSERVER_TEST)

#link local includes
target_include_directories(LV_TCP_test PUBLIC include libs)
target_include_directories(TelemetryLoopTest PUBLIC include libs)
target_include_directories(TCPServerTest PUBLIC include libs)

#link required libraries
target_link_libraries(LV_TCP_test PRIVATE data peripherals)
target_link_libraries(TelemetryLoopTest PRIVATE data peripherals)
target_link_libraries(TCPServerTest PRIVATE data peripherals app)

#put examples into out/tests
set_target_properties(LV_TCP_test
                      PROPERTIES
                      RUNTIME_OUTPUT_DIRECTORY "${PROJECT_SOURCE_DIR}/out/tests")
set_target_properties(TelemetryLoopTest
        PROPERTIES
        RUNTIME_OUTPUT_DIRECTORY "${PROJECT_SOURCE_DIR}/out/tests")
set_target_properties(TCPServerTest
        PROPERTIES
        RUNTIME_OUTPUT_DIRECTORY "${PROJECT_SOURCE_DIR}/out/tests")
