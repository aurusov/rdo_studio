#include "../../rdocommon.h"
#include "../../rdointerface.h"
#include <iostream>

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
	typedef rdo::smart_ptr<MyClass2> SPMyClass;
	std::cout << SPMyClass::object_type::className() << std::endl;
	std::vector<SPMyClass> list;
	list.push_back(rdo::Factory<MyClass2>::create(1));
	list.push_back(rdo::Factory<MyClass2>::create(3));
	list.push_back(rdo::Factory<MyClass2>::create(2));

	rdo::Interface<IMy2> lpIMy2 = list.back().query<IMy2>();
	if (lpIMy2)
		lpIMy2->fun2();
}
