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
	RDOArrayType atype1(NULL, g_int);
	RDOArrayValue avalue1(atype1);
	avalue1.insertItem(integerVal1);
	avalue1.insertItem(integerVal2);
	avalue1.insertItem(integerVal3);
	RDOValue arrayVal(avalue1);

	RDOValue itr(arrayVal.begin());

	itr = itr + RDOValue(1);

	tstring arrayStr = arrayVal.getAsString();
	tstring itStr    = itr.getAsString();

	for (RDOValue it = arrayVal.begin(); it != arrayVal.end(); ++it)
	{
		tstring itStr = it.getAsString();
		int i = 1;
	}

	int i = 1;
}