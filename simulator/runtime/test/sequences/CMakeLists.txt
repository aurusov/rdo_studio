#==============================================================================
# Copyright (c) 2011 Evgeny Proydakov <lord.tiran@gmail.com>
#==============================================================================

FILE(GLOB_RECURSE SOURCE_FILES RELATIVE ${CMAKE_CURRENT_SOURCE_DIR} *.cpp)
FILE(GLOB_RECURSE TEXT_FILES RELATIVE ${CMAKE_CURRENT_SOURCE_DIR} *.txt)
ADD_BOOST_TEST(test_runtime_sequences ${SOURCE_FILES} ${TEXT_FILES})

ADD_DEPENDENCIES(test_runtime_sequences rdo_runtime)
TARGET_LINK_LIBRARIES(test_runtime_sequences rdo_runtime)
