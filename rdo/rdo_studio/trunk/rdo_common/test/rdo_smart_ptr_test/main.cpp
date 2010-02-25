/*
 * copyright: (c) RDO-Team, 2009
 * filename : main.cpp
 * author   : Урусов Андрей
 * date     : 14.07.2009
 * bref     : Тест умных указателей
 * indent   : 4T
 */

// ====================================================================== PCH
// ====================================================================== INCLUDES
#include <vector>
#include <iostream>
// ====================================================================== SYNOPSIS
#include "rdo_common/rdosmart_ptr.h"
#include "rdo_common/rdosmart_ptr_unknow.h"
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

class MyClass2
{
public:
	ruint m_i1;
	ruint m_i2;
	ruint m_i3;
	ruint m_i4;
	ruint m_i5;

	MyClass2()
		: m_i1(2)
	{}

	~MyClass2()
	{}
};

void main()
{
	ruint size1 = sizeof(MyClass );
	ruint size2 = sizeof(MyClass2);
	ruint size3 = sizeof(rdo::smart_ptr<MyClass>);
	ruint size4 = sizeof(rdo::smart_ptr<MyClass2>);
	{
		rdo::smart_ptr<MyClass> obj = new MyClass();
		obj->m_i = 10;
	}
	{
		rdo::smart_ptr<MyClass> sobj = new MyClass();
		rdo::usmart_ptr uobj = sobj;
		PTR(MyClass) pObj = uobj.get<MyClass>();
		rdo::smart_ptr<MyClass> sobj2 = uobj;
		int i = 1;
	}
	int i = 1;
}
