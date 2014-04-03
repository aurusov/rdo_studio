/*!
  \copyright (c) RDO-Team, 2011
  \file      explicit_value.h
  \author    Урусов Андрей (rdo@rk9.bmstu.ru)
  \date      21.01.2012
  \brief     
  \indent    4T
*/

#ifndef _RDO_COMMON_EXPLICIT_VALUE_H_
#define _RDO_COMMON_EXPLICIT_VALUE_H_

// ----------------------------------------------------------------------- INCLUDES
// ----------------------------------------------------------------------- SYNOPSIS
#include "utils/src/common/rdocommon.h"
// --------------------------------------------------------------------------------

namespace rdo {

template <class T>
class explicit_value
{
public:
	explicit explicit_value(CREF(T) value)
		: m_value(value)
	{}

	operator T() const
	{
		return m_value;
	}

private:
	T m_value;
};

} // namespace rdo

#endif // _RDO_COMMON_EXPLICIT_VALUE_H_
