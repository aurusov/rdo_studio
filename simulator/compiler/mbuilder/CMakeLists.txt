#==============================================================================
# Copyright (c) 2011 Evgeny Proydakov <lord.tiran@gmail.com>,
#               2013 Andrey Urusov <drobus@gmail.com>
#==============================================================================

MESSAGE(STATUS "CREATE  RDO_MBUILDER  LIBRARY")

FIND_PACKAGE(Boost COMPONENTS system filesystem thread REQUIRED)
LINK_DIRECTORIES(${Boost_LIBRARY_DIRS})
INCLUDE_DIRECTORIES(${Boost_INCLUDE_DIRS})

INCLUDE_DIRECTORIES(${CMAKE_CURRENT_BINARY_DIR})

FILE(GLOB_RECURSE SOURCE_FILES RELATIVE ${CMAKE_CURRENT_SOURCE_DIR} *.cpp)
ADD_LIBRARY(rdo_mbuilder STATIC ${SOURCE_FILES})

INCLUDE(${PROJECT_SOURCE_DIR}/simulator/compiler/compiler.cmake)
SET_TARGET_PROPERTIES(rdo_mbuilder PROPERTIES FOLDER ${PROJECT_FOLDER})

ADD_DEPENDENCIES(rdo_mbuilder rdo_parser)

TARGET_LINK_LIBRARIES(rdo_mbuilder ${Boost_SYSTEM_LIBRARY} ${Boost_FILESYSTEM_LIBRARY} ${Boost_THREAD_LIBRARY})
TARGET_LINK_LIBRARIES(rdo_mbuilder rdo_utils)
TARGET_LINK_LIBRARIES(rdo_mbuilder rdo_parser)

INSTALL(TARGETS rdo_mbuilder DESTINATION lib)

IF(MSVC_IDE)
	SET(PCH_FILES
		pch.h
		pch.cpp
	)
	INCLUDE(${PROJECT_SOURCE_DIR}/cmake/pch.cmake)
	ADD_PCH("${SOURCE_FILES}" "${PCH_FILES}" simulator/compiler/mbuilder ./ pch)
ENDIF()
