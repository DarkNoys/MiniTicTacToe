project(tictactoe)

set(CMAKE_INSTALL_PREFIX ${CMAKE_CURRENT_SOURCE_DIR}/../)

include_directories(${CMAKE_CURRENT_BINARY_DIR} ${CMAKE_CURRENT_SOURCE_DIR}/header)


file(GLOB SERV "server/*.cpp")
file(GLOB CLIE "client/*.cpp")
file(GLOB TTTE "tictactoe/*.cpp")

add_definitions(-Wall -O2)

add_executable(client ${CLIE} ${TTTE})
target_link_libraries(client ncurses)	


add_executable(server ${SERV})
target_link_libraries(server pthread)	


