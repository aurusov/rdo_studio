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
class RDOTypeParam: public RDOType, public RDOParserObject, public RDOParserSrcInfo
{
public:
	RDOTypeParam(CREF(rdoRuntime::RDOType) type, CPTR(RDOParserObject) parent, CREF(RDOParserSrcInfo) src_info);
	virtual ~RDOTypeParam();

	CPTR(RDOTypeRange) range() const { return m_range; }

protected:
	PTR(RDOTypeRange) m_range;
};

CLOSE_RDO_PARSER_NAMESPACE

#endif //! _RDOPARSER_TYPE_PARAM_H_
