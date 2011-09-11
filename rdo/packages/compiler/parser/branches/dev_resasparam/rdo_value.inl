/*!
  \copyright (c) RDO-Team, 2011
  \file      rdo_value.inl
  \author    Урусов Андрей (rdo@rk9.bmstu.ru)
  \date      21.04.2010
  \brief     
  \indent    4T
*/

// ----------------------------------------------------------------------- INCLUDES
// ----------------------------------------------------------------------- SYNOPSIS
#include "simulator/compiler/parser/rdo_array.h"
#include "simulator/compiler/parser/type/atom.h"
// --------------------------------------------------------------------------------

OPEN_RDO_PARSER_NAMESPACE

inline RDOValue::RDOValue(CREF(rsint) value, CREF(RDOParserSrcInfo) src_info)
	: RDOParserSrcInfo(src_info)
	, m_value         (value   )
{
	m_pType = rdo::Factory<TypeInfo>::create(
		rdo::Factory<RDOType__int>::create(),
		src_info
	);
}

inline RDOValue::RDOValue(CREF(ruint) value, CREF(RDOParserSrcInfo) src_info)
	: RDOParserSrcInfo(src_info)
	, m_value         (value   )
{
	m_pType = rdo::Factory<TypeInfo>::create(
		rdo::Factory<RDOType__int>::create(),
		src_info
	);
}

inline RDOValue::RDOValue(CREF(double) value, CREF(RDOParserSrcInfo) src_info)
	: RDOParserSrcInfo(src_info)
	, m_value         (value   )
{
	m_pType = rdo::Factory<TypeInfo>::create(
		rdo::Factory<RDOType__real>::create(),
		src_info
	);
}

inline RDOValue::RDOValue(CREF(tstring) value, CREF(RDOParserSrcInfo) src_info)
	: RDOParserSrcInfo(src_info)
	, m_value         (value   )
{
	m_pType = rdo::Factory<TypeInfo>::create(
		rdo::Factory<RDOType__string>::create(),
		src_info
	);
}

inline RDOValue::RDOValue(CREF(RDOValue) value)
	: RDOParserSrcInfo(value.src_info())
	, m_value         (value.m_value   )
	, m_pType         (value.m_pType   )
	, m_pArray        (value.m_pArray  )
{}

CLOSE_RDO_PARSER_NAMESPACE
