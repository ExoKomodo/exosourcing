cmake_minimum_required(VERSION 3.31)
project(tests)

set(SOURCE_FILES
    test_event.cpp
    test_store.cpp
    test_stream.cpp
)
set(SOURCE_DIR_catch2 lib/catch2)
set(INCLUDE_DIR_catch2 ${SOURCE_DIR_catch2}/src)

include_directories(${INCLUDE_DIR_exo})
include_directories(${INCLUDE_DIR_catch2})

# Testing
enable_testing()

add_test(NAME cli.has_help COMMAND tests --help)

add_subdirectory(${SOURCE_DIR_catch2})
include(${SOURCE_DIR_catch2}/extras/Catch.cmake)
add_executable(tests ${SOURCE_FILES})
target_link_libraries(
    tests
    PUBLIC exo
    PRIVATE Catch2::Catch2WithMain)
catch_discover_tests(
    tests
    TEST_PREFIX
    "unittests."
    REPORTER
    XML
    OUTPUT_DIR
    .
    OUTPUT_PREFIX
    "unittests."
    OUTPUT_SUFFIX
    .xml)

install(TARGETS tests DESTINATION bin)
