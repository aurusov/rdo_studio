// ====================================================================== INCLUDES
#include <iostream>
#include <stdio.h>
// ====================================================================== SYNOPSIS
#include "rdo_lib\rdo_runtime\rdo_value.h"
#include "rdo_lib\rdo_runtime\rdo_array.h"
#include "rdo_lib\rdo_runtime\rdo_type.h"
// ===============================================================================

using namespace rdoRuntime;

#include "rdo_lib\rdo_runtime\test\rdo_array_test\arrayTestFunction.inl"

void main()
{
	arrayTestCreate();
	arrayTestInsert();
	arrayTestErase();
	arrayTestPPOperator();
	arrayTestOperatorPP();
	arrayTestMMOperator();
	arrayTestOperatorMM();

	int i = 1;
}