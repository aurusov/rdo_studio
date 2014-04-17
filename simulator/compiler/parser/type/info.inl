/*!
  \copyright (c) RDO-Team, 2011
  \file      info.inl
  \author    Урусов Андрей (rdo@rk9.bmstu.ru)
  \date      09.04.2011
  \brief     
  \indent    4T
*/

// ----------------------------------------------------------------------- INCLUDES
// ----------------------------------------------------------------------- SYNOPSIS
// --------------------------------------------------------------------------------

OPEN_RDO_PARSER_NAMESPACE

// --------------------------------------------------------------------------------
// -------------------- TypeInfo
// --------------------------------------------------------------------------------
template <class T>
inline LPTypeInfo TypeInfo::create(const RDOParserSrcInfo& srcInfo)
{
	LPRDOType pType = rdo::Factory<T>::create();
	ASSERT(pType);

	LPTypeInfo pTypeInfo = rdo::Factory<TypeInfo>::create(pType, srcInfo);
	ASSERT(pTypeInfo);

	return pTypeInfo;
}

inline TypeInfo::~TypeInfo()
{}

inline const LPRDOType& TypeInfo::type() const
{
	return m_pType;
}

inline RDOParserSrcInfo TypeInfo::src_info() const
{
	return m_srcInfo ? m_srcInfo.get() : RDOParserSrcInfo();
}

inline const RDOParserSrcInfo& TypeInfo::src_info(const RDOParserSrcInfo& srcInfo) const
{
	return m_srcInfo ? m_srcInfo.get() : srcInfo;
}

CLOSE_RDO_PARSER_NAMESPACE
