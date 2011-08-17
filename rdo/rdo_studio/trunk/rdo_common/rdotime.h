/*
  \copyright (c) RDO-Team, 2011
  \file      rdotime.h
  \author    Урусов Андрей (rdo@rk9.bmstu.ru)
  \date      10.05.2009
  \brief     
  \indent    4T
 */

#ifndef _RDOTIME_H_
#define _RDOTIME_H_

// ====================================================================== INCLUDES
// ====================================================================== SYNOPSIS
#include "rdo_common/rdotypes.h"
#include "rdo_common/rdomacros.h"
// ===============================================================================

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

#include "rdo_common/rdotime.inl"

#endif //! _RDOTIME_H_
