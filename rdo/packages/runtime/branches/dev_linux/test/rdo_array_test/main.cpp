/*!
  \copyright (c) RDO-Team, 2011
  \file      main.cpp
  \authors   Чирков Михаил
  \authors   Пройдаков Евгений (lord.tiran@gmail.com)
  \date      13.12.2009
  \brief     Тест массивов
  \indent    4T
*/

// ---------------------------------------------------------------------------- PCH
// ----------------------------------------------------------------------- INCLUDES
#include <iostream>
#include <stdio.h>
#define BOOST_TEST_MODULE RDORuntime_Array_Test
#include <boost/test/included/unit_test.hpp>
// ----------------------------------------------------------------------- SYNOPSIS
#include "simulator/runtime/rdo_value.h"
#include "simulator/runtime/rdo_array.h"
#include "simulator/runtime/rdo_type.h"
#include "simulator/runtime/test/rdo_array_test/arrayTestFunction.inl"
// --------------------------------------------------------------------------------

BOOST_AUTO_TEST_SUITE(RDORuntime_Array_Test)

BOOST_AUTO_TEST_CASE(RDORuntime_Array_Test)
{
	arrayTestCreate();
	arrayTestInsert();
	arrayTestErase();
	arrayTestPPOperator();
	arrayTestOperatorPP();
	arrayTestMMOperator();
	arrayTestOperatorMM();
}

BOOST_AUTO_TEST_SUITE_END() // RDORuntime_Array_Test