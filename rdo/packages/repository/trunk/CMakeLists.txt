#==============================================================================
# Copyright (c) 2011 Evgeny Proydakov <lord.tiran@gmail.com>
#==============================================================================

MESSAGE(STATUS "CREATE  RDO_REPOSITORY LIBRARY")

LINK_DIRECTORIES(${Boost_LIBRARY_DIRS})
INCLUDE_DIRECTORIES(${Boost_INCLUDE_DIRS})

INCLUDE_DIRECTORIES(${CMAKE_CURRENT_SOURCE_DIR})

SET(HEADER_FILES
	namespace.h
	rdorepository.h)

SET(SOURCE_FILES
	rdorepository.cpp)

SOURCE_GROUP(".repository" FILES 
	namespace.h
	rdorepository.h
	rdorepository.cpp)

ADD_LIBRARY(rdo_repository ${HEADER_FILES} ${SOURCE_FILES})

ADD_DEPENDENCIES(rdo_repository rdo_simulator)
ADD_DEPENDENCIES(rdo_repository rdo_kernel)
ADD_DEPENDENCIES(rdo_repository pugixml)

TARGET_LINK_LIBRARIES(rdo_repository ${Boost_SYSTEM_LIBRARY} ${Boost_FILESYSTEM_LIBRARY} ${Boost_THREAD_LIBRARY})
TARGET_LINK_LIBRARIES(rdo_repository rdo_simulator)
TARGET_LINK_LIBRARIES(rdo_repository rdo_kernel)
TARGET_LINK_LIBRARIES(rdo_repository pugixml)
