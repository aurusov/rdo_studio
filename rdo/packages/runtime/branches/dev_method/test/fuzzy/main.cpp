/*!
  \copyright (c) RDO-Team, 2011
  \file      main.cpp
  \authors   Урусов Андрей (rdo@rk9.bmstu.ru)
  \authors   Пройдаков Евгений (lord.tiran@gmail.com)
  \date      10.05.2009
  \brief     Тест нечёткой логики
  \indent    4T
*/

// ----------------------------------------------------------------------- PLATFORM
#include "utils/src/common/platform.h"
#ifdef COMPILER_MINGW
	#undef __STRICT_ANSI__
#endif
// ----------------------------------------------------------------------- INCLUDES
#define BOOST_TEST_MODULE RDORuntime_Fuzzy_Test
#include "utils/src/common/warning_disable.h"
#include <iostream>
#include <boost/test/included/unit_test.hpp>
#include "utils/src/common/warning_enable.h"
// ----------------------------------------------------------------------- SYNOPSIS
#include "simulator/runtime/rdo_runtime.h"
#include "simulator/runtime/rdo_fuzzy.h"
// --------------------------------------------------------------------------------

OPEN_RDO_RUNTIME_NAMESPACE

BOOST_AUTO_TEST_SUITE(RDORuntime_Fuzzy_Test)

BOOST_AUTO_TEST_CASE(DefineAreaTest)
{
	Error error;
	LPRDORuntime pRuntime = rdo::Factory<RDORuntime>::create(&error);
	BOOST_CHECK(pRuntime);

	LPDefineArea pDefineAreaEmpty = rdo::Factory<DefineArea>::create();
	BOOST_CHECK(pDefineAreaEmpty);

	LPDefineArea pDefineArea = rdo::Factory<DefineArea>::create(1.0, 5.0);
	BOOST_CHECK(pDefineArea);
}
BOOST_AUTO_TEST_CASE(FuzzySetTest)
{
	Error error;
	LPRDORuntime pRuntime = rdo::Factory<RDORuntime>::create(&error);
	BOOST_CHECK(pRuntime);

	LPFuzzySet pInfinityFuzzySet = rdo::Factory<FuzzySet>::create();
	BOOST_CHECK(pInfinityFuzzySet);

	LPFuzzySet pCopyFuzzySet = rdo::Factory<FuzzySet>::create(pInfinityFuzzySet);
	BOOST_CHECK(pCopyFuzzySet);

	LPDefineArea pDefineArea = rdo::Factory<DefineArea>::create(0.0, 100.0);
	BOOST_CHECK(pDefineArea);

	LPFuzzySet pFuzzySet = rdo::Factory<FuzzySet>::create(pDefineArea);
	BOOST_CHECK(pFuzzySet);

	LPFuzzySet pSet = rdo::Factory<FuzzySet>::create();

	tstring stringPresentation = pSet->getAsString();
	BOOST_CHECK(stringPresentation == "[empty value]");

	pSet->append(1,0.10);
	pSet->append(2,0.20);
	pSet->append(3,1.00);
	pSet->append(5,0.50);
	pSet->append(6,0.40);
	pSet->append(7,0.20);
	pSet->append(9,0.10);

	BOOST_CHECK(pSet->getAsString() == "<1/0.10> <2/0.20> <3/1.00> <5/0.50> <6/0.40> <7/0.20> <9/0.10>");

	RDOValue repeatValue = 3.0;
	pSet->append(repeatValue, 0.3);
	BOOST_CHECK(pSet->getAsString() == "<1/0.10> <2/0.20> <3/1.00> <5/0.50> <6/0.40> <7/0.20> <9/0.10>");

	pSet->operator[](repeatValue) = 0.3;
	BOOST_CHECK(pSet->getAsString() == "<1/0.10> <2/0.20> <3/0.30> <5/0.50> <6/0.40> <7/0.20> <9/0.10>");

	LPFuzzySet pConSet = MemberFunctionProperties::a_con(pSet);
	BOOST_CHECK(pConSet->getAsString() == "<1/0.01> <2/0.04> <3/0.09> <5/0.25> <6/0.16> <7/0.04> <9/0.01>");
	
	LPFuzzySet pDilSet = MemberFunctionProperties::a_dil(pSet);
	BOOST_CHECK(pDilSet->getAsString() == "<1/0.32> <2/0.45> <3/0.55> <5/0.71> <6/0.63> <7/0.45> <9/0.32>");

	LPFuzzySet pUnaryMinusSet = MemberFunctionProperties::u_minus(pSet);
	BOOST_CHECK(pUnaryMinusSet->getAsString() == "<-9/0.10> <-7/0.20> <-6/0.40> <-5/0.50> <-3/0.30> <-2/0.20> <-1/0.10>");

	LPFuzzySet pScaleSet = MemberFunctionProperties::u_scale(pSet, 4.0);
	BOOST_CHECK(pScaleSet->getAsString() == "<4/0.10> <8/0.20> <12/0.30> <20/0.50> <24/0.40> <28/0.20> <36/0.10>");

	LPFuzzySet pSupplement = MemberFunctionProperties::supplement(pSet);
	BOOST_CHECK(pSupplement->getAsString() == "<1/0.90> <2/0.80> <3/0.70> <5/0.50> <6/0.60> <7/0.80> <9/0.90>");

	LPFuzzySet pAlphaTest = MemberFunctionProperties::alpha(pSet, 0.3);
	BOOST_CHECK(pAlphaTest->getAsString() == "<3/0.30> <5/0.50> <6/0.40>");

	LPFuzzySet pMultTest = MemberFunctionProperties::a_mult(pSet, pSupplement);
	BOOST_CHECK(pMultTest->getAsString() == "<1/0.09> <2/0.16> <3/0.21> <5/0.25> <6/0.24> <7/0.16> <9/0.09>");

	LPFuzzySet pMultTestDown = MemberFunctionProperties::a_mult(pSet, pScaleSet);
	BOOST_CHECK(pMultTestDown->getAsString() == "[empty value]");

	RDOValue defuzzyficationValue = MemberFunctionProperties::defuzzyfication(pSet);
	BOOST_CHECK(defuzzyficationValue.getAsString() == "4.83333" );
}

