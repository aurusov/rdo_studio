#==============================================================================
# Copyright (c) 2011 Evgeny Proydakov <lord.tiran@gmail.com>
#==============================================================================

MESSAGE(STATUS "CREATE  RDO_SERVICE  LIBRARY")

INCLUDE_DIRECTORIES(${CMAKE_CURRENT_BINARY_DIR})

SET(HEADER_FILES
    procgui.h
    rdosimwin.h)

SET(SOURCE_FILES
    procgui.cpp
    rdosimwin.cpp)

ADD_LIBRARY(rdo_service ${HEADER_FILES} ${SOURCE_FILES})
ADD_DEPENDENCIES(rdo_service rdo_utils)
ADD_DEPENDENCIES(rdo_service rdo_mbuilder)
ADD_DEPENDENCIES(rdo_service rdo_parser)
ADD_DEPENDENCIES(rdo_service rdo_runtime)

TARGET_LINK_LIBRARIES(rdo_service ${Boost_SYSTEM_LIBRARY} ${Boost_FILESYSTEM_LIBRARY} ${Boost_THREAD_LIBRARY})
TARGET_LINK_LIBRARIES(rdo_service rdo_utils)
TARGET_LINK_LIBRARIES(rdo_service rdo_mbuilder)
TARGET_LINK_LIBRARIES(rdo_service rdo_parser)
TARGET_LINK_LIBRARIES(rdo_service rdo_runtime)
