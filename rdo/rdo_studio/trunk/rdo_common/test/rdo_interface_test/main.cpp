#include "../../rdocommon.h"
#include "../../rdointerface.h"
#include <iostream>

class IMy1
{
public:
	enum {EIMy1};
	virtual void fun1() = 0;
};

class IMy2
{
public:
	enum {EIMy2};
	virtual void fun2() = 0;
};

class IMy3
{
public:
	enum {EIMy3};
	virtual void fun3() = 0;
};

class IMy4
{
public:
	enum {EIMy4};
	virtual void fun4() = 0;
};

class MyClass: public rdo::IObjectBase, public IMy1, public IMy2
{
RDO_IOBJECT(MyClass, IObjectBase);

public:
	QUERY_INTERFACE(IMy1);
	QUERY_INTERFACE(IMy2);

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

public:
	QUERY_INTERFACE(IMy3);

	rdo::LocalInterface<void> queryInterface(ruint id)
	{
		switch (id)
		{
		case 3: return rdo::LocalInterface<void>(static_cast<IMy3*>(this));
		}
		return NULL;
	}

private:
	MyClass2(int i)
		: MyClass(i)
	{}
	void fun3()
	{
		std::cout << "void fun3()" << std::endl;
	}
};

void main()
{
	std::vector< rdo::LPIUnknow > list;
	rdo::smart_ptr<MyClass2> smptr = rdo::Factory<MyClass2>::create(1);
	list.push_back(&smptr);
	rdo::Interface<IMy3> ptr = list.back()->queryInterface(3);
	int i = 1;

//	rdo::Interface<IMy2> lpIMy2 = list.back().query<IMy2>();
//	if (lpIMy2)
//		lpIMy2->fun2();
}
