/*!
  \copyright (c) RDO-Team, 2012
  \file      update_i.cpp
  \author    Урусов Андрей (rdo@rk9.bmstu.ru)
  \date      24.01.2011
  \brief     
  \indent    4T
*/

// ---------------------------------------------------------------------------- PCH
#include "converter/smr2rdox/pch.h"
// ----------------------------------------------------------------------- INCLUDES
// ----------------------------------------------------------------------- SYNOPSIS
#include "converter/smr2rdox/update/update_i.h"
// --------------------------------------------------------------------------------

OPEN_RDO_CONVERTER_SMR2RDOX_NAMESPACE

// --------------------------------------------------------------------------------
// -------------------- IDocUpdate::Position
// --------------------------------------------------------------------------------
IDocUpdate::Position::Position(std::size_t pos)
	: m_position(pos)
{}

IDocUpdate::Position::Position(const Position& pos)
	: m_position(pos.m_position)
{}

std::size_t IDocUpdate::Position::get() const
{
	return m_position;
}

bool IDocUpdate::Position::begin() const
{
	return m_position == std::size_t(POSITION_BEGIN);
}

bool IDocUpdate::Position::end() const
{
	return m_position == std::size_t(POSITION_END);
}

bool IDocUpdate::Position::real() const
{
	return m_position != std::size_t(POSITION_BEGIN) && m_position != std::size_t(POSITION_END);
}

void IDocUpdate::Position::operator+= (const Position& pos)
{
	if (real() && pos.real())
	{
		m_position += pos.m_position;
	}
}

void IDocUpdate::Position::operator-= (const Position& pos)
{
	if (real() && pos.real())
	{
		m_position -= pos.m_position;
	}
}

IDocUpdate::Position IDocUpdate::Position::operator+ (const Position& pos) const
{
	if (real() && pos.real())
	{
		return Position(m_position + pos.m_position);
	}

	NEVER_REACH_HERE;
	return Position(0);
}

IDocUpdate::Position IDocUpdate::Position::operator- (const Position& pos) const
{
	if (real() && pos.real())
	{
		return Position(m_position - pos.m_position);
	}

	NEVER_REACH_HERE;
	return Position(0);
}

bool IDocUpdate::Position::operator<= (const Position& pos) const
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

bool IDocUpdate::Position::operator>= (const Position& pos) const
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

bool IDocUpdate::Position::operator< (const Position& pos) const
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

bool IDocUpdate::Position::operator> (const Position& pos) const
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

bool IDocUpdate::Position::operator== (const Position& pos) const
{
	return m_position == pos.m_position;
}

bool IDocUpdate::Position::operator!= (const Position& pos) const
{
	return !operator==(pos);
}

CLOSE_RDO_CONVERTER_SMR2RDOX_NAMESPACE
