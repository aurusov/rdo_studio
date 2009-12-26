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
#include "rdodebug.h"
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

#define INTERFACE_PREDECLARATION(I) \
typedef rdo::Interface<I> LP##I;

#define IID(I) \
Registered##I::ID

OPEN_RDO_NAMESPACE

class IUnknown;
typedef PTR(IUnknown) LPIUnknown;
class IGetUnknown;
typedef PTR(IGetUnknown) LPIGetUnknown;

template<class T> class Interface;

class UnknownPointer
{
public:
	UnknownPointer ();
	UnknownPointer (REF(IUnknown) smt_ptr    );
	UnknownPointer (LPIGetUnknown get_smt_ptr);
	UnknownPointer (PTR(void) aInterface, LPIUnknown smt_ptr);
	UnknownPointer (CREF(UnknownPointer) unknowPointer);
	~UnknownPointer();

	rbool               operator== (CREF(UnknownPointer) pointer) const;
	REF(UnknownPointer) operator=  (CREF(UnknownPointer) pointer);

	operator rbool() const;

	UnknownPointer                  query_cast  (ruint id);
	template<class I> Interface<I>  query_cast  ();
	template<class I> operator      Interface<I>();

	UnknownPointer                  query_cast  (ruint id) const;
	template<class I> Interface<I>  query_cast  () const;
	template<class I> operator      Interface<I>() const;

protected:
	PTR(void)   m_interface;

private:
	LPIUnknown  m_smt_ptr;
};

template<class I>
class Interface: public UnknownPointer
{
public:
	typedef Interface<I>   this_type;
	typedef UnknownPointer parent_type;

	Interface()
		: UnknownPointer(NULL, NULL)
	{}
	Interface(LPIGetUnknown get_smt_ptr)
		: UnknownPointer(get_smt_ptr ? get_smt_ptr->GetUnknown()->QueryInterface(GetInterface<I>::ID) : UnknownPointer())
	{}
	Interface(PTR(void) aInterface, LPIUnknown smt_ptr)
		: UnknownPointer(aInterface, smt_ptr)
	{}
	Interface(CREF(this_type) pInterface)
		: UnknownPointer(pInterface)
	{}
	REF(this_type) operator= (CREF(this_type) pointer)
	{
		parent_type::operator= (pointer);
		return *this;
	}
	operator rbool() const
	{
		return m_interface != NULL;
	}

