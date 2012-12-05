#==============================================================================
# Copyright (c) 2011 Evgeny Proydakov <lord.tiran@gmail.com>
#==============================================================================
# Welcome to the CMake build system for RDO STUDIO (UNIX VERSION).
# This is the main file where we prepare the general build environment
# and provide build configuration options.
#==============================================================================

CMAKE_MINIMUM_REQUIRED(VERSION 2.6.0)

PROJECT(rdo)

MESSAGE(STATUS "Create RDO")

# work with cmake
SET(CMAKE_MODULE_PATH ${PROJECT_SOURCE_DIR}/cmake)

# call system info script
INCLUDE(FindOS)
INCLUDE(add_boost_test)

IF(NOT MSVC)
    SET(CMAKE_C_FLAGS   "${CMAKE_C_FLAGS}   -Wall")
    SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall")
ELSE()
    STRING(REGEX REPLACE /W[0-4] /W4 CMAKE_CXX_FLAGS ${CMAKE_CXX_FLAGS})
ENDIF(NOT MSVC)

IF(NOT MSVC)
    SET(CMAKE_C_FLAGS   "${CMAKE_C_FLAGS}   -fPIC")
    SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fPIC -std=c++0x")
ENDIF(NOT MSVC)

IF(NOT CMAKE_BUILD_TYPE)
    SET(CMAKE_BUILD_TYPE Debug)
ENDIF(NOT CMAKE_BUILD_TYPE)

IF(NOT MSVC)
    IF(NOT BUILD_SHARED_LIBS)
        SET(BUILD_SHARED_LIBS ON)
    ENDIF(NOT BUILD_SHARED_LIBS)
#   SET(CDEFS "-m32 -o output32")
ENDIF(NOT MSVC)

SET(LIBRARY_OUTPUT_PATH ${CMAKE_CURRENT_BINARY_DIR})
SET(EXECUTABLE_OUTPUT_PATH ${CMAKE_CURRENT_BINARY_DIR})

# add the include folder to the include path
INCLUDE_DIRECTORIES(${PROJECT_SOURCE_DIR})

INCLUDE(CTest)
ENABLE_TESTING()

IF(MSVC)
    SET(Boost_USE_STATIC_LIBS    ON)
    SET(Boost_USE_MULTITHREADED  ON)
ENDIF(MSVC)

IF(WIN32)
    IF(NOT BOOST_ROOT)
        SET(BOOST_ROOT $ENV{BOOST_ROOT})
    ENDIF()
ENDIF(WIN32)

# set base directory
SET(RDO_BASE_DIRECTORY ${PROJECT_SOURCE_DIR})

IF(WIN32)
    SET(PATH_STRING $ENV{PATH})
    STRING(FIND "${PATH_STRING}" "thirdparty/bison_flex/bin" OUT)
    IF(${OUT} GREATER -1)
    ELSE()
        SET(ENV{PATH} "$ENV{PATH};${RDO_BASE_DIRECTORY}/thirdparty/bison_flex/bin")
    ENDIF()
ENDIF(WIN32)

SET(BISON_FLEX_DIRECTORY ${RDO_BASE_DIRECTORY}/thirdparty/bison_flex)

# Compile the rdo_common
SET(UTILS_PATH ${RDO_BASE_DIRECTORY}/utils)
ADD_SUBDIRECTORY(${UTILS_PATH})

# Compile the rdo_kernel
SET(KERNEL_PATH ${RDO_BASE_DIRECTORY}/kernel)
ADD_SUBDIRECTORY(${KERNEL_PATH})

# Compile XML plugin
SET(XML_PUGIXML_PATH ${RDO_BASE_DIRECTORY}/thirdparty/pugixml/scripts)
ADD_SUBDIRECTORY(${XML_PUGIXML_PATH})

# Compile the rdo_repository
SET(REPOSITORY_PATH ${RDO_BASE_DIRECTORY}/repository)
ADD_SUBDIRECTORY(${REPOSITORY_PATH})

# Compile rdo_converter
SET(CONVERTER_PATH ${RDO_BASE_DIRECTORY}/converter)
#IF(MSVC)
    ADD_SUBDIRECTORY(${CONVERTER_PATH})
#ENDIF(MSVC)

# Compile rdo_lib
SET(SIMULATOR_PATH ${RDO_BASE_DIRECTORY}/simulator)
ADD_SUBDIRECTORY(${SIMULATOR_PATH})

SET(APLICATIONS_DIRECTORY ${RDO_BASE_DIRECTORY}/app)
ADD_SUBDIRECTORY(${APLICATIONS_DIRECTORY})
