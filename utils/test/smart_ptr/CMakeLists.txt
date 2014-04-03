#==============================================================================
# Copyright (c) 2011 Evgeny Proydakov <lord.tiran@gmail.com>
#==============================================================================

FILE(GLOB_RECURSE SOURCE_FILES RELATIVE ${CMAKE_CURRENT_SOURCE_DIR} *.cpp)
ADD_BOOST_TEST(test_utils_smart_ptr ${SOURCE_FILES})

ADD_DEPENDENCIES(test_utils_smart_ptr rdo_utils)
TARGET_LINK_LIBRARIES(test_utils_smart_ptr rdo_utils)
