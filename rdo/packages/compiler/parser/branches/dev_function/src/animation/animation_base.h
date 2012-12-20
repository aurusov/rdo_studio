/*!
  \copyright (c) RDO-Team, 2012
  \file      animation_base.h
  \author    Урусов Андрей (rdo@rk9.bmstu.ru)
  \date      14.12.2012
  \brief     Базовый класс объектов анимации парсера
  \indent    4T
*/

#ifndef _SIMULATOR_COMPILER_PARSER_ANIMATION_BASE_H_
#define _SIMULATOR_COMPILER_PARSER_ANIMATION_BASE_H_

// ----------------------------------------------------------------------- INCLUDES
// ----------------------------------------------------------------------- SYNOPSIS
#include "simulator/runtime/rdoframe.h"
#include "simulator/compiler/parser/expression.h"
// --------------------------------------------------------------------------------

OPEN_RDO_PARSER_NAMESPACE

class RDOFRMCommandList: public virtual rdo::counter_reference
{
DECLARE_FACTORY(RDOFRMCommandList);
public:
	virtual CREF(tstring)                name() const = 0;
	virtual rdo::runtime::LPRDOFRMSprite list() const = 0;

	static LPExpression generateExpression(CREF(rdo::runtime::LPRDOCalc) pCalc, CREF(RDOParserSrcInfo) srcInfo);

protected:
	RDOFRMCommandList();
	virtual ~RDOFRMCommandList();
};
DECLARE_POINTER(RDOFRMCommandList);

CLOSE_RDO_PARSER_NAMESPACE

#endif // _SIMULATOR_COMPILER_PARSER_ANIMATION_BASE_H_
