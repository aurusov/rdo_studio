/*!
  \copyright (c) RDO-Team, 2011
  \file      main.cpp
  \author    Урусов Андрей (rdo@rk9.bmstu.ru)
  \authors   Пройдаков Евгений (lord.tiran@gmail.com)
  \date      10.05.2009
  \brief     Тест common-библиотеки
  \indent    4T
*/

// ----------------------------------------------------------------------- PLATFORM
#include "utils/platform.h"
// ---------------------------------------------------------------------------- PCH
// ----------------------------------------------------------------------- INCLUDES
#include <boost/regex.hpp>
#define BOOST_TEST_MODULE RDOCommon_Test
#include <boost/test/included/unit_test.hpp>
// ----------------------------------------------------------------------- SYNOPSIS
#include "utils/rdocommon.h"
#include "utils/rdofile.h"
#include "utils/rdotime.h"
#include "utils/test/utils_common_test/resource.h"
// --------------------------------------------------------------------------------

const tstring s_testFileName(_T("test_file"));
const tstring s_resourceStr1(_T("test_101"));
const tstring s_resourceStr2(_T("test_102 22"));
const tstring s_resourceStr3(_T("test_103 test_101 33 test_102 22"));
const ruint64 s_createTestLocalTime = 129557633912040000;

BOOST_AUTO_TEST_SUITE(RDOCommon_Test)

#ifdef OST_WINDOWS
BOOST_AUTO_TEST_CASE(RDOCommon_ResourceFormat)
{
	tstring str1 = rdo::format(IDS_STRING101);
	BOOST_CHECK(str1 == s_resourceStr1);
	tstring str2 = rdo::format(IDS_STRING102, 22);
	BOOST_CHECK(str2 == s_resourceStr2);
	tstring str3 = rdo::format(IDS_STRING103, str1.c_str(), 33, str2.c_str());
	BOOST_CHECK(str3.empty());
}
#endif // OST_WINDOWS

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

BOOST_AUTO_TEST_CASE(RDOCommon_FileSplitPathInLinux)
{
	tstring file(_T("/rdo/русский пробел/files/проект.smr"));
	tstring dir;
	tstring name;
	tstring ext;

	BOOST_CHECK(rdo::File::splitpath(file, dir, name, ext));
	BOOST_CHECK(dir  == _T("/rdo/русский пробел/files/"));
	BOOST_CHECK(name == _T("проект"));
	BOOST_CHECK(ext  == _T(".smr"));
}

BOOST_AUTO_TEST_CASE(RDOCommon_FileSplitByRootPathInLinux)
{
	tstring file(_T("/проект.smr"));
	tstring dir;
	tstring name;
	tstring ext;

	BOOST_CHECK(rdo::File::splitpath(file, dir, name, ext));
	BOOST_CHECK(dir  == _T("/"));
	BOOST_CHECK(name == _T("проект"));
	BOOST_CHECK(ext  == _T(".smr"));
}

#ifdef OST_WINDOWS
BOOST_AUTO_TEST_CASE(RDOCommon_FileSplitPathInWindows)
{
	tstring file(_T("С:/rdo/русский пробел/files/проект.smr"));
	tstring dir;
	tstring name;
	tstring ext;

	BOOST_CHECK(rdo::File::splitpath(file, dir, name, ext));
	BOOST_CHECK(dir  == _T("С:/rdo/русский пробел/files/"));
	BOOST_CHECK(name == _T("проект"));
	BOOST_CHECK(ext  == _T(".smr"));
}

BOOST_AUTO_TEST_CASE(RDOCommon_FileSplitByRootPathInWindows)
{
	tstring file(_T("С:/проект.smr"));
	tstring dir;
	tstring name;
	tstring ext;

	BOOST_CHECK(rdo::File::splitpath(file, dir, name, ext));
	BOOST_CHECK(dir  == _T("С:/"));
	BOOST_CHECK(name == _T("проект"));
	BOOST_CHECK(ext  == _T(".smr"));
}

BOOST_AUTO_TEST_CASE(RDOCommon_FileSplitPathInWindows_BackSlash)
{
	tstring file(_T("С:\\rdo\\русский пробел\\files\\проект.smr"));
	tstring dir;
	tstring name;
	tstring ext;

	BOOST_CHECK(rdo::File::splitpath(file, dir, name, ext));
	BOOST_CHECK(dir  == _T("С:\\rdo\\русский пробел\\files\\"));
	BOOST_CHECK(name == _T("проект"));
	BOOST_CHECK(ext  == _T(".smr"));
}

BOOST_AUTO_TEST_CASE(RDOCommon_FileSplitByRootPathInWindows_BackSlash)
{
	tstring file(_T("С:\\проект.smr"));
	tstring dir;
	tstring name;
	tstring ext;

	BOOST_CHECK(rdo::File::splitpath(file, dir, name, ext));
	BOOST_CHECK(dir  == _T("С:\\"));
	BOOST_CHECK(name == _T("проект"));
	BOOST_CHECK(ext  == _T(".smr"));
}
#endif // #endif

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

BOOST_AUTO_TEST_SUITE_END() // RDOCommon_Test
