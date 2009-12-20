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
	RDOValue integer = 1;
	RDOValue real    = 1.5;
	RDOValue booling = true;
	RDOValue string  = "string";
	RDOArrayType* arrayType_int    = new RDOArrayType(NULL,&integer.type());
	RDOArrayType* arrayType_real   = new RDOArrayType(NULL,&real.type());
	RDOArrayType* arrayType_bool   = new RDOArrayType(NULL,&booling.type());
	RDOArrayType* arrayType_string = new RDOArrayType(NULL,&string.type());
	RDOArrayType* arrayType_enum   = new RDOArrayType(NULL,enumType);
	RDOArrayType* arrayType1       = new RDOArrayType(NULL,arrayType_int);
	RDOArrayType* arrayType2       = new RDOArrayType(NULL,arrayType_real);
	RDOArrayType* arrayType3       = new RDOArrayType(NULL,arrayType_bool);
	RDOArrayType* arrayType4       = new RDOArrayType(NULL,arrayType_string);
	RDOArrayType* arrayType5       = new RDOArrayType(NULL,arrayType_enum);
	RDOArrayType* arrayType6       = new RDOArrayType(NULL,arrayType5);
	RDOArrayType* arrayType7       = new RDOArrayType(NULL,arrayType6);
	int i = 1;
}
