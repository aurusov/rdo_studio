/*!
  \copyright (c) RDO-Team, 2011
  \file      rdo_type_range.h
  \author    Урусов Андрей (rdo@rk9.bmstu.ru)
  \date      10.02.2010
  \brief     
  \indent    4T
*/

#ifndef _CONVERTOR_RDO_TYPE_RANGE_H_
#define _CONVERTOR_RDO_TYPE_RANGE_H_

// ----------------------------------------------------------------------- INCLUDES
// ----------------------------------------------------------------------- SYNOPSIS
#include "converter/smr2rdox/rdo_object.h"
#include "converter/smr2rdox/rdo_value.h"
#include "utils/smart_ptr/factory.h"
// --------------------------------------------------------------------------------

OPEN_RDO_CONVERTER_NAMESPACE

class Converter;

// --------------------------------------------------------------------------------
// -------------------- RDOTypeRangeRange
// --------------------------------------------------------------------------------
OBJECT(RDOTypeRangeRange) IS INSTANCE_OF(RDOParserSrcInfo)
{
DECLARE_FACTORY(RDOTypeRangeRange);
public:
	void             checkRange()                        const;
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

	LPRDOTypeRangeRange m_pRange;

	DECLARE_ITypeConverter;
};
typedef RDOTypeRange<RDOType__int>  RDOTypeIntRange;
typedef RDOTypeRange<RDOType__real> RDOTypeRealRange;
DECLARE_POINTER(RDOTypeIntRange);
DECLARE_POINTER(RDOTypeRealRange);

CLOSE_RDO_CONVERTER_NAMESPACE

#include "converter/smr2rdox/rdo_type_range.inl"

#endif // _CONVERTOR_RDO_TYPE_RANGE_H_
