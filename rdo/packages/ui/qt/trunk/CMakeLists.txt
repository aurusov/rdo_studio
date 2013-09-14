#==============================================================================
# Copyright (c) 2013 Evgeny Proydakov <lord.tiran@gmail.com>
#==============================================================================

MESSAGE(STATUS "CREATE  RDO_UI_QT LIBRARY")

INCLUDE_DIRECTORIES(${CMAKE_CURRENT_SOURCE_DIR})

FIND_PACKAGE(Qt5Widgets REQUIRED)

INCLUDE_DIRECTORIES(${Qt5Widgets_INCLUDE_DIRS})

SET(HEADER_FILES
    headers/animation/area.h
    headers/bitmap/bitmap.h
    headers/memdc/memdc.h
    headers/scroll_bar/scroll_bar.h
    pch/stdpch.h
    )

SET(SOURCE_FILES
    pch/stdpch.cpp
    src/bitmap/bitmap.cpp
    src/scroll_bar/scroll_bar.cpp
    )

ADD_LIBRARY(rdo_ui_qt ${HEADER_FILES} ${SOURCE_FILES})

INCLUDE(${PROJECT_SOURCE_DIR}/app/rdo_studio/projects/common/rdo_studio.cmake)
SET_TARGET_PROPERTIES(rdo_ui_qt PROPERTIES FOLDER ${APP_FOLDER}/.ui)

ADD_DEPENDENCIES(rdo_ui_qt rdo_utils)

TARGET_LINK_LIBRARIES(rdo_ui_qt rdo_utils)

IF(MSVC) # lotions for windows #

SOURCE_GROUP(".code" FILES 
    ${HEADER_FILES}
    ${SOURCE_FILES}
    )

ENDIF(MSVC)
