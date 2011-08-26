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
#include "rdo_common/rdodebug.h"
// --------------------------------------------------------------------------------

OPEN_RDO_NAMESPACE

template<class T>
inline void CounterReferenceReal<T>::addref()
{
	PTR(counter_reference) pCounter = dynamic_cast<PTR(counter_reference)>(this);
	ASSERT(pCounter);
	pCounter->m_intrusive_counter++;
}

template<class T>
inline void CounterReferenceReal<T>::release()
{
	PTR(counter_reference) pCounter = dynamic_cast<PTR(counter_reference)>(this);
	ASSERT(pCounter);
	pCounter->m_intrusive_counter--;
	if (pCounter->m_intrusive_counter == 0)
	{
		PTR(T) pObject = dynamic_cast<PTR(T)>(this);
		ASSERT(pObject);
		Factory<T>::destroy(pObject);
	}
}

template<class T>
inline interface_ptr<T>::interface_ptr()
	: m_pInterface(NULL)
	, m_pCounter  (NULL)
{}

template<class T>
inline interface_ptr<T>::interface_ptr(PTR(T) pInterface, LPICounterReference pCounter)
	: m_pInterface(pInterface)
	, m_pCounter  (pCounter  )
{
	if (m_pInterface)
		m_pCounter->addref();
}

template<class T>
inline interface_ptr<T>::interface_ptr(CREF(this_type) sptr)
	: m_pInterface(sptr.m_pInterface)
	, m_pCounter  (sptr.m_pCounter  )
{
	if (m_pInterface)
		m_pCounter->addref();
}

template<class T>
inline interface_ptr<T>::~interface_ptr()
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
inline interface_ptr<T>::operator rbool () const
{
	return m_pInterface != NULL;
}

template<class T>
inline CPTR(T) interface_ptr<T>::operator-> () const
{
	return m_pInterface;
}

template<class T>
inline PTR(T) interface_ptr<T>::operator-> ()
{
	return m_pInterface;
}

CLOSE_RDO_NAMESPACE
