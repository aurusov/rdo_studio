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
	RDOArrayType atype1(NULL,g_int);
	RDOArrayType atype2(NULL,g_int);
	RDOArrayType atype(NULL,RDOType::TypeID::t_array);
	RDOArrayValue avalue1(atype);
	RDOArrayValue avalue2(atype);
	RDOArrayValue avalue(atype);
	avalue1.insertItem(integerVal1);
	avalue1.insertItem(integerVal2);
	avalue1.insertItem(integerVal3);
	avalue2.insertItem(integerVal4);
	avalue2.insertItem(integerVal5);
	avalue2.insertItem(integerVal6);
	avalue.insertItem(avalue1);
	avalue.insertItem(avalue2);
	RDOValue arrayVal1(avalue1);
	RDOValue arrayVal2(avalue2);
	RDOValue arrayVal(avalue);

	RDOValue itr(arrayVal.begin());

	itr = itr + RDOValue(1);

	tstring ns = arrayVal.getAsString();
	
	int i = 1;
}