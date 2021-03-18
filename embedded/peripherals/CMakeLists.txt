project(peripheralslib VERSION 1.0
                       DESCRIPTION "Peripherals"
                       LANGUAGES CXX)
#Define sources for library
add_library(peripherals src/batt.cpp
                        src/braking.c
                        src/hv_iox.c
                        src/lv_iox.c
                        src/lv_iox.c
                        src/NCD9830DBR2G.c
                        src/retro.c
                        src/bms.c
                        src/can_devices.c
                        src/imu.c
                        src/mcp23017.c
                        src/proc_iox.c
                        src/rms.c)

# Run Autocoder
add_dependencies(peripherals autocoder)

target_include_directories(peripherals PUBLIC include)

target_link_libraries(peripherals PRIVATE data app)
target_link_libraries(peripherals PUBLIC drivers)

export(TARGETS peripherals FILE PeripheralsConfig.cmake)
