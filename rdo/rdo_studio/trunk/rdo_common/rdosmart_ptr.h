/*
 * copyright: (c) RDO-Team, 2009
 * filename : rdosmart_ptr.h
 * author   : Урусов Андрей
 * date     : 12.07.2009
 * bref     : 
 * indent   : 4T
 */

#ifndef _RDOSMART_PTR_H_
#define _RDOSMART_PTR_H_

// ====================================================================== INCLUDES
// ====================================================================== SYNOPSIS
#include "rdo_common/namespace.h"
#include "rdo_common/rdomacros.h"
#include "rdo_common/rdotypes.h"
#include "rdo_common/supersubclass.h"
#include "rdo_common/static_assert.h"
// ===============================================================================

OPEN_RDO_NAMESPACE

class smart_ptr_counter_reference
{
public:
	smart_ptr_counter_reference();

	void setSmartPtrCounterReference(PTR(ruint) pCounter);

	PTR(ruint) m_pCounter;
};

template<class T>
class smart_ptr
{
template<class P> friend class smart_ptr;
template<class P> friend class Factory;
public:
	typedef T            object_type;
	typedef smart_ptr<T> this_type;

	smart_ptr ();
	smart_ptr (CREF(this_type) sptr);
	//! Используется только для объектов-потомков от smart_ptr_counter_reference
	smart_ptr (PTR(T) object);
	template<class P>
	smart_ptr (CREF(smart_ptr<P>) sptr);
	~smart_ptr();

	REF(this_type) operator= (CREF(this_type) sptr);
	PTR(T)   get();
	CPTR(T)  get() const;

	operator rbool     () const;
	CPTR(T) operator-> () const;
	 PTR(T) operator-> ();

	template<class P>
 	rbool operator== (CREF(smart_ptr<P>) sptr) const;

	template <class P>
	smart_ptr<P> cast() const;

	template <class P>
	P lp_cast() const;

	rbool owner() const;

protected:
	void addref ();
	void release();

private:
	//! Вызываеися из фабрики, factory используется только для перегрузки
	smart_ptr(PTR(T) object, rbool factory);

	//! Вызывается из другого smart_ptr
	template<class P>
	smart_ptr(PTR(P) object, PTR(ruint) counter);

	template <rbool> void init();

	template <>	void init<false>()
	{
		STATIC_ASSERT(Object_can_use_smart_ptr_counter_reference);
	}
	template <>	void init<true>()
	{
		m_counter = m_object->m_pCounter;
	}

	PTR(ruint) m_counter;
	PTR(T)     m_object;

	rbool       inited           () const;
	CREF(ruint) counter          () const;
	REF(ruint)  counter          ();
	void        allocateCounter  ();
	void        deallocateCounter();
	void        clear            ();
};

template <class T>
class Factory
{
friend class smart_ptr<T>;
public:
	inline static smart_ptr<T> create()
	{
		PTR(T) pObject = new T();
		return init<SUPERSUBCLASS(smart_ptr_counter_reference, T)>(pObject);
	}
	template <typename P1>
	static smart_ptr<T> create(CREF(P1) p1)
	{
		PTR(T) pObject = new T(p1);
		return init<SUPERSUBCLASS(smart_ptr_counter_reference, T)>(pObject);
	}
	template <typename P1, typename P2>
	static smart_ptr<T> create(CREF(P1) p1, CREF(P2) p2)
	{
		PTR(T) pObject = new T(p1, p2);
		return init<SUPERSUBCLASS(smart_ptr_counter_reference, T)>(pObject);
	}
	template <typename P1, typename P2, typename P3>
	static smart_ptr<T> create(CREF(P1) p1, CREF(P2) p2, CREF(P3) p3)
	{
		PTR(T) pObject = new T(p1, p2, p3);
		return init<SUPERSUBCLASS(smart_ptr_counter_reference, T)>(pObject);
	}
	template <typename P1, typename P2, typename P3, typename P4>
	static smart_ptr<T> create(CREF(P1) p1, CREF(P2) p2, CREF(P3) p3, CREF(P4) p4)
	{
		PTR(T) pObject = new T(p1, p2, p3, p4);
		return init<SUPERSUBCLASS(smart_ptr_counter_reference, T)>(pObject);
	}
	template <typename P1, typename P2, typename P3, typename P4, typename P5>
	static smart_ptr<T> create(CREF(P1) p1, CREF(P2) p2, CREF(P3) p3, CREF(P4) p4, CREF(P5) p5)
	{
		PTR(T) pObject = new T(p1, p2, p3, p4, p5);
		return init<SUPERSUBCLASS(smart_ptr_counter_reference, T)>(pObject);
	}
	template <typename P1, typename P2, typename P3, typename P4, typename P5, typename P6>
	static smart_ptr<T> create(CREF(P1) p1, CREF(P2) p2, CREF(P3) p3, CREF(P4) p4, CREF(P5) p5, CREF(P6) p6)
	{
		PTR(T) pObject = new T(p1, p2, p3, p4, p5, p6);
		return init<SUPERSUBCLASS(smart_ptr_counter_reference, T)>(pObject);
	}
	template <typename P1, typename P2, typename P3, typename P4, typename P5, typename P6, typename P7>
	static smart_ptr<T> create(CREF(P1) p1, CREF(P2) p2, CREF(P3) p3, CREF(P4) p4, CREF(P5) p5, CREF(P6) p6, CREF(P7) p7)
	{
		PTR(T) pObject = new T(p1, p2, p3, p4, p5, p6, p7);
		return init<SUPERSUBCLASS(smart_ptr_counter_reference, T)>(pObject);
	}

private:
	template <rbool>
	static smart_ptr<T> init(PTR(T) object);

	template <>
	static smart_ptr<T> init<false>(PTR(T) object)
	{
		return smart_ptr<T>(object, true);
	}

	template <>
	static smart_ptr<T> init<true>(PTR(T) object)
	{
		smart_ptr<T> sobj(object, true);
		object->setSmartPtrCounterReference(sobj.m_counter);
		return sobj;
	}

	static void destroy(PTR(T) object)
	{
		delete object;
	}
};

#define DECLARE_FACTORY(TYPE)    friend class rdo::Factory<TYPE>;
#define DECLARE_POINTER(TYPE)    typedef rdo::smart_ptr<TYPE> LP##TYPE;
#define PREDECLARE_POINTER(TYPE) class TYPE; DECLARE_POINTER(TYPE);

CLOSE_RDO_NAMESPACE

#include "rdo_common/rdosmart_ptr.inl"

#endif //! _RDOSMART_PTR_H_
