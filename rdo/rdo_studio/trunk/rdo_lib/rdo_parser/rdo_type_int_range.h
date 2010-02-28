/*
 * copyright: (c) RDO-Team, 2010
 * filename : rdo_type_int_range.h
 * author   : Урусов Андрей
 * date     : 23.02.2010
 * bref     : 
 * indent   : 4T
 */

#ifndef _RDO_TYPE_INT_RANGE_H_
#define _RDO_TYPE_INT_RANGE_H_

// ====================================================================== INCLUDES
// ====================================================================== SYNOPSIS
#include "rdo_lib/rdo_parser/rdo_type.h"
#include "rdo_lib/rdo_parser/rdo_type_range.h"
#include "rdo_common/rdosmart_ptr.h"
// ===============================================================================

OPEN_RDO_PARSER_NAMESPACE

// ----------------------------------------------------------------------------
// ---------- RDOTypeIntRange
// ----------------------------------------------------------------------------
class RDOTypeIntRange: public RDOType
{
DECLARE_FACTORY(RDOTypeIntRange);
private:
	RDOTypeIntRange(CREF(LPRDOTypeRange) range);
	virtual ~RDOTypeIntRange();

	virtual tstring              name      ()                                const;
	virtual CPTR(RDOType)        type_cast (CREF(RDOType)              from) const;
	virtual rdoRuntime::RDOValue value_cast(CREF(rdoRuntime::RDOValue) from) const;
	DECLARE_IModelStructure;

	LPRDOTypeRange m_range;
};
typedef rdo::smart_ptr<RDOTypeIntRange> LPRDOTypeIntRange;

CLOSE_RDO_PARSER_NAMESPACE

#endif //! _RDO_TYPE_INT_RANGE_H_
