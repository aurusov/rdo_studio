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
// ====================================================================== SYNOPSIS
#include "rdo_common/namespace.h"
#include "rdo_common/rdodebug.h"
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

#define INTERFACE_REGISTRATOR(I, ID)                             \
class I;                                                         \
typedef rdo::InterfaceRegistrator<I, ID> Registered##I;          \
template <> class rdo::GetInterface<I>: public Registered##I {}; \
class ___InterfaceRegistrator___DoubleID___##ID {};              \
typedef rdo::Interface<I> LP##I;

#define INTERFACE_PREDECLARATION(I) \
typedef rdo::Interface<I> LP##I;

#define IID(I)    \
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
	UnknownPointer (REF(IUnknown) unknown    );
	UnknownPointer (LPIGetUnknown pGetUnknown);
	UnknownPointer (PTR(void) pInterface, LPIUnknown pUnknown);
	UnknownPointer (CREF(UnknownPointer) pointer);
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
	PTR(void)   m_pInterface;

private:
	LPIUnknown  m_pUnknown;
};

template<class I>
class Interface: public UnknownPointer
{
public:
	typedef Interface<I>   this_type;
	typedef UnknownPointer parent_type;

	Interface();
	Interface(LPIGetUnknown pGetUnknown);
	Interface(PTR(void) pInterface, LPIUnknown pUnknown);
	Interface(CREF(this_type) aInterface);

	REF(this_type) operator=     (CREF(this_type) aInterface);
	               operator rbool() const;

	PTR(I)  get();
	CPTR(I) get() const;

	PTR(I)  operator-> ();
	CPTR(I) operator-> () const;
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

template <class T>
class IFactory
{
private:
	class Counter: public IUnknown
	{
	friend class IFactory<T>;
	friend class IFactory<T>::Object;
	private:
		ruint  m_counter;
		PTR(T) m_pObject;

		Counter();
		operator UnknownPointer();

		virtual void           AddRef        ();
		virtual void           Release       ();
		virtual UnknownPointer QueryInterface(ruint id);
	};

public:
	class Object: public T
	{
	public:
		Object();

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

		virtual ~Object();

		virtual LPIUnknown GetUnknown();

	private:
		void create();

		Counter m_counter;
	};

	static UnknownPointer create();

	template <typename P1>
	static UnknownPointer create(CREF(P1) p1);

	template <typename P1, typename P2>
	static UnknownPointer create(CREF(P1) p1, CREF(P2) p2);

	template <typename P1, typename P2, typename P3>
	static UnknownPointer create(CREF(P1) p1, CREF(P2) p2, CREF(P3) p3);

	template <typename P1, typename P2, typename P3, typename P4>
	static UnknownPointer create(CREF(P1) p1, CREF(P2) p2, CREF(P3) p3, CREF(P4) p4);

	template <typename P1, typename P2, typename P3, typename P4, typename P5>
	static UnknownPointer create(CREF(P1) p1, CREF(P2) p2, CREF(P3) p3, CREF(P4) p4, CREF(P5) p5);

	template <typename P1, typename P2, typename P3, typename P4, typename P5, typename P6>
	static UnknownPointer create(CREF(P1) p1, CREF(P2) p2, CREF(P3) p3, CREF(P4) p4, CREF(P5) p5, CREF(P6) p6);

	template <typename P1, typename P2, typename P3, typename P4, typename P5, typename P6, typename P7>
	static UnknownPointer create(CREF(P1) p1, CREF(P2) p2, CREF(P3) p3, CREF(P4) p4, CREF(P5) p5, CREF(P6) p6, CREF(P7) p7);

	static void destroy(PTR(T) pObject);

private:
	static UnknownPointer init(PTR(Object) pObject);
};

CLOSE_RDO_NAMESPACE

#define  CAST_TO_UNKNOWN  public rdo::IGetUnknown

#define RF(A) rdo::IFactory<A>

#define DEFINE_IFACTORY(A)      friend class rdo::IFactory<A>; friend class rdo::IFactory<A>::Object;

#define QUERY_INTERFACE_BEGIN                           \
public:                                                 \
PTR(void) QueryInterface(ruint id)                      \
{

#define QUERY_INTERFACE_PARENT(A)                       \
	PTR(void) pIterface##A = A::QueryInterface(id);     \
	if (pIterface##A) return pIterface##A;

#define QUERY_INTERFACE(A)                              \
	if (id == IID(A)) return static_cast<PTR(A)>(this);

#define QUERY_INTERFACE_END                             \
	return NULL;                                        \
}

class IInit
{
public:
	virtual rbool init() = 0;
};
#define DECLARE_IInit     \
	virtual rbool init();

INTERFACE_REGISTRATOR(IInit, 0);

#include "rdo_common/rdointerface.inl"

#endif //! _RDOINTERFACE_H_
