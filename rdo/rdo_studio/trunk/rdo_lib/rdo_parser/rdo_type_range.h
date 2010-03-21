/*
 * copyright: (c) RDO-Team, 2009
 * filename : rdo_type_range.h
 * author   : Урусов Андрей
 * date     : 10.02.2010
 * bref     : 
 * indent   : 4T
 */

#ifndef _RDO_TYPE_RANGE_H_
#define _RDO_TYPE_RANGE_H_

// ====================================================================== INCLUDES
// ====================================================================== SYNOPSIS
#include "rdo_lib/rdo_parser/rdo_object.h"
#include "rdo_lib/rdo_parser/rdo_value.h"
#include "rdo_common/rdosmart_ptr.h"
// ===============================================================================

OPEN_RDO_PARSER_NAMESPACE

class RDOParser;

// ----------------------------------------------------------------------------
// ---------- RDOTypeRangeRange
// ----------------------------------------------------------------------------
class RDOTypeRangeRange: public RDOParserSrcInfo
{
DECLARE_FACTORY(RDOTypeRangeRange);
public:
	void           checkRange()                     const throw(...);
	void           checkValue(CREF(RDOValue) value) const throw(...);
	CREF(RDOValue) getMin    () const;
	CREF(RDOValue) getMax    () const;

private:
	RDOTypeRangeRange(CREF(RDOValue) min_value, CREF(RDOValue) max_value, CREF(RDOParserSrcInfo) src_info);
	virtual ~RDOTypeRangeRange();

	RDOValue m_min_value;
	RDOValue m_max_value;
};
DECLARE_POINTER(RDOTypeRangeRange);

// ----------------------------------------------------------------------------
// ---------- RDOTypeRange
// ----------------------------------------------------------------------------
template<class T>
class RDOTypeRange: public T
{
DECLARE_FACTORY(RDOTypeRange<T>);
public:
	CREF(LPRDOTypeRangeRange) range() const;

private:
	RDOTypeRange(CREF(LPRDOTypeRangeRange) range);
	virtual ~RDOTypeRange();

	virtual tstring                  name      () const;
	virtual RDOValue                 value_cast(CREF(RDOValue)  from, CREF(RDOParserSrcInfo) to_src_info, CREF(RDOParserSrcInfo) src_info) const;
	virtual PTR(rdoRuntime::RDOCalc) calc_cast (PTR(rdoRuntime::RDOCalc) pCalc, CREF(LPRDOType) pType) const;

	LPRDOTypeRangeRange m_range;
};
typedef RDOTypeRange<RDOType__int>  RDOTypeIntRange;
typedef RDOTypeRange<RDOType__real> RDOTypeRealRange;
DECLARE_POINTER(RDOTypeIntRange);
DECLARE_POINTER(RDOTypeRealRange);

CLOSE_RDO_PARSER_NAMESPACE

#include "rdo_lib/rdo_parser/rdo_type_range.inl"

#endif //! _RDO_TYPE_RANGE_H_
