#==============================================================================
# Copyright (c) 2011 Evgeny Proydakov <lord.tiran@gmail.com>
#==============================================================================

MESSAGE(STATUS "CREATE  TEST  FOR  RDO  TYPE")

SET(SOURCE_FILES
	main.cpp)

ADD_BOOST_TEST(test_rdo_type ${SOURCE_FILES})
ADD_DEPENDENCIES(test_rdo_type parser)
ADD_DEPENDENCIES(test_rdo_type rdo_mbuilder)
ADD_DEPENDENCIES(test_rdo_type rdo_kernel)
TARGET_LINK_LIBRARIES(test_rdo_type parser)
TARGET_LINK_LIBRARIES(test_rdo_type rdo_mbuilder)
TARGET_LINK_LIBRARIES(test_rdo_type rdo_kernel)