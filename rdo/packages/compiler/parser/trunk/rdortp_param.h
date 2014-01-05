/*!
  \copyright (c) RDO-Team, 2011
  \file      rdortp_param.h
  \authors   Барс Александр
  \authors   Урусов Андрей (rdo@rk9.bmstu.ru)
  \date      
  \brief     
  \indent    4T
*/

#ifndef _RDORTP_PARAM_H_
#define _RDORTP_PARAM_H_

// ----------------------------------------------------------------------- INCLUDES
// ----------------------------------------------------------------------- SYNOPSIS
#include "utils/src/smart_ptr/factory/factory.h"
#include "simulator/runtime/rdo_model_i.h"
#include "simulator/compiler/parser/param.h"
#include "simulator/compiler/parser/context/context.h"
#include "simulator/compiler/parser/context/context_find_i.h"
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
	, public Context
	, public IContextFind
{
DECLARE_FACTORY(RDORTPParam);
public:
	DECLARE_IModelStructure;
	DECLARE_IName;

private:
	RDORTPParam(CREF(LPTypeInfo) pType, CREF(LPRDOValue) pDefault, CREF(RDOParserSrcInfo) src_info);
	virtual ~RDORTPParam();

	virtual Context::FindResult onFindContext(const std::string& method, const Context::Params& params, const RDOParserSrcInfo& srcInfo) const;
};
DECLARE_POINTER(RDORTPParam);

CLOSE_RDO_PARSER_NAMESPACE

#endif // _RDORTP_PARAM_H_
