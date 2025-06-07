set(CMAKE_CXX_STANDARD 20)

add_executable(server "server.cc")
target_link_libraries(server core base pthread)

add_executable(client "client.cc")
target_link_libraries(client core base pthread)
