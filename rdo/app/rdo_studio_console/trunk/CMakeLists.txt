#==============================================================================
# Copyright (c) 2011 Evgeny Proydakov <lord.tiran@gmail.com>
#==============================================================================
# Cmake for RDO  STUDIO  CONSOLE  EXECUTABLE
#==============================================================================

MESSAGE(STATUS "CREATE  RDO  STUDIO  CONSOLE  EXECUTABLE")

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

ADD_DEPENDENCIES(rdo rdo_utils)
ADD_DEPENDENCIES(rdo rdo_kernel)
ADD_DEPENDENCIES(rdo rdo_repository)
ADD_DEPENDENCIES(rdo rdo_simulator)

TARGET_LINK_LIBRARIES(rdo ${Boost_PROGRAM_OPTIONS_LIBRARY})

TARGET_LINK_LIBRARIES(rdo rdo_utils)
TARGET_LINK_LIBRARIES(rdo rdo_kernel)
TARGET_LINK_LIBRARIES(rdo rdo_repository)
TARGET_LINK_LIBRARIES(rdo rdo_simulator)

IF(MSVC) # lotions for windows #

SOURCE_GROUP(".code" FILES 
	${SOURCE_FILES})
	
ENDIF(MSVC)
