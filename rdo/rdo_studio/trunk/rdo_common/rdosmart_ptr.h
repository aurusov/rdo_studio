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
		addref();
	}
	~smart_ptr()
	{
		release();
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

protected:
	REF(ruint) counter()
	{
		return *m_counter;
	}
	void addref()
	{
		counter()++;
	}
	void release()
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

private:
	PTR(ruint) m_counter;
	PTR(T)     m_object;

	void allocateCounter()
	{
		m_counter = new ruint;
	}
	void deallocateCounter()
	{
		delete m_counter;
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

#endif //! _RDOSMART_PTR_H_
