/*
 * copyright: (c) RDO-Team, 2009
 * filename : rdo_value.h
 * author   : Урусов Андрей
 * date     : 
 * bref     : 
 * indent   : 4T
 */

#ifndef _RDOPARSER_VALUE_H_
#define _RDOPARSER_VALUE_H_

// ====================================================================== INCLUDES
// ====================================================================== SYNOPSIS
#include "rdo_lib/rdo_parser/namespace.h"
#include "rdo_lib/rdo_parser/rdo_object.h"
#include "rdo_lib/rdo_parser/type/type.h"
#include "rdo_lib/rdo_runtime/rdo_value.h"
// ===============================================================================

OPEN_RDO_PARSER_NAMESPACE

PREDECLARE_POINTER(RDOArrayValue);

// ----------------------------------------------------------------------------
// ---------- RDOValue
// ----------------------------------------------------------------------------
class RDOValue: public RDOParserSrcInfo
{
public:
	explicit RDOValue(CREF(rsint)           value );
	explicit RDOValue(CREF(ruint)           value );
	explicit RDOValue(CREF(double)          value );
	explicit RDOValue(CREF(tstring)         value );
	explicit RDOValue(CREF(LPRDOArrayValue) pValue);

	explicit RDOValue(CREF(rdoRuntime::RDOValue) value, CREF(LPRDOType) pType, CREF(RDOParserSrcInfo) src_info);
	         RDOValue(CREF(RDOValue) value);
	         RDOValue(CREF(LPRDOType) pType, CREF(RDOParserSrcInfo) src_info = RDOParserSrcInfo());
	// Для t_identificator известно только имя, но не тип
	explicit RDOValue(CREF(RDOParserSrcInfo) src_info);
	// Неопределенный тип
	         RDOValue();

	CREF(LPRDOType)                   type() const;
	rdoRuntime::RDOType::TypeID     typeID() const;
	CREF(rdoRuntime::RDOValue)       value() const;
	CPTR(rdoRuntime::RDOValue) operator-> () const;
	CREF(LPRDOArrayValue)         getArray() const;

	void operator= (CREF(RDOValue) value);

	rbool defined () const;
	rbool constant() const;

	static RDOValue getIdentificator(CREF(tstring) identificator);

private:
	rdoRuntime::RDOValue m_value;
	LPRDOArrayValue      m_pArray;
	LPRDOType            m_pType;
};

CLOSE_RDO_PARSER_NAMESPACE

#include "rdo_lib/rdo_parser/rdo_value.inl"

#endif //! _RDOPARSER_VALUE_H_
