cmake_minimum_required(VERSION 3.21)

project(computer-club-test)
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED True)
set(SOURCES type-tests.cpp parser-tests.cpp state-tests.cpp)
find_package(GTest REQUIRED)
add_executable(tests ${SOURCES})
target_link_libraries(tests PUBLIC computer-club GTest::gtest GTest::gtest_main)