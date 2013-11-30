#==============================================================================
# Copyright (c) 2011 Evgeny Proydakov <lord.tiran@gmail.com>,
#               2013 Andrey Urusov <drobus@gmail.com>
#==============================================================================

MESSAGE(STATUS "CREATE  RDO_SIMULATOR  LIBRARY")

FIND_PACKAGE(Boost COMPONENTS system filesystem thread REQUIRED)
LINK_DIRECTORIES(${Boost_LIBRARY_DIRS})
INCLUDE_DIRECTORIES(${Boost_INCLUDE_DIRS})

INCLUDE_DIRECTORIES(${CMAKE_CURRENT_BINARY_DIR})

SET(CODE_FILES
	namespace.h
	rdosimwin.cpp
	rdosimwin.h
	show_mode.h
)

SET(SOURCE_FILES
	${CODE_FILES}
)

ADD_LIBRARY(rdo_simulator ${SOURCE_FILES})

INCLUDE(${PROJECT_SOURCE_DIR}/simulator/simulator.cmake)
SET_TARGET_PROPERTIES(rdo_simulator PROPERTIES FOLDER ${PROJECT_FOLDER}/.service)

ADD_DEPENDENCIES(rdo_simulator rdo_utils)
ADD_DEPENDENCIES(rdo_simulator rdo_runtime)
ADD_DEPENDENCIES(rdo_simulator rdo_parser)
ADD_DEPENDENCIES(rdo_simulator rdo_mbuilder)
ADD_DEPENDENCIES(rdo_simulator rdo_procgui)
ADD_DEPENDENCIES(rdo_simulator rdo_simulator_report)
ADD_DEPENDENCIES(rdo_simulator rdo_converter_smr2rdox)

TARGET_LINK_LIBRARIES(rdo_simulator ${Boost_SYSTEM_LIBRARY} ${Boost_FILESYSTEM_LIBRARY} ${Boost_THREAD_LIBRARY})
TARGET_LINK_LIBRARIES(rdo_simulator rdo_utils)
TARGET_LINK_LIBRARIES(rdo_simulator rdo_runtime)
TARGET_LINK_LIBRARIES(rdo_simulator rdo_parser)
TARGET_LINK_LIBRARIES(rdo_simulator rdo_mbuilder)
TARGET_LINK_LIBRARIES(rdo_simulator rdo_procgui)
TARGET_LINK_LIBRARIES(rdo_simulator rdo_simulator_report)
TARGET_LINK_LIBRARIES(rdo_simulator rdo_converter_smr2rdox)

INSTALL(TARGETS rdo_simulator DESTINATION lib)

IF(MSVC) # options for windows #

	SOURCE_GROUP(".code" FILES 
		${CODE_FILES}
	)

ENDIF(MSVC)
