/*!
  \copyright (c) RDO-Team, 2011
  \file      rdotime.inl
  \authors   Урусов Андрей (rdo@rk9.bmstu.ru)
  \authors   Пройдаков Евгений (lord.tiran@gmail.com)
  \date      10.05.2009
  \brief     
  \indent    4T
*/

// ----------------------------------------------------------------------- INCLUDES
#include <boost/date_time/posix_time/posix_time.hpp>
// ----------------------------------------------------------------------- SYNOPSIS
#include "utils/src/debug/rdodebug.h"
// --------------------------------------------------------------------------------

namespace rdo {

inline Time::Time()
	: m_value(Time::invalid().value())
{}

inline Time::Time(const Value& value)
	: m_value(value)
{}

inline bool Time::operator==(const Time& time)
{
	return m_value == time.m_value;
}

inline Time::operator Value() const
{
	return m_value;
}

inline std::string Time::asString() const
{
	if (m_value == Time::invalid())
	{
		return "[INVALID]";
	}
	else
	{
		return boost::posix_time::to_simple_string_type<char>(boost::posix_time::microsec_clock::local_time());
	}
}

inline const Time::Value& Time::value() const
{
	return m_value;
}

inline Time Time::utc()
{
	boost::posix_time::ptime startTime = boost::posix_time::time_from_string("1970-01-01 00:00:00.000");
	Time::Value delta = ( boost::posix_time::microsec_clock::universal_time() - startTime ).total_nanoseconds( );
	return delta;
}

inline Time Time::local()
{
	boost::posix_time::ptime startTime = boost::posix_time::time_from_string("1970-01-01 00:00:00.000");
	Time::Value delta = ( boost::posix_time::microsec_clock::local_time() - startTime ).total_nanoseconds( );
	return delta;
}

inline Time Time::invalid()
{
	return Time(Value(~0));
}

} // namespace rdo
