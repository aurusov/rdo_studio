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
#include "namespace.h"
#include "rdomacros.h"
#include "rdotypes.h"
// ===============================================================================

OPEN_RDO_NAMESPACE

template<class T>
class smart_ptr
{
public:
	typedef T            object_type;
	typedef smart_ptr<T> this_type;

	smart_ptr()
		: m_object (NULL)
		, m_counter(NULL)
	{}
	smart_ptr(PTR(T) obj)
		: m_object(obj)
	{
		if (m_object)
		{
			allocateCounter();
			counter() = 1;
		}
		else
		{
			m_counter = NULL;
		}
	}
	smart_ptr(CREF(this_type) sptr)
		: m_counter(sptr.m_counter)
		, m_object (sptr.m_object )
	{
		addref();
	}
	REF(this_type) operator= (CREF(this_type) sptr)
	{
		if (get() != NULL)
			clear();

		m_counter = sptr.m_counter;
		m_object  = sptr.m_object;
		addref();
		return *this;
	}
	~smart_ptr()
	{
		if (inited())
			clear();
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
	rbool owner() const
	{
		return inited() ? (counter() == 1) : false;
	}

protected:
	void addref()
	{
		if (inited())
			counter()++;
	}
	void release()
	{
		if (!inited())
			return;

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

private:
	PTR(ruint) m_counter;
	PTR(T)     m_object;

	rbool inited() const
	{
		return m_counter != NULL;
	}
	CREF(ruint) counter() const
	{
		return *m_counter;
	}
	REF(ruint) counter()
	{
		return *m_counter;
	}
	void allocateCounter()
	{
		m_counter = new ruint;
	}
	void deallocateCounter()
	{
		delete m_counter;
	}
	void clear()
	{
		release();
		if (counter() == 0)
			deallocateCounter();
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
	static smart_ptr<T> create(CREF(P1) p1, CREF(P2) p2, CREF(P2) p3)
	{
		return smart_ptr<T>(new T(p1, p2, p3));
	}
	template <typename P1, typename P2, typename P3, typename P4>
	static smart_ptr<T> create(CREF(P1) p1, CREF(P2) p2, CREF(P2) p3, CREF(P2) p4)
	{
		return smart_ptr<T>(new T(p1, p2, p3, p4));
	}
	template <typename P1, typename P2, typename P3, typename P4, typename P5>
	static smart_ptr<T> create(CREF(P1) p1, CREF(P2) p2, CREF(P2) p3, CREF(P2) p4, CREF(P2) p5)
	{
		return smart_ptr<T>(new T(p1, p2, p3, p4, p5));
	}
	template <typename P1, typename P2, typename P3, typename P4, typename P5, typename P6>
	static smart_ptr<T> create(CREF(P1) p1, CREF(P2) p2, CREF(P2) p3, CREF(P2) p4, CREF(P2) p5, CREF(P2) p6)
	{
		return smart_ptr<T>(new T(p1, p2, p3, p4, p5, p6));
	}
	template <typename P1, typename P2, typename P3, typename P4, typename P5, typename P6, typename P7>
	static smart_ptr<T> create(CREF(P1) p1, CREF(P2) p2, CREF(P2) p3, CREF(P2) p4, CREF(P2) p5, CREF(P2) p6, CREF(P2) p7)
	{
		return smart_ptr<T>(new T(p1, p2, p3, p4, p5, p6, p7));
	}
	static void destroy(PTR(T) obj)
	{
		delete obj;
	}
};

CLOSE_RDO_NAMESPACE

#endif //! _RDOSMART_PTR_H_
