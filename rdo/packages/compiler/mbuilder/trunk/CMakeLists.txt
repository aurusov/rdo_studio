#==============================================================================
# Copyright (c) 2011 Evgeny Proydakov <lord.tiran@gmail.com>,
#               2013 Andrey Urusov <drobus@gmail.com>
#==============================================================================

MESSAGE(STATUS "CREATE  RDO_MBUILDER  LIBRARY")

FIND_PACKAGE(Boost COMPONENTS system filesystem thread REQUIRED)
LINK_DIRECTORIES(${Boost_LIBRARY_DIRS})
INCLUDE_DIRECTORIES(${Boost_INCLUDE_DIRS})

INCLUDE_DIRECTORIES(${CMAKE_CURRENT_BINARY_DIR})

SET(COMMON_FILES
	namespace.h
)

SET(PCH_FILES
	pch.h
	pch.cpp
)

SET(PROCESS_FILES
	process.h
	process.cpp
)

SET(RESOURCES_FILES
	rdo_resources.h
	rdo_resources.cpp
)

SET(SOURCE_FILES
	${PCH_FILES}
	${COMMON_FILES}
	${PROCESS_FILES}
	${RESOURCES_FILES}
)

SET(NO_PCH_FILES
	${PCH_FILES}
)

ADD_LIBRARY(rdo_mbuilder STATIC ${SOURCE_FILES})

INCLUDE(${PROJECT_SOURCE_DIR}/simulator/compiler/compiler.cmake)
SET_TARGET_PROPERTIES(rdo_mbuilder PROPERTIES FOLDER ${PROJECT_FOLDER})

ADD_DEPENDENCIES(rdo_mbuilder rdo_parser)

TARGET_LINK_LIBRARIES(rdo_mbuilder ${Boost_SYSTEM_LIBRARY} ${Boost_FILESYSTEM_LIBRARY} ${Boost_THREAD_LIBRARY})
TARGET_LINK_LIBRARIES(rdo_mbuilder rdo_utils)
TARGET_LINK_LIBRARIES(rdo_mbuilder rdo_parser)

INSTALL(TARGETS rdo_mbuilder DESTINATION lib)

IF(MSVC) # options for windows #

	INCLUDE(${PROJECT_SOURCE_DIR}/cmake/pch.cmake)
	ADD_PCH("${SOURCE_FILES}" "${NO_PCH_FILES}" simulator/compiler/mbuilder ./ pch)

	SOURCE_GROUP(".common" FILES
		${COMMON_FILES}
	)

	SOURCE_GROUP(".pch" FILES
		${PCH_FILES}
	)

	SOURCE_GROUP(".process" FILES
		${PROCESS_FILES}
	)

	SOURCE_GROUP(".resources" FILES
		${RESOURCES_FILES}
	)

ENDIF(MSVC)
