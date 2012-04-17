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

	FuzzySet::FuzzySetDefinition pSetDefinition;
	typedef FuzzySet::FuzzySetDefinition::iterator It;
	typedef std::pair<It,rbool> Ins;

	Ins insVal1 = pSetDefinition.insert(std::pair<RDOValue,double>(5,0.5));
	BOOST_CHECK(insVal1.second);

	Ins insVal2 = pSetDefinition.insert(std::pair<RDOValue,double>(1,0.1));
	BOOST_CHECK(insVal2.second);

	Ins insVal3 = pSetDefinition.insert(std::pair<RDOValue,double>(2,0.2));
	BOOST_CHECK(insVal3.second);

	Ins insVal4 = pSetDefinition.insert(std::pair<RDOValue,double>(3,1.0));
	BOOST_CHECK(insVal4.second);

	Ins insVal5 = pSetDefinition.insert(std::pair<RDOValue,double>(6,0.4));
	BOOST_CHECK(insVal5.second);

	Ins insVal6 = pSetDefinition.insert(std::pair<RDOValue,double>(7,0.2));
	BOOST_CHECK(insVal6.second);

	Ins insVal7 = pSetDefinition.insert(std::pair<RDOValue,double>(9,0.1));
	BOOST_CHECK(insVal7.second);

	Ins insVal8 = pSetDefinition.insert(std::pair<RDOValue,double>(9,0.5));
	BOOST_CHECK(!insVal8.second);

	LPFuzzySet pSet = rdo::Factory<FuzzySet>::create(pSetDefinition);

	tstring stringPresentation = pSet->getAsString();
	std::cout << stringPresentation << std::endl;
	BOOST_CHECK(stringPresentation == _T("<1/0.10> <2/0.20> <3/1.00> <5/0.50> <6/0.40> <7/0.20> <9/0.10>"));

	RDOValue repeatValue = 3.0;
	pSet->append(repeatValue, 0.3);
	BOOST_CHECK(pSet);
	BOOST_CHECK(pSet->getAsString() == _T("<1/0.10> <2/0.20> <3/1.00> <5/0.50> <6/0.40> <7/0.20> <9/0.10>"));

	pSet->operator[](repeatValue) = 0.3;
	BOOST_CHECK(pSet->getAsString() == _T("<1/0.10> <2/0.20> <3/0.30> <5/0.50> <6/0.40> <7/0.20> <9/0.10>"));

	LPFuzzySet pConSet = MemberFunctionProperties::a_con(pSet);
	BOOST_CHECK(pConSet);
	tstring stringPresentation1 = pConSet->getAsString();
	std::cout << stringPresentation1 << std::endl;
	
	LPFuzzySet pDilSet = MemberFunctionProperties::a_dil(pSet);
	BOOST_CHECK(pDilSet);
	tstring stringPresentation2 = pDilSet->getAsString();
	std::cout << stringPresentation2 << std::endl;

	LPFuzzySet pUnaryMinusSet = MemberFunctionProperties::u_minus(pSet);
	BOOST_CHECK(pUnaryMinusSet);
	tstring stringPresentation3 = pUnaryMinusSet->getAsString();
	std::cout << stringPresentation3 << std::endl;

	LPFuzzySet pScaleSet = MemberFunctionProperties::u_scale(pSet, 4.0);
	BOOST_CHECK(pScaleSet);
	tstring stringPresentation4 = pScaleSet->getAsString();
	std::cout << stringPresentation4 << std::endl;

	LPFuzzySet pSupplement = MemberFunctionProperties::supplement(pSet);
	BOOST_CHECK(pSupplement);
	tstring stringPresentation5 = pSupplement->getAsString();
	std::cout << stringPresentation5 << std::endl;

	//LPFuzzySet pMultTest = MemberFunctionProperties::a_mult(pSet, pScaleSet);
	//BOOST_CHECK(pMultTest);
	//tstring stringPresentation6 = pSupplement->getAsString();
	//std::cout << stringPresentation6 << std::endl;

	LPFuzzySet pAlphaTest = MemberFunctionProperties::alpha(pSet, 0.3);
	BOOST_CHECK(pAlphaTest);
	tstring stringPresentation7 = pAlphaTest->getAsString();
	std::cout << "pAlphaTest: " <<stringPresentation7 <<std::endl;

	LPFuzzySet pMultTest = MemberFunctionProperties::a_mult(pSet, pSupplement);
	BOOST_CHECK(pMultTest);
	tstring stringPresentationMult = pMultTest->getAsString();
	std::cout << "pMultTest: " << stringPresentationMult <<std::endl;

	LPFuzzySet pMultTestDown = MemberFunctionProperties::a_mult(pSet, pScaleSet);
	BOOST_CHECK(pMultTestDown);
	tstring stringPresentationMultDown = pMultTestDown->getAsString();
	std::cout << "pMultTest:	" << stringPresentationMultDown <<std::endl;

	RDOValue defuzzyficationValue = MemberFunctionProperties::defuzzyfication(pSet);
	BOOST_CHECK(defuzzyficationValue);
	tstring stringPresentation9 = defuzzyficationValue.getAsString();
	std::cout << stringPresentation9 <<std::endl;
}

