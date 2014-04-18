/*!
  \copyright (c) RDO-Team, 2011-2012
  \file      bison_value_pair.inl
  \authors   Чирков Михаил
  \authors   Урусов Андрей (rdo@rk9.bmstu.ru)
  \date      02.12.2010
  \brief     
  \indent    4T
*/

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
