/*!
  \copyright (c) RDO-Team, 2011
  \file      ref_counter.inl
  \author    Урусов Андрей (rdo@rk9.bmstu.ru)
  \date      21.09.2011
  \brief     
  \indent    4T
*/

// ----------------------------------------------------------------------- INCLUDES
// ----------------------------------------------------------------------- SYNOPSIS
#include "utils/rdodebug.h"
#include "utils/smart_ptr/counter_reference.h"
// --------------------------------------------------------------------------------

OPEN_RDO_NAMESPACE

template<class T>
inline void RefCounter<T>::addref()
{
	PTR(counter_reference) pCounter = dynamic_cast<PTR(counter_reference)>(this);
	ASSERT(pCounter);
	pCounter->m_intrusive_counter++;
}

template<class T>
inline void RefCounter<T>::release()
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
inline rbool RefCounter<T>::owner() const
{
	CPTR(counter_reference) pCounter = dynamic_cast<CPTR(counter_reference)>(this);
	ASSERT(pCounter);
	return pCounter->m_intrusive_counter == 1;
}

CLOSE_RDO_NAMESPACE
