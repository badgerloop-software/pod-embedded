cmake_minimum_required(VERSION 3.1)

project(PodEmbedded VERSION 1.0
                    DESCRIPTION "Pod embedded")

# Run Autocoder when CMake is run
execute_process(COMMAND python ${PROJECT_SOURCE_DIR}/autocoding/autocode.py)

# Autocoder target
add_custom_target(autocoder COMMAND python ${PROJECT_SOURCE_DIR}/autocoding/autocode.py)

#put binaries in out/
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${PROJECT_SOURCE_DIR}/out/lib)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${PROJECT_SOURCE_DIR}/out/lib)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${PROJECT_SOURCE_DIR}/out)

set(CMAKE_CXX_STANDARD 11)

# Add cmake projects from subdirectories
add_subdirectory(embedded/app)
add_subdirectory(embedded/data)
add_subdirectory(embedded/drivers)
add_subdirectory(embedded/examples)
add_subdirectory(embedded/peripherals)
add_subdirectory(embedded/utils)
add_subdirectory(middleware)
add_subdirectory(Test)