#==============================================================================
# Copyright (c) 2011-2012 Evgeny Proydakov <lord.tiran@gmail.com>
#==============================================================================

MESSAGE(STATUS "CREATE  RDO_UTILS  LIBRARY ")

FIND_PACKAGE(Boost COMPONENTS system filesystem thread date_time locale REQUIRED)
LINK_DIRECTORIES(${Boost_LIBRARY_DIRS})
INCLUDE_DIRECTORIES(${Boost_INCLUDE_DIRS})

FILE(GLOB_RECURSE SOURCE_FILES RELATIVE ${CMAKE_CURRENT_SOURCE_DIR} src/*.cpp)

ADD_LIBRARY(rdo_utils STATIC ${SOURCE_FILES})
SET_TARGET_PROPERTIES(rdo_utils PROPERTIES LINKER_LANGUAGE CXX)

TARGET_LINK_LIBRARIES(rdo_utils ${Boost_SYSTEM_LIBRARY} ${Boost_FILESYSTEM_LIBRARY} ${Boost_THREAD_LIBRARY} ${Boost_DATE_TIME_LIBRARY} ${Boost_LOCALE_LIBRARY})

INSTALL(TARGETS rdo_utils DESTINATION lib)

SET(RDO_UTILS_TEST_PATH ${CMAKE_CURRENT_SOURCE_DIR}/test)
ADD_SUBDIRECTORY(${RDO_UTILS_TEST_PATH})
