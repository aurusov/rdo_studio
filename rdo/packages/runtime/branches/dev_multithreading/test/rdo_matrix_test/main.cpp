/*!
  \copyright (c) RDO-Team, 2011
  \file      main.cpp
  \authors   Чирков Михаил
  \authors   Пройдаков Евгений (lord.tiran@gmail.com)
  \authors   Урусов Андрей (rdo@rk9.bmstu.ru)
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

typedef  rdo::vector<rsint>                                            Container;
typedef  std::pair<rdoRuntime::LPRDOMatrixValue, rdoRuntime::RDOValue>  Matrix;

Matrix createMatrix(CREF(Container) data)
{
	rdoRuntime::LPRDOMatrixType  pType  = rdo::Factory<rdoRuntime::RDOMatrixType>::create(rdoRuntime::g_int);
	ASSERT(pType);
	rdoRuntime::LPRDOMatrixValue pValue = rdo::Factory<rdoRuntime::RDOMatrixValue>::create(pType);
	ASSERT(pValue);

	STL_FOR_ALL_CONST(data, it)
	{
		pValue->push_back(rdoRuntime::RDOValue(*it));
	}

	return std::make_pair(pValue, rdoRuntime::RDOValue(pType, pValue));
}

tstring getString(CREF(rdoRuntime::LPRDOMatrixValue) pMatrix, CREF(rdoRuntime::LPRDOMatrixIterator) pIt)
{
	if (!pIt->equal(pMatrix->end()))
	{
		return pIt->getValue().getAsString();
	}
	return _T("");
}

tstring getString(CREF(rdoRuntime::RDOValue) it, CREF(rdoRuntime::RDOValue) end)
{
	if (it != end)
	{
		return it.getAsString();
	}
	return _T("");
}

BOOST_AUTO_TEST_CASE(MatrixTestCreate)
{
	BOOST_CHECK(createMatrix(Container()(1)(2)(3)).second.getAsString() == _T("[1, 2, 3]"));
}

BOOST_AUTO_TEST_CASE(MatrixTestInsert)
{
	Matrix matrix1 = createMatrix(Container()(1)(2)(3));
	Matrix matrix2 = createMatrix(Container()(4)(5)(6));

	matrix1.first->insert(matrix1.first->begin()->next(), matrix2.first->begin(), matrix2.first->end());

	BOOST_CHECK(matrix1.second.getAsString() == _T("[1, 4, 5, 6, 2, 3]"));
}

BOOST_AUTO_TEST_CASE(MatrixTestErase)
{
	Matrix matrix = createMatrix(Container()(1)(2)(3));

	matrix.first->erase(matrix.first->begin()->next(), matrix.first->begin()->preInc(3));

	BOOST_CHECK(matrix.second.getAsString() == _T("[1]"));
}

BOOST_AUTO_TEST_CASE(MatrixTestIteratorPrePlus)
{
	Matrix matrix = createMatrix(Container()(1)(2)(3));

	tstring result;
	rdoRuntime::LPRDOMatrixIterator pIt = matrix.first->begin();
	while (!pIt->equal(matrix.first->end()))
	{
		result += getString(matrix.first, pIt->preInc(1));
	}
	BOOST_CHECK(result == _T("23"));
}

BOOST_AUTO_TEST_CASE(MatrixTestIteratorPostPlus)
{
	Matrix matrix = createMatrix(Container()(1)(2)(3));

	tstring result;
	rdoRuntime::LPRDOMatrixIterator pIt = matrix.first->begin();
	while (!pIt->equal(matrix.first->end()))
	{
		result += getString(matrix.first, pIt->postInc(1));
	}
	BOOST_CHECK(result == _T("123"));
}

BOOST_AUTO_TEST_CASE(MatrixTestIteratorPreMinus)
{
	Matrix matrix = createMatrix(Container()(1)(2)(3));

	tstring result;
	rdoRuntime::LPRDOMatrixIterator pIt = matrix.first->end();
	do
	{
		result += getString(matrix.first, pIt->preInc(-1));
	}
	while (!pIt->equal(matrix.first->begin()));

	BOOST_CHECK(result == _T("321"));
}

BOOST_AUTO_TEST_CASE(MatrixTestIteratorPostMinus)
{
	Matrix matrix = createMatrix(Container()(1)(2)(3));

	tstring result;
	rdoRuntime::LPRDOMatrixIterator pIt = matrix.first->end();
	do
	{
		result += getString(matrix.first, pIt->postInc(-1));
	}
	while (!pIt->equal(matrix.first->begin()));

	BOOST_CHECK(result == _T("32"));
}

BOOST_AUTO_TEST_CASE(MatrixTestValuePrePlus)
{
	Matrix matrix = createMatrix(Container()(1)(2)(3));

	tstring result;
	rdoRuntime::LPRDOMatrixIterator pIt  = matrix.first->begin();
	rdoRuntime::LPRDOMatrixIterator pEnd = matrix.first->end  ();
	rdoRuntime::RDOValue it (pIt,  pIt );
	rdoRuntime::RDOValue end(pEnd, pEnd);
	BOOST_CHECK(it != end);

	while (it != end)
	{
		result += getString(++it, end);
	}
	BOOST_CHECK(result == _T("23"));
}

BOOST_AUTO_TEST_CASE(MatrixTestValuePostPlus)
{
	Matrix matrix = createMatrix(Container()(1)(2)(3));

	tstring result;
	rdoRuntime::LPRDOMatrixIterator pIt  = matrix.first->begin();
	rdoRuntime::LPRDOMatrixIterator pEnd = matrix.first->end  ();
	rdoRuntime::RDOValue it (pIt,  pIt );
	rdoRuntime::RDOValue end(pEnd, pEnd);
	BOOST_CHECK(it != end);

	while (it != end)
	{
		result += getString(it++, end);
	}
	BOOST_CHECK(result == _T("123"));
}

BOOST_AUTO_TEST_CASE(MatrixTestValuePreMinus)
{
	Matrix matrix = createMatrix(Container()(1)(2)(3));

	tstring result;
	rdoRuntime::LPRDOMatrixIterator pIt    = matrix.first->end  ();
	rdoRuntime::LPRDOMatrixIterator pBegin = matrix.first->begin();
	rdoRuntime::LPRDOMatrixIterator pEnd   = matrix.first->end  ();
	rdoRuntime::RDOValue it   (pIt,    pIt   );
	rdoRuntime::RDOValue begin(pBegin, pBegin);
	rdoRuntime::RDOValue end  (pEnd,   pEnd  );
	BOOST_CHECK(it != begin);
	BOOST_CHECK(it == end  );

	do 
	{
		result += getString(--it, end);
	}
	while (it != begin);
	BOOST_CHECK(result == _T("321"));
}

BOOST_AUTO_TEST_CASE(MatrixTestValuePostMinus)
{
	Matrix matrix = createMatrix(Container()(1)(2)(3));

	tstring result;
	rdoRuntime::LPRDOMatrixIterator pIt    = matrix.first->end  ();
	rdoRuntime::LPRDOMatrixIterator pBegin = matrix.first->begin();
	rdoRuntime::LPRDOMatrixIterator pEnd   = matrix.first->end  ();
	rdoRuntime::RDOValue it   (pIt,    pIt   );
	rdoRuntime::RDOValue begin(pBegin, pBegin);
	rdoRuntime::RDOValue end  (pEnd,   pEnd  );
	BOOST_CHECK(it != begin);
	BOOST_CHECK(it == end  );

	do 
	{
		result += getString(it--, end);
	}
	while (it != begin);
	BOOST_CHECK(result == _T("32"));
}

BOOST_AUTO_TEST_SUITE_END() // RDORuntime_Matrix_Test
