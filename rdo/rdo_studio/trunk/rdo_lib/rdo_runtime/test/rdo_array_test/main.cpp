// ====================================================================== INCLUDES
#include <iostream>
#include <stdio.h>
// ====================================================================== SYNOPSIS
#include "rdo_lib\rdo_runtime\rdo_value.h"
#include "rdo_lib\rdo_runtime\rdo_enum.h"
#include "rdo_lib\rdo_runtime\rdo_array.h"
#include "rdo_lib\rdo_runtime\rdo_type.h"
// ===============================================================================

using namespace rdoRuntime;

void main()
{
	//RDOEnumType* enumType = new RDOEnumType(NULL, RDOEnumType::Enums("Значение_1")("Значение_2")("Значение_3"));
	RDOValue integerVal1(1);
	RDOValue integerVal2(2);
	RDOValue integerVal3(3);
	RDOValue integerVal4(4);
	RDOValue integerVal5(5);
	RDOValue integerVal6(6);

	RDOArrayType atype1(NULL, g_int);
	RDOArrayValue avalue1(atype1);
	avalue1.insertItem(integerVal1);
	avalue1.insertItem(integerVal2);
	avalue1.insertItem(integerVal3);
	RDOValue arrayVal1(avalue1);

	RDOArrayType atype2(NULL, g_int);
	RDOArrayValue avalue2(atype2);
	avalue2.insertItem(integerVal4);
	avalue2.insertItem(integerVal5);
	avalue2.insertItem(integerVal6);
	RDOValue arrayVal2(avalue2);

	arrayVal1.insert(arrayVal1.begin()+RDOValue(1),arrayVal2.begin(),arrayVal2.end());
	tstring arrayStr1 = arrayVal1.getAsString();

	arrayVal1.erase(arrayVal1.begin()+RDOValue(1),arrayVal1.begin()+RDOValue(4));
	tstring arrayStr2 = arrayVal1.getAsString();

	for (RDOValue it = arrayVal1.begin(); it != arrayVal1.end(); ++it)
	{
		tstring itStr = it.getAsString();
		int i = 1;
	}

	RDOValue it = arrayVal2.end();
	do 
	{
		--it;
		tstring itStr = it.getAsString();
		int i = 1;
	} while (it != arrayVal2.begin());

	int i = 1;
}