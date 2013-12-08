#==============================================================================
# Copyright (c) 2011 Evgeny Proydakov <lord.tiran@gmail.com>,
#               2013 Andrey Urusov <drobus@gmail.com>
#==============================================================================

MESSAGE(STATUS "CREATE  RDO_REPOSITORY LIBRARY")

FIND_PACKAGE(Boost COMPONENTS system filesystem thread REQUIRED)
LINK_DIRECTORIES(${Boost_LIBRARY_DIRS})
INCLUDE_DIRECTORIES(${Boost_INCLUDE_DIRS})

INCLUDE_DIRECTORIES(${CMAKE_CURRENT_SOURCE_DIR})

SET(REPOSITORY_FILES
	rdorepository.cpp
	rdorepository.h
)

SET(SOURCE_FILES
	${REPOSITORY_FILES}
)

ADD_LIBRARY(rdo_repository STATIC ${SOURCE_FILES})
SET_TARGET_PROPERTIES(rdo_repository PROPERTIES FOLDER .repository)

ADD_DEPENDENCIES(rdo_repository rdo_simulator)
ADD_DEPENDENCIES(rdo_repository rdo_kernel)
ADD_DEPENDENCIES(rdo_repository pugixml)

TARGET_LINK_LIBRARIES(rdo_repository ${Boost_SYSTEM_LIBRARY} ${Boost_FILESYSTEM_LIBRARY} ${Boost_THREAD_LIBRARY})
TARGET_LINK_LIBRARIES(rdo_repository rdo_simulator)
TARGET_LINK_LIBRARIES(rdo_repository rdo_kernel)
TARGET_LINK_LIBRARIES(rdo_repository pugixml)

INSTALL(TARGETS rdo_repository DESTINATION lib)

IF(MSVC) # options for windows #

	SOURCE_GROUP(".repository" FILES 
		${REPOSITORY_FILES}
	)

ENDIF(MSVC)
