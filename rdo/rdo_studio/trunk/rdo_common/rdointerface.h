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
#include "rdosmart_ptr.h"
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
class ___InterfaceRegistrator___DoubleID___##ID {}; \
typedef rdo::Interface<I> LP##I;

#define IID(I) \
Registered##I::ID

OPEN_RDO_NAMESPACE

class IUnknown;
typedef PTR(IUnknown) LPIUnknown;

template<class T> class Interface;

class UnknownPointer
{
public:
	UnknownPointer ();
	UnknownPointer (PTR(void) aInterface, LPIUnknown smt_ptr);
	UnknownPointer (CREF(UnknownPointer) unknowPointer);
	~UnknownPointer();

	operator rbool() const;

	UnknownPointer                  query_cast(ruint id);
	template<class I> Interface<I>  query_cast();

protected:
	PTR(void)   m_interface;

private:
	LPIUnknown  m_smt_ptr;
};

template<class T>
class Interface: public UnknownPointer
{
public:
	Interface(PTR(void) aInterface, LPIUnknown smt_ptr)
		: UnknownPointer(aInterface, smt_ptr)
	{}
	Interface(CREF(Interface<T>) unknowPointer)
		: UnknownPointer(unknowPointer)
	{}
	PTR(T) get()
	{
		return static_cast<PTR(T)>(m_interface);
	}
	CPTR(T) get() const
	{
		return static_cast<PTR(T)>(m_interface);
	}
	PTR(T) operator-> ()
	{
		return get();
	}
};

class IUnknown
{
public:
	virtual void           AddRef () = 0;
	virtual void           Release() = 0;
	virtual UnknownPointer QueryInterface(ruint id) = 0;
};
typedef PTR(IUnknown) LPIUnknown;

inline UnknownPointer::UnknownPointer()
	: m_interface(NULL)
	, m_smt_ptr  (NULL)
{}

inline UnknownPointer::UnknownPointer(PTR(void) aInterface, LPIUnknown smt_ptr)
	: m_interface(aInterface)
	, m_smt_ptr  (smt_ptr   )
{
	if (m_smt_ptr)
		m_smt_ptr->AddRef();
}

inline UnknownPointer::UnknownPointer(CREF(UnknownPointer) unknowPointer)
	: m_interface(unknowPointer.m_interface)
	, m_smt_ptr  (unknowPointer.m_smt_ptr  )
{
	if (m_smt_ptr)
		m_smt_ptr->AddRef();
}

inline UnknownPointer::~UnknownPointer()
{
	if (m_smt_ptr)
		m_smt_ptr->Release();
}

inline UnknownPointer::operator rbool () const
{
	return m_interface != NULL;
}

inline UnknownPointer UnknownPointer::query_cast(ruint id)
{
	return m_smt_ptr ? m_smt_ptr->QueryInterface(id) : UnknownPointer();
}

template<class I>
inline Interface<I> UnknownPointer::query_cast()
{
	UnknownPointer pointer = m_smt_ptr ? m_smt_ptr->QueryInterface(GetInterface<I>::ID) : UnknownPointer();
	return Interface<I>(pointer.m_interface, pointer.m_smt_ptr);
}

template<class T>
class SmartPtr: public smart_ptr<T>, public IUnknown
{
public:
	typedef SmartPtr<T> this_type;

	SmartPtr()
		: smart_ptr()
	{}
	SmartPtr(PTR(T) obj)
		: smart_ptr(obj)
	{}
	SmartPtr(CREF(this_type) object)
		: smart_ptr<T>(object)
	{}
	virtual ~SmartPtr()
	{}

	operator UnknownPointer ()
	{
		return get() ? UnknownPointer(NULL, this) : UnknownPointer();
	}
	template<class I> Interface<I> query_cast()
	{
		if (get() != NULL)
		{
			PTR(void) pInterface = get()->QueryInterface(GetInterface<I>::ID);
			if (pInterface)
				return Interface<I>(pInterface, this);
		}
		return Interface<I>(NULL, NULL);
	}

private:
	virtual void AddRef()
	{
		addref();
	}
	virtual void Release()
	{
		release();
	}
	virtual UnknownPointer QueryInterface(ruint id)
	{
		if (get() != NULL)
		{
			PTR(void) pInterface = get()->QueryInterface(id);
			if (pInterface)
				return UnknownPointer(pInterface, this);
		}
		return UnknownPointer();
	}
};

template <class T>
class IFactory
{
public:
	static SmartPtr<T> create()
	{
		return SmartPtr<T>(new T());
	}
	template <typename P1>
	static SmartPtr<T> create(CREF(P1) p1)
	{
		return SmartPtr<T>(new T(p1));
	}
	template <typename P1, typename P2>
	static SmartPtr<T> create(CREF(P1) p1, CREF(P2) p2)
	{
		return SmartPtr<T>(new T(p1, p2));
	}
	template <typename P1, typename P2, typename P3>
	static SmartPtr<T> create(CREF(P1) p1, CREF(P2) p2, CREF(P2) p3)
	{
		return SmartPtr<T>(new T(p1, p2, p3));
	}
	template <typename P1, typename P2, typename P3, typename P4>
	static SmartPtr<T> create(CREF(P1) p1, CREF(P2) p2, CREF(P2) p3, CREF(P2) p4)
	{
		return SmartPtr<T>(new T(p1, p2, p3, p4));
	}
	template <typename P1, typename P2, typename P3, typename P4, typename P5>
	static SmartPtr<T> create(CREF(P1) p1, CREF(P2) p2, CREF(P2) p3, CREF(P2) p4, CREF(P2) p5)
	{
		return SmartPtr<T>(new T(p1, p2, p3, p4, p5));
	}
	template <typename P1, typename P2, typename P3, typename P4, typename P5, typename P6>
	static SmartPtr<T> create(CREF(P1) p1, CREF(P2) p2, CREF(P2) p3, CREF(P2) p4, CREF(P2) p5, CREF(P2) p6)
	{
		return SmartPtr<T>(new T(p1, p2, p3, p4, p5, p6));
	}
	template <typename P1, typename P2, typename P3, typename P4, typename P5, typename P6, typename P7>
	static SmartPtr<T> create(CREF(P1) p1, CREF(P2) p2, CREF(P2) p3, CREF(P2) p4, CREF(P2) p5, CREF(P2) p6, CREF(P2) p7)
	{
		return SmartPtr<T>(new T(p1, p2, p3, p4, p5, p6, p7));
	}
	static void destroy(PTR(T) obj)
	{
		delete obj;
	}
};

CLOSE_RDO_NAMESPACE

#define DEFINE_THIS_TYPE(A)    typedef A this_type;
#define DEFINE_IPARENT_TYPE(P) typedef P iparent_type;
#define DEFINE_FACTORY(A)      friend class rdo::Factory<A>; friend class rdo::IFactory<A>;
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
PTR(void) QueryInterface(ruint id) \
{ \
	PTR(void) pIterface = iparent_type::QueryInterface(id); \
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
	PTR(void) QueryInterface(ruint id)
	{
		return NULL;
	}
};
CLOSE_RDO_NAMESPACE

#endif //! _RDOINTERFACE_H_
