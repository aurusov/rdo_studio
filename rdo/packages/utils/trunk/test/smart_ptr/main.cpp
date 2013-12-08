/*!
  \copyright (c) RDO-Team, 2011
  \file      main.cpp
  \author    Урусов Андрей (rdo@rk9.bmstu.ru)
  \authors   Пройдаков Евгений (lord.tiran@gmail.com)
  \date      14.07.2009
  \brief     Тест умных указателей
  \indent    4T
*/

// ----------------------------------------------------------------------- PLATFORM
#include "utils/src/common/platform.h"
#ifdef COMPILER_MINGW
	#undef __STRICT_ANSI__
#endif
// ----------------------------------------------------------------------- INCLUDES
#include <vector>
#include <list>
#include <iostream>
#define BOOST_TEST_MODULE RDOSmartPrtTest
#include <boost/test/included/unit_test.hpp>
// ----------------------------------------------------------------------- SYNOPSIS
#include "utils/src/smart_ptr/factory/factory.h"
#include "utils/src/smart_ptr/interface_ptr/interface_ptr.h"
#include "utils/src/debug/rdodebug.h"
// --------------------------------------------------------------------------------

class Log;
PTR(Log) g_pLog = NULL;

class Log: public std::list<rsint>
{
public:
	 Log() { g_pLog = this; }
	~Log() { g_pLog = NULL; }
};

const rsint MULTIPLEXER = 100;
const rsint CHECK1      = 7890;
const rsint CHECK2      = 5362;

OBJECT(MyClass)
{
DECLARE_FACTORY(MyClass)
public:
	rsint m_a;

	enum { DAFAUL_VALUE = 1 };

protected:
	 MyClass(): m_a(DAFAUL_VALUE) { g_pLog->push_back( m_a); }
	~MyClass()                    { g_pLog->push_back(-m_a); }
};

OBJECT_INTERFACE(IMyClass21)
{
	enum { DAFAUL_VALUE = 21 };

	virtual void ifun21() = 0;

	         IMyClass21() { g_pLog->push_back( DAFAUL_VALUE); }
	virtual ~IMyClass21() { g_pLog->push_back(-DAFAUL_VALUE); }
};

#define DECLARE_IMyClass21 \
public:                    \
	void ifun21();

CLASS(MyClass2):
	    IMPLEMENTATION_OF(IMyClass21)
	AND INSTANCE_OF      (MyClass   )
{
DECLARE_FACTORY(MyClass2)
public:
	rsint m_b;

	enum { DAFAUL_VALUE = 2 };

	rbool operator== (CREF(MyClass2) obj) const
	{
		return m_b == obj.m_b;
	}

protected:
	 MyClass2(): m_b(DAFAUL_VALUE) { g_pLog->push_back( m_b); }
	~MyClass2()                    { g_pLog->push_back(-m_b); }

	DECLARE_IMyClass21;
};
DECLARE_POINTER(MyClass2)

void MyClass2::ifun21()
{
	int i = 1;
	UNUSED(i);
}

OBJECT(MyClass3)
{
public:
	rsint m_c;

	enum { DAFAUL_VALUE = 3 };

	 MyClass3(): m_c(DAFAUL_VALUE) { g_pLog->push_back( m_c); }
	~MyClass3()                    { g_pLog->push_back(-m_c); }

	LPMyClass3 returnThis()
	{
		return this;
	}
};

CLASS(MyClass4): INSTANCE_OF(MyClass2)
{
DECLARE_FACTORY(MyClass4)
public:
	rsint m_d;

	enum { DAFAUL_VALUE = 4 };

private:
	 MyClass4(): m_d(DAFAUL_VALUE) { g_pLog->push_back( m_d); }
	~MyClass4()                    { g_pLog->push_back(-m_d); }
};
DECLARE_POINTER(MyClass4)

BOOST_AUTO_TEST_SUITE(RDOSmartPrtTest)

