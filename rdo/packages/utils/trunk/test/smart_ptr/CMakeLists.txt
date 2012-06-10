#==============================================================================
# Copyright (c) 2011 Evgeny Proydakov <lord.tiran@gmail.com>
#==============================================================================

SET(SOURCE_FILES
	main.cpp)

ADD_BOOST_TEST(test_rdo_utils_smart_ptr ${SOURCE_FILES})
ADD_DEPENDENCIES(test_rdo_utils_smart_ptr rdo_utils)
TARGET_LINK_LIBRARIES(test_rdo_utils_smart_ptr rdo_utils)