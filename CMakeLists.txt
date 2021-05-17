cmake_minimum_required(VERSION 3.19)
project(cc-cli)

set(CMAKE_CXX_STANDARD 17)
add_executable(cc-cli source/main.cpp source/include/CCApi.hpp source/CCApi.cpp)

target_include_directories(cc-cli PRIVATE source/include)

target_link_libraries(cc-cli curl)
target_link_libraries(cc-cli fmt)
