#==============================================================================
# Copyright (c) 2011 Evgeny Proydakov <lord.tiran@gmail.com>
#==============================================================================

MESSAGE(STATUS "CREATE  RDO_MBUILDER  LIBRARY")

INCLUDE_DIRECTORIES(${CMAKE_CURRENT_BINARY_DIR})

SET(HEADER_FILES
    namespace.h
    pch.h
    process.h
    rdo_resources.h)

SET(SOURCE_FILES
    pch.cpp
    process.cpp
    rdo_resources.cpp)

ADD_LIBRARY(rdo_mbuilder ${HEADER_FILES} ${SOURCE_FILES})

TARGET_LINK_LIBRARIES(rdo_mbuilder ${Boost_SYSTEM_LIBRARY} ${Boost_FILESYSTEM_LIBRARY} ${Boost_THREAD_LIBRARY})
TARGET_LINK_LIBRARIES(rdo_mbuilder rdo_utils)
