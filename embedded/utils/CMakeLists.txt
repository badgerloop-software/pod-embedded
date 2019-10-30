#Define exectutables
add_executable(gpioUtil gpioUtil.c)
add_executable(rmsProg rmsProg.c)

#Link library dependencies
target_link_libraries(gpioUtil PRIVATE drivers peripherals)
target_link_libraries(rmsProg PRIVATE drivers peripherals data)
