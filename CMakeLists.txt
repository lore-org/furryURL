include(CheckCXXCompilerFlag)
include(CheckCCompilerFlag)
include(CheckLinkerFlag)
include(CheckCXXSourceCompiles)
include(CheckIncludeFile)

cmake_minimum_required(VERSION 3.24)

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

# ----- Options -----

project(
    "furryURL"
    LANGUAGES C CXX
    VERSION 0.0.1
)

set(CPM_VERSION 0.42.0 CACHE STRING "Version of CPM to download")
set(CPM_HASH eed8973c5595c72d9ba785a5472e9750 CACHE STRING "Hash of the specific version of CPM")

# -------------------

# Non-configurable variables

set(CMAKE_MODULE_PATH "${CMAKE_CURRENT_SOURCE_DIR}/cmake;${CMAKE_MODULE_PATH}")

# sanitise project name
string(TOLOWER "${PROJECT_NAME}" SANITISED_PROJECT_NAME)
string(REGEX REPLACE "[^a-zA-Z0-9_\\.+-]" "_" SANITISED_PROJECT_NAME "${SANITISED_PROJECT_NAME}")

# sanitise name
string(TOLOWER "${APP_NAME}" SANITISED_APP_NAME)
string(REGEX REPLACE "[^a-zA-Z0-9_\\.+-]" "_" SANITISED_APP_NAME "${SANITISED_APP_NAME}")

# sanitise namespace
string(TOLOWER "${APP_NAMESPACE}" SANITISED_APP_NAMESPACE)
string(REGEX REPLACE "[^a-zA-Z0-9_\\$\\.]" "_" SANITISED_APP_NAMESPACE "${SANITISED_APP_NAMESPACE}")

math(EXPR APP_VERSION_CODE "((${PROJECT_VERSION_MAJOR}+0) * 50000000) + ((${PROJECT_VERSION_MINOR}+0) * 1000000) + ((${PROJECT_VERSION_PATCH}+0) * 10000) + ((${PROJECT_VERSION_TWEAK}+0) * 1)")

file(REAL_PATH "~" HOME_DIR EXPAND_TILDE)
file(TO_CMAKE_PATH ${HOME_DIR} HOME_DIR)

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

# Download CPM

set_if_empty(CPM_VERSION 0.42.0)
set_if_empty(CPM_HASH eed8973c5595c72d9ba785a5472e9750)

set(_cpm_file "${CMAKE_BINARY_DIR}/get_cpm.cmake")

if (EXISTS "${_cpm_file}")
    file(MD5 "${_cpm_file}" CURRENT_CPM_HASH)

    if (CURRENT_CPM_HASH STREQUAL CPM_HASH)
        set(CURRENT_CPM_VALID TRUE)
        message(STATUS "Found existing CPM v${CPM_VERSION} (MD5: ${CPM_HASH})")
    endif()
endif()

if (NOT CURRENT_CPM_VALID)
    message(STATUS "Downloading CPM v${CPM_VERSION} (MD5: ${CPM_HASH})")
    file(DOWNLOAD
        "https://github.com/cpm-cmake/CPM.cmake/releases/download/v${CPM_VERSION}/get_cpm.cmake"
        "${_cpm_file}"
        EXPECTED_HASH "MD5=${CPM_HASH}"
        SHOW_PROGRESS
    )
endif()

include("${_cpm_file}")

# Init project

add_library("${SANITISED_PROJECT_NAME}")
add_subdirectory(src)

target_include_directories("${SANITISED_PROJECT_NAME}" PUBLIC include)
target_include_directories("${SANITISED_PROJECT_NAME}" PUBLIC ${CMAKE_BINARY_DIR}/include)

# Configure submodules

# Build libpsl
find_package(Libpsl QUIET)
if (NOT Libpsl_FOUND)
    find_package(Python REQUIRED)

    execute_process(COMMAND ${Python_EXECUTABLE} -m ensurepip --upgrade)
    execute_process(COMMAND ${Python_EXECUTABLE} -m pip install meson>=0.60.0)

    include(libpsl)
endif()

CPMAddPackage(
    NAME curl
    VERSION 8.18.0
    GIT_REPOSITORY https://github.com/curl/curl.git
    GIT_TAG curl-8_18_0
)

target_include_directories("${SANITISED_PROJECT_NAME}" PUBLIC ${stb_SOURCE_DIR})

# Add libraries to project

target_link_libraries("${SANITISED_PROJECT_NAME}"
    libcurl
)