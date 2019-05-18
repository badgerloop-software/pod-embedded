# Badgerloop Pod IV
[![Build Status](https://travis-ci.org/badgerloop-software/pod.svg?branch=master)](https://travis-ci.org/badgerloop-software/pod)

*Developers: Ezra Boley, Eric Udlis, Rohan Daruwala, Luke Houge, Alex Vesel*

The master repository for Badgerloop's pod in the Hyperloop Competition

## Beaglebone Make Instructions

There are currently two sets of targets:

1) Making the main programs (`badgerloop_LV` and `badgerloop_HV`), placed in the `out/` folder

```
make
```

2) Making the examples, placed into the `out/tests` folder

```
make examples
```

### Adding Tests

In order to add a test or example, put a .c or .cpp file into the respective "example" folder. Make sure that your file contains a "main" function.

## Directory Structure
```
pod/
 |
 |--embedded/
 |  |--app/          
 |  |   |--main/         /* Contains the two main source files, one for each module */
 |  |   |--src/          /* General application source code (mostly the state machine) */
 |  |   |--include/      /* Headers for application code */
 |  |
 |  |--drivers/
 |  |  |--src/           /* Protocol specific functions (e.g. CAN or I2C) */
 |  |  |--include/       /* Headers defining our low level API */
 |  |
 |  |--peripherals/     
 |  |  |--src/           /* Device specific functions (e.g. for the Battery Management System) */
 |  |  |--include/       /* Headers defining our device level API */
 |  |
 |  |--examples/         /* Tests and utilities for using both low level and device drivers */
 |  |  |--sims/          /* Experimental python simulators for mimicing pod functionality */
 |  
 |--middleware/
 |  |--src/              /* Source files for the UDP and TCP servers that run on the pod */
 |  |--include/          /* Network API for transmitting data between modules and off the pod */
 |  |--examples/         /* Examples for testing the networking functionality */
 |
 ```
 
 ### Structural Conventions
 
 In order to ensure the program is built and linked correctly, there are a couple basic rules that should be followed. 
 
  1. Only .c and .cpp files should live in any given `src/` directory, and only .h files belong in the `include/` directories. Any other supporting files (e.g. READMEs) belong in at least one level above `src/` and `include/`. 
  2. The only source files containing a `main()` function should live either in `pod/embedded/app/main` or in any of the examples subdirectories. 
  3. Only two source files should live in the `pod/embedded/app/main` directory: `badgerloop_HV.cpp` and `badgerloop_LV.cpp`. If any other code is placed there it will either link incorrectly or fail to be compiled at all.
  
### Coding Conventions

- camelCase for function names and local variable names
- ALL_CAPS_WITH_UNDERSCORES for #defines and constants
- Make any function that will not be part of the API (helper functions) `static` so that it stays local to that file
- Avoid magic numbers and string constants when at all possible
- Comment on any parts of your code that could be considered difficult to understand at a glance
- Add comments on top of functions that are going to be a part of the interface others use to interact with your code
- Avoid global variables if possible
