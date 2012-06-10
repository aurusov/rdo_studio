/*!
  \copyright (c) RDO-Team, 2011
  \file      main.cpp
  \author    Урусов Андрей (rdo@rk9.bmstu.ru)
  \date      14.03.2010
  \brief     Тест типов парсера
  \indent    4T
*/

// ---------------------------------------------------------------------------- PCH
// ----------------------------------------------------------------------- INCLUDES
#include <vector>
#include <list>
#include <iostream>
#define BOOST_TEST_MODULE RDOParserType_Test
#include <boost/test/included/unit_test.hpp>
// ----------------------------------------------------------------------- SYNOPSIS
#include "simulator/compiler/parser/type/atom.h"
#include "simulator/runtime/rdo_resource.h" //! @todo непонятный инклюд, но нужен чтобы собиралось
// --------------------------------------------------------------------------------

BOOST_AUTO_TEST_SUITE(RDOParserType_Test)

BOOST_AUTO_TEST_CASE(RDOParserType_Test_int)
{
	rdoParse::LPRDOType pType = rdo::Factory<rdoParse::RDOType__int>::create();
	BOOST_CHECK(pType);
}

BOOST_AUTO_TEST_SUITE_END()
