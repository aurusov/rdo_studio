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
#include "utils/src/common/platform.h"
// ----------------------------------------------------------------------- INCLUDES
#define BOOST_TEST_MODULE RDOCommon_Test
#include "utils/src/common/warning_disable.h"
#include <boost/format.hpp>
#include <boost/test/included/unit_test.hpp>
#include "utils/src/common/warning_enable.h"
// ----------------------------------------------------------------------- SYNOPSIS
#include "utils/src/common/rdocommon.h"
#include "utils/src/file/rdofile.h"
#include "utils/src/time/rdotime.h"
#include "utils/src/locale/rdolocale.h"
// --------------------------------------------------------------------------------

const std::string s_testFileName("test_file");
const uint64_t s_createTestLocalTime = 129557633912040000;

BOOST_AUTO_TEST_SUITE(RDOCommon_Test)

BOOST_AUTO_TEST_CASE(RDOCommon_FileCreate)
{
	rdo::locale::initForTest();
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

#ifdef OST_LINUX
BOOST_AUTO_TEST_CASE(RDOCommon_FileSplitPathInLinux)
{
	boost::filesystem::path file(rdo::locale::convertToWStr("/rdo/русский пробел/files/проект.smr"));
	boost::filesystem::path dir;
	boost::filesystem::path name;
	boost::filesystem::path ext;

	BOOST_CHECK(rdo::File::splitpath(file, dir, name, ext));
	BOOST_CHECK(dir  == rdo::locale::convertToWStr("/rdo/русский пробел/files/"));
	BOOST_CHECK(name == rdo::locale::convertToWStr("проект"));
	BOOST_CHECK(ext  == rdo::locale::convertToWStr(".smr"));
	BOOST_CHECK(rdo::locale::convertFromWStr(dir.wstring()) == "/rdo/русский пробел/files/");
}

BOOST_AUTO_TEST_CASE(RDOCommon_FileSplitByRootPathInLinux)
{
	boost::filesystem::path file(rdo::locale::convertToWStr("/проект.smr"));
	boost::filesystem::path dir;
	boost::filesystem::path name;
	boost::filesystem::path ext;

	BOOST_CHECK(rdo::File::splitpath(file, dir, name, ext));
	BOOST_CHECK(dir  == rdo::locale::convertToWStr("/"));
	BOOST_CHECK(name == rdo::locale::convertToWStr("проект"));
	BOOST_CHECK(ext  == rdo::locale::convertToWStr(".smr"));
}
#endif

#ifdef OST_WINDOWS
BOOST_AUTO_TEST_CASE(RDOCommon_FileSplitPathInWindows)
{
	boost::filesystem::path file(rdo::locale::convertToWStr("C:/rdo/русский пробел/files/проект.smr"));
	boost::filesystem::path dir;
	boost::filesystem::path name;
	boost::filesystem::path ext;

	BOOST_CHECK(rdo::File::splitpath(file, dir, name, ext));
	BOOST_CHECK(dir  == rdo::locale::convertToWStr("C:\\rdo\\русский пробел\\files\\"));
	BOOST_CHECK(name == rdo::locale::convertToWStr("проект"));
	BOOST_CHECK(ext  == rdo::locale::convertToWStr(".smr"));
}

BOOST_AUTO_TEST_CASE(RDOCommon_FileSplitByRootPathInWindows)
{
	boost::filesystem::path file(rdo::locale::convertToWStr("C:/проект.smr"));
	boost::filesystem::path dir;
	boost::filesystem::path name;
	boost::filesystem::path ext;

	BOOST_CHECK(rdo::File::splitpath(file, dir, name, ext));
	BOOST_CHECK(dir  == rdo::locale::convertToWStr("C:\\"));
	BOOST_CHECK(name == rdo::locale::convertToWStr("проект"));
	BOOST_CHECK(ext  == rdo::locale::convertToWStr(".smr"));
}

BOOST_AUTO_TEST_CASE(RDOCommon_FileSplitPathInWindows_BackSlash)
{
	boost::filesystem::path file(rdo::locale::convertToWStr("C:\\rdo\\русский пробел\\files\\проект.smr"));
	boost::filesystem::path dir;
	boost::filesystem::path name;
	boost::filesystem::path ext;

	BOOST_CHECK(rdo::File::splitpath(file, dir, name, ext));
	BOOST_CHECK(dir  == rdo::locale::convertToWStr("C:\\rdo\\русский пробел\\files\\"));
	BOOST_CHECK(name == rdo::locale::convertToWStr("проект"));
	BOOST_CHECK(ext  == rdo::locale::convertToWStr(".smr"));
	BOOST_CHECK(rdo::locale::convertFromWStr(dir.wstring()) == "C:\\rdo\\русский пробел\\files\\");
}

BOOST_AUTO_TEST_CASE(RDOCommon_FileSplitByRootPathInWindows_BackSlash)
{
	boost::filesystem::path file(rdo::locale::convertToWStr("C:\\проект.smr"));
	boost::filesystem::path dir;
	boost::filesystem::path name;
	boost::filesystem::path ext;

	BOOST_CHECK(rdo::File::splitpath(file, dir, name, ext));
	BOOST_CHECK(dir  == rdo::locale::convertToWStr("C:\\"));
	BOOST_CHECK(name == rdo::locale::convertToWStr("проект"));
	BOOST_CHECK(ext  == rdo::locale::convertToWStr(".smr"));
}
#endif // #endif

BOOST_AUTO_TEST_CASE(RDOCommon_GetTempFile)
{
	std::set<boost::filesystem::path> nameSet;
	for (int i = 0; i < 15; ++i)
	{
		boost::filesystem::path fileName = rdo::File::getTempFileName();
		rdo::File::unlink(fileName);
		BOOST_CHECK(nameSet.find(fileName) == nameSet.end());
		nameSet.insert(fileName);
	}
}

BOOST_AUTO_TEST_CASE(RDOCommon_Time)
{
	rdo::Time timeValue = rdo::Time::local();
	BOOST_CHECK(timeValue > s_createTestLocalTime);
	std::cout << boost::str(boost::format("Today: %1%, is not it?") % timeValue.asString()) << std::endl;
}

BOOST_AUTO_TEST_SUITE_END() // RDOCommon_Test
