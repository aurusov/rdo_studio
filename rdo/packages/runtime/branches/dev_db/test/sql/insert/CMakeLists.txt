#==============================================================================
# Copyright (c) 2011 Igor Klevanets <cerevra@gmail.com>
#==============================================================================

SET(SOURCE_FILES
	main.cpp)

ADD_BOOST_TEST(test_rdo_runtime_sql_insert ${SOURCE_FILES})
ADD_DEPENDENCIES(test_rdo_runtime_sql_insert rdo_sql_general_db)
TARGET_LINK_LIBRARIES(test_rdo_runtime_sql_insert rdo_sql_general_db)