BOOST_AUTO_TEST_CASE(DefaultConstructor)
{
	Log log;
	{
		LPMyClass pMyClass = rdo::Factory<MyClass>::create();
		BOOST_CHECK(pMyClass);
		BOOST_CHECK(pMyClass->m_a == MyClass::DAFAUL_VALUE);

		pMyClass->m_a = 3;
		BOOST_CHECK(pMyClass->m_a == 3);
		pMyClass->m_a = MyClass::DAFAUL_VALUE;
	}
	BOOST_CHECK(log.size() == 2);
	AUTO(it, log.begin());
	BOOST_CHECK(*it     ==  MyClass::DAFAUL_VALUE);
	BOOST_CHECK(*(++it) == -MyClass::DAFAUL_VALUE);
}

BOOST_AUTO_TEST_CASE(CastSmartPtrToBool)
{
	Log log;
	{
		LPMyClass pMyClass2 = rdo::Factory<MyClass>::create();
		BOOST_CHECK((rbool)pMyClass2);
	}
	BOOST_CHECK(log.size() == 2);
	AUTO(it, log.begin());
	BOOST_CHECK(*it     ==  MyClass::DAFAUL_VALUE);
	BOOST_CHECK(*(++it) == -MyClass::DAFAUL_VALUE);
}

BOOST_AUTO_TEST_CASE(OperatorPtrEqual)
{
	Log log;
	{
		LPMyClass pMyClass1;
		BOOST_CHECK(!pMyClass1);

		LPMyClass pMyClass2 = rdo::Factory<MyClass>::create();
		BOOST_CHECK(pMyClass2);

		pMyClass1 = pMyClass2;
		BOOST_CHECK(pMyClass1      == pMyClass2     );
		BOOST_CHECK(pMyClass1->m_a == pMyClass2->m_a);

		pMyClass2 = NULL;
		BOOST_CHECK(!pMyClass2);
		BOOST_CHECK(pMyClass1);

		LPMyClass pMyClass3 = rdo::Factory<MyClass>::create();
		BOOST_CHECK(pMyClass3);
		pMyClass3->m_a *= MULTIPLEXER;

		pMyClass1 = pMyClass3;
		BOOST_CHECK(pMyClass1      == pMyClass3     );
		BOOST_CHECK(pMyClass1->m_a == pMyClass3->m_a);
	}
	BOOST_CHECK(log.size() == 4);
	AUTO(it, log.begin());
	BOOST_CHECK(*it     ==  MyClass::DAFAUL_VALUE              );
	BOOST_CHECK(*(++it) ==  MyClass::DAFAUL_VALUE              );
	BOOST_CHECK(*(++it) == -MyClass::DAFAUL_VALUE              );
	BOOST_CHECK(*(++it) == -MyClass::DAFAUL_VALUE * MULTIPLEXER);
}

BOOST_AUTO_TEST_CASE(CreateSmartPtrByThis)
{
	Log log;
	{
		PTR(MyClass3) pMyClass3Raw = new MyClass3();
		BOOST_CHECK(pMyClass3Raw);

		LPMyClass3 pMyClass3_1(pMyClass3Raw);
		BOOST_CHECK(pMyClass3_1);

		LPMyClass3 pMyClass3_2 = pMyClass3_1->returnThis();
		BOOST_CHECK(pMyClass3_2);
	}
	BOOST_CHECK(log.size() == 2);
	AUTO(it, log.begin());
	BOOST_CHECK(*it     ==  MyClass3::DAFAUL_VALUE);
	BOOST_CHECK(*(++it) == -MyClass3::DAFAUL_VALUE);
}

