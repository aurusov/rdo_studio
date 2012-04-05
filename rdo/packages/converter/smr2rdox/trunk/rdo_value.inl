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
// --------------------------------------------------------------------------------

OPEN_RDO_CONVERTER_NAMESPACE

inline RDOValue::RDOValue(CREF(rsint) value, CREF(RDOParserSrcInfo) src_info)
	: RDOParserSrcInfo(src_info)
	, m_value(value)
	, m_type (rdo::Factory<RDOType__int>::create())
{}

inline RDOValue::RDOValue(CREF(ruint) value, CREF(RDOParserSrcInfo) src_info)
	: RDOParserSrcInfo(src_info)
	, m_value(value)
	, m_type (rdo::Factory<RDOType__int>::create())
{}

inline RDOValue::RDOValue(CREF(double) value, CREF(RDOParserSrcInfo) src_info)
	: RDOParserSrcInfo(src_info)
	, m_value(value )
	, m_type (rdo::Factory<RDOType__real>::create())
{}

inline RDOValue::RDOValue(CREF(tstring) value, CREF(RDOParserSrcInfo) src_info)
	: RDOParserSrcInfo(src_info)
	, m_value         (value   )
	, m_type          (rdo::Factory<RDOType__string>::create())
{}

inline RDOValue::RDOValue(CREF(LPRDOValue) pValue)
	: RDOParserSrcInfo(pValue->src_info())
	, m_value         (pValue->m_value   )
	, m_type          (pValue->m_type    )
{}

CLOSE_RDO_CONVERTER_NAMESPACE
