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
// --------------------------------------------------------------------------------

BOOST_AUTO_TEST_SUITE(RDORuntime_Matrix_Test)

BOOST_AUTO_TEST_CASE(matrixTestCreate)
{
	#include "simulator/runtime/test/rdo_matrix_test/matrixCreate.inl"

	BOOST_CHECK(matrixVal1.getAsString() == _T("[1, 2, 3]"));
}

BOOST_AUTO_TEST_CASE(matrixTestInsert)
{
	#include "simulator/runtime/test/rdo_matrix_test/matrixCreate.inl"

	matrixVal1.insert(matrixVal1.begin() + rdoRuntime::RDOValue(1), matrixVal2.begin(), matrixVal2.end());
	
	BOOST_CHECK(matrixVal1.getAsString() == _T("[1, 4, 5, 6, 2, 3]"));
}

BOOST_AUTO_TEST_CASE(matrixTestErase)
{
	#include "simulator/runtime/test/rdo_matrix_test/matrixCreate.inl"

	matrixVal1.erase(matrixVal1.begin() + rdoRuntime::RDOValue(1), matrixVal1.begin() + rdoRuntime::RDOValue(3));
	
	BOOST_CHECK(matrixVal1.getAsString() == _T("[1]"));
}

BOOST_AUTO_TEST_CASE(matrixTestPPOperator)
{
	#include "simulator/runtime/test/rdo_matrix_test/matrixCreate.inl"

	tstring itStr = _T("");
	for (rdoRuntime::RDOValue it = matrixVal1.begin(); it != matrixVal1.end(); ++it)
	{
		itStr += it.getAsString();
	}
	BOOST_CHECK(itStr == _T("123"));
}

BOOST_AUTO_TEST_CASE(matrixTestOperatorPP)
{
	#include "simulator/runtime/test/rdo_matrix_test/matrixCreate.inl"

	tstring itStr = _T("");
	for (rdoRuntime::RDOValue it = matrixVal1.begin(); it != matrixVal1.end(); it++)
	{
		itStr += it.getAsString();
	}
	BOOST_CHECK(itStr == _T("123"));
}

BOOST_AUTO_TEST_CASE(matrixTestMMOperator)
{
	#include "simulator/runtime/test/rdo_matrix_test/matrixCreate.inl"

	tstring itStr = _T("");
	rdoRuntime::RDOValue it1 = matrixVal2.end();
	do 
	{
		--it1;
		itStr += it1.getAsString();
	} while (it1 != matrixVal2.begin());

	BOOST_CHECK(itStr == _T("654"));
}

BOOST_AUTO_TEST_CASE(matrixTestOperatorMM)
{
	#include "simulator/runtime/test/rdo_matrix_test/matrixCreate.inl"

	tstring itStr = _T("");
	rdoRuntime::RDOValue it1 = matrixVal2.end();
	do
	{
		it1--;
		itStr += it1.getAsString();
	} while (it1 != matrixVal2.begin());

	BOOST_CHECK(itStr == _T("654"));
}

BOOST_AUTO_TEST_SUITE_END() // RDORuntime_Matrix_Test
