/*
 * copyright: (c) RDO-Team, 2009
 * filename : main.cpp
 * author   : Урусов Андрей
 * date     : 14.07.09
 * bref     : Тест умных указателей
 * indent   : 4T
 */

// ====================================================================== PCH
// ====================================================================== INCLUDES
#include <vector>
#include <iostream>
// ====================================================================== SYNOPSIS
#include "rdo_common/rdosmart_ptr.h"
// ===============================================================================

class MyClass
{
public:
	ruint m_i;

	MyClass()
		: m_i(2)
	{}

	~MyClass()
	{}
};

void main()
{
	{
		rdo::smart_ptr<MyClass> obj = new MyClass();
		obj->m_i = 10;
	}
	int i = 1;
}
