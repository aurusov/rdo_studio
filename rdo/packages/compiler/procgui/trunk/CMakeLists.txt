#==============================================================================
# Copyright (c) 2012 Evgeny Proydakov <lord.tiran@gmail.com>,
#               2013 Andrey Urusov <drobus@gmail.com>
#==============================================================================

MESSAGE(STATUS "CREATE  PROCGUI  LIBRARY")

FIND_PACKAGE(Boost)
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

SET(PROCGUI_FILES
	procgui.cpp
	procgui.h
	procgui_datablock.cpp
	procgui_datablock.h
)

SET(SOURCE_FILES
	${PCH_FILES}
	${COMMON_FILES}
	${PROCGUI_FILES}
)

SET(NO_PCH_FILES
	${PCH_FILES}
)

ADD_LIBRARY(rdo_procgui STATIC ${SOURCE_FILES})
INCLUDE(${PROJECT_SOURCE_DIR}/simulator/compiler/compiler.cmake)
SET_TARGET_PROPERTIES(rdo_procgui PROPERTIES FOLDER ${PROJECT_FOLDER})

ADD_DEPENDENCIES(rdo_procgui rdo_utils)

TARGET_LINK_LIBRARIES(rdo_procgui rdo_utils)

INSTALL(TARGETS rdo_procgui DESTINATION lib)

IF(MSVC) # options for windows #

	INCLUDE(${PROJECT_SOURCE_DIR}/cmake/pch.cmake)
	ADD_PCH("${SOURCE_FILES}" "${NO_PCH_FILES}" simulator/compiler/procgui ./ pch)

	SOURCE_GROUP(".common" FILES
		${COMMON_FILES}
	)

	SOURCE_GROUP(".pch" FILES
		${PCH_FILES}
	)

	SOURCE_GROUP(".procgui" FILES
		${PROCGUI_FILES}
	)

ENDIF(MSVC)
