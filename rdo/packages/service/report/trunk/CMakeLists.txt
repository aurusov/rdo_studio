#==============================================================================
# Copyright (c) 2012 Evgeny Proydakov <lord.tiran@gmail.com>
#==============================================================================

MESSAGE(STATUS "CREATE  RDO_SIMULATOR_REPORT  LIBRARY")

INCLUDE_DIRECTORIES(${CMAKE_CURRENT_BINARY_DIR})

SET(HEADER_FILES
    comment.h
    namespace.h
    error_code.h
    file_message.h
    log_edit_line_info.h
    build_edit_line_info.h)

SET(SOURCE_FILES
    file_message.cpp
    log_edit_line_info.cpp
    build_edit_line_info.cpp)

ADD_LIBRARY(rdo_simulator_report ${HEADER_FILES} ${SOURCE_FILES})

ADD_DEPENDENCIES(rdo_simulator_report rdo_utils)
TARGET_LINK_LIBRARIES(rdo_simulator_report rdo_utils)


IF(MSVC) # lotions for windows #

SOURCE_GROUP(".code" FILES 
	${HEADER_FILES}
	${SOURCE_FILES})
	
ENDIF(MSVC)
