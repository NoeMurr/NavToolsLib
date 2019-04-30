cmake_minimum_required(VERSION 3.8)
project(NavToolsLib)

include(cmake/GitUtils.cmake)
enable_testing()

## setting the options --------------------------------------------------------
option(NTL_STATIC "build static library libnavtools" ON)
option(NTL_SHARED "build shared library libnavtools" OFF)

## dependencies ---------------------------------------------------------------
# fmt
find_package(fmt REQUIRED)

## Sources and includes -------------------------------------------------------
set(libnavtools_SRC
        ${CMAKE_CURRENT_SOURCE_DIR}/src/Longitude.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/src/DegreeValue.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/src/Latitude.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/src/Point.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/src/rhumbline.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/src/Course.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/src/Ellipsoid.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/src/GES.cpp
)

set(libnavtools_INCLUDE_FILE
        ${CMAKE_CURRENT_SOURCE_DIR}/hdr/navtools/Longitude.hpp
        ${CMAKE_CURRENT_SOURCE_DIR}/hdr/navtools/DegreeValue.hpp
        ${CMAKE_CURRENT_SOURCE_DIR}/hdr/navtools/Latitude.hpp
        ${CMAKE_CURRENT_SOURCE_DIR}/hdr/navtools/utils/mathUtils.hpp
        ${CMAKE_CURRENT_SOURCE_DIR}/hdr/navtools/Point.hpp
        ${CMAKE_CURRENT_SOURCE_DIR}/hdr/navtools/rhumbline.hpp
        ${CMAKE_CURRENT_SOURCE_DIR}/hdr/navtools/Course.hpp
        ${CMAKE_CURRENT_SOURCE_DIR}/hdr/navtools/Ellipsoid.hpp
        ${CMAKE_CURRENT_SOURCE_DIR}/hdr/navtools/GES.hpp
)

## checking if the library should be compiled ---------------------------------
if (NOT NTL_STATIC AND NOT NTL_SHARED)
    message(
        SEND_ERROR
        "Neither NTL_STATIC or NTL_SHARED has beed set to ON,"
        " skipping NavToolsLib"
    )
    return()
endif ()

# adding object lib
add_library(NavToolsLib OBJECT ${libnavtools_INCLUDE_FILE} ${libnavtools_SRC})
target_compile_features(NavToolsLib PRIVATE cxx_std_17)
target_compile_options(NavToolsLib PRIVATE -Wextra -Werror)
target_link_libraries(NavToolsLib fmt::fmt)

if (NTL_STATIC)
    add_library(NavToolsLib_static STATIC $<TARGET_OBJECTS:NavToolsLib>)
    target_include_directories(
        NavToolsLib_static INTERFACE
        $<INSTALL_INTERFACE:hdr/navtools>
    )
endif()

if (NTL_SHARED)
    add_library(NavToolsLib_shared SHARED $<TARGET_OBJECTS:NavToolsLib>)
    target_include_directories(
        NavToolsLib_shared INTERFACE
        $<INSTALL_INTERFACE:hdr/navtools>
    )
endif()

## create tests
add_executable(testNavToolsLib
        ${CMAKE_CURRENT_SOURCE_DIR}/tests/main-test.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/tests/Latitude-test.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/tests/Longitude-test.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/tests/Point-test.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/tests/rhumbline-test.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/tests/Course-test.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/tests/GES-test.cpp
)
target_include_directories(
    testNavToolsLib PRIVATE
    "$<BUILD_INTERFACE:${NavToolsLib_INCLUDE_DIRS}>"
    "$<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/deps/catch>"
)

target_link_libraries(
    testNavToolsLib ${NavToolsLib_LIBS}
    fmt::fmt
)
add_test(NavToolsLibTest testNavToolsLib)