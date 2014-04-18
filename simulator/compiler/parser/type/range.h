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
#include "utils/src/smart_ptr/factory/factory.h"
// --------------------------------------------------------------------------------

OPEN_RDO_PARSER_NAMESPACE

// --------------------------------------------------------------------------------
// -------------------- RDOTypeRangeRange
// --------------------------------------------------------------------------------
PREDECLARE_POINTER(RDOTypeRangeRange);
class RDOTypeRangeRange
	: public rdo::counter_reference
	, public RDOParserSrcInfo
{
DECLARE_FACTORY(RDOTypeRangeRange);
public:
	void              checkRange() const;
	void              checkValue(const LPRDOValue& pValue) const;
	const LPRDOValue& getMin    () const;
	const LPRDOValue& getMax    () const;

private:
	RDOTypeRangeRange(const LPRDOValue& pMinValue, const LPRDOValue& pMaxValue, const RDOParserSrcInfo& src_info);
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
	const LPRDOTypeRangeRange& range() const;

private:
	typedef T parent_type;

	RDOTypeRange(const LPRDOTypeRangeRange& range);
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
