project(middlewarelib VERSION 1.0
                      DESCRIPTION "middleware"
                      LANGUAGES CXX)

add_library(middleware src/connStat.c
                       src/HVTCPSocket.cpp
                       src/data_dump.cpp
                       src/LVTCPSocket.cpp
                       libs/PracticalSocket/PracticalSocket.cpp
                       src/TelemetryLoop.cpp)

# Run Autocoder
add_dependencies(middleware autocoder)

target_include_directories(middleware PUBLIC include libs)

target_link_libraries(middleware PRIVATE data peripherals app drivers)

export(TARGETS middleware FILE MiddlewareConfig.cmake)

#examples executables
add_executable(LV_TCP_test examples/LV_TCP_test.cpp src/LVTCPSocket.cpp)

#link local includes
target_include_directories(LV_TCP_test PUBLIC include)

#link required libraries
target_link_libraries(LV_TCP_test PRIVATE data peripherals)

#put examples into out/tests
set_target_properties(LV_TCP_test
                      PROPERTIES
                      RUNTIME_OUTPUT_DIRECTORY "${PROJECT_SOURCE_DIR}/out/tests")
