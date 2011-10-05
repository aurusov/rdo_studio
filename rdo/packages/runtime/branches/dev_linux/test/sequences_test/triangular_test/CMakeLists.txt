#==============================================================================
# Copyright (c) 2011 Evgeny Proydakov <lord.tiran@gmail.com>
#==============================================================================

SET(FILES
	data.txt)

SET(SOURCE_FILES
	main.cpp)

ADD_BOOST_TEST(test_rdo_runtime_sequences_triangular ${SOURCE_FILES} ${FILES})
ADD_DEPENDENCIES(test_rdo_runtime_sequences_triangular rdo_runtime)
TARGET_LINK_LIBRARIES(test_rdo_runtime_sequences_triangular rdo_runtime)