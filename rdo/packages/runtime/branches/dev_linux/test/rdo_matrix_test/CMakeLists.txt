SET(SOURCE_FILES
	main.cpp)

SET(INLINE_FILES
	matrixCreate.inl
	matrixTestFunction.inl)

ADD_BOOST_TEST(test_rdo_runtime_matrix ${SOURCE_FILES} ${INLINE_FILES})
ADD_DEPENDENCIES(test_rdo_runtime_matrix rdo_runtime)
TARGET_LINK_LIBRARIES(test_rdo_runtime_matrix rdo_runtime)
