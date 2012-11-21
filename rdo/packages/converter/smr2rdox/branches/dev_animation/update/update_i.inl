/*!
  \copyright (c) RDO-Team, 2011
  \file      update_i.inl
  \author    Урусов Андрей (rdo@rk9.bmstu.ru)
  \date      24.01.2011
  \brief     
  \indent    4T
*/

// ----------------------------------------------------------------------- INCLUDES
// ----------------------------------------------------------------------- SYNOPSIS
// --------------------------------------------------------------------------------

OPEN_RDO_CONVERTER_SMR2RDOX_NAMESPACE

// --------------------------------------------------------------------------------
// -------------------- IDocUpdate::Position
// --------------------------------------------------------------------------------
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
	switch(get())
	{
	case POSITION_BEGIN: return true;
	case POSITION_END  : switch (pos.get())
	                     {
	                     case POSITION_BEGIN: return false;
	                     case POSITION_END  : return true;
	                     default            : return false;
	                     }
	default            : switch (pos.get())
	                     {
	                     case POSITION_BEGIN: return false;
	                     case POSITION_END  : return true;
	                     default            : return m_position <= pos.m_position;
	                     }
	}
}

inline rbool IDocUpdate::Position::operator>= (CREF(Position) pos) const
{
	switch(get())
	{
	case POSITION_BEGIN: switch (pos.get())
	                     {
	                     case POSITION_BEGIN: return true;
	                     case POSITION_END  : return false;
	                     default            : return false;
	                     }
	case POSITION_END  : return true;
	default            : switch (pos.get())
	                     {
	                     case POSITION_BEGIN: return true;
	                     case POSITION_END  : return false;
	                     default            : return m_position >= pos.m_position;
	                     }
	}
}

inline rbool IDocUpdate::Position::operator< (CREF(Position) pos) const
{
	switch(get())
	{
	case POSITION_BEGIN: switch (pos.get())
	                     {
	                     case POSITION_BEGIN: NEVER_REACH_HERE; return false;
	                     case POSITION_END  : return true;
	                     default            : return true;
	                     }
	case POSITION_END  : switch (pos.get())
	                     {
	                     case POSITION_BEGIN: return false;
	                     case POSITION_END  : NEVER_REACH_HERE; return false;
	                     default            : return false;
	                     }
	default            : switch (pos.get())
	                     {
	                     case POSITION_BEGIN: return false;
	                     case POSITION_END  : return true;
	                     default            : return m_position < pos.m_position;
	                     }
	}
}

inline rbool IDocUpdate::Position::operator> (CREF(Position) pos) const
{
	switch(get())
	{
	case POSITION_BEGIN: switch (pos.get())
	                     {
	                     case POSITION_BEGIN: NEVER_REACH_HERE; return false;
	                     case POSITION_END  : return false;
	                     default            : return false;
	                     }
	case POSITION_END  : switch (pos.get())
	                     {
	                     case POSITION_BEGIN: return true;
	                     case POSITION_END  : NEVER_REACH_HERE; return false;
	                     default            : return true;
	                     }
	default            : switch (pos.get())
	                     {
	                     case POSITION_BEGIN: return true;
	                     case POSITION_END  : return false;
	                     default            : return m_position > pos.m_position;
	                     }
	}
}

inline rbool IDocUpdate::Position::operator== (CREF(Position) pos) const
{
	return m_position == pos.m_position;
}

inline rbool IDocUpdate::Position::operator!= (CREF(Position) pos) const
{
	return !operator==(pos);
}

CLOSE_RDO_CONVERTER_SMR2RDOX_NAMESPACE
