/*
 * copyright: (c) RDO-Team, 2010
 * filename : rdosmart_ptr_unknow.inl
 * author   : Урусов Андрей
 * date     : 24.02.2010
 * bref     : 
 * indent   : 4T
 */

// ====================================================================== INCLUDES
// ====================================================================== SYNOPSIS
#include "rdo_common/rdosmart_ptr_unknow.h"
#include "rdo_common/rdosmart_ptr_unknow_factory.h"
// ===============================================================================

OPEN_RDO_NAMESPACE

inline usmart_ptr::usmart_ptr()
	: m_object (NULL)
	, m_counter(NULL)
{}

inline usmart_ptr::usmart_ptr(PTR(void) obj)
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

inline usmart_ptr::usmart_ptr(CREF(usmart_ptr) usptr)
	: m_counter(usptr.m_counter)
	, m_object (usptr.m_object )
{
	addref();
}

template <class T>
inline usmart_ptr::usmart_ptr(CREF(smart_ptr<T>) sptr)
	: m_counter(sptr.m_counter)
	, m_object (sptr.m_object )
{
	addref();
}

inline REF(usmart_ptr) usmart_ptr::operator= (CREF(usmart_ptr) usptr)
{
	if (get<void>() != NULL)
		clear();

	m_counter = usptr.m_counter;
	m_object  = usptr.m_object;
	addref();
	return *this;
}

template <class T>
inline REF(usmart_ptr) usmart_ptr::operator= (CREF(smart_ptr<T>) sptr)
{
	if (get<void>() != NULL)
		clear();

	m_counter = sptr.m_counter;
	m_object  = sptr.m_object;
	addref();
	return *this;
}

inline usmart_ptr::~usmart_ptr()
{
	if (inited())
		clear();
}

template <class T>
inline usmart_ptr::operator smart_ptr<T> () const
{
	smart_ptr<T> sptr;
	sptr.m_counter = m_counter;
	sptr.m_object  = reinterpret_cast<PTR(T)>(m_object);
	sptr.addref();
	return sptr;
}

template <class T>
inline PTR(T) usmart_ptr::get()
{
	return reinterpret_cast<PTR(T)>(m_object);
}

template <class T>
inline CPTR(T) usmart_ptr::get() const
{
	return reinterpret_cast<CPTR(T)>(m_object);
}

inline usmart_ptr::operator rbool () const
{
	return get<void>() != NULL;
}

inline rbool usmart_ptr::owner() const
{
	return inited() ? (counter() == 1) : false;
}

inline void usmart_ptr::addref()
{
	if (inited())
		counter()++;
}

inline void usmart_ptr::release()
{
	if (!inited())
		return;

	counter()--;
	if (counter() == 0)
	{
		if (m_object)
		{
			UFactory::destroy(*this);
			m_object = NULL;
		}
	}
}

inline rbool usmart_ptr::inited() const
{
	return m_counter != NULL;
}

inline CREF(ruint) usmart_ptr::counter() const
{
	return *m_counter;
}

inline REF(ruint) usmart_ptr::counter()
{
	return *m_counter;
}

inline void usmart_ptr::allocateCounter()
{
	m_counter = new ruint;
}

inline void usmart_ptr::deallocateCounter()
{
	delete m_counter;
}

inline void usmart_ptr::clear()
{
	release();
	if (counter() == 0)
		deallocateCounter();
}

CLOSE_RDO_NAMESPACE
