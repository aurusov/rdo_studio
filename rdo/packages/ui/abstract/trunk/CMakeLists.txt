#==============================================================================
# Copyright (c) 2013 Evgeny Proydakov <lord.tiran@gmail.com>
#==============================================================================

MESSAGE(STATUS "CREATE  RDO_UI_ABSTRACT LIBRARY")

FIND_PACKAGE(Boost COMPONENTS system filesystem thread date_time locale REQUIRED)
LINK_DIRECTORIES(${Boost_LIBRARY_DIRS})
INCLUDE_DIRECTORIES(${Boost_INCLUDE_DIRS})

INCLUDE_DIRECTORIES(${CMAKE_CURRENT_SOURCE_DIR})

SET(MEMDC_FILES
	src/memdc/memdc.cpp
	headers/memdc/memdc.h
	headers/memdc/memdc.inl
)

SET(PCH_FILES
	pch/stdpch.cpp
	pch/stdpch.h
)

SET(SOURCE_FILES
	${MEMDC_FILES}
	${PCH_FILES}
)

ADD_LIBRARY(rdo_ui_abstract ${SOURCE_FILES})

INCLUDE(${PROJECT_SOURCE_DIR}/app/rdo_studio/projects/common/rdo_studio.cmake)
SET_TARGET_PROPERTIES(rdo_ui_abstract PROPERTIES FOLDER ${APP_FOLDER}/.ui)

ADD_DEPENDENCIES(rdo_ui_abstract rdo_utils)

TARGET_LINK_LIBRARIES(rdo_ui_abstract rdo_utils)

IF(MSVC) # options for windows #

	INCLUDE(${PROJECT_SOURCE_DIR}/cmake/pch.cmake)
	ADD_PCH("${SOURCE_FILES}" "${PCH_FILES}" pch/stdpch ui/abstract)

	SOURCE_GROUP(".memdc" FILES 
		${MEMDC_FILES}
	)

	SOURCE_GROUP(".pch" FILES 
		${PCH_FILES}
	)

ENDIF(MSVC)
