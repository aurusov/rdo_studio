/*
 * copyright: (c) RDO-Team, 2009
 * filename : rdo_type_range.inl
 * author   : Урусов Андрей
 * date     : 10.02.2010
 * bref     : 
 * indent   : 4T
 */

// ====================================================================== INCLUDES
// ====================================================================== SYNOPSIS
// ===============================================================================

OPEN_RDO_PARSER_NAMESPACE

template<class T>
inline RDOTypeRange::RDOTypeRange(PTR(RDOParser) parser)
	: RDOParserObject (parser)
	, RDOParserSrcInfo(      )
	, m_exist         (false )
	, m_min_value     (0     )
	, m_max_value     (0     )
{}

template<class T>
inline RDOTypeRange(PTR(RDOParser) parser, CREF(RDOTypeRange<T>) range)
	: RDOParserObject (parser           )
	, RDOParserSrcInfo(range.src_info() )
	, m_exist         (range.m_exist    )
	, m_min_value     (range.m_min_value)
	, m_max_value     (range.m_max_value)
{}

template<class T>
inline RDOTypeRange::RDOTypeRange(CREF(RDOTypeRange<T>) range)
	: RDOParserObject (range.parser()   )
	, RDOParserSrcInfo(range.src_info() )
	, m_exist         (range.m_exist    )
	, m_min_value     (range.m_min_value)
	, m_max_value     (range.m_max_value)
{}

template<class T>
inline RDOTypeRange::RDOTypeRange(PTR(RDOParser) parser, CREF(RDOParserSrcInfo) src_info)
	: RDOParserObject (parser  )
	, RDOParserSrcInfo(src_info)
	, m_exist         (false   )
	, m_min_value     (0       )
	, m_max_value     (0       )
{}

template<class T>
inline RDOTypeRange::RDOTypeRange(PTR(RDOParser) parser, CREF(T) min_value, CREF(T) max_value, CREF(RDOParserSrcInfo) src_info, CREF(YYLTYPE) max_value_pos)
	: RDOParserObject (parser   )
	, RDOParserSrcInfo(src_info )
	, m_exist         (true     )
	, m_min_value     (min_value)
	, m_max_value     (max_value)
{
	init(&max_value_pos);
}

template<class T>
inline RDOTypeRange::RDOTypeRange(PTR(RDOParser) parser, CREF(T) min_value, CREF(T) max_value)
	: RDOParserObject (parser   )
	, RDOParserSrcInfo(         )
	, m_exist         (true     )
	, m_min_value     (min_value)
	, m_max_value     (max_value)
{
	init(NULL);
}

template<class T>
inline RDOTypeRange::~RDOTypeRange()
{}

template<class T>
inline void RDOTypeRange::init(CPTR(YYLTYPE) const max_value_pos)
{
	if (max_value_pos && m_min_value > m_max_value)
	{
		parser()->error().error(*max_value_pos, _T("Левая граница диапазона должна быть меньше правой"));
	}
	setSrcText(rdo::format(_T("[%s..%s]"), rdoRuntime::RDOValue(m_min_value).getAsString().c_str(), rdoRuntime::RDOValue(m_max_value).getAsString().c_str()) );
}

template<class T>
inline rbool RDOTypeRange::isExist() const
{
	return m_exist;
}

template<class T>
inline T RDOTypeRange::getMin() const
{
	return m_min_value;
}

template<class T>
inline T RDOTypeRange::getMax() const
{
	return m_max_value;
}

CLOSE_RDO_PARSER_NAMESPACE
