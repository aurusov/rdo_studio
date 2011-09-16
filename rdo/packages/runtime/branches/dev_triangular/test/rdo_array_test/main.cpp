/*!
  \copyright (c) RDO-Team, 2011
  \file      main.cpp
  \authors   Чирков Михаил
  \date      13.12.2009
  \brief     Тест массивов
  \indent    4T
*/

// ---------------------------------------------------------------------------- PCH
// ----------------------------------------------------------------------- INCLUDES
#include <iostream>
#include <stdio.h>
// ----------------------------------------------------------------------- SYNOPSIS
#include "simulator/runtime/rdo_value.h"
#include "simulator/runtime/rdo_array.h"
#include "simulator/runtime/rdo_type.h"
#include "simulator/runtime/test/rdo_array_test/arrayTestFunction.inl"
// --------------------------------------------------------------------------------

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
