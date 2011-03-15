
#include <iostream>
#include <list>

class A
{
public:
	void fun()
	{
		std::cout << "A::fun()" << std::endl;
	}
};

class B
{
public:
	int fun2(double d, int i)
	{
		std::cout << "B::fun1(" << d << ", " << i << ")" << std::endl;
		return i + 1;
	}
};

class ICaller
{
public:
	virtual void call() = 0;
};
typedef ICaller* LPICaller;

typedef void (A::*AFun)();
typedef int  (B::*BFun)(double, int);

template <class T, class F>
class CallerAdapterBase
{
public:
	CallerAdapterBase(T* pObject, F pFunction)
		: m_pObject  (pObject  )
		, m_pFunction(pFunction)
	{}

protected:
	T* m_pObject;
	F  m_pFunction;
};

template <class T, class F>
class CallerAdapter0: public CallerAdapterBase<T, F>, public ICaller
{
public:
	CallerAdapter0(T* pObject, F pFunction)
		: CallerAdapterBase(pObject, pFunction)
	{}

private:
	void call()
	{
		(m_pObject->*m_pFunction)();
	}
};

template <class T, class F, class P1, class P2>
class CallerAdapter2: public CallerAdapterBase<T, F>, public ICaller
{
public:
	CallerAdapter2(T* pObject, F pFunction, const P1& p1, const P2& p2)
		: CallerAdapterBase(pObject, pFunction)
		, m_p1(p1)
		, m_p2(p2)
	{}

private:
	const P1& m_p1;
	const P2& m_p2;

	void call()
	{
		(m_pObject->*m_pFunction)(m_p1, m_p2);
	}
};

void main()
{
	A a;
	CallerAdapter0<A, AFun> adapter(&a, &A::fun);
	B b;
	CallerAdapter2<B, BFun, double, int> adapter2(&b, &B::fun2, 10, 20);

	typedef std::list<LPICaller> CallerList;
	CallerList callerList;
	callerList.push_back(&adapter);
	callerList.push_back(&adapter2);

	CallerList::const_iterator it = callerList.begin();
	while (it != callerList.end())
	{
		(*it)->call();
		++it;
	}

	int i = 1;
}
