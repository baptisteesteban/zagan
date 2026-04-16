cmake_minimum_required(VERSION 4.0)
project(Zagan)

option(ZAGAN_BUILD_TESTS "Build the tests for the Zagan library" ON)

add_subdirectory(zagan)

if (ZAGAN_BUILD_TESTS)
    enable_testing()
    add_subdirectory(tests)
endif(ZAGAN_BUILD_TESTS)