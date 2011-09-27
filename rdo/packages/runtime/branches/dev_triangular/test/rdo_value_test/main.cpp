/*!
  \copyright (c) RDO-Team, 2011
  \file      main.cpp
  \authors   Урусов Андрей (rdo@rk9.bmstu.ru)
  \date      13.07.2009
  \brief     Тест для RDOValue
  \indent    4T
*/

// ---------------------------------------------------------------------------- PCH
// ----------------------------------------------------------------------- INCLUDES
#include <iostream>
#include <boost/regex.hpp>
#define BOOST_TEST_MODULE RDOValue_Test
#include <boost/test/included/unit_test.hpp>
// ----------------------------------------------------------------------- SYNOPSIS
#include "simulator/runtime/rdo_runtime.h"
// --------------------------------------------------------------------------------

BOOST_AUTO_TEST_SUITE(RDOValue_Test)

BOOST_AUTO_TEST_CASE(RDOValue_String)
{
	const tstring str1 = _T("qqq");
	rdoRuntime::RDOValue value1(str1);
	BOOST_CHECK(value1.getString  () == str1);
	BOOST_CHECK(value1.getAsString() == str1);

	rdoRuntime::RDOValue value2 = value1;
	BOOST_CHECK(value2.getString  () == str1);
	BOOST_CHECK(value2.getAsString() == str1);
	BOOST_CHECK(value2 == value1);

	const tstring str2 = _T("aaa");
	value2 += str2;
	BOOST_CHECK(value2.getString  () == str1 + str2);
	BOOST_CHECK(value2.getAsString() == str1 + str2);
}

BOOST_AUTO_TEST_CASE(RDOValue_Resource)
{
	using namespace rdoRuntime;

	LPRDORuntime pRuntime = rdo::Factory<RDORuntime>::create();
	ASSERT(pRuntime);

	LPRDOResourceType pResourceType = rdo::Factory<RDOResourceType>::create(0);
	ASSERT(pResourceType);
	LPIResourceType pResourceFactory = pResourceType.interface_cast<IResourceType>();
	ASSERT(pResourceFactory);

	std::vector<RDOValue> paramList;
	paramList.push_back(RDOValue(1      ));
	paramList.push_back(RDOValue(2.2    ));
	paramList.push_back(RDOValue(_T("3")));

	LPRDOResource pResource = pResourceFactory->createRes(pRuntime, paramList, true, true);
	ASSERT(pResource);

	RDOValue value1(pResourceType, pResource);

	pRuntime  = NULL;
	value1    = RDOValue();

	BOOST_CHECK(pResource.owner());

	pResource = NULL;
}

BOOST_AUTO_TEST_SUITE_END()
