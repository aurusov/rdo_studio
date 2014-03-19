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
#include "utils/src/debug/rdodebug.h"
#include "utils/src/smart_ptr/ref_counter/counter_reference.h"
// --------------------------------------------------------------------------------

namespace rdo {

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

} // namespace rdo
