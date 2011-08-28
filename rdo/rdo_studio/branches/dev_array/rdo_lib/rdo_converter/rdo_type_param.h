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
#include "rdo_lib/rdo_converter/rdo_type.h"
#include "rdo_lib/rdo_converter/rdo_value.h"
#include "rdo_lib/rdo_converter/rdo_object.h"
#include "rdo_lib/rdo_runtime/rdo_model_i.h"
#include "rdo_common/smart_ptr/intrusive_ptr.h"
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
	LPRDOType type      ()                     const;
	RDOValue  value_cast(CREF(RDOValue) value) const;

	DECLARE_IModelStructure;

protected:
	RDOTypeParam(CREF(LPRDOType) type, CREF(RDOParserSrcInfo) src_info);
	virtual ~RDOTypeParam();

private:
	LPRDOType m_type;
};

CLOSE_RDO_CONVERTER_NAMESPACE

#endif //! _CONVERTOR_RDOCONVERTER_TYPE_PARAM_H_
