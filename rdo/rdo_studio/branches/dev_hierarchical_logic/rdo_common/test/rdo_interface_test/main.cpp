// ====================================================================== INCLUDES
#include <vector>
#include <iostream>
// ====================================================================== SYNOPSIS
#include "../../rdointerface.h"
// ===============================================================================

class IMy1
{
public:
	virtual void fun1() = 0;
};

class IMy2
{
public:
	virtual void fun2() = 0;
};

class IMy3
{
public:
	virtual void fun3() = 0;
};

class IMy4
{
public:
	virtual void fun4() = 0;
};

INTERFACE_REGISTRATOR(IMy1, 1);
INTERFACE_REGISTRATOR(IMy2, 2);
INTERFACE_REGISTRATOR(IMy3, 3);
INTERFACE_REGISTRATOR(IMy4, 4);

class MyClass: public rdo::IGetUnknown, public IMy1, public IMy2, public IInit
{
QUERY_INTERFACE_BEGIN
	QUERY_INTERFACE(IMy1)
	QUERY_INTERFACE(IMy2)
	QUERY_INTERFACE(IInit)
QUERY_INTERFACE_END

protected:
	MyClass(int i)
		: m_i(i)
	{
		std::cout << "MyClass(int " << i << ");" << std::endl;
	}
	~MyClass()
	{
		std::cout << "~MyClass(): " << m_i << std::endl;
	}
	rbool init()
	{
		return true;
	}

protected:
	int m_i;

private:
	void fun1()
	{
		std::cout << "void fun1(): " << m_i << std::endl;
	}
	void fun2()
	{
		std::cout << "void fun2(): " << m_i << std::endl;
	}
};

void fun(CREF(LPIMy1) my1)
{
	ASSERT(my1);
}

class MyClass2: public MyClass, public IMy3
{
DEFINE_FACTORY(MyClass2);
QUERY_INTERFACE_BEGIN
	QUERY_INTERFACE_PARENT(MyClass)
	QUERY_INTERFACE(IMy3)
QUERY_INTERFACE_END

private:
	MyClass2(int i): MyClass(i)
	{
		std::cout << "MyClass2(int " << i << ");" << std::endl;
	}
	~MyClass2()
	{
		std::cout << "~MyClass2()" << std::endl;
	}
	void fun3()
	{
		fun(this);

		LPIMy1 int1 = this;
		ASSERT(int1)
		std::cout << "void fun3(): " << m_i << std::endl;
	}
};

class MyClass3: public IMy3, public rdo::IGetUnknown
{
DEFINE_FACTORY(MyClass3);
QUERY_INTERFACE_BEGIN
	QUERY_INTERFACE(IMy3)
QUERY_INTERFACE_END

protected:
	MyClass3(int i)
		: m_i(i)
	{
		std::cout << "MyClass3(int " << i << ");" << std::endl;
	}
	~MyClass3()
	{
		std::cout << "~MyClass3(): " << m_i << std::endl;
	}

protected:
	int m_i;

private:
	void fun3()
	{
		std::cout << "void fun3(): " << m_i << std::endl;
	}
};

void main()
{
	typedef rdo::Interface<IMy3>     MyInterface;
	typedef std::vector<MyInterface> MyInterfaceList;
	MyInterfaceList list;
	rdo::UnknownPointer smptr = F(MyClass2)::create(1);
	ASSERT(smptr);
	smptr.query_cast<IMy1>()->fun1();
	smptr.query_cast<IMy3>()->fun3();
	MyInterface imy3 = smptr;
	rdo::UnknownPointer smptr2;
	smptr2 = F(MyClass2)::create(2);
	//! Проверка на operation==
	{
		rdo::UnknownPointer smptr2_2 = smptr2;
		if (smptr2_2 == smptr)
			std::cout << "smptr2_2 == smptr" << std::endl;
		if (smptr2_2 == smptr2)
			std::cout << "smptr2_2 == smptr2" << std::endl;
		rdo::Interface<IMy1> int1_1 = smptr;
		rdo::Interface<IMy1> int2_1 = smptr2;
		rdo::Interface<IMy1> int1_2 = smptr;
		if (int1_1 == int2_1)
			std::cout << "int1_1 == int2_1" << std::endl;
		if (int1_1 == int1_2)
			std::cout << "int1_1 == int1_2" << std::endl;
	}

	list.push_back(F(MyClass3)::create(3));
	list.push_back(smptr );
	list.push_back(smptr2);
	MyInterfaceList::iterator it = list.begin();
	while (it != list.end())
	{
		rdo::Interface<IMy1> ptr1;
		ptr1 = *it;
		if (ptr1)
			ptr1->fun1();

		rdo::Interface<IMy2> ptr2 = *it;
		if (ptr2)
			ptr2->fun2();

		rdo::Interface<IMy3> ptr3 = *it;
		if (ptr3)
			ptr3->fun3();

		it++;
	}
	int i = 1;
}
