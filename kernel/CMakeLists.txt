#==============================================================================
# Copyright (c) 2011 Evgeny Proydakov <lord.tiran@gmail.com>,
#               2013 Andrey Urusov <drobus@gmail.com>
#==============================================================================

MESSAGE(STATUS "CREATE  RDO_KERNEL LIBRARY")

IF(PROJECT_OS_LINUX)
	FIND_LIBRARY(PTHREAD pthread $ENV{LD_LIBRARY_PATH})
ENDIF(PROJECT_OS_LINUX)

FIND_PACKAGE(Boost COMPONENTS system filesystem thread REQUIRED)
LINK_DIRECTORIES(${Boost_LIBRARY_DIRS})
INCLUDE_DIRECTORIES(${Boost_INCLUDE_DIRS})

INCLUDE_DIRECTORIES(${CMAKE_CURRENT_SOURCE_DIR})

FILE(GLOB_RECURSE SOURCE_FILES RELATIVE ${CMAKE_CURRENT_SOURCE_DIR} *.cpp)
ADD_LIBRARY(rdo_kernel STATIC ${SOURCE_FILES})
SET_TARGET_PROPERTIES(rdo_kernel PROPERTIES FOLDER .kernel)

ADD_DEPENDENCIES(rdo_kernel rdo_utils)
ADD_DEPENDENCIES(rdo_kernel rdo_parser)

TARGET_LINK_LIBRARIES(rdo_kernel ${Boost_SYSTEM_LIBRARY} ${Boost_FILESYSTEM_LIBRARY} ${Boost_THREAD_LIBRARY})
TARGET_LINK_LIBRARIES(rdo_kernel rdo_utils)

INSTALL(TARGETS rdo_kernel DESTINATION lib)

IF(PROJECT_OS_LINUX OR CYGWIN)
	TARGET_LINK_LIBRARIES(rdo_kernel ${PTHREAD})
ENDIF(PROJECT_OS_LINUX OR CYGWIN)
