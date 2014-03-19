#==============================================================================
# Copyright (c) 2012 Evgeny Proydakov <lord.tiran@gmail.com>,
#               2013 Andrey Urusov <drobus@gmail.com>
#==============================================================================

MESSAGE(STATUS "CREATE  RDO_SIMULATOR_REPORT  LIBRARY")

FIND_PACKAGE(Boost)
INCLUDE_DIRECTORIES(${Boost_INCLUDE_DIRS})

INCLUDE_DIRECTORIES(${CMAKE_CURRENT_BINARY_DIR})

FILE(GLOB_RECURSE SOURCE_FILES RELATIVE ${CMAKE_CURRENT_SOURCE_DIR} *.cpp)
ADD_LIBRARY(rdo_simulator_report STATIC ${SOURCE_FILES})

INCLUDE(${PROJECT_SOURCE_DIR}/simulator/simulator.cmake)
SET_TARGET_PROPERTIES(rdo_simulator_report PROPERTIES FOLDER ${PROJECT_FOLDER}/.report)

ADD_DEPENDENCIES(rdo_simulator_report rdo_utils)
TARGET_LINK_LIBRARIES(rdo_simulator_report rdo_utils)

INSTALL(TARGETS rdo_simulator_report DESTINATION lib)
