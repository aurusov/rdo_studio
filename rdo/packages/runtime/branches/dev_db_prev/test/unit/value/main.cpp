/*!
  \copyright (c) RDO-Team, 2009-2012
  \file      main.cpp
  \authors   Урусов Андрей (rdo@rk9.bmstu.ru)
  \authors   Пройдаков Евгений (lord.tiran@gmail.com)
  \authors   Романов Ярослав (robot.xet@gmail.com)
  \authors   Клеванец Игорь (cerevra@gmail.com)
  \date      05.03.2013
  \brief     Юнит-тест для RDOValue
  \indent    4T
*/

// ---------------------------------------------------------------------------- PCH
// ----------------------------------------------------------------------- INCLUDES
#define BOOST_TEST_MODULE RDOValue_Test
#include <boost/test/included/unit_test.hpp>
// ----------------------------------------------------------------------- SYNOPSIS
#include "simulator/runtime/rdo_runtime.h"
#include "simulator/runtime/rdo_res_type.h"
// --------------------------------------------------------------------------------

OPEN_RDO_RUNTIME_NAMESPACE

#define CREATE_VALUE\
	LPRDOEnumType pRDOEnumType = rdo::Factory<RDOEnumType>::create();\
	pRDOEnumType->add(_T("first_record"));\
	pRDOEnumType->add(_T("second_record"));\
	rdo::runtime::RDOValue tempObject = rdo::runtime::RDOValue(pRDOEnumType,_T("second_record"));

rdo::runtime::RDOValue foo()
{
	CREATE_VALUE;
	return tempObject;
}

BOOST_AUTO_TEST_SUITE(RDOValue_Test)

BOOST_AUTO_TEST_CASE(RDOValue)
{
	CREATE_VALUE;
	rdo::runtime::RDOValue a = foo();

	BOOST_CHECK(tempObject.getAsString() == a.getAsString());
	BOOST_CHECK(tempObject.getEnum()->getValues() == a.getEnum()->getValues());
}

BOOST_AUTO_TEST_SUITE_END() // RDOValue_Test

CLOSE_RDO_RUNTIME_NAMESPACE
