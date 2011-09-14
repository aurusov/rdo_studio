void arrayTestCreate()
{
	#include "simulator/runtime/test/rdo_array_test/arrayCreate.inl"

	if(arrayVal1.getAsString() != _T("[1, 2, 3]")) TRACE("Ошибка в заполнении массива\n");
}

void arrayTestInsert()
{
	#include "simulator/runtime/test/rdo_array_test/arrayCreate.inl"

	arrayVal1.insert(arrayVal1.begin()+RDOValue(1),arrayVal2.begin(),arrayVal2.end());
	if(arrayVal1.getAsString() != _T("[1, 4, 5, 6, 2, 3]")) TRACE("Ошибка в методе insert()\n");
}

void arrayTestErase()
{	
	#include "simulator/runtime/test/rdo_array_test/arrayCreate.inl"

	arrayVal1.erase(arrayVal1.begin()+RDOValue(1),arrayVal1.begin()+RDOValue(3));
	if(arrayVal1.getAsString() != _T("[1]")) TRACE("Ошибка в методе erase()\n");
}

void arrayTestPPOperator()
{
	#include "simulator/runtime/test/rdo_array_test/arrayCreate.inl"

	tstring itStr = "";
	for (RDOValue it = arrayVal1.begin(); it != arrayVal1.end(); ++it)
	{
		itStr += it.getAsString();
	}
	if(itStr != _T("123")) TRACE("Ошибка в ++операторе\n");
}

void arrayTestOperatorPP()
{
	#include "simulator/runtime/test/rdo_array_test/arrayCreate.inl"

	tstring itStr = "";
	for (RDOValue it = arrayVal1.begin(); it != arrayVal1.end(); it++)
	{
		itStr += it.getAsString();
	}
	if(itStr != _T("123")) TRACE("Ошибка в операторе++\n");
}

void arrayTestMMOperator()
{
	#include "simulator/runtime/test/rdo_array_test/arrayCreate.inl"

	tstring itStr = "";
	RDOValue it1 = arrayVal2.end();
	do 
	{
		--it1;
		itStr += it1.getAsString();
	} while (it1 != arrayVal2.begin());
	if(itStr != _T("654")) TRACE("Ошибка в --операторе\n");
}

void arrayTestOperatorMM()
{
	#include "simulator/runtime/test/rdo_array_test/arrayCreate.inl"

	tstring itStr = "";
	RDOValue it1 = arrayVal2.end();
	do 
	{
		it1--;
		itStr += it1.getAsString();
	} while (it1 != arrayVal2.begin());
	if(itStr != _T("654")) TRACE("Ошибка в операторе--\n");
}