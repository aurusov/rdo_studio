/*
 * copyright: (c) RDO-Team, 2009
 * filename : rdortp_param.h
 * author   : Александ Барс, Урусов Андрей
 * date     : 
 * bref     : 
 * indent   : 4T
 */

#ifndef _RDORTP_PARAM_H_
#define _RDORTP_PARAM_H_

// ====================================================================== INCLUDES
// ====================================================================== SYNOPSIS
#include "rdo_lib/rdo_parser/param.h"
#include "rdo_lib/rdo_runtime/rdo_model_i.h"
#include "rdo_common/smart_ptr/intrusive_ptr.h"
// ===============================================================================

OPEN_RDO_PARSER_NAMESPACE

// ----------------------------------------------------------------------------
// ---------- RDORTPParam
// ----------------------------------------------------------------------------
PREDECLARE_POINTER(RDORTPResType);

CLASS(RDORTPParam):
	    INSTANCE_OF      (RDOParam       )
	AND IMPLEMENTATION_OF(IModelStructure )
	AND IMPLEMENTATION_OF(IName           )
{
DECLARE_FACTORY(RDORTPParam);
public:
	DECLARE_IModelStructure;
	DECLARE_IName;

private:
	RDORTPParam(CREF(LPRDOTypeParam) pParamType, CREF(RDOValue) default, CREF(RDOParserSrcInfo) src_info);
	virtual ~RDORTPParam();
};
DECLARE_POINTER(RDORTPParam);

CLOSE_RDO_PARSER_NAMESPACE

#endif //! _RDORTP_PARAM_H_
