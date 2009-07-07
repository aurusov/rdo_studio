/*
 * copyright: (c) RDO-Team, 2009
 * filename : rdointerface.h
 * author   : Урусов Андрей
 * date     : 05.07.2009
 * bref     : 
 * indent   : 4T
 */

#ifndef _RDOINTERFACE_H_
#define _RDOINTERFACE_H_

// ====================================================================== INCLUDES
#include <string>
// ====================================================================== SYNOPSIS
#include "namespace.h"
#include "rdomacros.h"
#include "rdotypes.h"
// ===============================================================================

OPEN_RDO_NAMESPACE

class IUnknow;
typedef PTR(IUnknow) LPIUnknow;

template<class T>
class Interface
{
public:
	Interface()
		: m_interface(NULL)
		, m_counter  (NULL)
	{}
	Interface(PTR(T) aInterface, LPIUnknow counter)
		: m_interface(aInterface)
		, m_counter  (counter   )
	{
		m_counter->AddRef();
	}
	~Interface()
	{
		if (m_counter)
			m_counter->Release();
	}
	operator rbool () const
	{
		return m_interface != NULL;
	}
	PTR(T) operator-> ()
	{
		return m_interface;
	}

private:
	PTR(T)     m_interface;
	LPIUnknow  m_counter;
};

class IUnknow
{
public:
	virtual void  AddRef () = 0;
	virtual void  Release() = 0;
	virtual Interface<void> queryInterface(ruint id) = 0;
};
typedef PTR(IUnknow) LPIUnknow;

template<class T>
class LocalInterface
{
public:
	LocalInterface(PTR(T) p)
		: m_pInterface(p)
	{}
	operator rbool () const
	{
		return m_pInterface != NULL;
	}
	PTR(T) get()
	{
		return m_pInterface;
	}
	PTR(T) operator-> ()
	{
		return get();
	}

public:
	PTR(T) m_pInterface;
};

template<class T>
class smart_ptr: public IUnknow
{
public:
	typedef T            object_type;
	typedef smart_ptr<T> this_type;

	smart_ptr(PTR(T) obj)
		: m_object(obj)
	{
		allocateCounter();
		counter() = 1;
	}
	smart_ptr(CREF(this_type) sptr)
		: m_counter(sptr.m_counter)
		, m_object (sptr.m_object )
	{
		AddRef();
	}
	~smart_ptr()
	{
		Release();
		if (counter() == 0)
			deallocateCounter();
	}
	PTR(T) get()
	{
		return m_object;
	}
	operator rbool () const
	{
		return get() != NULL;
	}
	PTR(T) operator-> ()
	{
		return get();
	}
	template<class I> Interface<I> query()
	{
		if (get() != NULL)
		{
			LocalInterface<I> localInterface = get()->query<I>();
			if (localInterface)
				return Interface<I>(localInterface.get(), this);
		}
		return Interface<I>();
	}

private:
	PTR(unsigned int) m_counter;
	PTR(T)            m_object;

	void allocateCounter()
	{
		m_counter = new unsigned int;
	}
	void deallocateCounter()
	{
		delete m_counter;
	}
	REF(unsigned int) counter()
	{
		return *m_counter;
	}
	virtual void AddRef()
	{
		counter()++;
	}
	virtual void Release()
	{
		counter()--;
		if (counter() == 0)
		{
			if (m_object)
			{
				Factory<T>::destroy(m_object);
				m_object = NULL;
			}
		}
	}
	virtual Interface<void> queryInterface(ruint id)
	{
		if (get() != NULL)
		{
			LocalInterface<void> localInterface = get()->queryInterface(id);
			if (localInterface)
				return Interface<void>(localInterface.get(), this);
		}
		return Interface<void>();
	}
};

template <class T>
class Factory
{
public:
	static smart_ptr<T> create()
	{
		return smart_ptr<T>(new T());
	}
	template <typename P1>
	static smart_ptr<T> create(P1 p1)
	{
		return smart_ptr<T>(new T(p1));
	}
	static void destroy(T* obj)
	{
		delete obj;
	}
};

CLOSE_RDO_NAMESPACE

#define QUERY_INTERFACE_NULL         template<class T> rdo::LocalInterface<T> query() { return NULL;                  }
#define QUERY_INTERFACE_PARENT(A)    template<class T> rdo::LocalInterface<T> query() { return A::query<T>();         }
#define QUERY_INTERFACE_PARENT2(A,B) template<class T> rdo::LocalInterface<T> query() { rdo::LocalInterface<T> lpintA = A::query<T>(); return lpintA ? lpintA : B::query<T>(); }
#define QUERY_INTERFACE(A)           template<>        rdo::LocalInterface<A> query() { return static_cast<A*>(this); }

#define DEFINE_THIS_TYPE(A)  typedef A this_type;
#define DEFINE_FACTORY(A)    friend class rdo::Factory<A>;
#define DEFINE_CLASS_NAME(A) static std::string className() { return #A; }

#define RDO_IOBJECT(A, BASE) \
protected: \
DEFINE_THIS_TYPE(A) \
DEFINE_FACTORY(A) \
public: \
DEFINE_CLASS_NAME(A) \
QUERY_INTERFACE_PARENT(BASE)

OPEN_RDO_NAMESPACE
class IObjectBase
{
public:
	QUERY_INTERFACE_NULL;
};
CLOSE_RDO_NAMESPACE

#endif //! _RDOINTERFACE_H_
