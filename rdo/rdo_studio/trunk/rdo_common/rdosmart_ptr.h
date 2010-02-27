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
// ===============================================================================

OPEN_RDO_NAMESPACE

template<class T>
class smart_ptr
{
public:
	typedef T            object_type;
	typedef smart_ptr<T> this_type;

	smart_ptr ();
	smart_ptr ( PTR(T)         obj );
	smart_ptr (CREF(this_type) sptr);
	~smart_ptr();

	REF(this_type) operator= (CREF(this_type) sptr);
	PTR(T)   get();
	CPTR(T)  get() const;

	operator rbool    () const;
	PTR(T) operator-> ();

	rbool owner() const;

protected:
	void addref ();
	void release();

private:
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
	static smart_ptr<T> create()
	{
		return smart_ptr<T>(new T());
	}
	template <typename P1>
	static smart_ptr<T> create(CREF(P1) p1)
	{
		return smart_ptr<T>(new T(p1));
	}
	template <typename P1, typename P2>
	static smart_ptr<T> create(CREF(P1) p1, CREF(P2) p2)
	{
		return smart_ptr<T>(new T(p1, p2));
	}
	template <typename P1, typename P2, typename P3>
	static smart_ptr<T> create(CREF(P1) p1, CREF(P2) p2, CREF(P3) p3)
	{
		return smart_ptr<T>(new T(p1, p2, p3));
	}
	template <typename P1, typename P2, typename P3, typename P4>
	static smart_ptr<T> create(CREF(P1) p1, CREF(P2) p2, CREF(P3) p3, CREF(P4) p4)
	{
		return smart_ptr<T>(new T(p1, p2, p3, p4));
	}
	template <typename P1, typename P2, typename P3, typename P4, typename P5>
	static smart_ptr<T> create(CREF(P1) p1, CREF(P2) p2, CREF(P3) p3, CREF(P4) p4, CREF(P5) p5)
	{
		return smart_ptr<T>(new T(p1, p2, p3, p4, p5));
	}
	template <typename P1, typename P2, typename P3, typename P4, typename P5, typename P6>
	static smart_ptr<T> create(CREF(P1) p1, CREF(P2) p2, CREF(P3) p3, CREF(P4) p4, CREF(P5) p5, CREF(P6) p6)
	{
		return smart_ptr<T>(new T(p1, p2, p3, p4, p5, p6));
	}
	template <typename P1, typename P2, typename P3, typename P4, typename P5, typename P6, typename P7>
	static smart_ptr<T> create(CREF(P1) p1, CREF(P2) p2, CREF(P3) p3, CREF(P4) p4, CREF(P5) p5, CREF(P6) p6, CREF(P7) p7)
	{
		return smart_ptr<T>(new T(p1, p2, p3, p4, p5, p6, p7));
	}

private:
	static void destroy(PTR(T) object)
	{
		delete object;
	}
};

#define DECLARE_FACTORY(TYPE) friend class rdo::Factory<TYPE>;

CLOSE_RDO_NAMESPACE

#include "rdo_common/rdosmart_ptr.inl"

#endif //! _RDOSMART_PTR_H_
