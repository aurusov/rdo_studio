#==============================================================================
# Copyright (c) 2013 Evgeny Proydakov <lord.tiran@gmail.com>
#==============================================================================

MESSAGE(STATUS "CREATE  RDO_UI_QT LIBRARY")

INCLUDE_DIRECTORIES(${CMAKE_CURRENT_SOURCE_DIR})

SET(HEADER_FILES
    headers/animation/area.h
    headers/bitmap/bitmap.h
    headers/memdc/memdc.h
    headers/scroll_bar/scroll_bar.h
    headers/validator/int_validator.h
    pch/stdpch.h
    )

SET(SOURCE_FILES
    pch/stdpch.cpp
    src/bitmap/bitmap.cpp
    src/scroll_bar/scroll_bar.cpp
    )

ADD_LIBRARY(rdo_ui_qt ${HEADER_FILES} ${SOURCE_FILES})
SET_TARGET_PROPERTIES(rdo_ui_qt PROPERTIES FOLDER ${LIBRARY_FOLDERS})

ADD_DEPENDENCIES(rdo_ui_qt rdo_utils)

TARGET_LINK_LIBRARIES(rdo_ui_qt rdo_utils)

IF(MSVC) # lotions for windows #

SOURCE_GROUP(".code" FILES 
    ${HEADER_FILES}
    ${SOURCE_FILES}
    )

ENDIF(MSVC)
