/*
 * copyright: (c) RDO-Team, 2011
 * filename : main.cpp
 * author   : Evgeny Proydakov
 * email	: lord.tiran@gmail.com
 * date     : 16.06.2011
 * bref     : Test for rdo_common interface
 * indent   : 4T
 */
 
#define BOOST_TEST_MODULE test_rdo_interface
#include <boost/test/included/unit_test.hpp>

BOOST_AUTO_TEST_SUITE(test_rdo_interface)

BOOST_AUTO_TEST_CASE(test_rdo_interface_1)
{
    BOOST_CHECK(true);
}

BOOST_AUTO_TEST_CASE(test_rdo_interface_2)
{
	BOOST_REQUIRE(false);
}

BOOST_AUTO_TEST_CASE(test_rdo_interface_3)
{
	BOOST_ERROR(false);
}

BOOST_AUTO_TEST_SUITE_END()