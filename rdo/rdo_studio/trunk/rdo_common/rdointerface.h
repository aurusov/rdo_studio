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
template <class T, ruint id>
class InterfaceRegistrator
{
public:
	typedef T Type;
	enum { ID = id };
};

template <class T> class GetInterface {};
CLOSE_RDO_NAMESPACE

#define INTERFACE_REGISTRATOR(I, ID) \
class I; \
typedef rdo::InterfaceRegistrator<I, ID> Registered##I; \
template <> class rdo::GetInterface<I>: public Registered##I {}; \
class ___InterfaceRegistrator___DoubleID___##ID {};

#define IID(I) \
Registered##I::ID

OPEN_RDO_NAMESPACE

class IUnknow;
typedef PTR(IUnknow) LPIUnknow;

template<class T> class Interface;

class UnknowPointer
{
public:
	UnknowPointer ();
	UnknowPointer (PTR(void) aInterface, LPIUnknow smt_ptr);
	UnknowPointer (CREF(UnknowPointer) unknowPointer);
	~UnknowPointer();

	operator rbool() const;

	UnknowPointer                   queryInterface(ruint id);
	template<class I> Interface<I>  queryInterface();

protected:
	PTR(void)  m_interface;

private:
	LPIUnknow  m_smt_ptr;
};

template<class T>
class Interface: public UnknowPointer
{
public:
	Interface(PTR(void) aInterface, LPIUnknow smt_ptr)
		: UnknowPointer(aInterface, smt_ptr)
	{}
	Interface(CREF(Interface<T>) unknowPointer)
		: UnknowPointer(unknowPointer)
	{}
	PTR(T) operator-> ()
	{
		return static_cast<PTR(T)>(m_interface);
	}
};

class IUnknow
{
public:
	virtual void          AddRef () = 0;
	virtual void          Release() = 0;
	virtual UnknowPointer queryInterface(ruint id) = 0;
};
typedef PTR(IUnknow) LPIUnknow;

inline UnknowPointer::UnknowPointer()
	: m_interface(NULL)
	, m_smt_ptr  (NULL)
{}

inline UnknowPointer::UnknowPointer(PTR(void) aInterface, LPIUnknow smt_ptr)
	: m_interface(aInterface)
	, m_smt_ptr  (smt_ptr   )
{
	m_smt_ptr->AddRef();
}

inline UnknowPointer::UnknowPointer(CREF(UnknowPointer) unknowPointer)
	: m_interface(unknowPointer.m_interface)
	, m_smt_ptr  (unknowPointer.m_smt_ptr  )
{
	m_smt_ptr->AddRef();
}

inline UnknowPointer::~UnknowPointer()
{
	if (m_smt_ptr)
		m_smt_ptr->Release();
}

inline UnknowPointer::operator rbool () const
{
	return m_interface != NULL;
}

inline UnknowPointer UnknowPointer::queryInterface(ruint id)
{
	return m_smt_ptr ? m_smt_ptr->queryInterface(id) : UnknowPointer();
}

template<class I>
inline Interface<I> UnknowPointer::queryInterface()
{
	UnknowPointer pointer = m_smt_ptr ? m_smt_ptr->queryInterface(GetInterface<I>::ID) : UnknowPointer();
	return Interface<I>(pointer.m_interface, pointer.m_smt_ptr);
}

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
	operator UnknowPointer ()
	{
		return get() ? UnknowPointer(NULL, this) : UnknowPointer();
	}
	template<class I> Interface<I> queryInterface()
	{
		if (get() != NULL)
		{
			PTR(void) pInterface = get()->queryInterface(GetInterface<I>::ID);
			if (pInterface)
				return Interface<I>(pInterface, this);
		}
		return Interface<I>(NULL, NULL);
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
	virtual UnknowPointer queryInterface(ruint id)
	{
		if (get() != NULL)
		{
			PTR(void) pInterface = get()->queryInterface(id);
			if (pInterface)
				return UnknowPointer(pInterface, this);
		}
		return UnknowPointer();
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
	static void destroy(PTR(T) obj)
	{
		delete obj;
	}
};

CLOSE_RDO_NAMESPACE

#define DEFINE_THIS_TYPE(A)    typedef A this_type;
#define DEFINE_IPARENT_TYPE(P) typedef P iparent_type;
#define DEFINE_FACTORY(A)      friend class rdo::Factory<A>;
#define DEFINE_CLASS_NAME(A)   static std::string className() { return #A; }

#define RDO_IOBJECT(A, BASE) \
protected: \
DEFINE_THIS_TYPE(A) \
DEFINE_FACTORY(A) \
DEFINE_IPARENT_TYPE(BASE) \
public: \
DEFINE_CLASS_NAME(A)

#define QUERY_INTERFACE_BEGIN \
public: \
PTR(void) queryInterface(ruint id) \
{ \
	PTR(void) pIterface = iparent_type::queryInterface(id); \
	if (pIterface) \
		return pIterface; \
 \
	switch (id) \
	{

#define QUERY_INTERFACE(A)        case IID(A): return static_cast<PTR(A)>(this);
#define QUERY_INTERFACE_END \
	} \
	return NULL; \
}

OPEN_RDO_NAMESPACE
class IObjectBase
{
public:
	PTR(void) queryInterface(ruint id)
	{
		return NULL;
	}
};
CLOSE_RDO_NAMESPACE

#endif //! _RDOINTERFACE_H_
