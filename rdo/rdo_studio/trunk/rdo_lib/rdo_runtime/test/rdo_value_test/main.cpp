/*
 * copyright: (c) RDO-Team, 2009
 * filename : main.cpp
 * author   : Урусов Андрей
 * date     : 13.07.2009
 * bref     : тест для RDOValue
 * indent   : 4T
 */

// ====================================================================== INCLUDES
#include <iostream>
// ====================================================================== SYNOPSIS
#include "rdo_value.h"
// ===============================================================================

using namespace rdoRuntime;

void main()
{
	RDOValue value1(tstring("qqq"));
	RDOValue value2 = value1;
	value2 = tstring("aaa");
	int i = 1;
}
