CMAKE_MINIMUM_REQUIRED(VERSION 2.6.0)

MESSAGE(STATUS "CREATE RDO_KERNEL LIBRARY")

# Find Boost
SET(Boost_USE_STATIC_LIBS   ON)
SET(Boost_USE_MULTITHREADED ON)
FIND_PACKAGE(Boost COMPONENTS system filesystem thread REQUIRED)

# Find Boost
LINK_DIRECTORIES(${Boost_LIBRARY_DIRS})
INCLUDE_DIRECTORIES(${Boost_INCLUDE_DIRS})

INCLUDE_DIRECTORIES(${CMAKE_CURRENT_SOURCE_DIR})

SET(HEADER_FILES	
	rdothread.h
	rdokernel.h)

SET(SOURCE_FILES
	rdothread.cpp
	rdokernel.cpp)

SOURCE_GROUP(".thread" FILES 
	rdothread.h
	rdothread.cpp)

SOURCE_GROUP(".kernel" FILES 
	rdokernel.h
	rdokernel.cpp)

ADD_LIBRARY(rdo_kernel ${HEADER_FILES} ${SOURCE_FILES})
ADD_DEPENDENCIES(rdo_kernel rdo_utils)

TARGET_LINK_LIBRARIES(rdo_kernel ${Boost_SYSTEM_LIBRARY} ${Boost_FILESYSTEM_LIBRARY} ${Boost_THREAD_LIBRARY})
TARGET_LINK_LIBRARIES(rdo_kernel rdo_utils)
