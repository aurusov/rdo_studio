/*!
  \copyright (c) RDO-Team, 2011
  \file      matrixTestFunction.inl
  \authors   Чирков Михаил
  \date      01.10.2010
  \brief     Тест матриц
  \indent    4T
*/

// ----------------------------------------------------------------------- INCLUDES
// ----------------------------------------------------------------------- SYNOPSIS
// --------------------------------------------------------------------------------

void matrixTestCreate()
{
	#include "simulator/runtime/test/rdo_matrix_test/matrixCreate.inl"

	if (matrixVal1.getAsString() != _T("[1, 2, 3]"))
	{
		TRACE(_T("Ошибка в заполнении матрицы\n"));
	}
}

void matrixTestInsert()
{
	#include "simulator/runtime/test/rdo_matrix_test/matrixCreate.inl"

	matrixVal1.insert(matrixVal1.begin() + rdoRuntime::RDOValue(1), matrixVal2.begin(), matrixVal2.end());
	if (matrixVal1.getAsString() != _T("[1, 4, 5, 6, 2, 3]"))
	{
		TRACE(_T("Ошибка в методе insert()\n"));
	}
}

void matrixTestErase()
{
	#include "simulator/runtime/test/rdo_matrix_test/matrixCreate.inl"

	matrixVal1.erase(matrixVal1.begin() + rdoRuntime::RDOValue(1), matrixVal1.begin() + rdoRuntime::RDOValue(3));
	if (matrixVal1.getAsString() != _T("[1]"))
	{
		TRACE(_T("Ошибка в методе erase()\n"));
	}
}

void matrixTestPPOperator()
{
	#include "simulator/runtime/test/rdo_matrix_test/matrixCreate.inl"

	tstring itStr = _T("");
	for (rdoRuntime::RDOValue it = matrixVal1.begin(); it != matrixVal1.end(); ++it)
	{
		itStr += it.getAsString();
	}
	if (itStr != _T("123"))
	{
		TRACE(_T("Ошибка в ++операторе\n"));
	}
}

void matrixTestOperatorPP()
{
	#include "simulator/runtime/test/rdo_matrix_test/matrixCreate.inl"

	tstring itStr = _T("");
	for (rdoRuntime::RDOValue it = matrixVal1.begin(); it != matrixVal1.end(); it++)
	{
		itStr += it.getAsString();
	}
	if (itStr != _T("123"))
	{
		TRACE(_T("Ошибка в операторе++\n"));
	}
}

void matrixTestMMOperator()
{
	#include "simulator/runtime/test/rdo_matrix_test/matrixCreate.inl"

	tstring itStr = _T("");
	rdoRuntime::RDOValue it1 = matrixVal2.end();
	do 
	{
		--it1;
		itStr += it1.getAsString();
	} while (it1 != matrixVal2.begin());
	if (itStr != _T("654"))
	{
		TRACE(_T("Ошибка в --операторе\n"));
	}
}

void matrixTestOperatorMM()
{
	#include "simulator/runtime/test/rdo_matrix_test/matrixCreate.inl"

	tstring itStr = _T("");
	rdoRuntime::RDOValue it1 = matrixVal2.end();
	do
	{
		it1--;
		itStr += it1.getAsString();
	} while (it1 != matrixVal2.begin());
	if (itStr != _T("654"))
	{
		TRACE(_T("Ошибка в операторе--\n"));
	}
}
