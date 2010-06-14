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
#include "rdo_common/smart_ptr/intrusive_ptr.h"
#include "rdo_common/smart_ptr/interface_ptr.h"
#include "rdo_common/rdodebug.h"
// ===============================================================================

OBJECT(MyClass)
{
DECLARE_FACTORY(MyClass)
public:
	ruint m_i;

protected:
	MyClass()
		: m_i(2)
	{
		int i = 1;
	}

	~MyClass()
	{
		int i = 1;
	}
};

OBJECT_INTERFACE(IMyClass21)
{
	virtual void ifun21() = 0;
};
#define DECLARE_IMyClass21 \
public:                    \
	void ifun21();

class MyClass2: public MyClass, public IMyClass21, public rdo::CounterReferenceReal<MyClass2>
{
DECLARE_FACTORY(MyClass2)
public:
	ruint m_k;

protected:
	MyClass2()
		: m_k(20)
	{
		int i = 1;
	}

	virtual ~MyClass2()
	{
		int i = 1;
	}
	DECLARE_IMyClass21;
};
DECLARE_POINTER(MyClass2)

void MyClass2::ifun21()
{
	int i = 1;
}

OBJECT(MyClass3)
{
public:
	ruint m_i;

	MyClass3()
		: m_i(3)
	{
		int i = 1;
	}

	~MyClass3()
	{
		int i = 1;
	}

	LPMyClass3 fun()
	{
		return this;
	}
};

class MyClass4: public MyClass2
{
DECLARE_FACTORY(MyClass4)
public:
	ruint m_a;

private:
	MyClass4()
		: m_a(24)
	{
		int i = 1;
	}

	virtual ~MyClass4()
	{
		int i = 1;
	}
};
DECLARE_POINTER(MyClass4)

void main()
{
	ruint size1 = sizeof(MyClass);
	ruint size2 = sizeof(LPMyClass);

	{ //! Создание через фабрику и тест конструтора копии

		LPMyClass pMyClass = rdo::Factory<MyClass>::create();
		{
			LPMyClass pMyClass2 = pMyClass;
		}
		pMyClass->m_i = 3;
	}

	{ //! Конструктор по-умолчанию, приведение указателя к rbool, оператор копирования, сброс указателя

		LPMyClass pMyClass1;
		ASSERT(!pMyClass1);
		LPMyClass pMyClass2 = rdo::Factory<MyClass>::create();
		ASSERT(pMyClass2);
		pMyClass1 = pMyClass2;
		ASSERT(pMyClass1);

		LPMyClass pMyClass3 = rdo::Factory<MyClass>::create();
		pMyClass2 = NULL;
		pMyClass1 = pMyClass3;
	}

	{ //! Создание указателя по типу (не только из фабрики), создание указателя по this
		PTR(MyClass3) pMyClass3 = new MyClass3();
		LPMyClass3    pMyClass4(pMyClass3);
		LPMyClass3    pMyClass5 = pMyClass4->fun();
	}

	{ //! Кастинг к предку

		LPMyClass2 pMyClass2 = rdo::Factory<MyClass2>::create();
		pMyClass2->m_i = 10;
		pMyClass2->m_k = 22;
		LPMyClass  pMyClass = pMyClass2.object_cast<MyClass>();
		pMyClass->m_i = 11;
	}

	{ //! Кастинг к интерфейсу

		LPMyClass2 pMyClass2 = rdo::Factory<MyClass2>::create();
		LPIMyClass21 pMyClass21 = pMyClass2.interface_cast<IMyClass21>();
		pMyClass2 = NULL;
		pMyClass21 = LPIMyClass21();
	}

	{ //! Кастинг к интерфейсу через потомка, хотя интерфейс заведен у предка

		LPMyClass4 pMyClass4 = rdo::Factory<MyClass4>::create();
		LPIMyClass21 pMyClass21 = pMyClass4.interface_cast<IMyClass21>();
		pMyClass4 = NULL;
		pMyClass21 = LPIMyClass21();
	}

	int i = 1;
}
