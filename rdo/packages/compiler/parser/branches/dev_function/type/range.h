/*!
  \copyright (c) RDO-Team, 2011
  \file      range.h
  \author    Урусов Андрей (rdo@rk9.bmstu.ru)
  \date      10.02.2010
  \brief     
  \indent    4T
*/

#ifndef _RDOPARSER_TYPE_RANGE_H_
#define _RDOPARSER_TYPE_RANGE_H_

// ----------------------------------------------------------------------- INCLUDES
// ----------------------------------------------------------------------- SYNOPSIS
#include "simulator/compiler/parser/rdo_object.h"
#include "simulator/compiler/parser/rdo_value.h"
#include "utils/smart_ptr/factory.h"
// --------------------------------------------------------------------------------

OPEN_RDO_PARSER_NAMESPACE

// --------------------------------------------------------------------------------
// -------------------- RDOTypeRangeRange
// --------------------------------------------------------------------------------
OBJECT(RDOTypeRangeRange) IS INSTANCE_OF(RDOParserSrcInfo)
{
DECLARE_FACTORY(RDOTypeRangeRange);
public:
	void             checkRange() const;
	void             checkValue(CREF(LPRDOValue) pValue) const;
	CREF(LPRDOValue) getMin    () const;
	CREF(LPRDOValue) getMax    () const;

private:
	RDOTypeRangeRange(CREF(LPRDOValue) pMinValue, CREF(LPRDOValue) pMaxValue, CREF(RDOParserSrcInfo) src_info);
	virtual ~RDOTypeRangeRange();

	LPRDOValue m_pMinValue;
	LPRDOValue m_pMaxValue;
};
DECLARE_POINTER(RDOTypeRangeRange);

// --------------------------------------------------------------------------------
// -------------------- RDOTypeRange
// --------------------------------------------------------------------------------
template<class T>
class RDOTypeRange: public T
{
DECLARE_FACTORY(RDOTypeRange<T>);
public:
	CREF(LPRDOTypeRangeRange) range() const;

private:
	typedef T parent_type;

	RDOTypeRange(CREF(LPRDOTypeRangeRange) range);
	virtual ~RDOTypeRange();

	LPRDOTypeRangeRange m_range;

	DECLARE_IType;
};
typedef RDOTypeRange<RDOType__int>  RDOTypeIntRange;
typedef RDOTypeRange<RDOType__real> RDOTypeRealRange;
DECLARE_POINTER(RDOTypeIntRange);
DECLARE_POINTER(RDOTypeRealRange);

CLOSE_RDO_PARSER_NAMESPACE

#include "simulator/compiler/parser/type/range.inl"

#endif // _RDOPARSER_TYPE_RANGE_H_
