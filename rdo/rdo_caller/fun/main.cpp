
#include <iostream>
#include <list>
#include <boost/thread/mutex.hpp>
#include <boost/thread/condition.hpp>
#include <boost/interprocess/sync/interprocess_condition.hpp>

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

class MutexMonicker
{
public:
	MutexMonicker(const boost::mutex& mutex)
		: m_mutex(mutex)
	{
		const_cast<boost::mutex&>(m_mutex).lock();
	}
	~MutexMonicker()
	{
		const_cast<boost::mutex&>(m_mutex).unlock();
	}

private:
	const boost::mutex& m_mutex;
};
#define MUTEX_MONICKER(NAME) MutexMonicker __##NAME##_MutexMonicker(NAME)

class Apartment
{
public:
	Apartment()
		: m_stoped(false)
	{
		m_pThread = new boost::thread(&Apartment::main, this);
	}

	void stop()
	{
		m_stoped = true;
		m_signal.notify_all();
	}

	bool empty() const
	{MUTEX_MONICKER(m_callerListMutext);
		return m_callerList.empty();
	}

	void call(LPICaller pCall)
	{
		{MUTEX_MONICKER(m_callerListMutext);
			m_callerList.push_back(pCall);
		}
		m_signal.notify_all();
	}

private:
	typedef std::list<LPICaller> CallerList;
	CallerList       m_callerList;
	boost::mutex     m_callerListMutext;
	boost::condition m_signal;
	boost::mutex     m_signalMutex;
	boost::thread*   m_pThread;
	volatile bool    m_stoped;

	void main()
	{
		std::cout << "thread start" << std::endl;

		while (true)
		{
			{
				boost::interprocess::scoped_lock<boost::mutex> monicker(m_signalMutex);
				m_signal.wait(monicker);
			}

			while (true)
			{
				LPICaller pCaller = NULL;
				{MUTEX_MONICKER(m_callerListMutext);

					if (!m_callerList.empty())
					{
						CallerList::iterator it = m_callerList.begin();
						pCaller = *it;
						m_callerList.erase(it);
					}
				}

				if (pCaller)
				{
					pCaller->call();
					delete pCaller;
				}
				else
				{
					break;
				}
			}

			if (m_stoped)
			{
				break;
			}
		}

		std::cout << "thread stop" << std::endl;
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

	ApartmentPointer<A> pA(&a);
	ApartmentPointer<B> pB(&b);
	for (unsigned int i = 0; i < 100; ++i)
	{
		pA.dispatch(&A::fun);
		pB.dispatch(&B::fun2, 10, 20);
	}

	while (!apartment.empty())
	{}

	apartment.stop();

	boost::xtime delay;
	boost::xtime_get(&delay, boost::TIME_UTC);
	delay.sec += 1;
	boost::thread::sleep(delay);
	int i = 1;
}
