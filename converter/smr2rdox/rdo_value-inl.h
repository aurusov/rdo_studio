/*!
  \copyright (c) RDO-Team, 2011
  \file      rdo_value-inl.h
  \author    Урусов Андрей (rdo@rk9.bmstu.ru)
  \date      21.04.2010
  \brief
  \indent    4T
*/

// ----------------------------------------------------------------------- INCLUDES
// ----------------------------------------------------------------------- SYNOPSIS
// --------------------------------------------------------------------------------

OPEN_RDO_CONVERTER_SMR2RDOX_NAMESPACE

inline RDOValue::RDOValue(const int& value, const RDOParserSrcInfo& src_info)
	: RDOParserSrcInfo(src_info)
	, m_value(value)
	, m_type (rdo::Factory<RDOType__int>::create())
{}

inline RDOValue::RDOValue(const std::size_t& value, const RDOParserSrcInfo& src_info)
	: RDOParserSrcInfo(src_info)
	, m_value(value)
	, m_type (rdo::Factory<RDOType__int>::create())
{}

inline RDOValue::RDOValue(const double& value, const RDOParserSrcInfo& src_info)
	: RDOParserSrcInfo(src_info)
	, m_value(value )
	, m_type (rdo::Factory<RDOType__real>::create())
{}

inline RDOValue::RDOValue(const std::string& value, const RDOParserSrcInfo& src_info)
	: RDOParserSrcInfo(src_info)
	, m_value         (value   )
	, m_type          (rdo::Factory<RDOType__string>::create())
{}

inline RDOValue::RDOValue(const LPRDOValue& pValue)
	: RDOParserSrcInfo(pValue->src_info())
	, m_value         (pValue->m_value   )
	, m_type          (pValue->m_type    )
{}

CLOSE_RDO_CONVERTER_SMR2RDOX_NAMESPACE
