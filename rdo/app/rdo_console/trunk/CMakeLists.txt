#==============================================================================
# Copyright (c) 2011 Evgeny Proydakov <lord.tiran@gmail.com>
#==============================================================================

MESSAGE(STATUS "CREATE  RDO  STUDIO  CONSOLE  EXECUTABLE")

FIND_PACKAGE(Boost COMPONENTS program_options thread REQUIRED)
LINK_DIRECTORIES(${Boost_LIBRARY_DIRS})
INCLUDE_DIRECTORIES(${Boost_INCLUDE_DIRS})

INCLUDE_DIRECTORIES(${CMAKE_CURRENT_SOURCE_DIR})

SET(HEADER_FILES
	terminate_codes.h
    controller_console_options.h
    controller_console_options_config.h
    rdo_console_controller.h
	rdo_event.h
	rdo_key_event.h
	rdo_mouse_event.h
	rdo_event_xml_parser.h
	rdo_event_xml_reader.h
	rdo_key_event_xml_reader.h
	rdo_mouse_event_xml_reader.h
	)

SET(SOURCE_FILES
    controller_console_options.cpp
    rdo_console_controller.cpp
    rdo_event.cpp
	rdo_key_event.cpp
	rdo_mouse_event.cpp
	rdo_event_xml_parser.cpp
	rdo_event_xml_reader.cpp
	rdo_key_event_xml_reader.cpp
	rdo_mouse_event_xml_reader.cpp
	)

SET(MAIN_FILES
    main.cpp
	)

ADD_EXECUTABLE(rdo ${HEADER_FILES} ${SOURCE_FILES} ${MAIN_FILES} ${COMMON_FILES} ${SCRIPT_FILES})
SET_TARGET_PROPERTIES(rdo PROPERTIES FOLDER ${APPS_FOLDERS})

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

IF(MSVC) # lotions for windows #

SOURCE_GROUP(".code" FILES
    ${HEADER_FILES}
    ${SOURCE_FILES}
    ${MAIN_FILES}
	)

SOURCE_GROUP(".console_controller" FILES
    rdo_console_controller.h
    rdo_console_controller.cpp
	)

SOURCE_GROUP(".console_options" FILES
    controller_console_options.h
    controller_console_options.cpp
    controller_console_options_config.h
	)

SOURCE_GROUP(".event" FILES
    rdo_event.h
    rdo_event.cpp
	rdo_key_event.h
	rdo_key_event.cpp
	rdo_mouse_event.h
	rdo_mouse_event.cpp
    )

SOURCE_GROUP(".xml" FILES
    rdo_event_xml_parser.h
	rdo_event_xml_parser.cpp
	rdo_event_xml_reader.h
	rdo_event_xml_reader.cpp
	rdo_key_event_xml_reader.h
	rdo_key_event_xml_reader.cpp
	rdo_mouse_event_xml_reader.h
	rdo_mouse_event_xml_reader.cpp
    )

SOURCE_GROUP(".doc" FILES
	${COMMON_FILES}
	)

SOURCE_GROUP(".scripts" FILES
	${SCRIPT_FILES}
	)

ENDIF(MSVC)
