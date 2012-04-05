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
	CREF(tstring)    name      () const { return src_info().src_text(); }
	LPRDOTypeParam   getType   () const { return m_pType;               }
	CREF(LPRDOValue) getDefault() const { return m_pDefault;            }

protected:
	RDOParam(CREF(tstring)          name,     CREF(LPRDOTypeParam) pType, CREF(LPRDOValue) pDefault = LPRDOValue(NULL));
	RDOParam(CREF(RDOParserSrcInfo) src_info, CREF(LPRDOTypeParam) pType, CREF(LPRDOValue) pDefault = LPRDOValue(NULL));
	virtual ~RDOParam();

private:
	LPRDOTypeParam m_pType;
	LPRDOValue     m_pDefault;

	void checkDefault();
};

CLOSE_RDO_CONVERTER_NAMESPACE

#endif // _CONVERTOR_PARAM_H_
