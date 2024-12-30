cmake_minimum_required(VERSION 3.31)
project(exo)

set(SOURCE_FILES main.cpp)

add_subdirectory(exo)

add_executable(demo ${SOURCE_FILES})
target_include_directories(demo PUBLIC include)
target_link_libraries(demo exo)
