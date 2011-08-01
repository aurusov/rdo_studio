/*
 * copyright: (c) RDO-Team, 2009
 * filename : rdotime.inl
 * author   : Урусов Андрей, Evgeny Proydakov
 * date     : 10.05.2009
 * bref     : 
 * indent   : 4T
 */

// ====================================================================== INCLUDES
#include <boost/date_time/posix_time/posix_time.hpp>
// ====================================================================== SYNOPSIS
#include "rdo_common/rdodebug.h"
// ===============================================================================

OPEN_RDO_NAMESPACE

inline Time::Time()
	: m_value(Time::invalid().value())
{}

inline Time::Time(CREF(Value) value)
	: m_value(value)
{}

inline rbool Time::operator== (CREF(Time) time)
{
	return m_value == time.m_value;
}

inline Time::operator Value() const
{
	return m_value;
}

inline tstring Time::asString() const
{
	if (m_value == Time::invalid())
	{
		return _T("[INVALID]");
	}
	else
	{
		return boost::posix_time::to_simple_string(boost::posix_time::microsec_clock::local_time());
	}
}

inline CREF(Time::Value) Time::value() const
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
	return ~0;
}

CLOSE_RDO_NAMESPACE
