// ====================================================================== INCLUDES
#include <iostream>
// ====================================================================== SYNOPSIS
#include "rdo_lib\rdo_runtime\rdo_value.h"
#include "rdo_lib\rdo_runtime\rdo_enum.h"
#include "rdo_lib\rdo_runtime\rdo_array.h"
#include "rdo_lib\rdo_runtime\rdo_type.h"
// ===============================================================================

using namespace rdoRuntime;

tstring printType(CPTR(RDOType) pType)
{
	switch (pType->typeID())
	{
	case RDOType::t_int  : return "int";
	case RDOType::t_real : return "real";
	case RDOType::t_bool : return "bool";
	case RDOType::t_array:
	{
		CPTR(RDOArrayType) arrayType = static_cast<CPTR(RDOArrayType)>(pType);
		return rdo::format(_T("array<%s>"), printType(arrayType->getArrayType()).c_str());
	}
	}
	return "unknown type";
}

void main()
{
	RDOEnumType* enumType = new RDOEnumType(NULL, RDOEnumType::Enums("Значение_1")("Значение_2")("Значение_3"));
	RDOValue integer = 1;
	RDOValue real    = 1.5;
	RDOValue booling = true;
	RDOValue string  = "string";
	PTR(RDOArrayType) arrayType_int    = new RDOArrayType(NULL,&integer.type());
	PTR(RDOArrayType) arrayType_real   = new RDOArrayType(NULL,&real.type());
	PTR(RDOArrayType) arrayType_string = new RDOArrayType(NULL,&string.type());
	PTR(RDOArrayType) arrayType_bool   = new RDOArrayType(NULL,&booling.type());
	PTR(RDOArrayType) arrayType_enum   = new RDOArrayType(NULL,enumType);
	PTR(RDOArrayType) arrayType5       = new RDOArrayType(NULL,arrayType_enum);
	PTR(RDOArrayType) arrayType6       = new RDOArrayType(NULL,(new RDOArrayType(NULL,arrayType_int)));

	std::cout << printType(arrayType6) << std::endl;

	//std::string className1 = arrayType_int->asString();
	//std::string className2 = arrayType_real->asString();
	//std::string className3 = arrayType_string->asString();
	//std::string className4 = arrayType_bool->asString();
	//std::string className5 = arrayType_enum->asString();
	//std::string className6 = arrayType5->asString();
	//std::string className7 = arrayType6->asString();

	int i = 1;
}