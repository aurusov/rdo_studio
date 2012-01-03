#==============================================================================
# Copyright (c) 2011 Evgeny Proydakov <lord.tiran@gmail.com>
#==============================================================================
# Cmake for RDO  STUDIO  CONSOLE  TEST
#==============================================================================

MESSAGE(STATUS "CREATE  RDO  STUDIO  CONSOLE  TEST  EXECUTABLE")

# Find Boost
IF(MSVC)
    SET(Boost_USE_STATIC_LIBS    ON)
    SET(Boost_USE_MULTITHREADED  ON)
ENDIF(MSVC)
FIND_PACKAGE(Boost COMPONENTS program_options REQUIRED)

SET(HEADER_FILES
    controller_console_options.h
    controller_console_options_config.h)

SET(SOURCE_FILES
    controller_console_options.cpp)

SET(MAIN_FILE 
    main.cpp)

ADD_EXECUTABLE(test_rdo_studio_console ${MAIN_FILE} ${HEADER_FILES} ${SOURCE_FILES})

ADD_DEPENDENCIES(test_rdo_studio_console rdo)
ADD_DEPENDENCIES(test_rdo_studio_console rdo_utils)

TARGET_LINK_LIBRARIES(test_rdo_studio_console rdo_utils)
TARGET_LINK_LIBRARIES(test_rdo_studio_console ${Boost_PROGRAM_OPTIONS_LIBRARY})

IF(MSVC)
    ADD_DEFINITIONS(-DRDO_STUDIO_CONSOLE_APP="${EXECUTABLE_OUTPUT_PATH}/${CMAKE_BUILD_TYPE}/rdo.exe")
ELSE()
    ADD_DEFINITIONS(-DRDO_STUDIO_CONSOLE_APP="${EXECUTABLE_OUTPUT_PATH}/rdo")
ENDIF(MSVC)

ADD_DEFINITIONS(-DRDO_STUDIO_CONSOLE_TEST_PATH="${CMAKE_CURRENT_SOURCE_DIR}/")

IF(MSVC) # lotions for windows #

SOURCE_GROUP(".code" FILES
    ${MAIN_FILE})

SOURCE_GROUP(".console_options" FILES
    controller_console_options.h
    controller_console_options.cpp
    controller_console_options_config.h)

ENDIF(MSVC)
