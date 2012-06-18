#==============================================================================
# Copyright (c) 2011 Evgeny Proydakov <lord.tiran@gmail.com>
#==============================================================================

MESSAGE(STATUS "CREATE  RDO  STUDIO  CONSOLE  TEST  EXECUTABLE")

FIND_PACKAGE(Boost COMPONENTS program_options REQUIRED)
LINK_DIRECTORIES(${Boost_LIBRARY_DIRS})
INCLUDE_DIRECTORIES(${Boost_INCLUDE_DIRS})

SET(HEADER_FILES
    ../controller_console_options.h
    ../controller_console_options_config.h)

SET(INLINE_FILES
    ../controller_console_options.inl)

SET(SOURCE_FILES
    ../controller_console_options.cpp)

SET(MAIN_FILE 
    ../main.cpp)

ADD_EXECUTABLE(rdo_test ${MAIN_FILE} ${HEADER_FILES} ${INLINE_FILES} ${SOURCE_FILES})

ADD_DEPENDENCIES(rdo_test rdo)
ADD_DEPENDENCIES(rdo_test rdo_utils)

TARGET_LINK_LIBRARIES(rdo_test rdo_utils)
TARGET_LINK_LIBRARIES(rdo_test ${Boost_PROGRAM_OPTIONS_LIBRARY})

IF(MSVC) # lotions for windows #

SOURCE_GROUP(".code" FILES
	../${HEADER_FILES}
	../${INLINE_FILES}
	../${SOURCE_FILES}
    ../${MAIN_FILE})

ENDIF(MSVC)
