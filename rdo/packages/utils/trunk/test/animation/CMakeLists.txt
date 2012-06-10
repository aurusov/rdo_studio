#==============================================================================
# Copyright (c) 2011 Evgeny Proydakov <lord.tiran@gmail.com>
#==============================================================================

SET(SOURCE_FILES
	main.cpp)

ADD_BOOST_TEST(test_rdo_animation ${SOURCE_FILES})
ADD_DEPENDENCIES(test_rdo_animation rdo_utils)
TARGET_LINK_LIBRARIES(test_rdo_animation rdo_utils)