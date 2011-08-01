/*
 * copyright: (c) RDO-Team, 2009
 * filename : main.cpp
 * author   : Урусов Андрей, Evgeny Proydakov
 * date     : 10.05.2009
 * bref     : Тест common-библиотеки
 * indent   : 4T
 */

// ====================================================================== PCH
// ====================================================================== INCLUDES
#include <boost/regex.hpp>
#define BOOST_TEST_MODULE test_rdo_common
#include <boost/test/included/unit_test.hpp>
// ====================================================================== SYNOPSIS
#include "rdo_common/rdocommon.h"
#include "rdo_common/rdofile.h"
#include "rdo_common/rdotime.h"
#include "rdo_common/test/rdo_common_test/resource.h"
// ===============================================================================

#define test_file_name "test_file"

#define create_test_local_time 129557633912040000
#define first_resource_string "test_101"
#define second_resource_string "test_102 22"
#define third_resource_string "test_103 test_101 33 test_102 22"

BOOST_AUTO_TEST_SUITE(test_rdo_common)

BOOST_AUTO_TEST_CASE(test_rdo_common_1)
{
	tstring str1 = rdo::format(IDS_STRING101);
	BOOST_CHECK(str1 == first_resource_string);
	tstring str2 = rdo::format(IDS_STRING102, 22);
	BOOST_CHECK(str2 == second_resource_string);
	tstring str3 = rdo::format(IDS_STRING103, str1.c_str(), 33, str2.c_str());
	BOOST_CHECK(str3 == third_resource_string);
}

BOOST_AUTO_TEST_CASE(test_create_file)
{
	BOOST_CHECK(rdo::File::create(_T(test_file_name)));
}

BOOST_AUTO_TEST_CASE(test_exist_file)
{
	BOOST_CHECK(rdo::File::exist(_T(test_file_name)));
}

BOOST_AUTO_TEST_CASE(test_read_only_file)
{
	BOOST_CHECK(!rdo::File::read_only(_T(test_file_name)));
}

BOOST_AUTO_TEST_CASE(test_remove_file)
{
	BOOST_CHECK(rdo::File::unlink(_T(test_file_name)));
}

BOOST_AUTO_TEST_CASE(test_rdo_check_data)
{
	rdo::Time timeValue = rdo::Time::local();
	BOOST_CHECK(timeValue > create_test_local_time);
	tstring   timeStr   = timeValue.asString();

	std::cout << "Today:  " << timeStr << "  is not it??";
}

BOOST_AUTO_TEST_SUITE_END()