// ====================================================================== INCLUDES
#include <iostream>
#include <stdio.h>
// ====================================================================== SYNOPSIS
#include "simulator/runtime/rdo_value.h"
#include "simulator/runtime/rdo_matrix.h"
#include "simulator/runtime/rdo_type.h"
// ===============================================================================

using namespace rdoRuntime;

#include "simulator/runtime/test/rdo_matrix_test/matrixTestFunction.inl"

void main()
{
	matrixTestCreate();
	matrixTestInsert();
	matrixTestErase();
	matrixTestPPOperator();
	matrixTestOperatorPP();
	matrixTestMMOperator();
	matrixTestOperatorMM();

	int i = 1;
}