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

class MyClass: public rdo::IObjectBase, public IMy1, public IMy2
{
RDO_IOBJECT(MyClass, IObjectBase);

QUERY_INTERFACE_BEGIN
	QUERY_INTERFACE(IMy1)
	QUERY_INTERFACE(IMy2)
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

private:
	int m_i;

	void unknow()
	{
		std::cout << "void unknow()" << std::endl;
	}
	void fun1()
	{
		std::cout << "void fun1()" << std::endl;
	}
	void fun2()
	{
		std::cout << "void fun2()" << std::endl;
	}
};

class MyClass2: public MyClass, public IMy3
{
RDO_IOBJECT(MyClass2, MyClass);

QUERY_INTERFACE_BEGIN
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
		std::cout << "void fun3()" << std::endl;
	}
};

void main()
{
	std::vector<rdo::UnknownPointer> list;
	rdo::SmartPtr<MyClass2> smptr = rdo::IFactory<MyClass2>::create(1);
	rdo::SmartPtr<MyClass2> smptr2;
	list.push_back(smptr);
	rdo::Interface<IMy1> ptr = list.back().query_cast<IMy1>();
	if (ptr)
	{
		ptr->fun1();
		rdo::Interface<IMy2> ptr2 = ptr.query_cast<IMy2>();
		if (ptr2)
			ptr2->fun2();
	}
	int i = 1;
}
