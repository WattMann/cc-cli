cmake_minimum_required(VERSION 3.19)
project(cc-cli)
set (CC_CLI_VERSION_MAJOR 1)
set (CC_CLI_VERSION_MINOR 0 )

configure_file(${PROJECT_SOURCE_DIR}/source/Version.h.in ${PROJECT_SOURCE_DIR}/source/Version.h)
include_directories(cc-cli "${PROJECT_BINARY_DIR}")

add_executable(cc-cli source/main.cpp source/api/CCApi.cpp source/Version.h source/api/CCApi.hpp)
add_compile_options("-Wall -Wextra -Werror -pedantic")

set_target_properties(cc-cli PROPERTIES CMAKE_CXX_STANDARD 17)
target_include_directories(cc-cli PRIVATE source/include)

target_link_libraries(cc-cli PRIVATE curl)
target_link_libraries(cc-cli PRIVATE fmt)

install(TARGETS cc-cli DESTINATION bin)

