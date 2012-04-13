/*!
  \copyright (c) RDO-Team, 2011
  \file      main.cpp
  \authors   Урусов Андрей (rdo@rk9.bmstu.ru)
  \authors   Пройдаков Евгений (lord.tiran@gmail.com)
  \date      10.05.2009
  \brief     Тест нечёткой логики
  \indent    4T
*/

// ---------------------------------------------------------------------------- PCH
// ----------------------------------------------------------------------- INCLUDES
#include <iostream>
#define BOOST_TEST_MODULE RDORuntime_Fuzzy_Test
#include <boost/test/included/unit_test.hpp>
// ----------------------------------------------------------------------- SYNOPSIS
#include "simulator/runtime/rdo_runtime.h"
#include "simulator/runtime/rdo_fuzzy.h"
// --------------------------------------------------------------------------------

OPEN_RDO_RUNTIME_NAMESPACE

BOOST_AUTO_TEST_SUITE(RDORuntime_Fuzzy_Test)

BOOST_AUTO_TEST_CASE(DefineAreaTest)
{
	LPRDORuntime pRuntime = rdo::Factory<RDORuntime>::create();
	BOOST_CHECK(pRuntime);

	LPDefineArea pDefineAreaEmpty = rdo::Factory<DefineArea>::create();
	BOOST_CHECK(pDefineAreaEmpty);

	LPDefineArea pDefineArea = rdo::Factory<DefineArea>::create(1.0, 5.0);
	BOOST_CHECK(pDefineArea);

	RDOValue testValueFalse = 100;
	RDOValue testValueTrue = 3.0;

	//BOOST_CHECK(!pDefineArea->inDomain(testValueFalse));
	//BOOST_CHECK(pDefineArea->inDomain(testValueTrue));

}



BOOST_AUTO_TEST_SUITE_END() // RDORuntime_Fuzzy_Test

	pValue1 = rdo::Factory<RDOFuzzyValue>::create(pValue1->operator+ (pValue2));
	BOOST_CHECK(pValue1);

	BOOST_CHECK(pValue1->getAsString() == _T("<10/0.50> <11/1.00> <12/1.00> <14/1.00> <15/1.00> <16/1.00> <17/1.00> <18/1.00> <19/1.00> <24/0.70> <33/1.00>"));
	BOOST_CHECK(pValue1->defuzzyfication().getAsString() == _T("21.1755"));
}

BOOST_AUTO_TEST_SUITE_END() // RDORuntime_Fuzzy_Test

CLOSE_RDO_RUNTIME_NAMESPACE
