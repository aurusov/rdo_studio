#==============================================================================
# Copyright (c) 2011 Evgeny Proydakov <lord.tiran@gmail.com>
#==============================================================================
# Cmake for RDO  STUDIO  CONSOLE  EXECUTABLE
#==============================================================================

MESSAGE(STATUS "CREATE  RDO  STUDIO  CONSOLE  EXECUTABLE")

# Find Boost
IF(MSVC)
    SET(Boost_USE_STATIC_LIBS    ON)
    SET(Boost_USE_MULTITHREADED  ON)
ENDIF(MSVC)
FIND_PACKAGE(Boost COMPONENTS program_options thread REQUIRED)

# Find Boost
LINK_DIRECTORIES(${Boost_LIBRARY_DIRS})
INCLUDE_DIRECTORIES(${Boost_INCLUDE_DIRS})

INCLUDE_DIRECTORIES(${CMAKE_CURRENT_SOURCE_DIR})

SET(HEADER_FILES
	terminate_codes.h
    controller_console_options.h
    rdo_studio_console_controller.h
    controller_console_options_config.h)

SET(SOURCE_FILES
    controller_console_options.cpp
    rdo_studio_console_controller.cpp)

SET(MAIN_FILES
    main.cpp)

ADD_EXECUTABLE(rdo ${HEADER_FILES} ${SOURCE_FILES} ${MAIN_FILES})

ADD_DEPENDENCIES(rdo rdo_utils)
ADD_DEPENDENCIES(rdo rdo_kernel)
ADD_DEPENDENCIES(rdo rdo_repository)
ADD_DEPENDENCIES(rdo rdo_simulator)

TARGET_LINK_LIBRARIES(rdo ${Boost_PROGRAM_OPTIONS_LIBRARY} ${Boost_THREAD_LIBRARY})

TARGET_LINK_LIBRARIES(rdo rdo_utils)
TARGET_LINK_LIBRARIES(rdo rdo_kernel)
TARGET_LINK_LIBRARIES(rdo rdo_repository)
TARGET_LINK_LIBRARIES(rdo rdo_simulator)

IF(MSVC) # lotions for windows #

SOURCE_GROUP(".code" FILES
	${HEADER_FILES}
	${SOURCE_FILES}
    ${MAIN_FILES})

SOURCE_GROUP(".console_controller" FILES
    rdo_studio_console_controller.h
    rdo_studio_console_controller.cpp)

SOURCE_GROUP(".console_options" FILES
    controller_console_options.h
    controller_console_options.cpp
    controller_console_options_config.h)

ENDIF(MSVC)
