/*!
  \copyright (c) RDO-Team, 2012
  \file      animation_sprite.h
  \author    Урусов Андрей (rdo@rk9.bmstu.ru)
  \date      14.12.2012
  \brief     Спрайт анимации в парсере
  \indent    4T
*/

#ifndef _SIMULATOR_COMPILER_PARSER_ANIMATION_SPRITE_H_
#define _SIMULATOR_COMPILER_PARSER_ANIMATION_SPRITE_H_

// ----------------------------------------------------------------------- INCLUDES
// ----------------------------------------------------------------------- SYNOPSIS
#include "simulator/compiler/parser/src/animation/animation_base.h"
#include "simulator/compiler/parser/src/function/function.h"
// --------------------------------------------------------------------------------

OPEN_RDO_PARSER_NAMESPACE

class RDOFRMSprite
	: public RDOFRMCommandList
	, public Function
{
DECLARE_FACTORY(RDOFRMSprite);
public:
	void end();

	CREF(tstring)                      name  () const { return src_info().src_text(); }
	CREF(rdo::runtime::LPRDOFRMSprite) sprite() const { return m_pSprite; }

private:
	RDOFRMSprite(CREF(RDOParserSrcInfo) src_info);

	rdo::runtime::LPRDOFRMSprite  m_pSprite;

	rdo::runtime::LPRDOFRMSprite list() const { return m_pSprite; }

	DECLARE_IContextFind;
};
DECLARE_POINTER(RDOFRMSprite);

CLOSE_RDO_PARSER_NAMESPACE

#endif // _SIMULATOR_COMPILER_PARSER_ANIMATION_SPRITE_H_
