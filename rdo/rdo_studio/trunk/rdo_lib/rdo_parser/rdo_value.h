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
#include "rdo_lib/rdo_runtime/rdo_value.h"
// ===============================================================================

OPEN_RDO_PARSER_NAMESPACE

// ----------------------------------------------------------------------------
// ---------- RDOValue
// ----------------------------------------------------------------------------
class RDOType;
DECLARE_POINTER(RDOType);

class RDOValue: public RDOParserSrcInfo
{
public:
	// Для атомарных типов, иначе throw в RDOType::getTypeByID
//	RDOValue(CREF(rdoRuntime::RDOValue) value, CREF(RDOParserSrcInfo) src_info = RDOParserSrcInfo());
	explicit RDOValue(CREF(rsint) value);
	explicit RDOValue(CREF(ruint) value);

	RDOValue(CREF(rdoRuntime::RDOValue) value, CREF(LPRDOType) type, CREF(RDOParserSrcInfo) src_info);
	RDOValue(CREF(RDOValue) value);
	RDOValue(CREF(LPRDOType) type, CREF(RDOParserSrcInfo) src_info = RDOParserSrcInfo());
	// Для t_identificator известно только имя, но не тип
	RDOValue(CREF(RDOParserSrcInfo) src_info);
	// Неопределенный тип
	RDOValue();

	CREF(LPRDOType)                   type() const;
	rdoRuntime::RDOType::TypeID     typeID() const;
	CREF(rdoRuntime::RDOValue)       value() const;
	CPTR(rdoRuntime::RDOValue) operator-> () const;

	rbool defined () const;
	rbool constant() const;

	static RDOValue getIdentificator(CREF(tstring) identificator);

private:
	rdoRuntime::RDOValue m_value;
	LPRDOType            m_type;
};

CLOSE_RDO_PARSER_NAMESPACE

#include "rdo_lib/rdo_parser/rdo_value.inl"

#endif //! _RDOPARSER_VALUE_H_
