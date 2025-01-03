cmake_minimum_required(VERSION 3.31)
project(exo C CXX)

set(SOURCE_FILES
    store.cpp
)

add_library(exo SHARED STATIC ${SOURCE_FILES})
target_include_directories(
    exo
    PUBLIC ${INCLUDE_DIR_exo} ${INCLUDE_DIR_stduuid} ${INCLUDE_DIR_stduuid}/include)
