// ====================================================================== INCLUDES
#include <iostream>
#include <stdio.h>
// ====================================================================== SYNOPSIS
#include "simulator/runtime/rdo_value.h"
#include "simulator/runtime/rdo_array.h"
#include "simulator/runtime/rdo_type.h"
// ===============================================================================

using namespace rdoRuntime;

#include "simulator/runtime/test/rdo_array_test/arrayTestFunction.inl"

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