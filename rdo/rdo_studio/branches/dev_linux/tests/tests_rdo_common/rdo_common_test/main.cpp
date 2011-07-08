/*
 * copyright: (c) RDO-Team, 2011
 * filename : main.cpp
 * author   : Evgeny Proydakov
 * email	: lord.tiran@gmail.com
 * date     : 16.06.2011
 * bref     : Test rdo_common
 * indent   : 4T
 */

#include "rdo_common/rdocommon.h"
#include "rdo_common/rdofile.h"
#include "rdo_common/rdotime.h"
 
#define BOOST_TEST_MODULE test_rdo_common
#include <boost/test/included/unit_test.hpp>

BOOST_AUTO_TEST_SUITE(test_rdo_common)

BOOST_AUTO_TEST_CASE(test_rdo_common_1)
{
    BOOST_CHECK(true);
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