#==============================================================================
# Copyright (c) 2011 Evgeny Proydakov <lord.tiran@gmail.com>
#==============================================================================

FIND_PACKAGE(Boost COMPONENTS chrono REQUIRED)
INCLUDE_DIRECTORIES(${Boost_INCLUDE_DIRS})

FILE(GLOB_RECURSE SOURCE_FILES RELATIVE ${CMAKE_CURRENT_SOURCE_DIR} *.cpp)
ADD_BOOST_TEST(test_runtime_calc ${SOURCE_FILES})

ADD_DEPENDENCIES(test_runtime_calc rdo_runtime)
TARGET_LINK_LIBRARIES(test_runtime_calc rdo_runtime)
TARGET_LINK_LIBRARIES(test_runtime_calc ${Boost_CHRONO_LIBRARY})
