/*!
  \copyright (c) RDO-Team, 2011
  \file      rdo_value.h
  \author    Урусов Андрей (rdo@rk9.bmstu.ru)
  \date      
  \brief     
  \indent    4T
*/

#ifndef _CONVERTOR_RDOCONVERTER_VALUE_H_
#define _CONVERTOR_RDOCONVERTER_VALUE_H_

// ----------------------------------------------------------------------- INCLUDES
// ----------------------------------------------------------------------- SYNOPSIS
#include "converter/smr2rdox/namespace.h"
#include "converter/smr2rdox/rdo_object.h"
#include "converter/smr2rdox/rdo_type.h"
#include "simulator/runtime/rdo_value.h"
// --------------------------------------------------------------------------------

OPEN_RDO_CONVERTER_NAMESPACE

// --------------------------------------------------------------------------------
// -------------------- RDOValue
// --------------------------------------------------------------------------------
OBJECT(RDOValue) IS INSTANCE_OF(RDOParserSrcInfo)
{
DECLARE_FACTORY(RDOValue);
public:
	CREF(LPRDOType)                   type() const;
	rdoRuntime::RDOType::TypeID     typeID() const;
	CREF(rdoRuntime::RDOValue)       value() const;
	CPTR(rdoRuntime::RDOValue) operator-> () const;

	rbool defined () const;
	rbool constant() const;

	static LPRDOValue getIdentificator(CREF(tstring) identificator);

private:
	explicit RDOValue(CREF(rsint)   value, CREF(RDOParserSrcInfo) src_info);
	explicit RDOValue(CREF(ruint)   value, CREF(RDOParserSrcInfo) src_info);
	explicit RDOValue(CREF(double)  value, CREF(RDOParserSrcInfo) src_info);
	explicit RDOValue(CREF(tstring) value, CREF(RDOParserSrcInfo) src_info);

	explicit RDOValue(CREF(rdoRuntime::RDOValue) value, CREF(LPRDOType) type, CREF(RDOParserSrcInfo) src_info);
	         RDOValue(CREF(LPRDOValue) pValue);
	         RDOValue(CREF(LPRDOType) type, CREF(RDOParserSrcInfo) src_info = RDOParserSrcInfo());
	// Для t_identificator известно только имя, но не тип
	explicit RDOValue(CREF(RDOParserSrcInfo) src_info);
	// Неопределенный тип
	         RDOValue();

	rdoRuntime::RDOValue m_value;
	LPRDOType            m_type;
};

CLOSE_RDO_CONVERTER_NAMESPACE

#include "converter/smr2rdox/rdo_value.inl"

#endif // _CONVERTOR_RDOCONVERTER_VALUE_H_
