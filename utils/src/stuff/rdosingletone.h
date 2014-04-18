/*!
  \copyright (c) RDO-Team, 2011
  \file      rdosingletone.h
  \author    Урусов Андрей (rdo@rk9.bmstu.ru)
  \date      26.07.2009
  \brief     
  \indent    4T
*/

#ifndef _UTILS_RDOSINGLETONE_H_
#define _UTILS_RDOSINGLETONE_H_

// ----------------------------------------------------------------------- INCLUDES
// ----------------------------------------------------------------------- SYNOPSIS
#include "utils/src/common/rdocommon.h"
// --------------------------------------------------------------------------------

namespace rdo {

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
	T* instance()
	{
		if (!m_object)
			m_object = new T();
		return m_object;
	}
	template <typename P1>
	T* instance(const P1& p1)
	{
		if (!m_object)
			m_object = new T(p1);
		return m_object;
	}

private:
	T* m_object;
};

} // namespace rdo

#endif // _UTILS_RDOSINGLETONE_H_
