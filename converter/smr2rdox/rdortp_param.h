/*!
  \copyright (c) RDO-Team, 2011
  \file      rdortp_param.h
  \authors   Барс Александр
  \authors   Урусов Андрей (rdo@rk9.bmstu.ru)
  \date      
  \brief     
  \indent    4T
*/

#ifndef _CONVERTOR_RDORTP_PARAM_H_
#define _CONVERTOR_RDORTP_PARAM_H_

// ----------------------------------------------------------------------- INCLUDES
// ----------------------------------------------------------------------- SYNOPSIS
#include "converter/smr2rdox/param.h"
#include "simulator/runtime/rdo_model_i.h"
#include "utils/src/smart_ptr/factory/factory.h"
// --------------------------------------------------------------------------------

OPEN_RDO_CONVERTER_SMR2RDOX_NAMESPACE

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
	RDORTPParam(const LPRDOTypeParam& pParamType, const LPRDOValue& pDefault, const RDOParserSrcInfo& src_info);
	virtual ~RDORTPParam();
};
DECLARE_POINTER(RDORTPParam);

CLOSE_RDO_CONVERTER_SMR2RDOX_NAMESPACE

#endif // _CONVERTOR_RDORTP_PARAM_H_
