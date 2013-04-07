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
// --------------------------------------------------------------------------------

OPEN_RDO_PARSER_NAMESPACE

class RDOFRMSprite: public RDOFRMCommandList
{
DECLARE_FACTORY(RDOFRMSprite);
private:
	RDOFRMSprite(CREF(RDOParserSrcInfo) src_info);
	virtual ~RDOFRMSprite();

	rdo::runtime::LPRDOFRMSprite m_pSprite;

	rdo::runtime::LPRDOFRMSprite list() const;
};
DECLARE_POINTER(RDOFRMSprite);

CLOSE_RDO_PARSER_NAMESPACE

#endif // _SIMULATOR_COMPILER_PARSER_ANIMATION_SPRITE_H_
