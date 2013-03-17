#==============================================================================
# Copyright (c) 2013 Evgeny Proydakov <lord.tiran@gmail.com>
#==============================================================================

MESSAGE(STATUS "CREATE  RDO_UI_ABSTRACT LIBRARY")

INCLUDE_DIRECTORIES(${CMAKE_CURRENT_SOURCE_DIR})

SET(HEADER_FILES
    pch/stdpch.h
    headers/memdc/memdc.h
    headers/memdc/memdc.inl
    )

SET(SOURCE_FILES
    pch/stdpch.cpp
    src/memdc/memdc.cpp
    )

ADD_LIBRARY(rdo_ui_abstract ${HEADER_FILES} ${SOURCE_FILES})
SET_TARGET_PROPERTIES(rdo_ui_abstract PROPERTIES FOLDER ${LIBRARY_FOLDERS})

ADD_DEPENDENCIES(rdo_ui_abstract rdo_utils)

TARGET_LINK_LIBRARIES(rdo_ui_abstract rdo_utils)

IF(MSVC) # lotions for windows #

SOURCE_GROUP(".code" FILES 
    ${HEADER_FILES}
    ${SOURCE_FILES}
    )

ENDIF(MSVC)
