
#include <iostream>
#include <list>

#ifdef WIN32
#    pragma warning ( push )
#    pragma warning ( disable : 4244 ) // Boost 1.35 pre release - warning C4244: '=' : conversion from '__w64 int' to 'unsigned int', possible loss of data
#    pragma warning ( disable : 4267 ) // Boost 1.35 pre release - warning C4267: 'return' : conversion from 'size_t' to 'int', possible loss of data
#    pragma warning ( disable : 4311 ) // Boost 1.35 pre release - warning C4311: 'type cast' : pointer truncation from 'void *const ' to 'long'
#    pragma warning ( disable : 4312 ) // Boost 1.35 pre release - warning C4312: 'type cast' : conversion from 'long' to 'void *' of greater size
#endif
#include <boost/thread/thread.hpp>
#include <boost/thread/condition.hpp>
#ifdef WIN32
#    pragma warning ( pop )
#endif

class ICaller
{
public:
	virtual void call() = 0;
};
typedef ICaller* LPICaller;

class Apartment: boost::thread
{
public:
	Apartment()
		: boost::thread(&Apartment::qqq1, this)
	{}

	void start()
	{
		m_pThread = new boost::thread(&Apartment::qqq2, this);
	}

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
	CallerList     m_callerList;
	boost::thread* m_pThread;

	void qqq1()
	{
		int i = 1;
	}
	void qqq2()
	{
		int i = 1;
	}
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

	apartment.start();

	ApartmentPointer<A> pA(&a);
	ApartmentPointer<B> pB(&b);
	pA.dispatch(&A::fun);
	pB.dispatch(&B::fun2, 10, 20);

	apartment.internal_call();

	int i = 1;
}
