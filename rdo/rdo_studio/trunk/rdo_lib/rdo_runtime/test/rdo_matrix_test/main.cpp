// ====================================================================== INCLUDES
#include <iostream>
#include <stdio.h>
// ====================================================================== SYNOPSIS
#include "rdo_lib\rdo_runtime\rdo_value.h"
#include "rdo_lib\rdo_runtime\rdo_matrix.h"
#include "rdo_lib\rdo_runtime\rdo_type.h"
// ===============================================================================

using namespace rdoRuntime;

#include "rdo_lib\rdo_runtime\test\rdo_matrix_test\matrixTestFunction.inl"

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