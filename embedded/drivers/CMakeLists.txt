project(driverslib VERSION 1.0
                   DESCRIPTION "Drivers for the pod"
                   LANGUAGES C)

add_library(drivers src/bbgpio.c
                    src/can.cpp
                    src/i2c.c)

# Run Autocoder
add_dependencies(drivers autocoder)

target_include_directories(drivers PUBLIC include)

export(TARGETS drivers FILE DriversConfig.cmake)
