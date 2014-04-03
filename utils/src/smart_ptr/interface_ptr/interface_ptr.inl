/*!
  \copyright (c) RDO-Team, 2011
  \file      interface_ptr.inl
  \author    Урусов Андрей (rdo@rk9.bmstu.ru)
  \date      12.06.2010
  \brief
  \indent    4T
*/

// ----------------------------------------------------------------------- INCLUDES
// ----------------------------------------------------------------------- SYNOPSIS
#include "utils/src/debug/rdodebug.h"
#include "utils/src/smart_ptr/intrusive_ptr/intrusive_ptr.h"
// --------------------------------------------------------------------------------

namespace rdo {

template<class T>
FORCE_INLINE interface_ptr<T>::interface_ptr()
	: m_pInterface(NULL)
	, m_pCounter  (NULL)
{}

template<class T>
FORCE_INLINE interface_ptr<T>::interface_ptr(PTR(T) pInterface, LPIRefCounter pCounter)
	: m_pInterface(pInterface)
	, m_pCounter  (pCounter  )
{
	if (m_pInterface)
		m_pCounter->addref();
}

template<class T>
FORCE_INLINE interface_ptr<T>::interface_ptr(CREF(this_type) sptr)
	: m_pInterface(sptr.m_pInterface)
	, m_pCounter  (sptr.m_pCounter  )
{
	if (m_pInterface)
		m_pCounter->addref();
}

template<class T>
FORCE_INLINE interface_ptr<T>::~interface_ptr()
{
	if (m_pInterface)
		m_pCounter->release();
}

template<class T>
inline REF(typename interface_ptr<T>::this_type) interface_ptr<T>::operator= (CREF(this_type) sptr)
{
	if (m_pInterface)
		m_pCounter->release();

	m_pInterface = sptr.m_pInterface;
	m_pCounter   = sptr.m_pCounter;

	if (m_pInterface)
		m_pCounter->addref();

	return *this;
}

template<class T>
FORCE_INLINE interface_ptr<T>::operator rbool () const
{
	return m_pInterface != NULL;
}

template<class T>
FORCE_INLINE CPTR(T) interface_ptr<T>::operator-> () const
{
	return m_pInterface;
}

template<class T>
FORCE_INLINE PTR(T) interface_ptr<T>::operator-> ()
{
	return m_pInterface;
}

} // namespace rdo
