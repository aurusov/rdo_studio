/*
 * copyright: (c) RDO-Team, 2011
 * filename : main.cpp
 * author   : Урусов Андрей, Evgeny Proydakov
 * date     : 07.07.2009
 * bref     : Тест интерфейсов
 * indent   : 4T
 */
 
// ====================================================================== PCH
// ====================================================================== INCLUDES
#include <vector>
#include <list>
#include <iostream>
#define BOOST_TEST_MODULE RdoInterfaceTest
#include <boost/test/included/unit_test.hpp>
// ====================================================================== SYNOPSIS
#include "rdo_common/rdointerface.h"
// ===============================================================================

#define DISABLE_CONSOLE_OUTPUT

const tstring strIMy1            = _T("void fun1():  ");
const tstring strIMy2            = _T("void fun2():  ");
const tstring strIMy3            = _T("void fun3():  ");
const tstring strMyClass1Create  = _T("MyClass1():"   );
const tstring strMyClass1Destroy = _T("~MyClass1():"  );
const tstring strMyClass2Create  = _T("MyClass2():"   );
const tstring strMyClass2Destroy = _T("~MyClass2():"  );
const tstring strMyClass3Create  = _T("MyClass3():"   );
const tstring strMyClass3Destroy = _T("~MyClass3():"  );

typedef std::list<tstring> LogList;
LogList logList;

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
	MyClass1(char i)
		: m_i(i)
	{
#ifndef DISABLE_CONSOLE_OUTPUT
		std::cout << strMyClass1Create << i << std::endl;
#endif
		logList.push_back(strMyClass1Create + i);
	}
	~MyClass1()
	{
#ifndef DISABLE_CONSOLE_OUTPUT
		std::cout << strMyClass1Destroy << m_i << std::endl;
#endif
		logList.push_back(strMyClass1Destroy);
	}
	rbool init()
	{
		return true;
	}

