/*
 * copyright: (c) RDO-Team, 2011
 * filename : main.cpp
 * author   : Proydakov Evgeny
 * email	: lord.tiran@gmail.com
 * date     : 16.06.2011
 * bref     : Test rdo_common smart pointer
 * indent   : 4T
 */

// ====================================================================== PCH
// ====================================================================== INCLUDES
#include <vector>
#include <list>
#include <iostream>
// ====================================================================== SYNOPSIS
#include "rdo_common/smart_ptr/intrusive_ptr.h"
#include "rdo_common/smart_ptr/interface_ptr.h"
#include "rdo_common/rdodebug.h"
// ===============================================================================
 
#define BOOST_TEST_MODULE test_rdo_smart_prt
#include <boost/test/included/unit_test.hpp>

BOOST_AUTO_TEST_SUITE(test_rdo_smart_prt)

BOOST_AUTO_TEST_CASE(test_rdo_smart_prt_1)
{
    BOOST_CHECK(true);
}

BOOST_AUTO_TEST_CASE(test_rdo_smart_prt_2)
{
	BOOST_REQUIRE(true);
}

BOOST_AUTO_TEST_CASE(test_rdo_smart_prt_3)
{
	BOOST_ERROR(true);
}

BOOST_AUTO_TEST_SUITE_END()