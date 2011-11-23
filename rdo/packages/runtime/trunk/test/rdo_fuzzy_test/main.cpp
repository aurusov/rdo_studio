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

using namespace rdoRuntime;

BOOST_AUTO_TEST_SUITE(RDORuntime_Fuzzy_Test)

BOOST_AUTO_TEST_CASE(RDORuntime_Fuzzy_Test)
{
	LPRDORuntime pRuntime = rdo::Factory<RDORuntime>::create();
	ASSERT(pRuntime);

	PTR(RDOFuzzySetDefinitionRangeDiscret) setDefinition1 = new RDOFuzzySetDefinitionRangeDiscret(0, 50);
	LPRDOFuzzyType type1 = rdo::Factory<RDOFuzzyType>::create(setDefinition1);

	RDOFuzzyValue value1(type1);
	value1.append(1, 1.0)(2, 1.0)(23, 1.0)(4, 1.0)(5, 1.0)(6, 1.0)(7, 1.0)(8, 1.0)(9, 1.0);
	std::cout << "value1: " << value1.getAsString() << std::endl;
	value1[0] = 0.5;
	std::cout << "value1: " << value1.getAsString() << std::endl;
	value1[14] = 0.7;
	std::cout << "value1: " << value1.getAsString() << std::endl;

	RDOFuzzyValue value3(value1.supplement());
	std::cout << "value3: " << value3.getAsString() << std::endl;

	RDOFuzzyValue value2(type1);
	value2.append(10, 1.0);
	std::cout << "value2: " << value2.getAsString() << std::endl;

	value1 = value1 + value2;

	std::cout << value1.getAsString() << std::endl;
	std::cout << value1.defuzzyfication().getAsString() << std::endl;

	int i = 1;
	UNUSED(i);
}

BOOST_AUTO_TEST_SUITE_END() // RDORuntime_Fuzzy_Test
