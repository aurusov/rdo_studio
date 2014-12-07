// ----------------------------------------------------------------------- INCLUDES
#include <boost/date_time/posix_time/posix_time.hpp>
// ----------------------------------------------------------------------- SYNOPSIS
#include "utils/src/time/rdotime.h"
#include "utils/src/debug/rdodebug.h"
// --------------------------------------------------------------------------------

namespace rdo {

Time::Time()
	: m_value(Time::invalid().value())
{}

Time::Time(const Value& value)
	: m_value(value)
{}

bool Time::operator==(const Time& time)
{
	return m_value == time.m_value;
}

Time::operator Value() const
{
	return m_value;
}

std::string Time::asString() const
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

const Time::Value& Time::value() const
{
	return m_value;
}

Time Time::utc()
{
	boost::posix_time::ptime startTime = boost::posix_time::time_from_string("1970-01-01 00:00:00.000");
	Time::Value delta = ( boost::posix_time::microsec_clock::universal_time() - startTime ).total_nanoseconds( );
	return delta;
}

Time Time::local()
{
	boost::posix_time::ptime startTime = boost::posix_time::time_from_string("1970-01-01 00:00:00.000");
	Time::Value delta = ( boost::posix_time::microsec_clock::local_time() - startTime ).total_nanoseconds( );
	return delta;
}

Time Time::invalid()
{
	return Time(Value(~0));
}

} // namespace rdo
