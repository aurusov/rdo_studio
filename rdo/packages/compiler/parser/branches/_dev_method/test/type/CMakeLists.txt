#==============================================================================
# Copyright (c) 2011 Evgeny Proydakov <lord.tiran@gmail.com>
#==============================================================================

MESSAGE(STATUS "CREATE  TEST  FOR  RDO  TYPE")

FILE(GLOB_RECURSE SOURCE_FILES RELATIVE ${CMAKE_CURRENT_SOURCE_DIR} *.cpp)
ADD_BOOST_TEST(test_parser_type ${SOURCE_FILES})

ADD_DEPENDENCIES(test_parser_type rdo_parser)
ADD_DEPENDENCIES(test_parser_type rdo_mbuilder)
ADD_DEPENDENCIES(test_parser_type rdo_kernel)
TARGET_LINK_LIBRARIES(test_parser_type rdo_parser)
TARGET_LINK_LIBRARIES(test_parser_type rdo_mbuilder)
TARGET_LINK_LIBRARIES(test_parser_type rdo_kernel)
