/*!
  \copyright (c) RDO-Team, 2011
  \file      animation_frame.h
  \authors   Барс Александр
  \authors   Урусов Андрей (rdo@rk9.bmstu.ru)
  \date      Фрейм анимации в парсере
  \brief     
  \indent    4T
*/

#ifndef _SIMULATOR_COMPILER_PARSER_ANIMATION_FRAME_H_
#define _SIMULATOR_COMPILER_PARSER_ANIMATION_FRAME_H_

// ----------------------------------------------------------------------- INCLUDES
// ----------------------------------------------------------------------- SYNOPSIS
#include "simulator/compiler/parser/src/animation/animation_base.h"
// --------------------------------------------------------------------------------

OPEN_RDO_PARSER_NAMESPACE

// --------------------------------------------------------------------------------
// -------------------- RDOFRMFrame
// --------------------------------------------------------------------------------
class RDOFRMFrame: public RDOFRMCommandList
{
DECLARE_FACTORY(RDOFRMFrame);
public:
	const rdo::runtime::LPRDOFRMFrame& frame() const;

private:
	RDOFRMFrame(const RDOParserSrcInfo& srcInfo);
	virtual ~RDOFRMFrame();

	rdo::runtime::LPRDOFRMFrame m_pFrame;

	rdo::runtime::LPRDOFRMSprite list() const;
};
DECLARE_POINTER(RDOFRMFrame);

CLOSE_RDO_PARSER_NAMESPACE

#endif // _SIMULATOR_COMPILER_PARSER_ANIMATION_FRAME_H_