BOOST_AUTO_TEST_CASE(StaticCast)
{
	Log log;
	{
		LPMyClass2 pMyClass2 = rdo::Factory<MyClass2>::create();
		BOOST_CHECK(pMyClass2);
		pMyClass2->m_a *= MULTIPLEXER;
		pMyClass2->m_b *= MULTIPLEXER;

		LPMyClass pMyClass = pMyClass2.object_static_cast<MyClass>();
		BOOST_CHECK(pMyClass->m_a == MyClass::DAFAUL_VALUE * MULTIPLEXER);

		++pMyClass->m_a;
		BOOST_CHECK(pMyClass->m_a == MyClass::DAFAUL_VALUE * MULTIPLEXER + 1);
		BOOST_CHECK(pMyClass->m_a == pMyClass2->m_a);
	}
	BOOST_CHECK(log.size() == 6);
	AUTO(it, log.begin());
	BOOST_CHECK(*it     ==  IMyClass21 ::DAFAUL_VALUE                   );
	BOOST_CHECK(*(++it) ==   MyClass   ::DAFAUL_VALUE                   );
	BOOST_CHECK(*(++it) ==   MyClass2  ::DAFAUL_VALUE                   );
	BOOST_CHECK(*(++it) == -(MyClass2  ::DAFAUL_VALUE * MULTIPLEXER    ));
	BOOST_CHECK(*(++it) == -(MyClass   ::DAFAUL_VALUE * MULTIPLEXER + 1));
	BOOST_CHECK(*(++it) == -(IMyClass21::DAFAUL_VALUE                  ));
}

BOOST_AUTO_TEST_CASE(InterfaceCast)
{
	Log log;
	{
		LPMyClass2 pMyClass2 = rdo::Factory<MyClass2>::create();
		BOOST_CHECK(pMyClass2);

		LPIMyClass21 pMyClass21 = pMyClass2.interface_cast<IMyClass21>();
		BOOST_CHECK(pMyClass21);
		BOOST_CHECK(pMyClass2 == pMyClass21);

		pMyClass2 = NULL;
		BOOST_CHECK(!pMyClass2);
		BOOST_CHECK(pMyClass21);

		log.push_back(CHECK1);
		pMyClass21 = LPIMyClass21();
		BOOST_CHECK(!pMyClass21);
		log.push_back(CHECK2);
	}
	BOOST_CHECK(log.size() == 8);
	AUTO(it, log.begin());
	BOOST_CHECK(*it     ==  IMyClass21::DAFAUL_VALUE);
	BOOST_CHECK(*(++it) ==  MyClass   ::DAFAUL_VALUE);
	BOOST_CHECK(*(++it) ==  MyClass2  ::DAFAUL_VALUE);
	BOOST_CHECK(*(++it) ==  CHECK1                  );
	BOOST_CHECK(*(++it) == -MyClass2  ::DAFAUL_VALUE);
	BOOST_CHECK(*(++it) == -MyClass   ::DAFAUL_VALUE);
	BOOST_CHECK(*(++it) == -IMyClass21::DAFAUL_VALUE);
	BOOST_CHECK(*(++it) ==  CHECK2                  );
}

BOOST_AUTO_TEST_CASE(InterfaceCastThroughChild)
{
	Log log;
	{
		LPMyClass4 pMyClass4 = rdo::Factory<MyClass4>::create();
		BOOST_CHECK(pMyClass4);

		LPIMyClass21 pMyClass21 = pMyClass4.interface_cast<IMyClass21>();
		BOOST_CHECK(pMyClass4 == pMyClass21);
		pMyClass4 = NULL;
		BOOST_CHECK(!pMyClass4);
		BOOST_CHECK(pMyClass21);

		log.push_back(CHECK1);
		pMyClass21 = LPIMyClass21();
		BOOST_CHECK(!pMyClass21);
		log.push_back(CHECK2);
	}
	BOOST_CHECK(log.size() == 10);
	AUTO(it, log.begin());
	BOOST_CHECK(*it     ==  IMyClass21::DAFAUL_VALUE);
	BOOST_CHECK(*(++it) ==  MyClass   ::DAFAUL_VALUE);
	BOOST_CHECK(*(++it) ==  MyClass2  ::DAFAUL_VALUE);
	BOOST_CHECK(*(++it) ==  MyClass4  ::DAFAUL_VALUE);
	BOOST_CHECK(*(++it) ==  CHECK1                  );
	BOOST_CHECK(*(++it) == -MyClass4  ::DAFAUL_VALUE);
	BOOST_CHECK(*(++it) == -MyClass2  ::DAFAUL_VALUE);
	BOOST_CHECK(*(++it) == -MyClass   ::DAFAUL_VALUE);
	BOOST_CHECK(*(++it) == -IMyClass21::DAFAUL_VALUE);
	BOOST_CHECK(*(++it) ==  CHECK2                  );
}

