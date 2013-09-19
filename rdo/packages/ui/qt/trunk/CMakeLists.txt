#==============================================================================
# Copyright (c) 2013 Evgeny Proydakov <lord.tiran@gmail.com>
#==============================================================================

MESSAGE(STATUS "CREATE  RDO_UI_QT LIBRARY")

FIND_PACKAGE(Boost COMPONENTS REQUIRED)
INCLUDE_DIRECTORIES(${Boost_INCLUDE_DIRS})

FIND_PACKAGE(Qt5Widgets REQUIRED)
INCLUDE_DIRECTORIES(${Qt5Widgets_INCLUDE_DIRS})

INCLUDE_DIRECTORIES(${CMAKE_CURRENT_SOURCE_DIR})

SET(ANIMATION_FILES
	headers/animation/area.h
)

SET(BITMAP_FILES
	src/bitmap/bitmap.cpp
	headers/bitmap/bitmap.h
)

SET(MEMDC_FILES
	headers/memdc/memdc.h
)

SET(PCH_FILES
	pch/stdpch.cpp
	pch/stdpch.h
)

SET(SCROLLBAR_FILES
	src/scroll_bar/scroll_bar.cpp
	headers/scroll_bar/scroll_bar.h
)

SET(SOURCE_FILES
	${PCH_FILES}
	${ANIMATION_FILES}
	${BITMAP_FILES}
	${MEMDC_FILES}
	${SCROLLBAR_FILES}
)

ADD_LIBRARY(rdo_ui_qt ${SOURCE_FILES})

INCLUDE(${PROJECT_SOURCE_DIR}/app/rdo_studio/projects/common/rdo_studio.cmake)
SET_TARGET_PROPERTIES(rdo_ui_qt PROPERTIES FOLDER ${APP_FOLDER}/.ui)

ADD_DEPENDENCIES(rdo_ui_qt rdo_utils)

TARGET_LINK_LIBRARIES(rdo_ui_qt rdo_utils)

IF(MSVC) # options for windows #

	INCLUDE(${PROJECT_SOURCE_DIR}/cmake/pch.cmake)
	ADD_PCH("${SOURCE_FILES}" "${PCH_FILES}" ui/qt pch stdpch)

	SOURCE_GROUP(".animation" FILES 
		${ANIMATION_FILES}
	)

	SOURCE_GROUP(".bitmap" FILES 
		${BITMAP_FILES}
	)

	SOURCE_GROUP(".memdc" FILES 
		${MEMDC_FILES}
	)

	SOURCE_GROUP(".pch" FILES 
		${PCH_FILES}
	)

	SOURCE_GROUP(".scroll_bar" FILES 
		${SCROLLBAR_FILES}
	)

ENDIF(MSVC)
