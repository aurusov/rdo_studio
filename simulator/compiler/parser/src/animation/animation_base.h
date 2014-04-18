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
#include "simulator/compiler/parser/context/context.h"
#include "simulator/compiler/parser/context/context_find_i.h"
#include "simulator/compiler/parser/src/function/function.h"
// --------------------------------------------------------------------------------

OPEN_RDO_PARSER_NAMESPACE

class RDOFRMCommandList
	: public Context
	, public IContextFind
{
DECLARE_FACTORY(RDOFRMCommandList);
public:
	const std::string& name() const;
	LPExpression expression() const;
	void end();

	virtual rdo::runtime::LPRDOFRMSprite list() const = 0;

	static LPExpression generateExpression(const rdo::runtime::LPRDOCalc& pCalc, const RDOParserSrcInfo& srcInfo);

protected:
	RDOFRMCommandList(const RDOParserSrcInfo& srcInfo);
	virtual ~RDOFRMCommandList();

	const LPFunction& function() const;

private:
	LPFunction m_pFunction;

	DECLARE_IContextFind;
};
DECLARE_POINTER(RDOFRMCommandList);

CLOSE_RDO_PARSER_NAMESPACE

#endif // _SIMULATOR_COMPILER_PARSER_ANIMATION_BASE_H_
