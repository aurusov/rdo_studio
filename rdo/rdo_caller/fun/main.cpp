
#include <iostream>
#include <list>

class ICaller
{
public:
	virtual void call() = 0;
};
typedef ICaller* LPICaller;

class Apartment
{
public:
	Apartment()
	{}

	void call(LPICaller pCall)
	{
		m_callerList.push_back(pCall);
	}

	void internal_call()
	{
		CallerList::const_iterator it = m_callerList.begin();
		while (it != m_callerList.end())
		{
			(*it)->call();
			delete *it;
			++it;
		}
	}

private:
	typedef std::list<LPICaller> CallerList;
	CallerList m_callerList;
};

template <class T>
class ApartmentPointer
{
public:
	ApartmentPointer(T* pObject)
		: m_pObject(pObject)
	{
		m_pApartment = m_pObject->getApartment();
	}

	template <class F>
	void dispatch(F pFunction)
	{
		CallerAdapter0<T, F>* pAdapter = new CallerAdapter0<T, F>(m_pObject, pFunction);
		m_pApartment->call(pAdapter);
	}

	template <class F, class P1, class P2>
	void dispatch(F pFunction, const P1& p1, const P2& p2)
	{
		CallerAdapter2<T, F, P1, P2>* pAdapter = new CallerAdapter2<T, F, P1, P2>(m_pObject, pFunction, p1, p2);
		m_pApartment->call(pAdapter);
	}

private:
	T*         m_pObject;
	Apartment* m_pApartment;
};

class ApartmentObject
{
public:
	ApartmentObject()
		: m_pApartment(NULL)
	{}

	void setApartment(Apartment* pApartment)
	{
		m_pApartment = pApartment;
	}

	Apartment* getApartment() const
	{
		return m_pApartment;
	}

private:
	Apartment* m_pApartment;
};

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

class A: public ApartmentObject
{
public:
	void fun()
	{
		std::cout << "A::fun()" << std::endl;
	}
};

class B: public ApartmentObject
{
public:
	int fun2(double d, int i)
	{
		std::cout << "B::fun1(" << d << ", " << i << ")" << std::endl;
		return i + 1;
	}
};

void main()
{
	Apartment apartment;
	A a;
	B b;
	a.setApartment(&apartment);
	b.setApartment(&apartment);

	ApartmentPointer<A> pA(&a);
	ApartmentPointer<B> pB(&b);
	pA.dispatch(&A::fun);
	pB.dispatch(&B::fun2, 10, 20);

	apartment.internal_call();

	int i = 1;
}
