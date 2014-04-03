/*!
  \copyright (c) RDO-Team, 2011
  \file      main.cpp
  \author    Урусов Андрей (rdo@rk9.bmstu.ru)
  \authors   Пройдаков Евгений (lord.tiran@gmail.com)
  \date      07.07.2009
  \brief     Тест интерфейсов
  \indent    4T
*/

// ----------------------------------------------------------------------- PLATFORM
#include "utils/src/common/platform.h"
// ----------------------------------------------------------------------- INCLUDES
#define BOOST_TEST_MODULE RDOInterfaceTest
#include "utils/src/common/warning_disable.h"
#include <list>
#include <vector>
#include <iostream>
#include <boost/test/included/unit_test.hpp>
#include "utils/src/common/warning_enable.h"
// ----------------------------------------------------------------------- SYNOPSIS
#include "utils/src/interface/rdointerface.h"
// --------------------------------------------------------------------------------

#define DISABLE_CONSOLE_OUTPUT

const tstring strIMy1            = "void fun1():  ";
const tstring strIMy2            = "void fun2():  ";
const tstring strIMy3            = "void fun3():  ";
const tstring strMyClass1Create  = "MyClass1():";
const tstring strMyClass1Destroy = "~MyClass1():";
const tstring strMyClass2Create  = "MyClass2():";
const tstring strMyClass2Destroy = "~MyClass2():";
const tstring strMyClass3Create  = "MyClass3():";
const tstring strMyClass3Destroy = "~MyClass3():";
const tchar   initFValue         = '1';
const tchar   initSValue         = '2';
const tchar   initTValue         = '3';

typedef std::list<tstring> LogList;
LogList s_logList;

class IMy1
{
public:
	virtual tstring fun1() = 0;
};

class IMy2
{
public:
	virtual tstring fun2() = 0;
};

class IMy3
{
public:
	virtual tstring fun3() = 0;
};

class IMy4
{
public:
	virtual tstring fun4() = 0;
};

INTERFACE_REGISTRATOR(IMy1, 1);
INTERFACE_REGISTRATOR(IMy2, 2);
INTERFACE_REGISTRATOR(IMy3, 3);
INTERFACE_REGISTRATOR(IMy4, 4);

class MyClass1: public rdo::IGetUnknown, public IMy1, public IMy2, public IInit
{
QUERY_INTERFACE_BEGIN
	QUERY_INTERFACE(IMy1)
	QUERY_INTERFACE(IMy2)
	QUERY_INTERFACE(IInit)
QUERY_INTERFACE_END

protected:
	MyClass1(tchar i)
		: m_i(i)
	{
#ifndef DISABLE_CONSOLE_OUTPUT
		std::cout << strMyClass1Create << i << std::endl;
#endif
		s_logList.push_back(strMyClass1Create + i);
	}
	~MyClass1()
	{
#ifndef DISABLE_CONSOLE_OUTPUT
		std::cout << strMyClass1Destroy << m_i << std::endl;
#endif
		s_logList.push_back(strMyClass1Destroy + m_i);
	}
	rbool init()
	{
		return true;
	}

protected:
	tchar m_i;

private:
	tstring fun1()
	{
#ifndef DISABLE_CONSOLE_OUTPUT
		std::cout << strIMy1 << m_i << std::endl;
#endif
		return strIMy1 + m_i;
	}
	tstring fun2()
	{
#ifndef DISABLE_CONSOLE_OUTPUT
		std::cout << strIMy2 << m_i << std::endl;
#endif
		return strIMy2 + m_i;
	}
};

class MyClass2: public MyClass1, public IMy3
{
DEFINE_IFACTORY(MyClass2);
QUERY_INTERFACE_BEGIN
	QUERY_INTERFACE_PARENT(MyClass1)
	QUERY_INTERFACE(IMy3)
QUERY_INTERFACE_END

private:
	MyClass2(tchar i)
		: MyClass1(i)
	{
#ifndef DISABLE_CONSOLE_OUTPUT
		std::cout << strMyClass2Create << i << std::endl;
#endif
		s_logList.push_back(strMyClass2Create + i);
	}
	virtual ~MyClass2()
	{
#ifndef DISABLE_CONSOLE_OUTPUT
		std::cout << strMyClass2Destroy << m_i << std::endl;
#endif
		s_logList.push_back(strMyClass2Destroy + m_i);
	}
	tstring fun3()
	{
		ASSERT(this);
		LPIMy1 int1 = this;
		ASSERT(int1)

#ifndef DISABLE_CONSOLE_OUTPUT
		std::cout << strIMy3 << m_i << std::endl;
#endif
		return strIMy3 + m_i;
	}
};

