/*!
  \copyright (c) RDO-Team, 2011
  \file      rdosingletone.h
  \author    Урусов Андрей (rdo@rk9.bmstu.ru)
  \date      26.07.2009
  \brief     
  \indent    4T
*/

#ifndef _RDOSINGLETONE_H_
#define _RDOSINGLETONE_H_

// ====================================================================== INCLUDES
// ====================================================================== SYNOPSIS
#include "rdo_common/namespace.h"
#include "rdo_common/rdomacros.h"
#include "rdo_common/rdotypes.h"
// ===============================================================================

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

#endif //! _RDOSINGLETONE_H_