BOOST_AUTO_TEST_CASE(CompareSmartPtr)
{
	Log log;
	{
		LPMyClass2 pMyClass1 = rdo::Factory<MyClass2>::create();
		BOOST_CHECK(pMyClass1);

		LPMyClass  pMyClass2 = pMyClass1.object_static_cast<MyClass>();
		BOOST_CHECK(pMyClass2);

		BOOST_CHECK(pMyClass1 == pMyClass2);
		BOOST_CHECK(!(pMyClass1 != pMyClass2));

		LPMyClass3 pMyClass3 = rdo::Factory<MyClass3>::create();
		BOOST_CHECK(pMyClass3);

		BOOST_CHECK(pMyClass1 != pMyClass3);
		BOOST_CHECK(!(pMyClass1 == pMyClass3));
	}
	BOOST_CHECK(log.size() == 8);
	AUTO(it, log.begin());
	BOOST_CHECK(*it     ==  IMyClass21::DAFAUL_VALUE);
	BOOST_CHECK(*(++it) ==  MyClass   ::DAFAUL_VALUE);
	BOOST_CHECK(*(++it) ==  MyClass2  ::DAFAUL_VALUE);
	BOOST_CHECK(*(++it) ==  MyClass3  ::DAFAUL_VALUE);
	BOOST_CHECK(*(++it) == -MyClass3  ::DAFAUL_VALUE);
	BOOST_CHECK(*(++it) == -MyClass2  ::DAFAUL_VALUE);
	BOOST_CHECK(*(++it) == -MyClass   ::DAFAUL_VALUE);
	BOOST_CHECK(*(++it) == -IMyClass21::DAFAUL_VALUE);
}

BOOST_AUTO_TEST_CASE(CompareSmartPtrByValue)
{
	Log log;
	{
		LPMyClass2 pMyClass21 = rdo::Factory<MyClass2>::create();
		BOOST_CHECK(pMyClass21);

		LPMyClass2 pMyClass22 = rdo::Factory<MyClass2>::create();
		BOOST_CHECK(pMyClass22);

		BOOST_CHECK(pMyClass21.compare(pMyClass22));

		++pMyClass22->m_b;
		BOOST_CHECK(!pMyClass21.compare(pMyClass22));
		--pMyClass22->m_b;
	}
	BOOST_CHECK(log.size() == 12);
	AUTO(it, log.begin());
	BOOST_CHECK(*it     ==  IMyClass21::DAFAUL_VALUE);
	BOOST_CHECK(*(++it) ==  MyClass   ::DAFAUL_VALUE);
	BOOST_CHECK(*(++it) ==  MyClass2  ::DAFAUL_VALUE);
	BOOST_CHECK(*(++it) ==  IMyClass21::DAFAUL_VALUE);
	BOOST_CHECK(*(++it) ==  MyClass   ::DAFAUL_VALUE);
	BOOST_CHECK(*(++it) ==  MyClass2  ::DAFAUL_VALUE);
	BOOST_CHECK(*(++it) == -MyClass2  ::DAFAUL_VALUE);
	BOOST_CHECK(*(++it) == -MyClass   ::DAFAUL_VALUE);
	BOOST_CHECK(*(++it) == -IMyClass21::DAFAUL_VALUE);
	BOOST_CHECK(*(++it) == -MyClass2  ::DAFAUL_VALUE);
	BOOST_CHECK(*(++it) == -MyClass   ::DAFAUL_VALUE);
	BOOST_CHECK(*(++it) == -IMyClass21::DAFAUL_VALUE);
}

BOOST_AUTO_TEST_SUITE_END() // RDOSmartPrtTest
