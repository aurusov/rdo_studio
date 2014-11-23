/*!
  \copyright (c) RDO-Team, 2012
  \file      animation_base.cpp
  \author    Урусов Андрей (rdo@rk9.bmstu.ru)
  \date      14.12.2012
  \brief     Базовый класс объектов анимации парсера
  \indent    4T
*/

// ---------------------------------------------------------------------------- PCH
#include "simulator/compiler/parser/pch.h"
// ----------------------------------------------------------------------- INCLUDES
#include <boost/bind.hpp>
// ----------------------------------------------------------------------- SYNOPSIS
#include "simulator/compiler/parser/src/animation/animation_base.h"
#include "simulator/compiler/parser/src/animation/animation_sprite.h"
#include "simulator/compiler/parser/rdoparser.h"
// --------------------------------------------------------------------------------

OPEN_RDO_PARSER_NAMESPACE

RDOFRMCommandList::RDOFRMCommandList(const RDOParserSrcInfo& srcInfo)
{
	m_pFunction = rdo::Factory<Function>::create(
		rdo::Factory<TypeInfo>::delegate<RDOType__void>(srcInfo),
		srcInfo
	);
	ASSERT(m_pFunction);

	RDOParser::s_parser()->insertFRMCommandList(this);
	RDOParser::s_parser()->contextStack()->push(this);
	m_pFunction->pushContext();
}

RDOFRMCommandList::~RDOFRMCommandList()
{}

const std::string& RDOFRMCommandList::name() const
{
	return m_pFunction->src_text();
}

LPExpression RDOFRMCommandList::expression() const
{
	return m_pFunction->expression();
}

const LPFunction& RDOFRMCommandList::function() const
{
	return m_pFunction;
}

namespace
{

LPExpression sprite(const LPRDOFRMSprite& sprite)
{
	return sprite->expression();
}

}

Context::LPFindResult RDOFRMCommandList::onFindContext(const std::string& method, const Context::Params& params, const RDOParserSrcInfo& /*srcInfo*/) const
{
	if (method == Context::METHOD_GET)
	{
		LPRDOFRMSprite pSprite = RDOParser::s_parser()->findFRMSprite(params.identifier());
		return pSprite
			? rdo::Factory<FindResult>::create(CreateExpression(boost::bind(&sprite, pSprite)))
			: rdo::Factory<FindResult>::create();
	}

	return rdo::Factory<FindResult>::create();
}

void RDOFRMCommandList::end()
{
	m_pFunction->popContext();
	RDOParser::s_parser()->contextStack()->pop<RDOFRMCommandList>();
	list()->setSpriteCalc(expression()->calc());
}

LPExpression RDOFRMCommandList::generateExpression(const rdo::runtime::LPRDOCalc& pCalc, const RDOParserSrcInfo& srcInfo)
{
	ASSERT(pCalc);

	LPTypeInfo pType = rdo::Factory<TypeInfo>::delegate<RDOType__void>(srcInfo);
	ASSERT(pType);

	LPExpression pExpression = rdo::Factory<Expression>::create(pType, pCalc, srcInfo);
	ASSERT(pExpression);

	return pExpression;
}

CLOSE_RDO_PARSER_NAMESPACE
