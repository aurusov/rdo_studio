/*
 * copyright: (c) RDO-Team, 2009
 * filename : type_param.h
 * author   : Урусов Андрей
 * date     : 12.02.2010
 * bref     : 
 * indent   : 4T
 */

#ifndef _RDOPARSER_TYPE_PARAM_H_
#define _RDOPARSER_TYPE_PARAM_H_

// ====================================================================== INCLUDES
// ====================================================================== SYNOPSIS
#include "rdo_lib/rdo_parser/type/type.h"
#include "rdo_lib/rdo_parser/rdo_value.h"
#include "rdo_lib/rdo_parser/rdo_object.h"
#include "rdo_lib/rdo_runtime/rdo_model_i.h"
#include "rdo_common/smart_ptr/intrusive_ptr.h"
// ===============================================================================

OPEN_RDO_PARSER_NAMESPACE

// ----------------------------------------------------------------------------
// ---------- RDOTypeParam
// ----------------------------------------------------------------------------
OBJECT(RDOTypeParam)
	IS INSTANCE_OF       (RDOParserSrcInfo)
	AND IMPLEMENTATION_OF(IModelStructure )
{
DECLARE_FACTORY(RDOTypeParam);
public:
	LPRDOType type      ()                     const;
	RDOValue  value_cast(CREF(RDOValue) value) const;

	DECLARE_IModelStructure;

protected:
	RDOTypeParam(CREF(LPRDOType) type, CREF(RDOParserSrcInfo) src_info);
	virtual ~RDOTypeParam();

private:
	LPRDOType m_type;
};

CLOSE_RDO_PARSER_NAMESPACE

#endif //! _RDOPARSER_TYPE_PARAM_H_
