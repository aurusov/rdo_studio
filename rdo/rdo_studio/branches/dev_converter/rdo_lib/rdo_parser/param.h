/*
 * copyright: (c) RDO-Team, 2011
 * filename : param.h
 * author   : Урусов Андрей
 * date     : 09.01.2011
 * bref     : 
 * indent   : 4T
 */

#ifndef _PARSER_PARAM_H_
#define _PARSER_PARAM_H_

// ====================================================================== INCLUDES
// ====================================================================== SYNOPSIS
#include "rdo_lib/rdo_parser/rdo_object.h"
#include "rdo_lib/rdo_parser/rdo_value.h"
#include "rdo_lib/rdo_parser/rdo_type_param.h"
// ===============================================================================

OPEN_RDO_PARSER_NAMESPACE

// ----------------------------------------------------------------------------
// ---------- RDOParam
// ----------------------------------------------------------------------------
OBJECT(RDOParam) IS INSTANCE_OF(RDOParserSrcInfo)
{
DECLARE_FACTORY(RDOParam)
public:
	CREF(tstring)  name      () const { return src_info().src_text(); }
	LPRDOTypeParam getType   () const { return m_pType;               }
	CREF(RDOValue) getDefault() const { return m_default;             }

protected:
	RDOParam(CREF(tstring)          name,     CREF(LPRDOTypeParam) pType, RDOValue default = RDOValue());
	RDOParam(CREF(RDOParserSrcInfo) src_info, CREF(LPRDOTypeParam) pType, RDOValue default = RDOValue());
	virtual ~RDOParam();

private:
	LPRDOTypeParam m_pType;
	RDOValue       m_default;

	void checkDefault();
};

CLOSE_RDO_PARSER_NAMESPACE

#endif //! _PARSER_PARAM_H_
