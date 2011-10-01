#==============================================================================
# Copyright (c) 2011 Evgeny Proydakov <lord.tiran@gmail.com>
#==============================================================================

SET(SOURCE_FILES
	main.cpp)

SET(INLINE_FILES
	arrayCreate.inl
	arrayTestFunction.inl)

ADD_BOOST_TEST(test_rdo_runtime_array ${SOURCE_FILES} ${INLINE_FILES})
ADD_DEPENDENCIES(test_rdo_runtime_array rdo_runtime)
TARGET_LINK_LIBRARIES(test_rdo_runtime_array rdo_runtime)