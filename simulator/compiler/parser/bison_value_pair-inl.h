// ----------------------------------------------------------------------- INCLUDES
// ----------------------------------------------------------------------- SYNOPSIS
// --------------------------------------------------------------------------------

OPEN_RDO_PARSER_NAMESPACE

// --------------------------------------------------------------------------------
// -------------------- BisonValuePair
// --------------------------------------------------------------------------------
template <class T1, class T2>
inline BisonValuePair<T1, T2>::BisonValuePair(const T1& first, const T2& second)
	: m_value(std::make_pair(first, second))
{}

template <class T1, class T2>
inline BisonValuePair<T1, T2>::~BisonValuePair()
{}

template <class T1, class T2>
inline const T1& BisonValuePair<T1, T2>::getFirst() const
{
	return m_value.first;
}

template <class T1, class T2>
inline const T2& BisonValuePair<T1, T2>::getSecond() const
{
	return m_value.second;
}

CLOSE_RDO_PARSER_NAMESPACE
