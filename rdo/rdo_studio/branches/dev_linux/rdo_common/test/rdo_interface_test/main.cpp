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
#include <string>
#include <iostream>
#define BOOST_TEST_MODULE RdoInterfaceTest
#include <boost/test/included/unit_test.hpp>
// ====================================================================== SYNOPSIS
#include "rdo_common/rdointerface.h"
// ===============================================================================

#define DISABLE_CONSOLE_OUTPUT

const tstring void_fun_1       = "void fun1():  ";
const tstring void_fun_2       = "void fun2():  ";
const tstring void_fun_3       = "void fun3():  ";
const tstring create_myclass1  = "MyClass1():";
const tstring destroy_myclass1 = "~MyClass1():";
const tstring create_myclass2  = "MyClass2():";
const tstring destroy_myclass2 = "~MyClass2():";
const tstring create_myclass3  = "MyClass3():";
const tstring destroy_myclass3 = "~MyClass3():";

std::list<tstring> logList;

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
		std::cout << create_myclass1 << i << std::endl;
#endif
		logList.push_back(create_myclass1 + i);
	}
	~MyClass1()
	{
#ifndef DISABLE_CONSOLE_OUTPUT
		std::cout << destroy_myclass1 << m_i << std::endl;
#endif
		logList.push_back(destroy_myclass1);
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
		std::cout << void_fun_1 << m_i << std::endl;
#endif
		return void_fun_1 + m_i;
	}
	tstring fun2()
	{
#ifndef DISABLE_CONSOLE_OUTPUT
		std::cout << void_fun_2 << m_i << std::endl;
#endif
		return void_fun_2 + m_i;
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
	MyClass2(char i): MyClass1(i)
	{
#ifndef DISABLE_CONSOLE_OUTPUT
		std::cout << create_myclass2 << i << std::endl;
#endif
		logList.push_back(create_myclass2 + i);
	}
	~MyClass2()
	{
#ifndef DISABLE_CONSOLE_OUTPUT
		std::cout << destroy_myclass2 << m_i << std::endl;
#endif
		logList.push_back(destroy_myclass2);
	}
	tstring fun3()
	{
		ASSERT(this);
		LPIMy1 int1 = this;
		ASSERT(int1)

#ifndef DISABLE_CONSOLE_OUTPUT
		std::cout << void_fun_3 << m_i << std::endl;
#endif
		return void_fun_3 + m_i;
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
		std::cout << create_myclass3 << i << std::endl;
#endif
		logList.push_back(create_myclass3 + i);
	}
	~MyClass3()
	{
#ifndef DISABLE_CONSOLE_OUTPUT
		std::cout << destroy_myclass3 << m_i << std::endl;
#endif
		logList.push_back(destroy_myclass3);
	}

protected:
	char m_i;

private:
	tstring fun3()
	{
#ifndef DISABLE_CONSOLE_OUTPUT
		std::cout << void_fun_3 << m_i << std::endl;
#endif
		return void_fun_3 + m_i;
	}
};

typedef rdo::Interface<IMy3>     MyInterface;
typedef std::vector<MyInterface> MyInterfaceList;

BOOST_AUTO_TEST_SUITE(RdoInterfaceTest)

BOOST_AUTO_TEST_CASE(checking_on_the_same_operator)
{
	char initValue = '2';
	rdo::UnknownPointer smptr = F(MyClass2)::create(initValue);
	BOOST_REQUIRE(smptr);

	std::list<tstring>::iterator it = logList.begin();
	BOOST_CHECK_EQUAL(*it, create_myclass1 + initValue);
	BOOST_CHECK_EQUAL(*(++it), create_myclass2 + initValue);
	logList.clear();

	BOOST_CHECK_EQUAL(smptr.query_cast<IMy1>()->fun1(), void_fun_1 + initValue);
	BOOST_CHECK_EQUAL(smptr.query_cast<IMy3>()->fun3(), void_fun_3 + initValue);

	MyInterface imy3 = smptr;

	initValue = '7';

	rdo::UnknownPointer smptr2;
	smptr2 = F(MyClass2)::create(initValue);
	it = logList.begin();
	BOOST_CHECK_EQUAL(*it, create_myclass1 + initValue);
	BOOST_CHECK_EQUAL(*(++it), create_myclass2 + initValue);
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

BOOST_AUTO_TEST_CASE(validation_failure_in_the_previous_test_1)
{
	std::list<tstring>::iterator it = logList.begin();
	BOOST_CHECK_EQUAL(*it, destroy_myclass2);
	++it;
	BOOST_CHECK_EQUAL(*it, destroy_myclass1);
	++it;
	BOOST_CHECK_EQUAL(*it, destroy_myclass2);
	++it;
	BOOST_CHECK_EQUAL(*it, destroy_myclass1);
	logList.clear();
}

BOOST_AUTO_TEST_CASE(test_rdo_interface_working)
{
	char initFValue = '2';
	rdo::UnknownPointer smptr = F(MyClass2)::create(initFValue);
	std::list<tstring>::iterator iter = logList.begin();
	BOOST_CHECK_EQUAL(*iter, create_myclass1 + initFValue);
	BOOST_CHECK_EQUAL(*(++iter), create_myclass2 + initFValue);
	logList.clear();

	char initSValue = '5';
	rdo::UnknownPointer smptr2 = F(MyClass2)::create(initSValue);
	iter = logList.begin();
	BOOST_CHECK_EQUAL(*iter, create_myclass1 + initSValue);
	BOOST_CHECK_EQUAL(*(++iter), create_myclass2 + initSValue);
	logList.clear();

	MyInterfaceList myList;

	char initTValue = '9';
	myList.push_back(F(MyClass3)::create(initTValue));
	myList.push_back(smptr);
	myList.push_back(smptr2);
	
	char symbolT = '0';

	MyInterfaceList::const_iterator endIt = myList.end();
	for(MyInterfaceList::iterator it = myList.begin(); it != endIt; ++it)
	{
		rdo::Interface<IMy1> ptr1;
		ptr1 = *it;
		if (ptr1) {
			symbolT = (*it == smptr ? initFValue : (*it == smptr2 ? initSValue : initTValue));
			BOOST_CHECK_EQUAL(ptr1->fun1(), void_fun_1 + symbolT);
		}

		rdo::Interface<IMy2> ptr2 = *it;
		if (ptr2) {
			symbolT = (*it == smptr ? initFValue : (*it == smptr2 ? initSValue : initTValue));
			BOOST_CHECK_EQUAL(ptr2->fun2(), void_fun_2 + symbolT);
		}

		rdo::Interface<IMy3> ptr3 = *it;
		if (ptr3) {
			symbolT = (*it == smptr ? initFValue : (*it == smptr2 ? initSValue : initTValue));
			BOOST_CHECK_EQUAL(ptr3->fun3(), void_fun_3 + symbolT);
		}
	}
	logList.clear();
}

BOOST_AUTO_TEST_CASE(validation_failure_in_the_previous_test_2)
{
	std::list<tstring>::iterator it = logList.begin();
	BOOST_CHECK_EQUAL(*it, destroy_myclass3);
	++it;
	BOOST_CHECK_EQUAL(*it, destroy_myclass2);
	++it;
	BOOST_CHECK_EQUAL(*it, destroy_myclass1);
	++it;
	BOOST_CHECK_EQUAL(*it, destroy_myclass2);
	++it;
	BOOST_CHECK_EQUAL(*it, destroy_myclass1);
	logList.clear();
}

BOOST_AUTO_TEST_SUITE_END()