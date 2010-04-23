/*
 * copyright: (c) RDO-Team, 2009
 * filename : rdo_value.inl
 * author   : Урусов Андрей
 * date     : 21.04.2010
 * bref     : 
 * indent   : 4T
 */

// ====================================================================== INCLUDES
// ====================================================================== SYNOPSIS
#include "rdo_lib/rdo_parser/rdo_type.h"
// ===============================================================================

OPEN_RDO_PARSER_NAMESPACE

//template<class T>
//inline RDOValue::RDOValue(CREF(T) value)
//{
//	STATIC_ASSERT(Dont_use_this_type_for_RDOValue);
//}
//

inline RDOValue::RDOValue(CREF(rsint) value)
	: m_value(value)
	, m_type (g_int)
{}

inline RDOValue::RDOValue(CREF(ruint) value)
	: m_value(value)
	, m_type (g_int)
{}

inline RDOValue::RDOValue(CREF(double) value)
	: m_value(value )
	, m_type (g_real)
{}

inline RDOValue::RDOValue(CREF(RDOValue) value)
	: RDOParserSrcInfo(value.src_info())
	, m_value         (value.value()   )
	, m_type          (value.type()    )
{}

CLOSE_RDO_PARSER_NAMESPACE
