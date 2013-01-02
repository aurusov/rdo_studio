#==============================================================================
# Copyright (c) 2012 Evgeny Proydakov <lord.tiran@gmail.com>
#==============================================================================

MESSAGE(STATUS "CREATE  PROCGUI  LIBRARY")

FIND_PACKAGE(Boost)
LINK_DIRECTORIES(${Boost_LIBRARY_DIRS})
INCLUDE_DIRECTORIES(${Boost_INCLUDE_DIRS})

INCLUDE_DIRECTORIES(${CMAKE_CURRENT_BINARY_DIR})

SET(HEADER_FILES
    pch.h
    procgui.h
    procgui_datablock.h
    )

SET(SOURCE_FILES
    pch.cpp
    procgui.cpp
    procgui_datablock.cpp
    )

ADD_LIBRARY(rdo_procgui ${HEADER_FILES} ${SOURCE_FILES})
SET_TARGET_PROPERTIES(rdo_procgui PROPERTIES FOLDER ${LIBRARY_FOLDERS})

ADD_DEPENDENCIES(rdo_procgui rdo_utils)

TARGET_LINK_LIBRARIES(rdo_procgui rdo_utils)

IF(MSVC) # lotions for windows #

SOURCE_GROUP(".code" FILES 
    ${HEADER_FILES}
    ${SOURCE_FILES}
    )

ENDIF(MSVC)
