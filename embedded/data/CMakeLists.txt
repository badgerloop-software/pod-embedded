project(datalib VERSION 1.0
                DESCRIPTION "data"
                LANGUAGES CXX)

add_library(data src/filters.c)

target_include_directories(data PUBLIC include)

export(TARGETS data FILE DataConfig.cmake)
