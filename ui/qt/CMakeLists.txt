#==============================================================================
# Copyright (c) 2013 Evgeny Proydakov <lord.tiran@gmail.com>
#==============================================================================

MESSAGE(STATUS "CREATE  RDO_UI_QT LIBRARY")

FIND_PACKAGE(Boost COMPONENTS REQUIRED)
INCLUDE_DIRECTORIES(${Boost_INCLUDE_DIRS})

FIND_PACKAGE(Qt5Widgets REQUIRED)
INCLUDE_DIRECTORIES(${Qt5Widgets_INCLUDE_DIRS})

INCLUDE_DIRECTORIES(${CMAKE_CURRENT_SOURCE_DIR})

FILE(GLOB_RECURSE SOURCE_FILES RELATIVE ${CMAKE_CURRENT_SOURCE_DIR} *.cpp)
ADD_LIBRARY(rdo_ui_qt STATIC ${SOURCE_FILES})

INCLUDE(${PROJECT_SOURCE_DIR}/app/rdo_studio/projects/common/rdo_studio.cmake)
SET_TARGET_PROPERTIES(rdo_ui_qt PROPERTIES FOLDER ${APP_FOLDER}/.ui)

ADD_DEPENDENCIES(rdo_ui_qt rdo_utils)

TARGET_LINK_LIBRARIES(rdo_ui_qt rdo_utils)

IF(MSVC_IDE)
	SET(PCH_FILES
		pch/stdpch.cpp
		pch/stdpch.h
	)
	INCLUDE(${PROJECT_SOURCE_DIR}/cmake/pch.cmake)
	ADD_PCH("${SOURCE_FILES}" "${PCH_FILES}" ui/qt pch stdpch)
ENDIF()
