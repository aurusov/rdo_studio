/*!
  \copyright (c) RDO-Team, 2011
  \file      main.cpp
  \author    Урусов Андрей (rdo@rk9.bmstu.ru)
  \authors   Пройдаков Евгений (lord.tiran@gmail.com)
  \date      10.05.2009
  \brief     Тест common-библиотеки
  \indent    4T
*/

// ---------------------------------------------------------------------------- PCH
// ----------------------------------------------------------------------- INCLUDES
#include <boost/regex.hpp>
#define BOOST_TEST_MODULE RDOCommon_Test
#include <boost/test/included/unit_test.hpp>
// ----------------------------------------------------------------------- SYNOPSIS
#include "rdo_common/rdocommon.h"
#include "rdo_common/rdofile.h"
#include "rdo_common/rdotime.h"
#include "rdo_common/test/rdo_common_test/resource.h"
// --------------------------------------------------------------------------------

const tstring s_testFileName(_T("test_file"));
const tstring s_resourceStr1(_T("test_101"));
const tstring s_resourceStr2(_T("test_102 22"));
const tstring s_resourceStr3(_T("test_103 test_101 33 test_102 22"));
const ruint64 s_createTestLocalTime = 129557633912040000;

BOOST_AUTO_TEST_SUITE(RDOCommon_Test)

#ifdef WIN32
BOOST_AUTO_TEST_CASE(RDOCommon_ResourceFormat)
{
	tstring str1 = rdo::format(IDS_STRING101);
	BOOST_CHECK(str1 == s_resourceStr1);
	tstring str2 = rdo::format(IDS_STRING102, 22);
	BOOST_CHECK(str2 == s_resourceStr2);
	tstring str3 = rdo::format(IDS_STRING103, str1.c_str(), 33, str2.c_str());
	BOOST_CHECK(str3 == s_resourceStr3);
}
#endif

BOOST_AUTO_TEST_CASE(RDOCommon_FileCreate)
{
	BOOST_CHECK(rdo::File::create(s_testFileName));
}

BOOST_AUTO_TEST_CASE(RDOCommon_FileExist)
{
	BOOST_CHECK(rdo::File::exist(s_testFileName));
}

BOOST_AUTO_TEST_CASE(RDOCommon_FileReadOnly)
{
	BOOST_CHECK(!rdo::File::read_only(s_testFileName));
}

BOOST_AUTO_TEST_CASE(RDOCommon_FileRemove)
{
	BOOST_CHECK(rdo::File::unlink(s_testFileName));
}

BOOST_AUTO_TEST_CASE(RDOCommon_GetTempFile)
{
	std::set<tstring> name_set;
	for(int i = 0; i < 15; ++i)
	{
		tstring file_name = rdo::File::getTempFileName();
		BOOST_CHECK(name_set.find(file_name) == name_set.end());
		name_set.insert(file_name);
	}
}

BOOST_AUTO_TEST_CASE(RDOCommon_Time)
{
	rdo::Time timeValue = rdo::Time::local();
	BOOST_CHECK(timeValue > s_createTestLocalTime);
	std::cout << _T("Today:  ") << timeValue.asString() << _T("  is not it?");
}

BOOST_AUTO_TEST_SUITE_END()