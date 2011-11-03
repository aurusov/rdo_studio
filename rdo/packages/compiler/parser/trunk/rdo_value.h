/*!
  \copyright (c) RDO-Team, 2011
  \file      simulator/compiler/parser/rdo_value.h
  \author    Урусов Андрей (rdo@rk9.bmstu.ru)
  \date      
  \brief     
  \indent    4T
*/

#ifndef _RDOPARSER_VALUE_H_
#define _RDOPARSER_VALUE_H_

// ----------------------------------------------------------------------- INCLUDES
// ----------------------------------------------------------------------- SYNOPSIS
#include "utils/smart_ptr/intrusive_ptr.h"
#include "simulator/compiler/parser/namespace.h"
#include "simulator/compiler/parser/rdo_object.h"
#include "simulator/compiler/parser/type/info.h"
#include "simulator/runtime/rdo_value.h"
// --------------------------------------------------------------------------------

OPEN_RDO_PARSER_NAMESPACE

PREDECLARE_POINTER(RDOArrayValue);

// --------------------------------------------------------------------------------
// -------------------- RDOValue
// --------------------------------------------------------------------------------
OBJECT(RDOValue) IS INSTANCE_OF(RDOParserSrcInfo)
{
DECLARE_FACTORY(RDOValue);
public:
	CREF(LPTypeInfo)              typeInfo() const;
	rdoRuntime::RDOType::TypeID     typeID() const;
	CREF(rdoRuntime::RDOValue)       value() const;
	CPTR(rdoRuntime::RDOValue) operator-> () const;
	CREF(LPRDOArrayValue)         getArray() const;

	rbool defined () const;
	rbool constant() const;

	static LPRDOValue getIdentificator(CREF(tstring) identificator);

private:
	explicit RDOValue(CREF(rsint)           value, CREF(RDOParserSrcInfo) src_info);
	explicit RDOValue(CREF(ruint)           value, CREF(RDOParserSrcInfo) src_info);
	explicit RDOValue(CREF(double)          value, CREF(RDOParserSrcInfo) src_info);
	explicit RDOValue(CREF(tstring)         value, CREF(RDOParserSrcInfo) src_info);
	explicit RDOValue(CREF(LPRDOArrayValue) pValue);

	explicit RDOValue(CREF(rdoRuntime::RDOValue) value, CREF(RDOParserSrcInfo) src_info, CREF(LPTypeInfo) pType);
	         RDOValue(CREF(LPRDOValue) pValue);
	         RDOValue(CREF(LPTypeInfo) pType);
	// Для t_identificator известно только имя, но не тип
	explicit RDOValue(CREF(RDOParserSrcInfo) src_info);
	// Неопределенный тип
	         RDOValue();

	rdoRuntime::RDOValue m_value;
	LPRDOArrayValue      m_pArray;
	LPTypeInfo           m_pType;
};

CLOSE_RDO_PARSER_NAMESPACE

#include "simulator/compiler/parser/rdo_value.inl"

#endif // _RDOPARSER_VALUE_H_
