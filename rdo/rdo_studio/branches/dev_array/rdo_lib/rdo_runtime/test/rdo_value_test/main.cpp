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

	RDOArrayType* arrayType  = new RDOArrayType(NULL, enumType);
	RDOArrayType* arrayType2 = new RDOArrayType(NULL, arrayType);

	std::string className = arrayType2->asString();

	int i = 1;
}
