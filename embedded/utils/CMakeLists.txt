#Define exectutables
add_executable(gpioUtil gpioUtil.c)
add_executable(rmsProg rmsProg.c)

#Link library dependencies
target_link_libraries(gpioUtil PRIVATE drivers peripherals)
target_link_libraries(rmsProg PRIVATE drivers peripherals data)

#set output directory to out/utils
set_target_properties(gpioUtil rmsProg
                      PROPERTIES
                      RUNTIME_OUTPUT_DIRECTORY "${PROJECT_SOURCE_DIR}/out/utils")
