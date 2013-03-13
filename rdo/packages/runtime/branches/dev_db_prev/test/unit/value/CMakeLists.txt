#==============================================================================
# Copyright (c) 2013 Klevanets Igor <cerevra@gmail.com>
#==============================================================================

SET(SOURCE_FILES
	main.cpp)

ADD_BOOST_TEST(test_unit_rdo_runtime_value ${SOURCE_FILES})
ADD_DEPENDENCIES(test_unit_rdo_runtime_value rdo_runtime)
TARGET_LINK_LIBRARIES(test_unit_rdo_runtime_value rdo_runtime)