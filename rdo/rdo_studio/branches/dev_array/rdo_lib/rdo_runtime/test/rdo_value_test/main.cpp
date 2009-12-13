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

	RDOArrayType* arrayType = new RDOArrayType();

	int i = 1;
}
