/*!
  \copyright (c) RDO-Team, 2011
  \file      main.cpp
  \authors   Чирков Михаил
  \authors   Пройдаков Евгений (lord.tiran@gmail.com)
  \date      01.10.2010
  \brief     Тест матриц
  \indent    4T
*/

// ---------------------------------------------------------------------------- PCH
// ----------------------------------------------------------------------- INCLUDES
#include <iostream>
#include <stdio.h>
#define BOOST_TEST_MODULE RDORuntime_Matrix_Test
#include <boost/test/included/unit_test.hpp>
// ----------------------------------------------------------------------- SYNOPSIS
#include "simulator/runtime/rdo_value.h"
#include "simulator/runtime/rdo_matrix.h"
#include "simulator/runtime/rdo_type.h"
#include "simulator/runtime/test/rdo_matrix_test/matrixTestFunction.inl"
// --------------------------------------------------------------------------------

BOOST_AUTO_TEST_SUITE(RDORuntime_Matrix_Test)

BOOST_AUTO_TEST_CASE(RDORuntime_Matrix_Test)
{
	matrixTestCreate();
	matrixTestInsert();
	matrixTestErase();
	matrixTestPPOperator();
	matrixTestOperatorPP();
	matrixTestMMOperator();
	matrixTestOperatorMM();
}

BOOST_AUTO_TEST_SUITE_END() // RDORuntime_Matrix_Test
