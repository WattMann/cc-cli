cmake_minimum_required(VERSION 3.19)
project(cc_cli)

set(CMAKE_CXX_STANDARD 17)
include_directories(cc_cli source/include)
add_executable(cc_cli source/main.cpp source/include/CCApi.hpp source/include/CCApi.cpp)

target_link_libraries(cc_cli curl)
target_link_libraries(cc_cli fmt)
