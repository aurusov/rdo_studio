/*!
  \copyright (c) RDO-Team, 2011
  \file      stack.inl
  \author    Урусов Андрей (rdo@rk9.bmstu.ru)
  \date      12.12.2009
  \brief     
  \indent    4T
*/

// ----------------------------------------------------------------------- INCLUDES
// ----------------------------------------------------------------------- SYNOPSIS
#include "utils/src/debug/rdodebug.h"
// --------------------------------------------------------------------------------

OPEN_RDO_NAMESPACE

template<class T>
inline void stack<T>::push(CREF(T) item)
{
	m_container.push_back(item);
}

template<class T>
inline void stack<T>::pop()
{
	ASSERT(!empty());

	m_container.pop_back();
}

template<class T>
inline rbool stack<T>::empty() const
{
	return m_container.empty();
}

template<class T>
inline ruint stack<T>::size() const
{
	return m_container.size();
}

template<class T>
inline CREF(T) stack<T>::top() const
{
	ASSERT(!empty());

	return m_container.back();
}

template<class T>
inline REF(T) stack<T>::top()
{
	ASSERT(!empty());

	return m_container.back();
}

CLOSE_RDO_NAMESPACE
