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
#include "rdo_lib/rdo_parser/rdo_value.h"
#include "rdo_lib/rdo_parser/namespace.h"
#include "rdo_lib/rdo_parser/rdoparser_object.h"
#include "rdo_lib/rdo_runtime/rdo_type.h"
#include "rdo_lib/rdo_runtime/rdo_value.h"
// ===============================================================================

OPEN_RDO_PARSER_NAMESPACE

class RDOParser;

// ----------------------------------------------------------------------------
// ---------- RDOTypeRange
// ----------------------------------------------------------------------------
class RDOTypeRange: public RDOParserObject, public RDOParserSrcInfo
{
public:
	RDOTypeRange(PTR(RDOParser) parser);
	RDOTypeRange(PTR(RDOParser) parser, CREF(RDOTypeRange) range);
	RDOTypeRange(CREF(RDOTypeRange) range);
	RDOTypeRange(PTR(RDOParser) parser, CREF(RDOValue) min_value, CREF(RDOValue) max_value, CREF(RDOParserSrcInfo) src_info);
	virtual ~RDOTypeRange();

	rbool          isExist() const;
	CREF(RDOValue) getMin () const;
	CREF(RDOValue) getMax () const;

private:
	RDOValue m_min_value;
	RDOValue m_max_value;

	void init();
};

CLOSE_RDO_PARSER_NAMESPACE

#include "rdo_lib/rdo_parser/rdo_type_range.inl"

#endif //! _RDO_TYPE_RANGE_H_
