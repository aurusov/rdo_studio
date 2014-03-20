#==============================================================================
# Copyright (c) 2011 Evgeny Proydakov <lord.tiran@gmail.com>,
#               2013 Andrey Urusov <drobus@gmail.com>
#==============================================================================

MESSAGE(STATUS "CREATE  RDO  STUDIO  CONSOLE  EXECUTABLE")

FIND_PACKAGE(Boost COMPONENTS program_options thread REQUIRED)
LINK_DIRECTORIES(${Boost_LIBRARY_DIRS})
INCLUDE_DIRECTORIES(${Boost_INCLUDE_DIRS})

INCLUDE_DIRECTORIES(${CMAKE_CURRENT_SOURCE_DIR})

SET(APP_NAME rdo)
CONFIGURE_FILE(${CMAKE_MODULE_PATH}/build.version.cmake ${CMAKE_CURRENT_BINARY_DIR}/build_version.h)
INCLUDE_DIRECTORIES(${CMAKE_CURRENT_BINARY_DIR})
SET(RESOURCES_FILES
	${CMAKE_CURRENT_BINARY_DIR}/build_version.h
)

FILE(GLOB_RECURSE SOURCE_FILES RELATIVE ${CMAKE_CURRENT_SOURCE_DIR} *.cpp)
ADD_EXECUTABLE(rdo ${SOURCE_FILES})

ADD_DEPENDENCIES(rdo rdo_utils)
ADD_DEPENDENCIES(rdo rdo_kernel)
ADD_DEPENDENCIES(rdo rdo_repository)
ADD_DEPENDENCIES(rdo rdo_simulator)
ADD_DEPENDENCIES(rdo rdo_simulator_report)

TARGET_LINK_LIBRARIES(rdo ${Boost_PROGRAM_OPTIONS_LIBRARY} ${Boost_THREAD_LIBRARY})

TARGET_LINK_LIBRARIES(rdo rdo_utils)
TARGET_LINK_LIBRARIES(rdo rdo_kernel)
TARGET_LINK_LIBRARIES(rdo rdo_repository)
TARGET_LINK_LIBRARIES(rdo rdo_simulator)
TARGET_LINK_LIBRARIES(rdo rdo_simulator_report)

INSTALL(TARGETS rdo DESTINATION bin)
