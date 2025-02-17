project(intrusive-lib)

set(HEADERS intrusive_list.h)
set(SOURCES intrusive_list.cpp)

add_library(intrusive-list ${SOURCES} ${HEADERS})
target_include_directories(intrusive-list PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})
