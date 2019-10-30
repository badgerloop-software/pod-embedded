project(middlewarelib VERSION 1.0
                      DESCRIPTION "middleware"
                      LANGUAGES CXX)

add_library(middleware src/connStat.c
                       src/HVTCPSocket.cpp
                       src/HV_Telem_Recv.cpp
                       src/LVTelemetry_Loop.cpp
                       src/data_dump.cpp
                       src/HVTelemetry_Loop.cpp
                       src/LVTCPSocket.cpp
                       src/PracticalSocket.cpp)

target_include_directories(middleware PUBLIC include include/jsonlib)

target_link_libraries(middleware PRIVATE data peripherals app drivers)

export(TARGETS middleware FILE MiddlewareConfig.cmake)

#examples executables
add_executable(HV_Test examples/HV_Test.cpp)
add_executable(LV_DataLoop_test examples/LV_DataLoop_test.cpp)
add_executable(LV_TCP_test examples/LV_TCP_test.cpp src/LVTCPSocket.cpp)

#link local includes
target_include_directories(HV_Test PUBLIC include)
target_include_directories(LV_DataLoop_test PUBLIC include)
target_include_directories(LV_TCP_test PUBLIC include)

#link required libraries
target_link_libraries(HV_Test PRIVATE middleware peripherals data)
target_link_libraries(LV_DataLoop_test PRIVATE middleware peripherals)
target_link_libraries(LV_TCP_test PRIVATE data peripherals)
