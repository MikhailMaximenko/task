cmake_minimum_required(VERSION 3.21)
project(computer-club)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED True)


add_subdirectory(lib)
add_subdirectory(src)
add_subdirectory(test)

add_executable(task main.cpp)
set(MAIN_PROJECT_SRC_LIST computer-club-util)

target_link_libraries(computer-club PUBLIC intrusive-list)
target_link_libraries(task PUBLIC computer-club)

target_include_directories(task PUBLIC "${PROJECT_BINARY_DIR}" "${PROJECT_SOURCE_DIR}/lib" "${PROJECT_SOURCE_DIR}/src")