	PTR(I) get()
	{
		return static_cast<PTR(I)>(m_interface);
	}
	CPTR(I) get() const
	{
		return static_cast<PTR(I)>(m_interface);
	}
	PTR(I) operator-> ()
	{
		return get();
	}
	CPTR(I) operator-> () const
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

class IGetUnknown
{
public:
	virtual LPIUnknown GetUnknown() = 0;
};
typedef PTR(IGetUnknown) LPIGetUnknown;

inline UnknownPointer::UnknownPointer()
	: m_interface(NULL)
	, m_smt_ptr  (NULL)
{}

inline UnknownPointer::UnknownPointer(REF(IUnknown) smt_ptr)
	: m_interface(NULL   )
	, m_smt_ptr  (&smt_ptr)
{
	if (m_smt_ptr)
		m_smt_ptr->AddRef();
}

inline UnknownPointer::UnknownPointer(LPIGetUnknown get_smt_ptr)
	: m_interface(NULL)
{
	m_smt_ptr = get_smt_ptr ? get_smt_ptr->GetUnknown() : NULL;
	if (m_smt_ptr)
		m_smt_ptr->AddRef();
}

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

inline rbool UnknownPointer::operator== (CREF(UnknownPointer) pointer) const
{
	return m_smt_ptr == pointer.m_smt_ptr;
}

inline REF(UnknownPointer) UnknownPointer::operator= (CREF(UnknownPointer) pointer)
{
	if (m_smt_ptr)
		m_smt_ptr->Release();

	m_interface = pointer.m_interface;
	m_smt_ptr   = pointer.m_smt_ptr;
	if (m_smt_ptr)
		m_smt_ptr->AddRef();

	return *this;
}

inline UnknownPointer::operator rbool () const
{
	return m_smt_ptr != NULL;
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

template<class I>
inline UnknownPointer::operator Interface<I> ()
{
	return query_cast<I>();
}

inline UnknownPointer UnknownPointer::query_cast(ruint id) const
{
	return m_smt_ptr ? m_smt_ptr->QueryInterface(id) : UnknownPointer();
}

template<class I>
inline Interface<I> UnknownPointer::query_cast() const
{
	UnknownPointer pointer = m_smt_ptr ? m_smt_ptr->QueryInterface(GetInterface<I>::ID) : UnknownPointer();
	return Interface<I>(pointer.m_interface, pointer.m_smt_ptr);
}

template<class I>
inline UnknownPointer::operator Interface<I> () const
{
	return query_cast<I>();
}

template <class T>
class Factory
{
private:
	class Counter: public IUnknown
	{
	friend class Factory<T>;
	friend class Factory<T>::Object;
	private:
		ruint  m_counter;
		PTR(T) m_object;

		Counter()
			: m_counter(0)
		{}
		operator UnknownPointer()
		{
			return UnknownPointer(NULL, this);
		}

		virtual void AddRef()
		{
			m_counter++;
		}
		virtual void Release()
		{
			m_counter--;
			if (m_counter == 0)
			{
				if (m_object)
				{
					Factory<T>::destroy(m_object);
				}
			}
		}
		virtual UnknownPointer QueryInterface(ruint id)
		{
			PTR(void) pInterface = m_object->QueryInterface(id);
			return pInterface ? UnknownPointer(pInterface, this) : UnknownPointer();
		}
	};

public:
	class Object: public T
	{
	public:
		Object()
			: T()
		{
			create();
		}

		template <typename P1>
		Object(CREF(P1) p1)
			: T(p1)
		{
			create();
		}

		template <typename P1, typename P2>
		Object(CREF(P1) p1, CREF(P2) p2)
			: T(p1, p2)
		{
			create();
		}

		template <typename P1, typename P2, typename P3>
		Object(CREF(P1) p1, CREF(P2) p2, CREF(P3) p3)
			: T(p1, p2, p3)
		{
			create();
		}

		template <typename P1, typename P2, typename P3, typename P4>
		Object(CREF(P1) p1, CREF(P2) p2, CREF(P3) p3, CREF(P4) p4)
			: T(p1, p2, p3, p4)
		{
			create();
		}

		template <typename P1, typename P2, typename P3, typename P4, typename P5>
		Object(CREF(P1) p1, CREF(P2) p2, CREF(P3) p3, CREF(P4) p4, CREF(P5) p5)
			: T(p1, p2, p3, p4, p5)
		{
			create();
		}

		template <typename P1, typename P2, typename P3, typename P4, typename P5, typename P6>
		Object(CREF(P1) p1, CREF(P2) p2, CREF(P3) p3, CREF(P4) p4, CREF(P5) p5, CREF(P6) p6)
			: T(p1, p2, p3, p4, p5, p6)
		{
			create();
		}

		template <typename P1, typename P2, typename P3, typename P4, typename P5, typename P6, typename P7>
		Object(CREF(P1) p1, CREF(P2) p2, CREF(P3) p3, CREF(P4) p4, CREF(P5) p5, CREF(P6) p6, CREF(P7) p7)
			: T(p1, p2, p3, p4, p5, p6, p7)
		{
			create();
		}

		virtual ~Object()
		{}

		virtual LPIUnknown GetUnknown()
		{
			return &m_counter;
		}
	private:
		void create()
		{
			m_counter.m_object = this;
		}
		Counter m_counter;
	};

	static UnknownPointer create()
	{
		return init(new Object());
	}
	template <typename P1>
	static UnknownPointer create(CREF(P1) p1)
	{
		return init(new Object(p1));
	}
	template <typename P1, typename P2>
	static UnknownPointer create(CREF(P1) p1, CREF(P2) p2)
	{
		return init(new Object(p1, p2));
	}
	template <typename P1, typename P2, typename P3>
	static UnknownPointer create(CREF(P1) p1, CREF(P2) p2, CREF(P3) p3)
	{
		return init(new Object(p1, p2, p3));
	}
	template <typename P1, typename P2, typename P3, typename P4>
	static UnknownPointer create(CREF(P1) p1, CREF(P2) p2, CREF(P3) p3, CREF(P4) p4)
	{
		return init(new Object(p1, p2, p3, p4));
	}
	template <typename P1, typename P2, typename P3, typename P4, typename P5>
	static UnknownPointer create(CREF(P1) p1, CREF(P2) p2, CREF(P3) p3, CREF(P4) p4, CREF(P5) p5)
	{
		return init(new Object(p1, p2, p3, p4, p5));
	}
	template <typename P1, typename P2, typename P3, typename P4, typename P5, typename P6>
	static UnknownPointer create(CREF(P1) p1, CREF(P2) p2, CREF(P3) p3, CREF(P4) p4, CREF(P5) p5, CREF(P6) p6)
	{
		return init(new Object(p1, p2, p3, p4, p5, p6));
	}
	template <typename P1, typename P2, typename P3, typename P4, typename P5, typename P6, typename P7>
	static UnknownPointer create(CREF(P1) p1, CREF(P2) p2, CREF(P3) p3, CREF(P4) p4, CREF(P5) p5, CREF(P6) p6, CREF(P7) p7)
	{
		return init(new Object(p1, p2, p3, p4, p5, p6, p7));
	}
	static void destroy(PTR(T) object)
	{
		delete object;
	}

private:
	static UnknownPointer init(PTR(Object) object)
	{
		UnknownPointer uPointer(object ? *object->GetUnknown() : UnknownPointer());
		LPIInit iInit = uPointer;
		if (iInit)
		{
			if (!iInit->init())
			{
				iInit    = 0;
				uPointer = 0;
				return UnknownPointer();
			}
		}
		return uPointer;
	}
};

CLOSE_RDO_NAMESPACE

#define  CAST_TO_UNKNOWN  public rdo::IGetUnknown

#define F(A) rdo::Factory<A>

#define DEFINE_FACTORY(A)      friend class rdo::Factory<A>; friend class rdo::Factory<A>::Object;

#define QUERY_INTERFACE_BEGIN \
public: \
PTR(void) QueryInterface(ruint id) \
{

#define QUERY_INTERFACE_PARENT(A) \
	PTR(void) pIterface##A = A::QueryInterface(id); \
	if (pIterface##A) return pIterface##A;

#define QUERY_INTERFACE(A) \
	if (id == IID(A)) return static_cast<PTR(A)>(this);

#define QUERY_INTERFACE_END \
	return NULL; \
}

class IInit
{
public:
	virtual rbool init() = 0;
};
#define DECLARE_IInit \
	virtual rbool init();

INTERFACE_REGISTRATOR(IInit, 0);

#endif //! _RDOINTERFACE_H_
