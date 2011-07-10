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
// ====================================================================== SYNOPSIS
#include "rdo_common/rdocommon.h"
#include "rdo_common/rdofile.h"
#include "rdo_common/rdotime.h"
#include "rdo_common/test/rdo_common_test/resource.h"
// ===============================================================================
 
#define BOOST_TEST_MODULE test_rdo_common
#include <boost/test/included/unit_test.hpp>

BOOST_AUTO_TEST_SUITE(test_rdo_common)

BOOST_AUTO_TEST_CASE(test_rdo_common_1)
{
	tstring str1 = rdo::format(IDS_STRING101);
	tstring str2 = rdo::format(IDS_STRING102, 22);
	tstring str3 = rdo::format(IDS_STRING103, str1.c_str(), 33, str2.c_str());

	rbool exist     = rdo::File::exist    (_T("Копия main.cpp"));
	rbool read_only = rdo::File::read_only(_T("Копия main.cpp"));
	rbool unlink    = rdo::File::unlink   (_T("Копия main.cpp"));

	rdo::Time time     = rdo::Time::local();
	tstring   time_str = time.asString();

	int i = 1;
}

BOOST_AUTO_TEST_CASE(test_rdo_common_2)
{
	BOOST_REQUIRE(false);
}

BOOST_AUTO_TEST_CASE(test_rdo_common_3)
{
	BOOST_ERROR(false);
}

BOOST_AUTO_TEST_SUITE_END()