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
#include "rdo_lib/rdo_runtime/rdo_type.h"
#include "rdo_lib/rdo_runtime/rdo_value.h"
#include "rdo_common/rdosmart_ptr.h"
// ===============================================================================

OPEN_RDO_PARSER_NAMESPACE

class RDOParser;

// ----------------------------------------------------------------------------
// ---------- RDOTypeRange
// ----------------------------------------------------------------------------
class RDOTypeRange: public RDOParserSrcInfo
{
DECLARE_FACTORY(RDOTypeRange);
public:
	void           check  ();
	rbool          isExist() const;
	CREF(RDOValue) getMin () const;
	CREF(RDOValue) getMax () const;

private:
	RDOTypeRange(CREF(RDOValue) min_value, CREF(RDOValue) max_value, CREF(RDOParserSrcInfo) src_info);
	virtual ~RDOTypeRange();

	RDOValue m_min_value;
	RDOValue m_max_value;
};
DECLARE_POINTER(RDOTypeRange);

CLOSE_RDO_PARSER_NAMESPACE

#include "rdo_lib/rdo_parser/rdo_type_range.inl"

#endif //! _RDO_TYPE_RANGE_H_
