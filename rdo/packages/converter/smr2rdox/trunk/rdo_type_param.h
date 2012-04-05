/*!
  \copyright (c) RDO-Team, 2011
  \file      rdo_type_param.h
  \author    Урусов Андрей (rdo@rk9.bmstu.ru)
  \date      12.02.2010
  \brief     
  \indent    4T
*/

#ifndef _CONVERTOR_RDOCONVERTER_TYPE_PARAM_H_
#define _CONVERTOR_RDOCONVERTER_TYPE_PARAM_H_

// ----------------------------------------------------------------------- INCLUDES
// ----------------------------------------------------------------------- SYNOPSIS
#include "converter/smr2rdox/rdo_type.h"
#include "converter/smr2rdox/rdo_value.h"
#include "converter/smr2rdox/rdo_object.h"
#include "simulator/runtime/rdo_model_i.h"
#include "utils/smart_ptr/factory.h"
// --------------------------------------------------------------------------------

OPEN_RDO_CONVERTER_NAMESPACE

// --------------------------------------------------------------------------------
// -------------------- RDOTypeParam
// --------------------------------------------------------------------------------
OBJECT(RDOTypeParam)
	IS INSTANCE_OF       (RDOParserSrcInfo)
	AND IMPLEMENTATION_OF(IModelStructure )
{
DECLARE_FACTORY(RDOTypeParam);
public:
	LPRDOType   type      ()                        const;
	LPRDOValue  value_cast(CREF(LPRDOValue) pValue) const;

	DECLARE_IModelStructure;

protected:
	RDOTypeParam(CREF(LPRDOType) pType, CREF(RDOParserSrcInfo) src_info);
	virtual ~RDOTypeParam();

private:
	LPRDOType m_pType;
};

CLOSE_RDO_CONVERTER_NAMESPACE

#endif // _CONVERTOR_RDOCONVERTER_TYPE_PARAM_H_
