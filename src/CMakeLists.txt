set(HEADERS event.h computer_club.h parser.h state.h types.h)
set(SOURCES event.cpp computer_club.cpp parser.cpp state.cpp)

add_library(computer-club ${HEADERS} ${SOURCES})
target_include_directories(computer-club PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})
