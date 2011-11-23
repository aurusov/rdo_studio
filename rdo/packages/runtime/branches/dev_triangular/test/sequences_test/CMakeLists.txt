#==============================================================================
# Copyright (c) 2011 Evgeny Proydakov <lord.tiran@gmail.com>
#==============================================================================

SET(SOURCE_FILES
	main.cpp)

SET(TEXT_FILES
	data_exponential.txt
	data_normal.txt
	data_trinagular.txt
	data_uniform.txt)

ADD_BOOST_TEST(test_rdo_sequences ${SOURCE_FILES} ${TEXT_FILES})
ADD_DEPENDENCIES(test_rdo_sequences rdo_runtime)
TARGET_LINK_LIBRARIES(test_rdo_sequences rdo_runtime)