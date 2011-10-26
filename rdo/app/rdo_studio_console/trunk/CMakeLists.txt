#==============================================================================
# Copyright (c) 2011 Evgeny Proydakov <lord.tiran@gmail.com>
#==============================================================================
# Cmake for RDO  STUDIO  CONSOLE  EXECUTABLE
#==============================================================================

CMAKE_MINIMUM_REQUIRED(VERSION 2.6.0)

MESSAGE(STATUS "create  RDO  STUDIO  CONSOLE  EXECUTABLE")

# Find Boost
IF(MSVC)
    SET(Boost_USE_STATIC_LIBS   ON)
    SET(Boost_USE_MULTITHREADED ON)
ENDIF(MSVC)
FIND_PACKAGE(Boost COMPONENTS program_options REQUIRED)

# Find Boost
LINK_DIRECTORIES(${Boost_LIBRARY_DIRS})
INCLUDE_DIRECTORIES(${Boost_INCLUDE_DIRS})

INCLUDE_DIRECTORIES(${CMAKE_CURRENT_SOURCE_DIR})

SET(SOURCE_FILES
    main.cpp)

ADD_EXECUTABLE(rdo ${SOURCE_FILES})
TARGET_LINK_LIBRARIES(rdo ${Boost_PROGRAM_OPTIONS_LIBRARY})
