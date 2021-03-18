# How to use CMake 

This document gives an overview of the basics of cmake and how to use
it to make libraries and executables for `pod-embedded`.

## Cmake basics

CMake projects are specified using a `CMakeFiles.txt` in each
library/exectutable directory.

### Basic CMakeFiles.txt layout
A top level CMakeFiles.txt always contains a `cmake_minimum_required`
and a `project` declaration. The rest is populated with `add_subdirectory`,
calls, which include CMakeFiles.txts from subdirectories.

### Variables
CMake varaibles are specified using `set(VARIABLE_NAME value...)`, which
sets the variable `VARIABLE_NAME` to any subsequent values (multiple values
will make a list.)

Eg:

```
#set importantDirectory to /usr/local/bin
set(importantDirectory /usr/local/bin) 

#set libraries to list containing middleware, drivers, utils
set(libraries middleware 
              drivers 
              utils) 
```

The fundametnal primitives when defining a cmake project are
executables and libraries. 

## Making a library

To create a library that is imported by other executables, we use the following

```
#Define the project for the library
project(foobarlib VERSION 1.0
                  DESCRIPTION "The foobar library"
                  LANGUAGES CXX)

#Define the library target called foobar with all of its source directories
#Files specified with respect to CMakeLists.txt for the library
add_library(foobar src/somefile.c
                   src/bin.c
                   src/baz.c)

#Add any header files by specifying the include directory, in this case 
#called include
target_include_directories(foobar PUBLIC include)

#add any library dependencies
target_link_libraries(foobar PUBLIC someotherlibrary verynicedependency)
target_link_libraries(foobar PRIVATE thisonelibrary)

#export library to cmake file so it can be imported elsewhere
export(TARGETS foobar FILE FoobarConfig.cmake)
```

In this example we define a project called `foobarlib` in its own CMakeLists.txt
and define a library called `foobar` with its respective source files.
Then any header files are added by specifying an include directory with `target_include_directories(...)`. Then we link any other libraries `foobar` depends on with `target_link_libraries(...)`. Finally we export the library using `export(...)` so the library can be imported elsewhere in the project.

### PUBLIC vs PRIVATE vs INTERFACE
Whenever a dependency is specified for a project, you must specify if that dependency
is `PUBLIC`, `PRIVATE` or `INTERFACE`. This changes the availability of the dependencies
you're adding to any project that depends on the library you're writing.

For example, if library `foo` depends on library `bar` publically, then
any library that depends on `foo` will be linked with headers from `bar`.

The rule of thumb is:

 * If your source files depend on the library, make it `PRIVATE`.
 * If your header files depend on the library, make it `INTERFACE`
 * If both of the above are true, make it `PUBLIC`

Dependencies for executables should always be `PRIVATE`.

## Making an executable

To make an executable, simply use

```
add_executable(binaryName, sourceFile1.c
                           sourceFile2.c
                           sourceFileN.c) 
```

and specify includes and dependencies with `target_include_directories(...)` and `target_link_libraries(...)` in the same way as with libraries.