protected:
	char m_i;

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
	MyClass2(char i)
		: MyClass1(i)
	{
#ifndef DISABLE_CONSOLE_OUTPUT
		std::cout << strMyClass2Create << i << std::endl;
#endif
		logList.push_back(strMyClass2Create + i);
	}
	~MyClass2()
	{
#ifndef DISABLE_CONSOLE_OUTPUT
		std::cout << strMyClass2Destroy << m_i << std::endl;
#endif
		logList.push_back(strMyClass2Destroy);
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
	MyClass3(char i)
		: m_i(i)
	{
#ifndef DISABLE_CONSOLE_OUTPUT
		std::cout << strMyClass3Create << i << std::endl;
#endif
		logList.push_back(strMyClass3Create + i);
	}
	~MyClass3()
	{
#ifndef DISABLE_CONSOLE_OUTPUT
		std::cout << strMyClass3Destroy << m_i << std::endl;
#endif
		logList.push_back(strMyClass3Destroy);
	}

protected:
	char m_i;

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

BOOST_AUTO_TEST_SUITE(RdoInterfaceTest)

BOOST_AUTO_TEST_CASE(RdoInterfaceTest_CheckingOnTheSameOperator)
{
	char initValue = _T('2');
	rdo::UnknownPointer smptr = F(MyClass2)::create(initValue);
	BOOST_REQUIRE(smptr);

	LogList::iterator it = logList.begin();
	BOOST_CHECK_EQUAL(*it,     strMyClass1Create + initValue);
	BOOST_CHECK_EQUAL(*(++it), strMyClass2Create + initValue);
	logList.clear();

	BOOST_CHECK_EQUAL(smptr.query_cast<IMy1>()->fun1(), strIMy1 + initValue);
	BOOST_CHECK_EQUAL(smptr.query_cast<IMy3>()->fun3(), strIMy3 + initValue);

	MyInterface imy3 = smptr;

	initValue = _T('7');

	rdo::UnknownPointer smptr2;
	smptr2 = F(MyClass2)::create(initValue);
	it = logList.begin();
	BOOST_CHECK_EQUAL(*it, strMyClass1Create + initValue);
	BOOST_CHECK_EQUAL(*(++it), strMyClass2Create + initValue);
	logList.clear();

	rdo::UnknownPointer smptr2_2 = smptr2;
	//BOOST_CHECK(smptr2_2 == smptr);
	//BOOST_CHECK_EQUAL(smptr2_2, smptr);
	BOOST_CHECK(smptr2_2 == smptr2);

	rdo::Interface<IMy1> int1_1 = smptr;
	rdo::Interface<IMy1> int2_1 = smptr2;
	rdo::Interface<IMy1> int1_2 = smptr;
	//BOOST_CHECK(int1_1 == int2_1);
	//BOOST_CHECK_EQUAL(int1_1, int2_1);
	BOOST_CHECK(int1_1 == int1_2);
}

BOOST_AUTO_TEST_CASE(RdoInterfaceTest_ValidationFailureInThePreviousTest1)
{
	LogList::iterator it = logList.begin();
	BOOST_CHECK_EQUAL(*it, strMyClass2Destroy);
	++it;
	BOOST_CHECK_EQUAL(*it, strMyClass1Destroy);
	++it;
	BOOST_CHECK_EQUAL(*it, strMyClass2Destroy);
	++it;
	BOOST_CHECK_EQUAL(*it, strMyClass1Destroy);
	logList.clear();
}

BOOST_AUTO_TEST_CASE(RdoInterfaceTest_Working)
{
	char initFValue = _T('2');
	rdo::UnknownPointer smptr = F(MyClass2)::create(initFValue);
	LogList::iterator iter = logList.begin();
	BOOST_CHECK_EQUAL(*iter, strMyClass1Create + initFValue);
	BOOST_CHECK_EQUAL(*(++iter), strMyClass2Create + initFValue);
	logList.clear();

	char initSValue = _T('5');
	rdo::UnknownPointer smptr2 = F(MyClass2)::create(initSValue);
	iter = logList.begin();
	BOOST_CHECK_EQUAL(*iter, strMyClass1Create + initSValue);
	BOOST_CHECK_EQUAL(*(++iter), strMyClass2Create + initSValue);
	logList.clear();

	MyInterfaceList myList;

	char initTValue = _T('9');
	myList.push_back(F(MyClass3)::create(initTValue));
	myList.push_back(smptr );
	myList.push_back(smptr2);
	
	char symbolT = _T('0');

	STL_FOR_ALL_CONST(myList, it)
	{
		rdo::Interface<IMy1> ptr1;
		ptr1 = *it;
		if (ptr1)
		{
			symbolT = (*it == smptr ? initFValue : (*it == smptr2 ? initSValue : initTValue));
			BOOST_CHECK_EQUAL(ptr1->fun1(), strIMy1 + symbolT);
		}

		rdo::Interface<IMy2> ptr2 = *it;
		if (ptr2)
		{
			symbolT = (*it == smptr ? initFValue : (*it == smptr2 ? initSValue : initTValue));
			BOOST_CHECK_EQUAL(ptr2->fun2(), strIMy2 + symbolT);
		}

		rdo::Interface<IMy3> ptr3 = *it;
		if (ptr3)
		{
			symbolT = (*it == smptr ? initFValue : (*it == smptr2 ? initSValue : initTValue));
			BOOST_CHECK_EQUAL(ptr3->fun3(), strIMy3 + symbolT);
		}
	}
	logList.clear();
}

BOOST_AUTO_TEST_CASE(RdoInterfaceTest_ValidationFailureInThePreviousTest2)
{
	LogList::iterator it = logList.begin();
	BOOST_CHECK_EQUAL(*it, strMyClass3Destroy);
	++it;
	BOOST_CHECK_EQUAL(*it, strMyClass2Destroy);
	++it;
	BOOST_CHECK_EQUAL(*it, strMyClass1Destroy);
	++it;
	BOOST_CHECK_EQUAL(*it, strMyClass2Destroy);
	++it;
	BOOST_CHECK_EQUAL(*it, strMyClass1Destroy);
	logList.clear();
}

BOOST_AUTO_TEST_SUITE_END()
