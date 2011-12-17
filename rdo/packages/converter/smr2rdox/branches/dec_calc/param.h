/*!
  \copyright (c) RDO-Team, 2011
  \file      param.h
  \author    Урусов Андрей (rdo@rk9.bmstu.ru)
  \date      09.01.2011
  \brief     
  \indent    4T
*/

#ifndef _CONVERTOR_PARAM_H_
#define _CONVERTOR_PARAM_H_

// ----------------------------------------------------------------------- INCLUDES
// ----------------------------------------------------------------------- SYNOPSIS
#include "converter/smr2rdox/rdo_object.h"
#include "converter/smr2rdox/rdo_value.h"
#include "converter/smr2rdox/rdo_type_param.h"
// --------------------------------------------------------------------------------

OPEN_RDO_CONVERTER_NAMESPACE

// --------------------------------------------------------------------------------
// -------------------- RDOParam
// --------------------------------------------------------------------------------
OBJECT(RDOParam) IS INSTANCE_OF(RDOParserSrcInfo)
{
DECLARE_FACTORY(RDOParam)
public:
	CREF(tstring)  name      () const { return src_info().src_text(); }
	LPRDOTypeParam getType   () const { return m_pType;               }
	CREF(RDOValue) getDefault() const { return m_default;             }

protected:
	RDOParam(CREF(tstring)          name,     CREF(LPRDOTypeParam) pType, RDOValue defaultValue = RDOValue());
	RDOParam(CREF(RDOParserSrcInfo) src_info, CREF(LPRDOTypeParam) pType, RDOValue defaultValue = RDOValue());
	virtual ~RDOParam();

private:
	LPRDOTypeParam m_pType;
	RDOValue       m_default;

	void checkDefault();
};

CLOSE_RDO_CONVERTER_NAMESPACE

#endif // _CONVERTOR_PARAM_H_
