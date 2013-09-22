#==============================================================================
# Copyright (c) 2012 Evgeny Proydakov <lord.tiran@gmail.com>,
#               2013 Andrey Urusov <drobus@gmail.com>
#==============================================================================

MESSAGE(STATUS "CREATE  RDO_SIMULATOR_REPORT  LIBRARY")

FIND_PACKAGE(Boost)
INCLUDE_DIRECTORIES(${Boost_INCLUDE_DIRS})

INCLUDE_DIRECTORIES(${CMAKE_CURRENT_BINARY_DIR})

SET(CODE_FILES
	build_edit_line_info.cpp
	build_edit_line_info.h
	comment.h
	error_code.h
	file_message.cpp
	file_message.h
	log_edit_line_info.cpp
	log_edit_line_info.h
	namespace.h
)

SET(SOURCE_FILES
	${CODE_FILES}
)

ADD_LIBRARY(rdo_simulator_report ${SOURCE_FILES})

INCLUDE(${PROJECT_SOURCE_DIR}/simulator/simulator.cmake)
SET_TARGET_PROPERTIES(rdo_simulator_report PROPERTIES FOLDER ${PROJECT_FOLDER}/.report)

ADD_DEPENDENCIES(rdo_simulator_report rdo_utils)
TARGET_LINK_LIBRARIES(rdo_simulator_report rdo_utils)

INSTALL(TARGETS rdo_simulator_report DESTINATION lib)

IF(MSVC) # options for windows #

	SOURCE_GROUP(".code" FILES 
		${CODE_FILES}
	)

ENDIF(MSVC)