class MyClass3: public IMy3, public rdo::IGetUnknown
{
DEFINE_IFACTORY(MyClass3);
QUERY_INTERFACE_BEGIN
	QUERY_INTERFACE(IMy3)
QUERY_INTERFACE_END

protected:
	MyClass3(tchar i)
		: m_i(i)
	{
#ifndef DISABLE_CONSOLE_OUTPUT
		std::cout << strMyClass3Create << i << std::endl;
#endif
		s_logList.push_back(strMyClass3Create + i);
	}
	virtual ~MyClass3()
	{
#ifndef DISABLE_CONSOLE_OUTPUT
		std::cout << strMyClass3Destroy << m_i << std::endl;
#endif
		s_logList.push_back(strMyClass3Destroy + m_i);
	}

protected:
	tchar m_i;

private:
	tstring fun3()
	{
#ifndef DISABLE_CONSOLE_OUTPUT
		std::cout << strIMy3 << m_i << std::endl;
#endif
		return strIMy3 + m_i;
	}
};

typedef rdo::Interface<IMy3>     MyInterface;
typedef std::vector<MyInterface> MyInterfaceList;

BOOST_AUTO_TEST_SUITE(RDOInterfaceTest)

BOOST_AUTO_TEST_CASE(RdoInterfaceTest)
{
	{
		rdo::UnknownPointer smptr = RF(MyClass2)::create(initFValue);
		BOOST_REQUIRE(smptr);

		BOOST_CHECK(s_logList.size() == LogList::size_type(2));
		LogList::const_iterator it = s_logList.begin();
		BOOST_CHECK(*it == strMyClass1Create + initFValue);
		BOOST_CHECK(*(++it) == strMyClass2Create + initFValue);
		s_logList.clear();

		BOOST_CHECK(smptr.query_cast<IMy1>()->fun1() == strIMy1 + initFValue);
		BOOST_CHECK(smptr.query_cast<IMy3>()->fun3() == strIMy3 + initFValue);

		MyInterface         imy3 = smptr;
		rdo::UnknownPointer smptr2;
		smptr2 = RF(MyClass2)::create(initSValue);
		BOOST_CHECK(s_logList.size() == LogList::size_type(2));
		it = s_logList.begin();
		BOOST_CHECK(*it == strMyClass1Create + initSValue);
		BOOST_CHECK(*(++it) == strMyClass2Create + initSValue);
		s_logList.clear();

		rdo::UnknownPointer smptr2_2 = smptr2;
		BOOST_CHECK(!(smptr2_2 == smptr));
		BOOST_CHECK(smptr2_2 == smptr2);

		rdo::Interface<IMy1> int1_1 = smptr;
		rdo::Interface<IMy1> int2_1 = smptr2;
		rdo::Interface<IMy1> int1_2 = smptr;
		BOOST_CHECK(!(int1_1 == int2_1));
		BOOST_CHECK(int1_1 == int1_2);

		MyInterfaceList myInterfaceList;
		myInterfaceList.push_back(RF(MyClass3)::create(initTValue));
		myInterfaceList.push_back(smptr);
		myInterfaceList.push_back(smptr2);

		tchar symbolT = '0';
		STL_FOR_ALL_CONST(myInterfaceList, it)
		{
			rdo::Interface<IMy1> ptr1;
			ptr1 = *it;
			if (ptr1)
			{
				symbolT = (*it == smptr ? initFValue : (*it == smptr2 ? initSValue : initTValue));
				BOOST_CHECK(ptr1->fun1() == strIMy1 + symbolT);
			}

			rdo::Interface<IMy2> ptr2 = *it;
			if (ptr2)
			{
				symbolT = (*it == smptr ? initFValue : (*it == smptr2 ? initSValue : initTValue));
				BOOST_CHECK(ptr2->fun2() == strIMy2 + symbolT);
			}

			rdo::Interface<IMy3> ptr3 = *it;
			if (ptr3)
			{
				symbolT = (*it == smptr ? initFValue : (*it == smptr2 ? initSValue : initTValue));
				BOOST_CHECK(ptr3->fun3() == strIMy3 + symbolT);
			}
		}
		s_logList.clear();
	}

	BOOST_CHECK(s_logList.size() == LogList::size_type(5));
	LogList::const_iterator it = s_logList.begin();
	BOOST_CHECK(*it == strMyClass3Destroy + initTValue);
	++it;
	BOOST_CHECK(*it == strMyClass2Destroy + initSValue);
	++it;
	BOOST_CHECK(*it == strMyClass1Destroy + initSValue);
	++it;
	BOOST_CHECK(*it == strMyClass2Destroy + initFValue);
	++it;
	BOOST_CHECK(*it == strMyClass1Destroy + initFValue);
	s_logList.clear();
}

BOOST_AUTO_TEST_SUITE_END() // RDOInterfaceTest
