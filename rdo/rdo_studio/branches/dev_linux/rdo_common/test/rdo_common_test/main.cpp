/*
 * copyright: (c) RDO-Team, 2011
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
#include "rdo_common/test/rdo_common_test/config-tiles.h"
// ===============================================================================

BOOST_AUTO_TEST_SUITE(test_rdo_common)

BOOST_AUTO_TEST_CASE(test_rdo_common_1)
{
	// TODO Необходимо подключить файл ресурсов. Восстанавливай удаленный rdo_common_test.vcproj, см. его там. 
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
	rdo::Time timeValue = rdo::Time::local();
	tstring   timeStr   = timeValue.asString();

	//! Какой смысл проверять через regex ? - как бы вы предложили проверить ???
	boost::regex expression("(.*)");
	boost::cmatch what; 
	BOOST_CHECK(boost::regex_match(timeStr.c_str(), what, expression));
}

BOOST_AUTO_TEST_SUITE_END()