cmake_minimum_required(VERSION 3.31)

project(exosourcing
  LANGUAGES CXX C)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
set(CMAKE_INSTALL_PREFIX ${PROJECT_SOURCE_DIR})

set(INCLUDE_DIR_exo ${PROJECT_SOURCE_DIR}/include)

add_subdirectory(src)

# Don't even look at tests if we're not top level
if(NOT PROJECT_IS_TOP_LEVEL)
  return()
endif()
include(CTest)
add_subdirectory(test)
