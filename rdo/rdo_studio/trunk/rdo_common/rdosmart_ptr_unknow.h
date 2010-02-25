/*
 * copyright: (c) RDO-Team, 2010
 * filename : rdosmart_ptr_unknow.h
 * author   : Урусов Андрей
 * date     : 24.02.2010
 * bref     : 
 * indent   : 4T
 */

#ifndef _RDOSMART_PTR_UNKNOW_H_
#define _RDOSMART_PTR_UNKNOW_H_

// ====================================================================== INCLUDES
// ====================================================================== SYNOPSIS
#include "rdo_common/namespace.h"
#include "rdo_common/rdomacros.h"
#include "rdo_common/rdotypes.h"
#include "rdo_common/rdosmart_ptr.h"
// ===============================================================================

OPEN_RDO_NAMESPACE

class usmart_ptr
{
public:
	usmart_ptr()
		: m_object (NULL)
		, m_counter(NULL)
	{}
	usmart_ptr(PTR(void) obj)
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
	usmart_ptr(CREF(usmart_ptr) usptr)
		: m_counter(usptr.m_counter)
		, m_object (usptr.m_object )
	{
		addref();
	}
	template <class T>
	usmart_ptr(CREF(smart_ptr<T>) sptr)
		: m_counter(sptr.m_counter)
		, m_object (sptr.m_object )
	{
		addref();
	}
	REF(usmart_ptr) operator= (CREF(usmart_ptr) usptr)
	{
		if (get<void>() != NULL)
			clear();

		m_counter = usptr.m_counter;
		m_object  = usptr.m_object;
		addref();
		return *this;
	}
	template <class T>
	REF(usmart_ptr) operator= (CREF(smart_ptr<T>) sptr)
	{
		if (get<void>() != NULL)
			clear();

		m_counter = sptr.m_counter;
		m_object  = sptr.m_object;
		addref();
		return *this;
	}
	~usmart_ptr()
	{
		if (inited())
			clear();
	}
	template <class T>
	operator smart_ptr<T> () const
	{
		smart_ptr<T> sptr;
		sptr.m_counter = m_counter;
		sptr.m_object  = reinterpret_cast<PTR(T)>(m_object);
		sptr.addref();
		return sptr;
	}
	template <class T>
	PTR(T) get()
	{
		return reinterpret_cast<PTR(T)>(m_object);
	}
	template <class T>
	CPTR(T) get() const
	{
		return reinterpret_cast<CPTR(T)>(m_object);
	}
	operator rbool () const
	{
		return get<void>() != NULL;
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
	PTR(void)  m_object;

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

#endif //! _RDOSMART_PTR_UNKNOW_H_
