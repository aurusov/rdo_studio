// ====================================================================== INCLUDES
#include <iostream>
// ====================================================================== SYNOPSIS
#include "rdo_value.h"
#include "rdo_enum.h"
#include "rdo_array.h"
#include "rdo_type.h"
// ===============================================================================

using namespace rdoRuntime;

void main()
{
	RDOEnumType* enumType = new RDOEnumType(NULL, RDOEnumType::Enums("Red")("Green")("Type"));

	RDOValue value1(*enumType, "Red"  );
	RDOValue value2(*enumType, "Green");
	RDOValue value3(*enumType, "Green");
	bool equal, equal1;
	equal = value1 == value2;
	equal1 = value2 == value3;

	RDOArrayType* arrayType = new RDOArrayType(NULL, RDOArrayType::ArrayType);
	//RDOArrayType::Arrays r;
	rbool a;
	a=enumType->exist("qqq");
	int i = 1;
}
