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

BOOST_AUTO_TEST_CASE(RDORuntime_Fuzzy_Test)
{
	LPRDORuntime pRuntime = rdo::Factory<RDORuntime>::create();
	BOOST_CHECK(pRuntime);

	LPRDOFuzzySetDefinitionRangeDiscret pSetDefinition = rdo::Factory<RDOFuzzySetDefinitionRangeDiscret>::create(0, 50);
	BOOST_CHECK(pSetDefinition);

	LPRDOFuzzyType pFuzzyType = rdo::Factory<RDOFuzzyType>::create(pSetDefinition);
	BOOST_CHECK(pFuzzyType);

	LPRDOFuzzyValue pValue1 = rdo::Factory<RDOFuzzyValue>::create(pFuzzyType);
	BOOST_CHECK(pValue1);

	pValue1->append(1, 1.0)(2, 1.0)(23, 1.0)(4, 1.0)(5, 1.0)(6, 1.0)(7, 1.0)(8, 1.0)(9, 1.0);
	BOOST_CHECK(pValue1->getAsString() == _T("<1/1.00> <2/1.00> <4/1.00> <5/1.00> <6/1.00> <7/1.00> <8/1.00> <9/1.00> <23/1.00>"));

	pValue1->operator[](0) = 0.5;
	BOOST_CHECK(pValue1->getAsString() == _T("<0/0.50> <1/1.00> <2/1.00> <4/1.00> <5/1.00> <6/1.00> <7/1.00> <8/1.00> <9/1.00> <23/1.00>"));

	pValue1->operator[](14) = 0.7;
	BOOST_CHECK(pValue1->getAsString() == _T("<0/0.50> <1/1.00> <2/1.00> <4/1.00> <5/1.00> <6/1.00> <7/1.00> <8/1.00> <9/1.00> <14/0.70> <23/1.00>"));

	LPRDOFuzzyValue pValue3 = rdo::Factory<RDOFuzzyValue>::create(pValue1->supplement());
	BOOST_CHECK(pValue3);
	BOOST_CHECK(pValue3->getAsString() == _T("<0/0.50> <3/1.00> <10/1.00> <11/1.00> <12/1.00> <13/1.00> <14/0.30> <15/1.00> <16/1.00> <17/1.00> <18/1.00> <19/1.00> <20/1.00> <21/1.00> <22/1.00> <24/1.00> <25/1.00> <26/1.00> <27/1.00> <28/1.00> <29/1.00> <30/1.00> <31/1.00> <32/1.00> <33/1.00> <34/1.00> <35/1.00> <36/1.00> <37/1.00> <38/1.00> <39/1.00> <40/1.00> <41/1.00> <42/1.00> <43/1.00> <44/1.00> <45/1.00> <46/1.00> <47/1.00> <48/1.00> <49/1.00> <50/1.00>"));

	LPRDOFuzzyValue pValue2 = rdo::Factory<RDOFuzzyValue>::create(pFuzzyType);
	BOOST_CHECK(pValue2);
	pValue2->append(10, 1.0);
	BOOST_CHECK(pValue2->getAsString() == _T("<10/1.00>"));

	pValue1 = rdo::Factory<RDOFuzzyValue>::create(pValue1->operator+ (pValue2));
	BOOST_CHECK(pValue1);

	BOOST_CHECK(pValue1->getAsString() == _T("<10/0.50> <11/1.00> <12/1.00> <14/1.00> <15/1.00> <16/1.00> <17/1.00> <18/1.00> <19/1.00> <24/0.70> <33/1.00>"));
	BOOST_CHECK(pValue1->defuzzyfication().getAsString() == _T("21.1755"));
}

BOOST_AUTO_TEST_SUITE_END() // RDORuntime_Fuzzy_Test

CLOSE_RDO_RUNTIME_NAMESPACE
