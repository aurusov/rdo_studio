/*
 * copyright: (c) RDO-Team, 2011
 * filename : info.inl
 * author   : Урусов Андрей
 * date     : 09.04.2011
 * bref     : 
 * indent   : 4T
 */

// ====================================================================== INCLUDES
// ====================================================================== SYNOPSIS
// ===============================================================================

OPEN_RDO_PARSER_NAMESPACE

// ----------------------------------------------------------------------------
// ---------- TypeInfo
// ----------------------------------------------------------------------------
inline TypeInfo::TypeInfo(CREF(LPTypeInfo) pTypeInfo)
	: m_pType  (pTypeInfo->m_pType  )
	, m_srcInfo(pTypeInfo->m_srcInfo)
{}

inline TypeInfo::TypeInfo(CREF(LPRDOType) pType, CREF(RDOParserSrcInfo) srcInfo)
	: m_pType  (pType  )
	, m_srcInfo(srcInfo)
{}

inline TypeInfo::~TypeInfo()
{}

inline CREF(LPRDOType) TypeInfo::type() const
{
	return m_pType;
}

inline CREF(RDOParserSrcInfo) TypeInfo::src_info(CREF(RDOParserSrcInfo) srcInfo) const
{
	return m_srcInfo ? m_srcInfo.get() : srcInfo;
}

CLOSE_RDO_PARSER_NAMESPACE
