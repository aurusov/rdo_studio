/*
 * copyright: (c) RDO-Team, 2009
 * filename : rdo_type_range.h
 * author   : Урусов Андрей
 * date     : 10.02.2010
 * bref     : 
 * indent   : 4T
 */

#ifndef _RDOPARSER_TYPE_RANGE_H_
#define _RDOPARSER_TYPE_RANGE_H_

// ====================================================================== INCLUDES
// ====================================================================== SYNOPSIS
#include "rdo_lib/rdo_parser/rdoparser_object.h"
#include "rdo_lib/rdo_runtime/rdo_type.h"
#include "rdo_lib/rdo_runtime/rdo_value.h"
// ===============================================================================

OPEN_RDO_PARSER_NAMESPACE

// ----------------------------------------------------------------------------
// ---------- RDOTypeRange
// ----------------------------------------------------------------------------
template<class T>
class RDOTypeRange: public RDOParserObject, public RDOParserSrcInfo
{
public:
	RDOTypeRange(PTR(RDOParser) parser);
	RDOTypeRange(PTR(RDOParser) parser, CREF(RDOTypeRange<T>) range);
	RDOTypeRange( CREF(RDOTypeRange<T>) range );
	RDOTypeRange( PTR(RDOParser) parser, CREF(RDOParserSrcInfo) src_info );
	RDOTypeRange( PTR(RDOParser) parser, CREF(T) min_value, CREF(T) max_value, CREF(RDOParserSrcInfo) src_info, CREF(YYLTYPE) max_value_pos);
	RDOTypeRange( PTR(RDOParser) parser, CREF(T) min_value, CREF(T) max_value );
	virtual ~RDOTypeRange();

	rbool isExist() const;
	T     getMin () const;
	T     getMax () const;

private:
	rbool m_exist;
	T     m_min_value;
	T     m_max_value;

	void init( const YYLTYPE* const max_value_pos );
};

CLOSE_RDO_PARSER_NAMESPACE

#include "rdo_lib/rdo_parser/rdo_type_range.inl"

#endif //! _RDOPARSER_TYPE_RANGE_H_
