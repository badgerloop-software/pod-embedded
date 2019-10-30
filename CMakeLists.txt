cmake_minimum_required(VERSION 3.1)

project(PodEmbedded VERSION 1.0
                    DESCRIPTION "Pod embedded")

add_subdirectory(embedded/app)
add_subdirectory(embedded/data)
add_subdirectory(embedded/drivers)
add_subdirectory(embedded/examples)
add_subdirectory(embedded/peripherals)
add_subdirectory(embedded/utils)
add_subdirectory(middleware)
