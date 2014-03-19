#==============================================================================
# Copyright (c) 2011 Evgeny Proydakov <lord.tiran@gmail.com>,
#               2013 Andrey Urusov <drobus@gmail.com>
#==============================================================================

MESSAGE(STATUS "CREATE  RDO_SIMULATOR  LIBRARY")

FIND_PACKAGE(Boost COMPONENTS system filesystem thread REQUIRED)
LINK_DIRECTORIES(${Boost_LIBRARY_DIRS})
INCLUDE_DIRECTORIES(${Boost_INCLUDE_DIRS})

INCLUDE_DIRECTORIES(${CMAKE_CURRENT_BINARY_DIR})

FILE(GLOB_RECURSE SOURCE_FILES RELATIVE ${CMAKE_CURRENT_SOURCE_DIR} *.cpp)
ADD_LIBRARY(rdo_simulator STATIC ${SOURCE_FILES})

INCLUDE(${PROJECT_SOURCE_DIR}/simulator/simulator.cmake)
SET_TARGET_PROPERTIES(rdo_simulator PROPERTIES FOLDER ${PROJECT_FOLDER}/.service)

ADD_DEPENDENCIES(rdo_simulator rdo_utils)
ADD_DEPENDENCIES(rdo_simulator rdo_runtime)
ADD_DEPENDENCIES(rdo_simulator rdo_parser)
ADD_DEPENDENCIES(rdo_simulator rdo_mbuilder)
ADD_DEPENDENCIES(rdo_simulator rdo_procgui)
ADD_DEPENDENCIES(rdo_simulator rdo_simulator_report)
ADD_DEPENDENCIES(rdo_simulator rdo_converter_smr2rdox)

TARGET_LINK_LIBRARIES(rdo_simulator ${Boost_SYSTEM_LIBRARY} ${Boost_FILESYSTEM_LIBRARY} ${Boost_THREAD_LIBRARY})
TARGET_LINK_LIBRARIES(rdo_simulator rdo_utils)
TARGET_LINK_LIBRARIES(rdo_simulator rdo_runtime)
TARGET_LINK_LIBRARIES(rdo_simulator rdo_parser)
TARGET_LINK_LIBRARIES(rdo_simulator rdo_mbuilder)
TARGET_LINK_LIBRARIES(rdo_simulator rdo_procgui)
TARGET_LINK_LIBRARIES(rdo_simulator rdo_simulator_report)
TARGET_LINK_LIBRARIES(rdo_simulator rdo_converter_smr2rdox)

INSTALL(TARGETS rdo_simulator DESTINATION lib)
