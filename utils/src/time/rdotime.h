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
#include <cstdint>
// ----------------------------------------------------------------------- SYNOPSIS
#include "utils/src/common/rdocommon.h"
// --------------------------------------------------------------------------------

namespace rdo {

class Time
{
public:
	typedef uint64_t Value;

	Time();
	Time(const Value& value);

	bool operator==(const Time& time);
	operator Value() const;

	std::string asString() const;

	const Value& value() const;

	static Time utc(); // total_nanoseconds
	static Time local(); // total_nanoseconds
	static Time invalid();

private:
	Value  m_value;
};

} // namespace rdo

#include "utils/src/time/rdotime.inl"

#endif // _UTILS_RDOTIME_H_
