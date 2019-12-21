cmake_minimum_required(VERSION 3.8)
project(NavToolsLib)

## setting the options ========================================================
option(NTL_STATIC "build static library libnavtools" ON)
option(NTL_SHARED "build shared library libnavtools" OFF)

## dependencies ===============================================================
# fmt
find_package(fmt REQUIRED)
set(CATCH_2_DIR ${CMAKE_CURRENT_SOURCE_DIR}/deps/catch)

## Sources and includes =======================================================
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

# Compilation of the library objects ==========================================
add_library(NavToolsLib OBJECT ${libnavtools_INCLUDE_FILE} ${libnavtools_SRC})
target_compile_features(NavToolsLib PRIVATE cxx_std_17)
target_compile_options(NavToolsLib PRIVATE -Wextra -Werror -Wpedantic)
target_link_libraries(NavToolsLib fmt::fmt)

# Creation of static library ==================================================
if (${NTL_STATIC})
    add_library(NavToolsLib_static STATIC $<TARGET_OBJECTS:NavToolsLib>)
    target_include_directories(
        NavToolsLib_static INTERFACE
        $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/hdr>
        $<INSTALL_INTERFACE:include>
    )
endif()

# Creation of shared library =================================================
if (${NTL_SHARED})
    add_library(NavToolsLib_shared SHARED $<TARGET_OBJECTS:NavToolsLib>)
    target_include_directories(
        NavToolsLib_shared INTERFACE
        $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/hdr>
        $<INSTALL_INTERFACE:include>
    )
endif()

## Tests ======================================================================
enable_testing()
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
    "$<BUILD_INTERFACE:${CATCH_2_DIR}>"
)

if (${NTL_STATIC})
    message(STATUS "Static library found... Linking tests with it")
    target_link_libraries(
        testNavToolsLib
        NavToolsLib_static
        fmt::fmt
    )
elseif(${NTL_SHARED})
    message(STATUS "Shared library found... Linking tests with it")
    target_link_libraries(
        testNavToolsLib
        NavToolsLib_shared
        fmt::fmt
    )
endif ()

# 1 test foreach tag
add_test(ntl::Latitude testNavToolsLib [latitude])
add_test(ntl::Longitude testNavToolsLib [longitude])
add_test(ntl::Point testNavToolsLib [point])
add_test(ntl::Rhumbline testNavToolsLib [rhumbline])
add_test(ntl::GES testNavToolsLib [GES])
add_test(ntl::Course testNavToolsLib [course])
