#ifndef RDOTIME_H
#define RDOTIME_H

// ====================================================================== INCLUDES
#include <io.h>
// ====================================================================== SYNOPSIS
#include "rdotypes.h"
#include "rdomacros.h"
// ===============================================================================

namespace rdo
{

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

	static Time utc    ();
	static Time local  ();
	static Time invalid();

private:
	Value  m_value;
};

} //! namespace rdo

#include "rdotime.inl"

#endif //! RDOTIME_H