BOOST_AUTO_TEST_CASE(TermTest)
{
	LPRDOFuzzyTerm pTerm = rdo::Factory<RDOFuzzyTerm>::create();
	tstring name = pTerm->getName();
	std::cout << name << std::endl;
	BOOST_CHECK(pTerm);

	LPRDORuntime pRuntime = rdo::Factory<RDORuntime>::create();
	BOOST_CHECK(pRuntime);

	LPRDOFuzzyType pType = rdo::Factory<RDOFuzzyType>::create();
	BOOST_CHECK(pType);

	LPDefineArea pDefineArea = rdo::Factory<DefineArea>::create(0.0, 100.0);
	BOOST_CHECK(pDefineArea);

	LPFuzzySet pFuzzySet = rdo::Factory<FuzzySet>::create(pType, *pDefineArea);
	BOOST_CHECK(pFuzzySet);

	LPRDOFuzzyTerm pTerm1 = rdo::Factory<RDOFuzzyTerm>::create(_T("term1"), pFuzzySet);
	BOOST_CHECK(pTerm1);
	tstring name1 = pTerm1->getName();
	std::cout << name1 << std::endl;
}

BOOST_AUTO_TEST_CASE(VariableTest)
{
	LPRDOLingvoVariable pVariable = rdo::Factory<RDOLingvoVariable>::create();
	BOOST_CHECK(pVariable);

	LPRDORuntime pRuntime = rdo::Factory<RDORuntime>::create();
	BOOST_CHECK(pRuntime);

	LPRDOFuzzyType pType = rdo::Factory<RDOFuzzyType>::create();
	BOOST_CHECK(pType);

	LPDefineArea pDefineArea = rdo::Factory<DefineArea>::create(0.0, 100.0);
	BOOST_CHECK(pDefineArea);

	LPFuzzySet pFuzzySet = rdo::Factory<FuzzySet>::create(pType, *pDefineArea);
	BOOST_CHECK(pFuzzySet);

	pFuzzySet->append(5,0.5);
	BOOST_CHECK(pFuzzySet);

	pFuzzySet->append(1,0.1);
	BOOST_CHECK(pFuzzySet);

	pFuzzySet->append(2,0.2);
	BOOST_CHECK(pFuzzySet);

	pFuzzySet->append(3,1.0);
	BOOST_CHECK(pFuzzySet);

	pFuzzySet->append(6,0.4);
	BOOST_CHECK(pFuzzySet);

	pFuzzySet->append(7,0.2);
	BOOST_CHECK(pFuzzySet);

	pFuzzySet->append(9,0.1);
	BOOST_CHECK(pFuzzySet);

	LPRDOFuzzyTerm pTerm1 = rdo::Factory<RDOFuzzyTerm>::create(_T("term1"), pFuzzySet);
	BOOST_CHECK(pTerm1);

	pVariable->append(pTerm1->getName(), pTerm1->getFuzzySetDefinition());
	pVariable->setName(_T("testName"));

	RDOValue value = 1.0;
	LPRDOLingvoVariable pVariable2 = rdo::Factory<RDOLingvoVariable>::create(value, *pVariable);
	BOOST_CHECK(pVariable2);

	RDOValue valueTrueOn0_2 = 7.0;
	LPRDOLingvoVariable fuzzyVariable1 = MemberFunctionProperties::fuzzyfication(valueTrueOn0_2, *pVariable);
	BOOST_CHECK(fuzzyVariable1);

	RDOValue valueTrueOn0_0 = 10.0;
	LPRDOLingvoVariable fuzzyVariable2 = MemberFunctionProperties::fuzzyfication(valueTrueOn0_0, *pVariable);
	BOOST_CHECK(fuzzyVariable2);
}
BOOST_AUTO_TEST_CASE(MemberFunctionProperties)
{
	
}

	

BOOST_AUTO_TEST_SUITE_END() // RDORuntime_Fuzzy_Test
CLOSE_RDO_RUNTIME_NAMESPACE