BOOST_AUTO_TEST_CASE(TermTest)
{
	LPFuzzySet pSet = rdo::Factory<FuzzySet>::create();
	tstring testname = "test";
	LPRDOFuzzyTerm pTerm = rdo::Factory<RDOFuzzyTerm>::create(testname, pSet);
	BOOST_CHECK(pTerm->getName() == "test");

	
	Error error;
	LPRDORuntime pRuntime = rdo::Factory<RDORuntime>::create(&error);
	BOOST_CHECK(pRuntime);

	LPDefineArea pDefineArea = rdo::Factory<DefineArea>::create(0.0, 100.0);
	BOOST_CHECK(pDefineArea);

	LPFuzzySet pFuzzySet = rdo::Factory<FuzzySet>::create(pDefineArea);
	BOOST_CHECK(pFuzzySet);

	LPRDOFuzzyTerm pTerm3 = rdo::Factory<RDOFuzzyTerm>::create("term", pFuzzySet);
	BOOST_CHECK(pTerm3->getName() == "term");
}

BOOST_AUTO_TEST_CASE(VariableTest)
{
	LPDefineArea pDefineArea1 = rdo::Factory<DefineArea>::create(0.0, 20.0);
	BOOST_CHECK(pDefineArea1);

	LPDefineArea pDefineArea2 = rdo::Factory<DefineArea>::create(10.0, 20.0);
	BOOST_CHECK(pDefineArea2);
	
	LPFuzzySet pFuzzySet1 = rdo::Factory<FuzzySet>::create(pDefineArea1);
	BOOST_CHECK(pFuzzySet1);

	LPFuzzySet pFuzzySet2 = rdo::Factory<FuzzySet>::create(pDefineArea2);
	BOOST_CHECK(pFuzzySet2);

	LPRDOFuzzyTerm pTerm1 = rdo::Factory<RDOFuzzyTerm>::create("term1", pFuzzySet1);
	BOOST_CHECK(pTerm1);

	LPRDOFuzzyTerm pTerm2 = rdo::Factory<RDOFuzzyTerm>::create("term2", pFuzzySet2);
	BOOST_CHECK(pTerm2);

	tstring name1 = pTerm1->getName();
	BOOST_CHECK(name1 == "term1");
	LPRDOLingvoVariable pVariable = rdo::Factory<RDOLingvoVariable>::create(pTerm1,"test");
	BOOST_CHECK(pVariable);

	Error error;
	LPRDORuntime pRuntime = rdo::Factory<RDORuntime>::create(&error);
	BOOST_CHECK(pRuntime);

	LPDefineArea pDefineArea = rdo::Factory<DefineArea>::create(0.0, 100.0);
	BOOST_CHECK(pDefineArea);

	LPFuzzySet pFuzzySet = rdo::Factory<FuzzySet>::create(pDefineArea);
	BOOST_CHECK(pFuzzySet);

	pFuzzySet->append(5,0.5);
	BOOST_CHECK(pFuzzySet->getAsString() == "<5/0.50>");

	pFuzzySet->append(1,0.1);
	BOOST_CHECK(pFuzzySet->getAsString() == "<1/0.10> <5/0.50>");

	pFuzzySet->append(2,0.2);
	BOOST_CHECK(pFuzzySet->getAsString() == "<1/0.10> <2/0.20> <5/0.50>");

	pFuzzySet->append(3,1.0);
	BOOST_CHECK(pFuzzySet->getAsString() == "<1/0.10> <2/0.20> <3/1.00> <5/0.50>");

	pFuzzySet->append(6,0.4);
	BOOST_CHECK(pFuzzySet->getAsString() == "<1/0.10> <2/0.20> <3/1.00> <5/0.50> <6/0.40>");

	pFuzzySet->append(7,0.2);
	BOOST_CHECK(pFuzzySet->getAsString() == "<1/0.10> <2/0.20> <3/1.00> <5/0.50> <6/0.40> <7/0.20>");

	pFuzzySet->append(9,0.1);
	BOOST_CHECK(pFuzzySet->getAsString() == "<1/0.10> <2/0.20> <3/1.00> <5/0.50> <6/0.40> <7/0.20> <9/0.10>");

	LPRDOFuzzyTerm pTerm4 = rdo::Factory<RDOFuzzyTerm>::create("term4", pFuzzySet);
	BOOST_CHECK(pTerm4);

	pVariable->append(pTerm4->getName(), pTerm4->getFuzzySetDefinition());
	pVariable->setName("testName");

	RDOValue value = 1.0;
	LPRDOLingvoVariable pVariable2 = rdo::Factory<RDOLingvoVariable>::create(value, pVariable);
	BOOST_CHECK(pVariable2);

	RDOValue valueTrueOn0_2 = 7.0;
	LPRDOLingvoVariable fuzzyVariable1 = MemberFunctionProperties::fuzzyfication(valueTrueOn0_2, pVariable);
	BOOST_CHECK(fuzzyVariable1);

	RDOValue valueTrueOn0_0 = 10.0;
	LPRDOLingvoVariable fuzzyVariable2 = MemberFunctionProperties::fuzzyfication(valueTrueOn0_0, pVariable);
	BOOST_CHECK(fuzzyVariable2);
}

BOOST_AUTO_TEST_SUITE_END() // RDORuntime_Fuzzy_Test
CLOSE_RDO_RUNTIME_NAMESPACE
