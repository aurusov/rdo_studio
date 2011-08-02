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
#define BOOST_TEST_MODULE rdoCommonTest
#include <boost/test/included/unit_test.hpp>
// ====================================================================== SYNOPSIS
#include "rdo_common/rdocommon.h"
#include "rdo_common/rdofile.h"
#include "rdo_common/rdotime.h"
#include "rdo_common/test/rdo_common_test/resource.h"
// ===============================================================================

const tstring testFileName(_T("test_file"));
const tstring resourceStr1(_T("test_101"));
const tstring resourceStr2(_T("test_102 22"));
const tstring resourceStr3(_T("test_103 test_101 33 test_102 22"));
const ruint64 createTestLocalTime = 129557633912040000;

BOOST_AUTO_TEST_SUITE(rdoCommonTest)

BOOST_AUTO_TEST_CASE(rdoCommonTest_ResourceFormat)
{
	tstring str1 = rdo::format(IDS_STRING101);
	BOOST_CHECK(str1 == resourceStr1);
	tstring str2 = rdo::format(IDS_STRING102, 22);
	BOOST_CHECK(str2 == resourceStr2);
	tstring str3 = rdo::format(IDS_STRING103, str1.c_str(), 33, str2.c_str());
	BOOST_CHECK(str3 == resourceStr3);
}

BOOST_AUTO_TEST_CASE(rdoCommonTest_File_Create)
{
	BOOST_CHECK(rdo::File::create(testFileName));
}

BOOST_AUTO_TEST_CASE(rdoCommonTest_File_Exist)
{
	BOOST_CHECK(rdo::File::exist(testFileName));
}

BOOST_AUTO_TEST_CASE(rdoCommonTest_File_ReadOnly)
{
	BOOST_CHECK(!rdo::File::read_only(testFileName));
}

BOOST_AUTO_TEST_CASE(rdoCommonTest_File_Remove)
{
	BOOST_CHECK(rdo::File::unlink(testFileName));
}

BOOST_AUTO_TEST_CASE(rdoCommonTest_Time)
{
	rdo::Time timeValue = rdo::Time::local();
	BOOST_CHECK(timeValue > createTestLocalTime);
	std::cout << _T("Today:  ") << timeValue.asString() << _T("  is not it?");
}

BOOST_AUTO_TEST_SUITE_END()
