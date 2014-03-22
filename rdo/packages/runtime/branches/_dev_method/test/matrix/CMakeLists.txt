#==============================================================================
# Copyright (c) 2011 Evgeny Proydakov <lord.tiran@gmail.com>
#==============================================================================

FILE(GLOB_RECURSE SOURCE_FILES RELATIVE ${CMAKE_CURRENT_SOURCE_DIR} *.cpp)
ADD_BOOST_TEST(test_runtime_matrix ${SOURCE_FILES})

ADD_DEPENDENCIES(test_runtime_matrix rdo_runtime)
TARGET_LINK_LIBRARIES(test_runtime_matrix rdo_runtime)
