#==============================================================================
# Copyright (c) 2011 Evgeny Proydakov <lord.tiran@gmail.com>
#==============================================================================

MESSAGE(STATUS "CREATE  RDO_SIMULATOR  LIBRARY")

INCLUDE_DIRECTORIES(${CMAKE_CURRENT_BINARY_DIR})

SET(HEADER_FILES
    procgui.h
	namespace.h
    rdosimwin.h
    procgui_datablock.h)

SET(SOURCE_FILES
    procgui.cpp
    rdosimwin.cpp
    procgui_datablock.cpp)

ADD_LIBRARY(rdo_simulator ${HEADER_FILES} ${SOURCE_FILES})

ADD_DEPENDENCIES(rdo_simulator rdo_utils)
ADD_DEPENDENCIES(rdo_simulator rdo_parser)
ADD_DEPENDENCIES(rdo_simulator rdo_runtime)
ADD_DEPENDENCIES(rdo_simulator rdo_mbuilder)
IF(MSVC)
	ADD_DEPENDENCIES(rdo_simulator rdo_converter_smr2rdox)
ENDIF(MSVC)

TARGET_LINK_LIBRARIES(rdo_simulator ${Boost_SYSTEM_LIBRARY} ${Boost_FILESYSTEM_LIBRARY} ${Boost_THREAD_LIBRARY})
TARGET_LINK_LIBRARIES(rdo_simulator rdo_utils)
TARGET_LINK_LIBRARIES(rdo_simulator rdo_parser)
TARGET_LINK_LIBRARIES(rdo_simulator rdo_runtime)
TARGET_LINK_LIBRARIES(rdo_simulator rdo_mbuilder)
IF(MSVC)
	TARGET_LINK_LIBRARIES(rdo_simulator rdo_converter_smr2rdox)
ENDIF(MSVC)

IF(MSVC) # lotions for windows #

SOURCE_GROUP(".code" FILES 
	${HEADER_FILES}
	${SOURCE_FILES})
	
ENDIF(MSVC)
