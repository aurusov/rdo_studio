#ifndef _RDORTP_PARAM_H_
#define _RDORTP_PARAM_H_

// ----------------------------------------------------------------------- INCLUDES
// ----------------------------------------------------------------------- SYNOPSIS
#include "utils/src/smart_ptr/factory/factory.h"
#include "simulator/runtime/rdo_model_i.h"
#include "simulator/compiler/parser/param.h"
// --------------------------------------------------------------------------------

OPEN_RDO_PARSER_NAMESPACE

// --------------------------------------------------------------------------------
// -------------------- RDORTPParam
// --------------------------------------------------------------------------------
PREDECLARE_POINTER(RDORTPResType);

class RDORTPParam
	: public RDOParam
	, public IModelStructure
	, public IName
{
DECLARE_FACTORY(RDORTPParam);
public:
	DECLARE_IModelStructure;
	DECLARE_IName;

private:
	RDORTPParam(const LPTypeInfo& pType, const LPRDOValue& pDefault, const RDOParserSrcInfo& src_info);
	virtual ~RDORTPParam();
};
DECLARE_POINTER(RDORTPParam);

CLOSE_RDO_PARSER_NAMESPACE

#endif // _RDORTP_PARAM_H_
