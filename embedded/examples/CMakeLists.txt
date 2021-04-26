#Define example executables
add_executable(bmsDisplay bmsDisplay.cpp)
add_executable(dashTest dashTest.cpp)
add_executable(navTest navTest.cpp)
add_executable(brakingTest brakingTest.cpp)
add_executable(gpioIOXRickMortyTest gpioIOXRickMortyTest.cpp)
add_executable(oldMotorTest oldMotorTest.c)
add_executable(retroTest retroTest.c)
add_executable(stateTest stateTest.cpp)
add_executable(can_test can_test.c)
add_executable(imu_test imu_test.c)
add_executable(adcTest adcTest.cpp)

#Link library dependencies for each example
target_link_libraries(imu_test PRIVATE peripherals data)
target_link_libraries(can_test PRIVATE drivers peripherals data)
target_link_libraries(stateTest PRIVATE peripherals data app)
target_link_libraries(retroTest PRIVATE drivers peripherals data)
target_link_libraries(oldMotorTest PRIVATE drivers middleware peripherals data)
target_link_libraries(dashTest PRIVATE middleware peripherals data app)
target_link_libraries(navTest PRIVATE middleware peripherals data app)
target_link_libraries(brakingTest PRIVATE drivers middleware peripherals data)
target_link_libraries(bmsDisplay PRIVATE middleware peripherals data)
target_link_libraries(gpioIOXRickMortyTest PRIVATE drivers peripherals data)
target_link_libraries(adcTest PRIVATE drivers peripherals data)

# Run Autocoder
add_dependencies(imu_test autocoder)
add_dependencies(can_test autocoder)
add_dependencies(stateTest autocoder)
add_dependencies(retroTest autocoder)
add_dependencies(oldMotorTest autocoder)
add_dependencies(dashTest autocoder)
add_dependencies(navTest autocoder)
add_dependencies(brakingTest autocoder)
add_dependencies(bmsDisplay autocoder)
add_dependencies(gpioIOXRickMortyTest autocoder)
add_dependencies(adcTest autocoder)


#Make sure each executable generates in out/tests
set_target_properties(bmsDisplay
                      dashTest
                      navTest
                      brakingTest
                      gpioIOXRickMortyTest 
                      adcTest
                      oldMotorTest
                      retroTest
                      stateTest
                      can_test
                      imu_test
                      PROPERTIES
                      RUNTIME_OUTPUT_DIRECTORY ${PROJECT_SOURCE_DIR}/out/tests)
