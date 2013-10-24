#==============================================================================
# Copyright (c) 2011 Evgeny Proydakov <lord.tiran@gmail.com>,
#               2013 Andrey Urusov <drobus@gmail.com>
#==============================================================================

MESSAGE(STATUS "CREATE  RDO  STUDIO  CONSOLE  EXECUTABLE")

FIND_PACKAGE(Boost COMPONENTS program_options thread REQUIRED)
LINK_DIRECTORIES(${Boost_LIBRARY_DIRS})
INCLUDE_DIRECTORIES(${Boost_INCLUDE_DIRS})

INCLUDE_DIRECTORIES(${CMAKE_CURRENT_SOURCE_DIR})

SET(APP_NAME rdo)
CONFIGURE_FILE(${CMAKE_MODULE_PATH}/build.version.cmake ${CMAKE_CURRENT_SOURCE_DIR}/build_version.h)
SET(RESOURCES_FILES
	build_version.h
)

SET(CODE_FILES
	main.cpp
	terminate_codes.h
)

SET(CONSOLE_CONTROLLER_FILES
	rdo_console_controller.h
	rdo_console_controller.cpp
)

SET(CONSOLE_OPTIONS_FILES
	controller_console_options.h
	controller_console_options.cpp
	controller_console_options_config.h
)

SET(EVENT_FILES
	rdo_event.h
	rdo_event.cpp
	rdo_key_event.h
	rdo_key_event.cpp
	rdo_mouse_event.h
	rdo_mouse_event.cpp
)

SET(XML_FILES
	rdo_event_xml_parser.h
	rdo_event_xml_parser.cpp
	rdo_event_xml_reader.h
	rdo_event_xml_reader.cpp
	rdo_key_event_xml_reader.h
	rdo_key_event_xml_reader.cpp
	rdo_mouse_event_xml_reader.h
	rdo_mouse_event_xml_reader.cpp
)

SET(SOURCE_FILES
	${RESOURCES_FILES}
	${CODE_FILES}
	${CONSOLE_CONTROLLER_FILES}
	${CONSOLE_OPTIONS_FILES}
	${EVENT_FILES}
	${XML_FILES}
	${DOC_FILES}
	${SCRIPT_FILES}
)

ADD_EXECUTABLE(rdo ${SOURCE_FILES})

INCLUDE(${PROJECT_SOURCE_DIR}/app/rdo_console/projects/common/rdo_studio_console.cmake)
SET_TARGET_PROPERTIES(rdo PROPERTIES FOLDER ${APP_FOLDER})

ADD_DEPENDENCIES(rdo rdo_utils)
ADD_DEPENDENCIES(rdo rdo_kernel)
ADD_DEPENDENCIES(rdo rdo_repository)
ADD_DEPENDENCIES(rdo rdo_simulator)
ADD_DEPENDENCIES(rdo rdo_simulator_report)

TARGET_LINK_LIBRARIES(rdo ${Boost_PROGRAM_OPTIONS_LIBRARY} ${Boost_THREAD_LIBRARY})

TARGET_LINK_LIBRARIES(rdo rdo_utils)
TARGET_LINK_LIBRARIES(rdo rdo_kernel)
TARGET_LINK_LIBRARIES(rdo rdo_repository)
TARGET_LINK_LIBRARIES(rdo rdo_simulator)
TARGET_LINK_LIBRARIES(rdo rdo_simulator_report)

INSTALL(TARGETS rdo DESTINATION bin)

IF(MSVC) # options for windows #

	SOURCE_GROUP(".code" FILES
		${CODE_FILES}
	)

	SOURCE_GROUP(".console_controller" FILES
		${CONSOLE_CONTROLLER_FILES}
	)

	SOURCE_GROUP(".console_options" FILES
		${CONSOLE_OPTIONS_FILES}
	)

	SOURCE_GROUP(".event" FILES
		${EVENT_FILES}
	)

	SOURCE_GROUP(".xml" FILES
		${XML_FILES}
	)

	SOURCE_GROUP(".doc" FILES
		${DOC_FILES}
	)

	SOURCE_GROUP(".scripts" FILES
		${SCRIPT_FILES}
	)

ENDIF(MSVC)
