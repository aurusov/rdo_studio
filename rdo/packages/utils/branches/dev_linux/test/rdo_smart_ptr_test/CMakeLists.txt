SET(SOURCE_FILES
	main.cpp)

ADD_BOOST_TEST(test_rdo_utils_smart_prt ${SOURCE_FILES})
ADD_DEPENDENCIES(test_rdo_utils_smart_prt rdo_utils)
TARGET_LINK_LIBRARIES(test_rdo_utils_smart_prt rdo_utils)