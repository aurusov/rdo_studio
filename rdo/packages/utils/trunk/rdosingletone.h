/*!
  \copyright (c) RDO-Team, 2011
  \file      rdosingletone.h
  \author    Урусов Андрей (rdo@rk9.bmstu.ru)
  \date      26.07.2009
  \brief     
  \indent    4T
*/

#ifndef _LIB_COMMON_RDOSINGLETONE_H_
#define _LIB_COMMON_RDOSINGLETONE_H_

// ----------------------------------------------------------------------- INCLUDES
// ----------------------------------------------------------------------- SYNOPSIS
#include "utils/namespace.h"
#include "utils/rdomacros.h"
#include "utils/rdotypes.h"
// --------------------------------------------------------------------------------

OPEN_RDO_NAMESPACE

template<class T>
class SingleTone
{
public:
	typedef T             object_type;
	typedef SingleTone<T> this_type;

	SingleTone()
		: m_object(NULL)
	{}
	~SingleTone()
	{
		if (m_object)
			delete m_object;
	}
	PTR(T) instance()
	{
		if (!m_object)
			m_object = new T();
		return m_object;
	}
	template <typename P1>
	PTR(T) instance(CREF(P1) p1)
	{
		if (!m_object)
			m_object = new T(p1);
		return m_object;
	}

private:
	PTR(T) m_object;
};

CLOSE_RDO_NAMESPACE

#endif // _LIB_COMMON_RDOSINGLETONE_H_
