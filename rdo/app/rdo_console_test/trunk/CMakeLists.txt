#==============================================================================
# Copyright (c) 2011 Evgeny Proydakov <lord.tiran@gmail.com>
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

SET(INLINE_FILES
    controller_console_options.inl)

SET(SOURCE_FILES
    controller_console_options.cpp)

SET(MAIN_FILE 
    main.cpp)

ADD_EXECUTABLE(rdo_test ${MAIN_FILE} ${HEADER_FILES} ${INLINE_FILES} ${SOURCE_FILES})

ADD_DEPENDENCIES(rdo_test rdo)
ADD_DEPENDENCIES(rdo_test rdo_utils)

TARGET_LINK_LIBRARIES(rdo_test rdo_utils)
TARGET_LINK_LIBRARIES(rdo_test ${Boost_PROGRAM_OPTIONS_LIBRARY})

IF(MSVC) # lotions for windows #

SOURCE_GROUP(".code" FILES
    ${MAIN_FILE})

SOURCE_GROUP(".console_options" FILES
    controller_console_options.h
    controller_console_options.cpp
    controller_console_options_config.h)

ENDIF(MSVC)

#==============================================================================
# temp
#==============================================================================

#ADD_BOOST_TEST(test_rdo_studio_console test.cpp)

#IF(MSVC)
#    ADD_DEFINITIONS(-DRDO_TEST_APP="${EXECUTABLE_OUTPUT_PATH}/${CMAKE_BUILD_TYPE}/rdo_test.exe")
#ELSE()
#    ADD_DEFINITIONS(-DRDO_TEST_APP="${EXECUTABLE_OUTPUT_PATH}/rdo_test")
#ENDIF(MSVC)

#ADD_DEFINITIONS(-DRDO_TEST_APP_PATH="${CMAKE_CURRENT_SOURCE_DIR}/")
