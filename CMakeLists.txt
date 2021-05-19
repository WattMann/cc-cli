cmake_minimum_required(VERSION 3.19)

project(cc-cli VERSION 1.0 LANGUAGES CXX)

add_executable(cc-cli source/main.cpp source/include/CCApi.hpp source/CCApi.cpp)

string(
        APPEND _options
        "$<IF:$<CXX_COMPILER_ID:MSVC>,"
            "/W4;$<$<CONFIG:RELEASE>:/O2>,"
            "-Wall;-Wextra;-Werror;"
                "$<$<CONFIG:RELEASE>:-O3>"
                "$<$<CXX_COMPILER_ID:Clang>:-stdlib=libc++>"
        ">"
)

add_compile_options("${_options}")

set_target_properties(cc-cli PROPERTIES CMAKE_CXX_STANDARD 17)
target_include_directories(cc-cli PRIVATE source/include)

target_link_libraries(cc-cli PRIVATE curl)
target_link_libraries(cc-cli PRIVATE fmt)

install(TARGETS cc-cli DESTINATION bin)

