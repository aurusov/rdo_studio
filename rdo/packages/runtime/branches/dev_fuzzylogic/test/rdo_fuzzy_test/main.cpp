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
}
BOOST_AUTO_TEST_CASE(FuzzySetTest)
{
	LPRDORuntime pRuntime = rdo::Factory<RDORuntime>::create();
	BOOST_CHECK(pRuntime);

	LPRDOFuzzyType pType = rdo::Factory<RDOFuzzyType>::create();
	BOOST_CHECK(pType);

	LPFuzzySet pInfinityFuzzySet = rdo::Factory<FuzzySet>::create(pType);
	BOOST_CHECK(pInfinityFuzzySet);

	LPFuzzySet pCopyFuzzySet = rdo::Factory<FuzzySet>::create(pInfinityFuzzySet);
	BOOST_CHECK(pCopyFuzzySet);

	LPDefineArea pDefineArea = rdo::Factory<DefineArea>::create(0.0, 100.0);
	BOOST_CHECK(pDefineArea);

	LPFuzzySet pFuzzySet = rdo::Factory<FuzzySet>::create(pType, *pDefineArea);
	BOOST_CHECK(pFuzzySet);

//	LPFuzzySet pFuzzySet = rdo::Factory<FuzzySet>::create()
}



BOOST_AUTO_TEST_SUITE_END() // RDORuntime_Fuzzy_Test
CLOSE_RDO_RUNTIME_NAMESPACE