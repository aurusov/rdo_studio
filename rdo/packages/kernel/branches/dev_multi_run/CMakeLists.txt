#==============================================================================
# Copyright (c) 2011 Evgeny Proydakov <lord.tiran@gmail.com>
#==============================================================================

MESSAGE(STATUS "CREATE  RDO_KERNEL LIBRARY")

IF(PROJECT_OS_LINUX)
    FIND_LIBRARY(PTHREAD pthread $ENV{LD_LIBRARY_PATH})
ENDIF(PROJECT_OS_LINUX)

FIND_PACKAGE(Boost COMPONENTS system filesystem thread REQUIRED)
LINK_DIRECTORIES(${Boost_LIBRARY_DIRS})
INCLUDE_DIRECTORIES(${Boost_INCLUDE_DIRS})

INCLUDE_DIRECTORIES(${CMAKE_CURRENT_SOURCE_DIR})

SET(HEADER_FILES	
	rdothread.h
	rdokernel.h)

SET(SOURCE_FILES
	rdothread.cpp
	rdokernel.cpp)

ADD_LIBRARY(rdo_kernel ${HEADER_FILES} ${SOURCE_FILES})
SET_TARGET_PROPERTIES(rdo_kernel PROPERTIES FOLDER ${LIBRARY_FOLDERS})

ADD_DEPENDENCIES(rdo_kernel rdo_utils)
ADD_DEPENDENCIES(rdo_kernel rdo_parser)

TARGET_LINK_LIBRARIES(rdo_kernel ${Boost_SYSTEM_LIBRARY} ${Boost_FILESYSTEM_LIBRARY} ${Boost_THREAD_LIBRARY})
TARGET_LINK_LIBRARIES(rdo_kernel rdo_utils)

INSTALL(TARGETS rdo_kernel DESTINATION lib)

IF(PROJECT_OS_LINUX OR CYGWIN)
	TARGET_LINK_LIBRARIES(rdo_kernel ${PTHREAD})
ENDIF(PROJECT_OS_LINUX OR CYGWIN)

IF(MSVC) # lotions for windows #

SOURCE_GROUP(".thread" FILES 
	rdothread.h
	rdothread.cpp)

SOURCE_GROUP(".kernel" FILES 
	rdokernel.h
	rdokernel.cpp)
	
ENDIF(MSVC)
