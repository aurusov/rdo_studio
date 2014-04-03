/*!
  \copyright (c) RDO-Team, 2011
  \file      rdotime.h
  \author    Урусов Андрей (rdo@rk9.bmstu.ru)
  \date      10.05.2009
  \brief     
  \indent    4T
*/

#ifndef _UTILS_RDOTIME_H_
#define _UTILS_RDOTIME_H_

// ----------------------------------------------------------------------- INCLUDES
// ----------------------------------------------------------------------- SYNOPSIS
#include "utils/src/common/rdocommon.h"
// --------------------------------------------------------------------------------

namespace rdo {

class Time
{
public:
	typedef  ruint64  Value;

	Time();
	Time(CREF(Value) value);

	rbool operator== (CREF(Time) time);
	operator Value() const;

	tstring asString() const;

	CREF(Value) value() const;

	static Time utc    (); // total_nanoseconds
	static Time local  (); // total_nanoseconds
	static Time invalid();

private:
	Value  m_value;
};

} // namespace rdo

#include "utils/src/time/rdotime.inl"

#endif // _UTILS_RDOTIME_H_
