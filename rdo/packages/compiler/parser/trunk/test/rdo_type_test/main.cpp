/*!
  \copyright (c) RDO-Team, 2011
  \file      main.cpp
  \author    Урусов Андрей (rdo@rk9.bmstu.ru)
  \date      14.03.2010
  \brief     Тест типов парсера
  \indent    4T
*/

// ---------------------------------------------------------------------------- PCH
// ----------------------------------------------------------------------- INCLUDES
#include <vector>
#include <list>
#include <iostream>
// ----------------------------------------------------------------------- SYNOPSIS
#include "simulator/compiler/parser/type/atom.h"
#include "simulator/runtime/rdo_resource.h" //! @todo непонятный инклюд, но нужен чтобы собиралось
// --------------------------------------------------------------------------------

using namespace rdoParse;

void main()
{
	LPRDOType pType = rdo::Factory<RDOType__int>::create();
	ASSERT(pType);
	int i = 1;
}
