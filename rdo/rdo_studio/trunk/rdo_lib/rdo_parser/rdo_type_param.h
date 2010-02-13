/*
 * copyright: (c) RDO-Team, 2009
 * filename : rdo_type_param.h
 * author   : Урусов Андрей
 * date     : 12.02.2010
 * bref     : 
 * indent   : 4T
 */

#ifndef _RDOPARSER_TYPE_PARAM_H_
#define _RDOPARSER_TYPE_PARAM_H_

// ====================================================================== INCLUDES
// ====================================================================== SYNOPSIS
#include "rdo_lib/rdo_parser/rdo_type.h"
#include "rdo_lib/rdo_parser/rdo_object.h"
// ===============================================================================

OPEN_RDO_PARSER_NAMESPACE

class RDOTypeRange;

// ----------------------------------------------------------------------------
// ---------- RDOTypeParam
// ----------------------------------------------------------------------------
class RDOTypeParam: public RDOType, public RDOParserSrcInfo
{
public:
	CPTR(RDOTypeRange) range() const { return m_range; }

protected:
	RDOTypeParam(CREF(rdoRuntime::RDOType) type, CREF(RDOParserSrcInfo) src_info);
	virtual ~RDOTypeParam();

	PTR(RDOTypeRange) m_range;
};

// ----------------------------------------------------------------------------
// ---------- RDOTypeParamInt
// ----------------------------------------------------------------------------
class RDOTypeParamInt: RDOTypeParam
{
public:
	RDOTypeParamInt(PTR(RDOTypeRange) range, CREF(RDOParserSrcInfo) src_info);
};

CLOSE_RDO_PARSER_NAMESPACE

#endif //! _RDOPARSER_TYPE_PARAM_H_
