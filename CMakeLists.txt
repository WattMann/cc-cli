cmake_minimum_required(VERSION 3.16)
project(cc-cli)

set (CC_CLI_VERSION_MAJOR 2)
set (CC_CLI_VERSION_MINOR 1)
set (CC_CLI_PLATFORM "unknown")
list(APPEND CMAKE_MODULE_PATH ${CMAKE_BINARY_DIR})
list(APPEND CMAKE_PREFIX_PATH ${CMAKE_BINARY_DIR})

if(NOT EXISTS "${CMAKE_BINARY_DIR}/cmake/conan.cmake")
   message(STATUS "Downloading conan.cmake from https://github.com/conan-io/cmake-conan")
   file(DOWNLOAD "https://raw.githubusercontent.com/conan-io/cmake-conan/master/conan.cmake"
                  "${CMAKE_BINARY_DIR}/cmake/conan.cmake")
endif()

include("${CMAKE_BINARY_DIR}/cmake/conan.cmake")

if (UNIX)
  if (APPLE)
    set(CC_CLI_PLATFORM "mac")
  else ()
    set(CC_CLI_PLATFORM "linux")
  endif ()
elseif (CMAKE_SYSTEM_NAME MATCHES "Windows")
    set(CC_CLI_PLATFORM "win")
else ()
  message(FATAL_ERROR "Unsupported OS '${CMAKE_SYSTEM_NAME}'")
endif ()


conan_cmake_configure(REQUIRES fmt/7.1.3 openssl/1.1.1k libcurl/7.77.0 nlohmann_json/3.9.1
                      GENERATORS cmake_find_package)
conan_cmake_autodetect(settings)
conan_cmake_install(PATH_OR_REFERENCE .
                    BUILD missing
                    REMOTE conan-center
                    SETTINGS ${settings})
find_package(fmt)
find_package(OpenSSL)
find_package(CURL)
find_package(nlohmann_json)

link_libraries(fmt::fmt OpenSSL::OpenSSL CURL::CURL nlohmann_json::nlohmann_json)

configure_file(${PROJECT_SOURCE_DIR}/source/Version.hpp.in ${PROJECT_SOURCE_DIR}/source/Version.hpp)
include_directories(cc-cli "${PROJECT_BINARY_DIR}")

add_executable(cc-cli source/main.cpp
					  source/api/CCApi.cpp
					  source/Version.hpp
					  source/api/CCApi.hpp)

if (MSVC)
	set(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} /MT")
	set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} /MTd")
	set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} /Wall /permissive-")
	
	set_target_properties(cc-cli PROPERTIES LINK_FLAGS "/ENTRY:mainCRTStartup /SUBSYSTEM:console")
else()
    add_compile_options("-Wall -Wextra -Werror -Wpedantic")
endif()


set_target_properties(cc-cli PROPERTIES CMAKE_CXX_STANDARD 20)
target_include_directories(cc-cli PRIVATE source/include)

install(TARGETS cc-cli DESTINATION bin)
