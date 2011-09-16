/*!
  \copyright (c) RDO-Team, 2011
  \file      main.cpp
  \authors   Чирков Михаил
  \date      01.10.2010
  \brief     Тест матриц
  \indent    4T
*/

// ---------------------------------------------------------------------------- PCH
// ----------------------------------------------------------------------- INCLUDES
#include <iostream>
#include <stdio.h>
// ----------------------------------------------------------------------- SYNOPSIS
#include "simulator/runtime/rdo_value.h"
#include "simulator/runtime/rdo_matrix.h"
#include "simulator/runtime/rdo_type.h"
#include "simulator/runtime/test/rdo_matrix_test/matrixTestFunction.inl"
// --------------------------------------------------------------------------------

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
