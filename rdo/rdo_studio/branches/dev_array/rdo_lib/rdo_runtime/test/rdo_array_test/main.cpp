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
	RDOValue* integerVal1 = new RDOValue(1);
	RDOValue* integerVal2 = new RDOValue(2);
	RDOValue* integerVal3 = new RDOValue(3);
	RDOValue* integerVal4 = new RDOValue(4);
	RDOValue* integerVal5 = new RDOValue(5);
	RDOValue* integerVal6 = new RDOValue(6);
	RDOArrayType* atype1 = new RDOArrayType(NULL,RDOType::TypeID::t_int);
	RDOArrayType* atype2 = new RDOArrayType(NULL,RDOType::TypeID::t_int);
	RDOArrayType* atype = new RDOArrayType(NULL,RDOType::TypeID::t_array);
	RDOArrayValue* avalue1 = new RDOArrayValue(*atype);
	RDOArrayValue* avalue2 = new RDOArrayValue(*atype);
	RDOArrayValue* avalue = new RDOArrayValue(*atype);
	avalue1->insertItem(*integerVal1);
	avalue1->insertItem(*integerVal2);
	avalue1->insertItem(*integerVal3);
	avalue2->insertItem(*integerVal4);
	avalue2->insertItem(*integerVal5);
	avalue2->insertItem(*integerVal6);
	avalue->insertItem(*avalue1);
	avalue->insertItem(*avalue2);
	RDOValue* arrayVal = new RDOValue(*avalue);
	RDOValue extractVal = arrayVal->getArray().extractItem(1).getArray().extractItem(1);
	tstring w = arrayVal->getAsString();
	*arrayVal = *arrayVal + *integerVal2;
	tstring q = arrayVal->getAsString();
	
	int i = 1;
}