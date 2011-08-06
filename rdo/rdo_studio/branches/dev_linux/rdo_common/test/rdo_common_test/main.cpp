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
#define BOOST_TEST_MODULE RDOCommon_Test
#include <boost/test/included/unit_test.hpp>
// ====================================================================== SYNOPSIS
#include "rdo_common/rdocommon.h"
#include "rdo_common/rdofile.h"
#include "rdo_common/rdotime.h"
#include "rdo_common/test/rdo_common_test/resource.h"
// ===============================================================================

const tstring s_testFileName(_T("test_file.test"));
const tstring s_testFileNameDirectory(_T("test_directory/test_file.test"));
const tstring s_resourceStr1(_T("test_101"));
const tstring s_resourceStr2(_T("test_102 22"));
const tstring s_resourceStr3(_T("test_103 test_101 33 test_102 22"));
const ruint64 s_createTestLocalTime = 129557633912040000;

BOOST_AUTO_TEST_SUITE(RDOCommon_Test)

BOOST_AUTO_TEST_CASE(RDOCommon_Test)
{
	{
		BOOST_CHECK(rdo::File::create(s_testFileName));
	}

	{
		tstring fileName;
		tstring fileDir;
		tstring fileExt;
		BOOST_CHECK(rdo::File::splitpath(s_testFileNameDirectory, fileDir, fileName, fileExt));
		BOOST_CHECK(fileName == tstring("test_file"));
		BOOST_CHECK(fileDir == tstring("test_directory/"));
		BOOST_CHECK(fileExt == tstring(".test"));
	}

	{
		BOOST_CHECK(rdo::File::exist(s_testFileName));
	}

	{
		BOOST_CHECK(!rdo::File::read_only(s_testFileName));
	}

	{
		BOOST_CHECK(rdo::File::unlink(s_testFileName));
	}

	{
		rdo::Time timeValue = rdo::Time::local();
		BOOST_CHECK(timeValue > s_createTestLocalTime);
		std::cout << _T("Today:  ") << timeValue.asString() << _T("  is not it?");
	}
}

BOOST_AUTO_TEST_SUITE_END()
