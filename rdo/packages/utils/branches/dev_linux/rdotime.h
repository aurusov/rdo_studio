/*!
  \copyright (c) RDO-Team, 2011
  \file      rdotime.h
  \author    ������ ������ (rdo@rk9.bmstu.ru)
  \date      10.05.2009
  \brief     
  \indent    4T
*/

#ifndef _UTILS_RDOTIME_H_
#define _UTILS_RDOTIME_H_

// ----------------------------------------------------------------------- INCLUDES
// ----------------------------------------------------------------------- SYNOPSIS
#include "utils/namespace.h"
#include "utils/rdotypes.h"
#include "utils/rdomacros.h"
// --------------------------------------------------------------------------------

OPEN_RDO_NAMESPACE

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

CLOSE_RDO_NAMESPACE

#include "utils/rdotime.inl"

#endif // _UTILS_RDOTIME_H_
