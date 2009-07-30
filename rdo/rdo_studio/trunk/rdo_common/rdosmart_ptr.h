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
	CPTR(T) get() const
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
				delete m_object;
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

CLOSE_RDO_NAMESPACE

#endif //! _RDOSMART_PTR_H_
