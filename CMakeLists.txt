cmake_minimum_required(VERSION 3.24)

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

project(
    "furryURL"
    LANGUAGES C CXX
    VERSION 0.0.1
)

set(CPM_VERSION 0.42.0 CACHE STRING "Version of CPM to download")
set(CPM_HASH eed8973c5595c72d9ba785a5472e9750 CACHE STRING "Hash of the specific version of CPM")

# Global Utilities

list(APPEND CMAKE_MODULE_PATH CMAKE_MODULE_PATH "${CMAKE_CURRENT_SOURCE_DIR}/cmake")

if (CMAKE_BUILD_TYPE STREQUAL "Debug" OR CMAKE_BUILD_TYPE STREQUAL "RelWithDebInfo")
    set(DEBUG ON)
else()
    set(DEBUG OFF)
endif()
set(DEBUG "${DEBUG}" CACHE BOOL "Whether to build debug libraries or not")

function(set_if_empty VARIABLE VALUE)
    if (NOT DEFINED "${VARIABLE}")
        set("${VARIABLE}" "${VALUE}" PARENT_SCOPE)
    endif()
endfunction()

# Init project

add_library("${PROJECT_NAME}")
add_subdirectory(src)

target_include_directories("${PROJECT_NAME}" PUBLIC include)
target_include_directories("${PROJECT_NAME}" PUBLIC ${CMAKE_BINARY_DIR}/include)

# Configure submodules

# Build libpsl
find_package(Libpsl QUIET)
if (NOT Libpsl_FOUND)
    find_package(Python 3.7 REQUIRED)

    execute_process(COMMAND ${Python_EXECUTABLE} -m ensurepip --upgrade)
    execute_process(COMMAND ${Python_EXECUTABLE} -m pip install meson>=0.60.0)

    include(libpsl)
endif()

# Add CPM
include(get_cpm)

CPMAddPackage(
    NAME curl
    VERSION 8.18.0
    GIT_REPOSITORY https://github.com/curl/curl.git
    GIT_TAG curl-8_18_0
)

target_include_directories("${PROJECT_NAME}" PUBLIC ${stb_SOURCE_DIR})

# Add libraries to project

target_link_libraries("${PROJECT_NAME}"
    libcurl
)