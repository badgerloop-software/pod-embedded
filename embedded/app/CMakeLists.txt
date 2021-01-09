#setup library for the stuff in src/ because it's used elsewhere >_>
project(applib VERSION 1.0
               DESCRIPTION "main"
               LANGUAGES CXX)

#yeah, it's annoying, but glob has problems with needing to run cmake again,
#and it's SUPER discouraged.
set(dependecies src/bms_fault_checking.c
                src/nav.cpp
                src/state_machine.c
                src/init.c
                src/pressure_fault_checking.c
                src/states.c
                src/motor.c
                src/rms_fault_checking.c
                src/transitions.c)

#required libraries for the main pod executables
set(libraries middleware 
              data 
              peripherals 
              drivers
              Threads::Threads) #so pthreads work

#configure app library
add_library(app ${dependecies})
target_include_directories(app PUBLIC include)
target_link_libraries(app PRIVATE ${libraries})

# Run Autocoder
add_dependencies(app autocoder)

#export app library so it can be used externally
export(TARGETS app FILE AppLibConfig.cmake)

#configure executable
find_package(Threads REQUIRED)
add_executable(badgerloop main/badgerloop.cpp ${dependecies})
target_link_libraries(badgerloop PRIVATE ${libraries})
target_include_directories(badgerloop PRIVATE include)
