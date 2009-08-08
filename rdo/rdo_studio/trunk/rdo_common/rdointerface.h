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
	UnknownPointer (REF(IUnknown) smt_ptr);
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

template<class T>
class Interface: public UnknownPointer
{
public:
	typedef Interface<T>   this_type;
	typedef UnknownPointer parent_type;

	Interface()
		: UnknownPointer(NULL, NULL)
	{}
	Interface(PTR(void) nullPointer)
		: UnknownPointer(nullPointer)
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
	CPTR(T) operator-> () const
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

inline UnknownPointer::UnknownPointer(REF(IUnknown) smt_ptr)
	: m_interface(NULL   )
	, m_smt_ptr  (&smt_ptr)
{
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
public:
	static UnknownPointer create()
	{
		return _create(new T());
	}
	template <typename P1>
	static UnknownPointer create(CREF(P1) p1)
	{
		return _create(new T(p1));
	}
	template <typename P1, typename P2>
	static UnknownPointer create(CREF(P1) p1, CREF(P2) p2)
	{
		return _create(new T(p1, p2));
	}
	template <typename P1, typename P2, typename P3>
	static UnknownPointer create(CREF(P1) p1, CREF(P2) p2, CREF(P3) p3)
	{
		return _create(new T(p1, p2, p3));
	}
	template <typename P1, typename P2, typename P3, typename P4>
	static UnknownPointer create(CREF(P1) p1, CREF(P2) p2, CREF(P3) p3, CREF(P4) p4)
	{
		return _create(new T(p1, p2, p3, p4));
	}
	template <typename P1, typename P2, typename P3, typename P4, typename P5>
	static UnknownPointer create(CREF(P1) p1, CREF(P2) p2, CREF(P3) p3, CREF(P4) p4, CREF(P5) p5)
	{
		return _create(new T(p1, p2, p3, p4, p5));
	}
	template <typename P1, typename P2, typename P3, typename P4, typename P5, typename P6>
	static UnknownPointer create(CREF(P1) p1, CREF(P2) p2, CREF(P3) p3, CREF(P4) p4, CREF(P5) p5, CREF(P6) p6)
	{
		return _create(new T(p1, p2, p3, p4, p5, p6));
	}
	template <typename P1, typename P2, typename P3, typename P4, typename P5, typename P6, typename P7>
	static UnknownPointer create(CREF(P1) p1, CREF(P2) p2, CREF(P3) p3, CREF(P4) p4, CREF(P5) p5, CREF(P6) p6, CREF(P7) p7)
	{
		return _create(new T(p1, p2, p3, p4, p5, p6, p7));
	}
	static void destroy(PTR(T) obj)
	{
		delete obj;
	}

private:
	static UnknownPointer _create(PTR(T) object)
	{
		PTR(Counter) counter = new Counter(object);
		object->m_counter = counter;
		return *counter;
	}
	class Counter: public IUnknown
	{
	friend class Factory<T>;
	private:
		ruint  m_counter;
		PTR(T) m_object;

		Counter(PTR(T) object)
			: m_object (object)
			, m_counter(0     )
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
					delete this;
				}
			}
		}
		virtual UnknownPointer QueryInterface(ruint id)
		{
			PTR(void) pInterface = m_object->QueryInterface(id);
			return pInterface ? UnknownPointer(pInterface, this) : UnknownPointer();
		}
	};
};

CLOSE_RDO_NAMESPACE

#define F(A) rdo::Factory<A>

#define DEFINE_THIS_TYPE(A)    typedef A this_type;
#define DEFINE_FACTORY(A)      friend class rdo::Factory<A>;
#define DEFINE_CLASS_NAME(A)   static std::string className() { return #A; }

#define RDO_IOBJECT(A) \
private: \
rdo::LPIUnknown m_counter; \
operator rdo::UnknownPointer()       { return rdo::UnknownPointer(NULL, m_counter); } \
operator rdo::UnknownPointer() const { return rdo::UnknownPointer(NULL, m_counter); } \
protected: \
DEFINE_THIS_TYPE(A) \
DEFINE_FACTORY(A) \
public: \
DEFINE_CLASS_NAME(A)

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

#endif //! _RDOINTERFACE_H_
