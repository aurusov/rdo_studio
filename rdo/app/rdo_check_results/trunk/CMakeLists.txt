#==============================================================================
# Copyright (c) 2011-2012 Evgeny Proydakov <lord.tiran@gmail.com>,
#               2013      Andrey Urusov <drobus@gmail.com>
#==============================================================================

MESSAGE(STATUS "CREATE  RDO  STUDIO  CONSOLE  TEST  EXECUTABLE")

FIND_PACKAGE(Boost COMPONENTS program_options REQUIRED)
LINK_DIRECTORIES(${Boost_LIBRARY_DIRS})
INCLUDE_DIRECTORIES(${Boost_INCLUDE_DIRS})

SET(CODE_FILE
	main.cpp
)

SET(CONSOLE_OPTIONS_FILES
	controller_console_options.h
	controller_console_options.inl
	controller_console_options.cpp
	controller_console_options_config.h
)

SET(SOURCE_FILES
	${CODE_FILE}
	${CONSOLE_OPTIONS_FILES}
)

ADD_EXECUTABLE(rdo_check_results ${SOURCE_FILES})

INCLUDE(${PROJECT_SOURCE_DIR}/app/rdo_check_results/projects/common/rdo_check_results.cmake)
SET_TARGET_PROPERTIES(rdo_check_results PROPERTIES FOLDER ${APP_FOLDER})

ADD_DEPENDENCIES(rdo_check_results rdo)
ADD_DEPENDENCIES(rdo_check_results rdo_utils)

TARGET_LINK_LIBRARIES(rdo_check_results rdo_utils)
TARGET_LINK_LIBRARIES(rdo_check_results ${Boost_PROGRAM_OPTIONS_LIBRARY})

IF(MSVC) # options for windows #

	SOURCE_GROUP(".code" FILES
		${CODE_FILE}
	)

	SOURCE_GROUP(".console_options" FILES
		${CONSOLE_OPTIONS_FILES}
	)

ENDIF(MSVC)
