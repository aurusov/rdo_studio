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
#include <list>
#include <iostream>
// ====================================================================== SYNOPSIS
#include "rdo_common/rdosmart_ptr.h"
#include "rdo_common/rdosmart_ptr_wrapper.h"
// ===============================================================================

class MyClass
{
DECLARE_FACTORY(MyClass)
public:
	ruint m_i;

protected:
	MyClass()
		: m_i(2)
	{}

	~MyClass()
	{}
};
typedef rdo::smart_ptr<MyClass> LPMyClass;

class MyClass2: public MyClass
{
DECLARE_FACTORY(MyClass2)
public:
	ruint m_i1;
	ruint m_i2;
	ruint m_i3;
	ruint m_i4;
	ruint m_i5;

private:
	MyClass2()
		: m_i1(2)
	{}

	~MyClass2()
	{}
};
typedef rdo::smart_ptr<MyClass2> LPMyClass2;

class MyClass3
{
DECLARE_FACTORY(MyClass3)
public:
	ruint m_i1;

private:
	MyClass3()
		: m_i1(2)
	{}
	~MyClass3()
	{}
};
typedef rdo::smart_ptr<MyClass3> LPMyClass3;

void main()
{
	ruint size1 = sizeof(MyClass );
	ruint size2 = sizeof(MyClass2);
	ruint size3 = sizeof(rdo::smart_ptr<MyClass>);
	ruint size4 = sizeof(rdo::smart_ptr<MyClass2>);
	{
		rdo::smart_ptr<MyClass> obj = rdo::Factory<MyClass>::create();
		obj->m_i = 10;
	}
	{
		rdo::smart_ptr<MyClass> sobj = rdo::Factory<MyClass>::create();
		int i = 1;
	}
	{
		std::list<rdo::LPISmartPtrWrapper> container;
		for (ruint i = 0; i < 100; i++)
		{
			rdo::smart_ptr<MyClass> obj1 = rdo::Factory<MyClass>::create();
			rdo::smart_ptr_wrapper<MyClass> obj1wr(obj1);
			container.push_back(&obj1wr);
		}
		int i = 1;
	}
	{
		LPMyClass2 obj21 = rdo::Factory<MyClass2>::create();
		LPMyClass  obj11 = obj21;
		LPMyClass2 obj22 = obj11.cast<MyClass2>();
		LPMyClass2 obj23 = obj11.lp_cast<LPMyClass2>();
//		LPMyClass3 obj31 = obj21.cast<MyClass3>(); //! Не должно компилироваться
		int i = 1;
	}

	int i = 1;
}
