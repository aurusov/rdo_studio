/*
 * copyright: (c) RDO-Team, 2011
 * filename : main.cpp
 * author   : Evgeny Proydakov
 * email	: lord.tiran@gmail.com
 * date     : 16.06.2011
 * bref     : Test rdo_common
 * indent   : 4T
 */

// ====================================================================== PCH
// ====================================================================== INCLUDES
#include <boost/regex.hpp>
// ====================================================================== SYNOPSIS
#include "rdo_common/rdocommon.h"
#include "rdo_common/rdofile.h"
#include "rdo_common/rdotime.h"
#include "rdo_common/test/rdo_common_test/resource.h"

#include <config-tiles.h>
// ===============================================================================

#define BOOST_TEST_MODULE test_rdo_common
#include <boost/test/included/unit_test.hpp>

BOOST_AUTO_TEST_SUITE(test_rdo_common)

BOOST_AUTO_TEST_CASE(test_rdo_common_1)
{
	// TODO HELP ME !!!!
	tstring str1 = rdo::format(IDS_STRING101);
	tstring str2 = rdo::format(IDS_STRING102, 22);
	tstring str3 = rdo::format(IDS_STRING103, str1.c_str(), 33, str2.c_str());
}

BOOST_AUTO_TEST_CASE(test_create_file)
{
	BOOST_CHECK(rdo::File::create(_T(tstring(resource_directory) + tstring(test_file_name))));
}

BOOST_AUTO_TEST_CASE(test_exist_file)
{
	BOOST_CHECK(rdo::File::exist(_T(tstring(resource_directory) + tstring(test_file_name))));
}

BOOST_AUTO_TEST_CASE(test_read_only_file)
{
	BOOST_CHECK(!rdo::File::read_only(_T(tstring(resource_directory) + tstring(test_file_name))));
}

BOOST_AUTO_TEST_CASE(test_remove_file)
{
	BOOST_CHECK(rdo::File::unlink(_T(tstring(resource_directory) + tstring(test_file_name))));
}

BOOST_AUTO_TEST_CASE(test_rdo_check_data)
{
	// TODO EDIT REGEX
	rdo::Time time1     = rdo::Time::local();
	tstring   time_str = time1.asString();

	boost::regex expression("(.*)");
	boost::cmatch what; 
	BOOST_CHECK(boost::regex_match(time_str.c_str(), what, expression));
}

BOOST_AUTO_TEST_SUITE_END()