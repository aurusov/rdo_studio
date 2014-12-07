#pragma once

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
