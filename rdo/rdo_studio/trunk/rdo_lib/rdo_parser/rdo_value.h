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
#include "rdo_lib/rdo_parser/rdo_type.h"
#include "rdo_lib/rdo_parser/rdoparser_object.h"
#include "rdo_lib/rdo_runtime/rdo_value.h"
// ===============================================================================

OPEN_RDO_PARSER_NAMESPACE

// ----------------------------------------------------------------------------
// ---------- RDOValue
// ----------------------------------------------------------------------------
class RDOValue: public RDOParserSrcInfo
{
public:
	// Для атомарных типов, иначе throw в RDOType::getTypeByID
	RDOValue(CREF(rdoRuntime::RDOValue) value, CREF(RDOParserSrcInfo) src_info = RDOParserSrcInfo())
		: RDOParserSrcInfo(src_info                                    )
		, m_value         (value                                       )
		, m_type          (&RDOType::getTypeByID(value.type().typeID()))
	{}
	RDOValue(CREF(rdoRuntime::RDOValue) value, CREF(RDOType) type, CREF(RDOParserSrcInfo) src_info)
		: RDOParserSrcInfo(src_info)
		, m_value         (value   )
		, m_type          (&type   )
	{}
	RDOValue(CREF(RDOValue) value)
		: RDOParserSrcInfo(value.src_info())
		, m_value         (value.value()   )
		, m_type          (&value.type()   )
	{}
	RDOValue(CREF(RDOType) type, CREF(RDOParserSrcInfo) src_info = RDOParserSrcInfo())
		: RDOParserSrcInfo(src_info   )
		, m_value         (type.type())
		, m_type          (&type      )
	{}
	// Для t_identificator известно только имя, но не тип
	RDOValue(CREF(RDOParserSrcInfo) src_info)
		: RDOParserSrcInfo(src_info)
		, m_value         (rdoRuntime::RDOValue(src_info.src_text(), rdoRuntime::g_identificator))
		, m_type          (&rdoParse::g_identificator                                            )
	{}
	// Неопределенный тип
	RDOValue()
		: RDOParserSrcInfo()
		, m_value         (rdoRuntime::RDOValue(rdoRuntime::g_unknow))
		, m_type          (&rdoParse::g_unknow                       )
	{}

	CREF(RDOType)                     type() const { return *m_type;          }
	rdoRuntime::RDOType::TypeID     typeID() const { return type()->typeID(); }
	CREF(rdoRuntime::RDOValue)       value() const { return  m_value;         }
	CPTR(rdoRuntime::RDOValue) operator-> () const { return &m_value;         }

	rbool defined () const { return m_value.typeID() != rdoRuntime::RDOType::t_unknow; }
	rbool constant() const
	{
		return
			m_value.typeID() == rdoRuntime::RDOType::t_int  ||
			m_value.typeID() == rdoRuntime::RDOType::t_real ||
			m_value.typeID() == rdoRuntime::RDOType::t_bool ||
			m_value.typeID() == rdoRuntime::RDOType::t_string;
	}

	static RDOValue getIdentificator(CREF(tstring) identificator)
	{
		return RDOValue(RDOParserSrcInfo(identificator));
	}

private:
	rdoRuntime::RDOValue m_value;
	CPTR(RDOType)        m_type;
};

CLOSE_RDO_PARSER_NAMESPACE

#endif //! _RDOPARSER_VALUE_H_
