#==============================================================================
# Copyright (c) 2011 Evgeny Proydakov <lord.tiran@gmail.com>
#==============================================================================

MESSAGE(STATUS "CREATE  RDO_MBUILDER  LIBRARY")

FIND_PACKAGE(Boost COMPONENTS system filesystem thread REQUIRED)
LINK_DIRECTORIES(${Boost_LIBRARY_DIRS})
INCLUDE_DIRECTORIES(${Boost_INCLUDE_DIRS})

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
SET_TARGET_PROPERTIES(rdo_mbuilder PROPERTIES FOLDER ${LIBRARY_FOLDERS})

ADD_DEPENDENCIES(rdo_mbuilder rdo_parser)

TARGET_LINK_LIBRARIES(rdo_mbuilder ${Boost_SYSTEM_LIBRARY} ${Boost_FILESYSTEM_LIBRARY} ${Boost_THREAD_LIBRARY})
TARGET_LINK_LIBRARIES(rdo_mbuilder rdo_utils)
TARGET_LINK_LIBRARIES(rdo_mbuilder rdo_parser)

INSTALL(TARGETS rdo_mbuilder DESTINATION lib)

IF(MSVC) # lotions for windows #

SOURCE_GROUP(".common" FILES 
	namespace.h)

SOURCE_GROUP(".pch" FILES 
	pch.h
	pch.cpp)

SOURCE_GROUP(".process" FILES 
	process.h
	process.cpp)

SOURCE_GROUP(".resources" FILES 
	rdo_resources.h
	rdo_resources.cpp)

ENDIF(MSVC)
