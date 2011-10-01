#==============================================================================
# Copyright (c) 2011 Evgeny Proydakov <lord.tiran@gmail.com>
#==============================================================================
# Welcome to the CMake build system for RDO STUDIO (UNIX VERSION).
# This is the main file where we prepare the general build environment
# and provide build configuration options.
#==============================================================================

CMAKE_MINIMUM_REQUIRED(VERSION 2.6.0)

PROJECT(rdo_nix_studio)

MESSAGE(STATUS "Cmake version : " ${CMAKE_VERSION})
MESSAGE(STATUS "CMake generates " ${CMAKE_GENERATOR})

MESSAGE(STATUS "System: " ${CMAKE_SYSTEM})
MESSAGE(STATUS "Processor: " ${CMAKE_HOST_SYSTEM_PROCESSOR})

IF(${CMAKE_SYSTEM_NAME} STREQUAL Windows)
IF(MSVC)
MESSAGE(STATUS "Compiler: MSVC")
ENDIF(MSVC)
IF(BORLAND) 
    MESSAGE (STATUS "Compiler: BCC")
ENDIF(BORLAND) 
ELSE(${CMAKE_SYSTEM_NAME} STREQUAL Linux) 
    MESSAGE(STATUS "Only GCC is supported on Linux")
ENDIF(${CMAKE_SYSTEM_NAME} STREQUAL Windows)

# set warning level to maximum Compiler
#IF(MSVC)
#     STRING(REGEX REPLACE /W[0-4] /W4 CMAKE_CXX_FLAGS ${CMAKE_CXX_FLAGS})
#ENDIF(MSVC)

# temp setting
IF(NOT CMAKE_BUILD_TYPE)
    SET(CMAKE_BUILD_TYPE Debug)
ENDIF(NOT CMAKE_BUILD_TYPE)

IF(NOT MSVC)
# build libraries type
IF(NOT BUILD_SHARED_LIBS)
    SET(BUILD_SHARED_LIBS ON)
ENDIF(NOT BUILD_SHARED_LIBS)
ENDIF(NOT MSVC)

IF(NOT MSVC)
    SET(CDEFS "-m32 -o output32")
    SET(CMAKE_C_FLAGS  ${CDEFS})
    SET(CMAKE_CXX_FLAGS ${CDEFS})
ENDIF(NOT MSVC)

SET(LIBRARY_OUTPUT_PATH ${CMAKE_CURRENT_BINARY_DIR})
SET(EXECUTABLE_OUTPUT_PATH ${CMAKE_CURRENT_BINARY_DIR})

# add the include folder to the include path
INCLUDE_DIRECTORIES(${PROJECT_SOURCE_DIR})

IF(NOT MSVC)
    INCLUDE(CTest)
    ENABLE_TESTING()
ENDIF(NOT MSVC)

# set base directory
SET(RDO_BASE_DIRECTORY ${PROJECT_SOURCE_DIR})

SET(CMAKE_MODULE_PATH ${RDO_BASE_DIRECTORY}/cmake)
INCLUDE(add_boost_test)

# Compile the rdo_common
SET(UTILS_PATH ${RDO_BASE_DIRECTORY}/utils)
ADD_SUBDIRECTORY(${UTILS_PATH})

# Compile the rdo_kernel
SET(KERNEL_PATH ${RDO_BASE_DIRECTORY}/kernel)
#ADD_SUBDIRECTORY(${KERNEL_PATH})

# Compile the rdo_repository
SET(REPOSITORY_PATH ${RDO_BASE_DIRECTORY}/repository)
#ADD_SUBDIRECTORY(${REPOSITORY_PATH})

# Compile rdo_lib
SET(SIMULATOR_PATH ${RDO_BASE_DIRECTORY}/simulator)
ADD_SUBDIRECTORY(${SIMULATOR_PATH})
