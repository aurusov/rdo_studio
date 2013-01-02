#==============================================================================
# Copyright (c) 2011-2012 Evgeny Proydakov <lord.tiran@gmail.com>
#==============================================================================

MESSAGE(STATUS "CREATE  RDO  STUDIO  CONSOLE  TEST  EXECUTABLE")

FIND_PACKAGE(Boost COMPONENTS program_options REQUIRED)
LINK_DIRECTORIES(${Boost_LIBRARY_DIRS})
INCLUDE_DIRECTORIES(${Boost_INCLUDE_DIRS})

SET(HEADER_FILES
    controller_console_options.h
    controller_console_options_config.h
	)

SET(INLINE_FILES
    controller_console_options.inl
	)

SET(SOURCE_FILES
    controller_console_options.cpp
	)

SET(MAIN_FILE 
    main.cpp
	)

ADD_EXECUTABLE(rdo_check_results ${MAIN_FILE} ${HEADER_FILES} ${INLINE_FILES} ${SOURCE_FILES})
SET_TARGET_PROPERTIES(rdo_check_results PROPERTIES FOLDER ${APPS_FOLDERS})

ADD_DEPENDENCIES(rdo_check_results rdo)
ADD_DEPENDENCIES(rdo_check_results rdo_utils)

TARGET_LINK_LIBRARIES(rdo_check_results rdo_utils)
TARGET_LINK_LIBRARIES(rdo_check_results ${Boost_PROGRAM_OPTIONS_LIBRARY})

IF(MSVC) # lotions for windows #

SOURCE_GROUP(".code" FILES
    ${MAIN_FILE}
	)

SOURCE_GROUP(".console_options" FILES
    controller_console_options.h
	controller_console_options.inl
    controller_console_options.cpp
    controller_console_options_config.h
	)

ENDIF(MSVC)
