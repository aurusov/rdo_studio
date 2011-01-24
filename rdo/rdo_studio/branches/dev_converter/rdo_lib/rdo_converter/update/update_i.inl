/*
 * copyright: (c) RDO-Team, 2011
 * filename : update_i.inl
 * author   : Урусов Андрей
 * date     : 24.01.2011
 * bref     : 
 * indent   : 4T
 */

// ====================================================================== INCLUDES
// ====================================================================== SYNOPSIS
// ===============================================================================

OPEN_RDO_CONVERTER_NAMESPACE

// ----------------------------------------------------------------------------
// ---------- IDocUpdate::Position
// ----------------------------------------------------------------------------
inline IDocUpdate::Position::Position(ruint pos)
	: m_position(pos)
{}

inline IDocUpdate::Position::Position(CREF(Position) pos)
	: m_position(pos.m_position)
{}

inline ruint IDocUpdate::Position::get() const
{
	return m_position;
}

inline rbool IDocUpdate::Position::begin() const
{
	return m_position == POSITION_BEGIN;
}

inline rbool IDocUpdate::Position::end() const
{
	return m_position == POSITION_END;
}

inline rbool IDocUpdate::Position::real() const
{
	return m_position != POSITION_BEGIN && m_position != POSITION_END;
}

inline void IDocUpdate::Position::operator+= (CREF(Position) pos)
{
	if (real() && pos.real())
	{
		m_position += pos.m_position;
	}
}

inline void IDocUpdate::Position::operator-= (CREF(Position) pos)
{
	if (real() && pos.real())
	{
		m_position -= pos.m_position;
	}
}

inline IDocUpdate::Position IDocUpdate::Position::operator+ (CREF(Position) pos) const
{
	if (real() && pos.real())
	{
		return Position(m_position + pos.m_position);
	}

	NEVER_REACH_HERE;
	return Position(0);
}

inline IDocUpdate::Position IDocUpdate::Position::operator- (CREF(Position) pos) const
{
	if (real() && pos.real())
	{
		return Position(m_position - pos.m_position);
	}

	NEVER_REACH_HERE;
	return Position(0);
}

inline rbool IDocUpdate::Position::operator<= (CREF(Position) pos) const
{
	if (real() && pos.real())
	{
		return m_position <= pos.m_position;
	}

	if (begin())
	{
		return true;
	}
	else if (end())
	{
		return pos.end() ? true : false;
	}

	NEVER_REACH_HERE;
	return false;
}

inline rbool IDocUpdate::Position::operator>= (CREF(Position) pos) const
{
	if (real() && pos.real())
	{
		return m_position >= pos.m_position;
	}

	if (begin())
	{
		return pos.begin() ? true : false;
	}
	else if (end())
	{
		return true;
	}

	NEVER_REACH_HERE;
	return false;
}

inline rbool IDocUpdate::Position::operator< (CREF(Position) pos) const
{
	if (real() && pos.real())
	{
		return m_position < pos.m_position;
	}

	if (begin())
	{
		return pos.begin() ? false : true;
	}
	else if (end())
	{
		return false;
	}

	NEVER_REACH_HERE;
	return false;
}

inline rbool IDocUpdate::Position::operator> (CREF(Position) pos) const
{
	if (real() && pos.real())
	{
		return m_position > pos.m_position;
	}

	if (begin())
	{
		return false;
	}
	else if (end())
	{
		return pos.end() ? false : true;
	}

	NEVER_REACH_HERE;
	return false;
}

inline rbool IDocUpdate::Position::operator== (CREF(Position) pos) const
{
	return m_position == pos.m_position;
}

inline rbool IDocUpdate::Position::operator!= (CREF(Position) pos) const
{
	return !operator==(pos);
}

CLOSE_RDO_CONVERTER_NAMESPACE
