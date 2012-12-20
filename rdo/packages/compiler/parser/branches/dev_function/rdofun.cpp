/*!
  \copyright (c) RDO-Team, 2011
  \file      rdofun.cpp
  \authors   Барс Александр
  \authors   Урусов Андрей (rdo@rk9.bmstu.ru)
  \date      
  \brief     
  \indent    4T
*/

// ---------------------------------------------------------------------------- PCH
#include "simulator/compiler/parser/pch.h"
// ----------------------------------------------------------------------- INCLUDES
#include <boost/foreach.hpp>
// ----------------------------------------------------------------------- SYNOPSIS
#include "simulator/compiler/parser/rdofun.h"
#include "simulator/compiler/parser/rdoparser.h"
#include "simulator/compiler/parser/rdortp.h"
#include "simulator/compiler/parser/rdorss.h"
#include "simulator/compiler/parser/rdopat.h"
#include "simulator/compiler/parser/src/animation/animation_frame.h"
#include "simulator/compiler/parser/rdodpt.h"
#include "simulator/compiler/parser/rdoparser_lexer.h"
#include "simulator/compiler/parser/type/range.h"
#include "simulator/runtime/rdo_random_distribution.h"
#include "simulator/runtime/rdo_runtime.h"
#include "simulator/runtime/rdoframe.h"
#include "simulator/runtime/calc/calc_base.h"
#include "simulator/runtime/calc/operation/calc_arithm.h"
// --------------------------------------------------------------------------------

OPEN_RDO_PARSER_NAMESPACE

int funlex(PTR(YYSTYPE) lpval, PTR(YYLTYPE) llocp, PTR(void) lexer)
{
	LEXER->m_lpval = lpval;
	LEXER->m_lploc = llocp;
	return LEXER->yylex();
}

void funerror(PTR(char) message)
{
	UNUSED(message);
}

// --------------------------------------------------------------------------------
// -------------------- RDOFUNDoubleToIntByResult
// --------------------------------------------------------------------------------
void RDOFUNDoubleToIntByResult::roundCalc()
{
	STL_FOR_ALL(m_intOrDouble, it)
	{
		(*it)->needRound();
	}
}

// --------------------------------------------------------------------------------
// -------------------- RDOFUNBase
// --------------------------------------------------------------------------------
RDOFUNBase::RDOFUNBase(CREF(RDOParserSrcInfo) srcInfo)
	: RDOParserSrcInfo(srcInfo)
{}

RDOFUNBase::RDOFUNBase(CREF(LPExpression) pExpression)
	: m_pExpression(pExpression)
{
	ASSERT(m_pExpression);
}

CREF(LPExpression) RDOFUNBase::expression() const
{
	ASSERT(m_pExpression);
	return m_pExpression;
}

// --------------------------------------------------------------------------------
// -------------------- RDOFUNLogic
// --------------------------------------------------------------------------------
RDOFUNLogic::RDOFUNLogic(CREF(LPRDOFUNArithm) pArithm)
	: RDOFUNBase(pArithm->expression())
{
	if (pArithm->typeID() != rdo::runtime::RDOType::t_bool)
	{
		RDOParser::s_parser()->error().error(src_info(), rdo::format(_T("Ожидается логическое выражение, найдено: %s"), src_text().c_str()));
	}
}

RDOFUNLogic::RDOFUNLogic(CREF(LPExpression) pExpression, rbool hideWarning)
	: RDOFUNBase(pExpression)
{
	rdo::runtime::RDOSrcInfo srcInfo = m_pExpression->src_info();
	srcInfo.setSrcFileType(src_filetype());
	m_pExpression->setSrcInfo(srcInfo);
	if (!hideWarning)
	{
		rdo::runtime::LPRDOCalcConst pConstCalc = m_pExpression->calc().object_dynamic_cast<rdo::runtime::RDOCalcConst>();
		if (pConstCalc)
		{
			if (pConstCalc->getValue().getAsBool())
			{
				RDOParser::s_parser()->error().warning(pConstCalc->srcInfo(), rdo::format(_T("Логическое выражение всегда истинно: %s"), pConstCalc->srcInfo().src_text().c_str()));
			}
			else
			{
				RDOParser::s_parser()->error().warning(pConstCalc->srcInfo(), rdo::format(_T("Логическое выражение всегда ложно: %s"), pConstCalc->srcInfo().src_text().c_str()));
			}
		}
	}
}

RDOFUNLogic::~RDOFUNLogic()
{}

LPRDOFUNLogic RDOFUNLogic::generateTrue(CREF(RDOParserSrcInfo) srcInfo)
{
	LPExpression pExpression = rdo::Factory<Expression>::create(
		rdo::Factory<TypeInfo>::delegate<RDOType__bool>(srcInfo),
		rdo::Factory<rdo::runtime::RDOCalcConst>::create(true),
		srcInfo
	);
	ASSERT(pExpression);

	LPRDOFUNLogic pLogic = rdo::Factory<RDOFUNLogic>::create(pExpression, false);
	ASSERT(pLogic);

	return pLogic;
}

rdo::runtime::LPRDOCalc RDOFUNLogic::getCalc(rdo::runtime::RDOType::TypeID id)
{
	if (id != rdo::runtime::RDOType::t_real)
	{
		m_intOrDouble.roundCalc();
	}
	return m_pExpression->calc();
}

LPRDOFUNLogic RDOFUNLogic::createLogic(CREF(rdo::runtime::LPRDOCalc) pCalc)
{
	ASSERT(pCalc);

	LPExpression pExpression = rdo::Factory<Expression>::create(
		rdo::Factory<TypeInfo>::delegate<RDOType__bool>(pCalc->srcInfo()),
		pCalc,
		pCalc->srcInfo()
	);
	ASSERT(pExpression);

	LPRDOFUNLogic pLogic = rdo::Factory<RDOFUNLogic>::create(pExpression, false);
	ASSERT(pLogic);

	pLogic->setSrcInfo(pCalc->srcInfo());

	return pLogic;
}

template <class T>
LPRDOFUNLogic RDOFUNLogic::generateLogic(CREF(LPRDOFUNLogic) pSecond)
{
	ASSERT(pSecond);

	rdo::runtime::LPRDOCalc pCalc = rdo::runtime::RDOCalcBinaryBase::generateCalc<T>(m_pExpression->calc(), pSecond->m_pExpression->calc());
	ASSERT(pCalc);
	LPRDOFUNLogic pLogic = createLogic(pCalc);
	pLogic->m_intOrDouble.insert(m_intOrDouble, pSecond->m_intOrDouble);
	return pLogic;
}

template <class T>
LPRDOFUNLogic RDOFUNLogic::generateLogic(CREF(RDOSrcInfo::Position) position)
{
	rdo::runtime::LPRDOCalc pCalc = rdo::runtime::RDOCalcUnaryBase::generateCalc<T>(position, m_pExpression->calc());
	ASSERT(pCalc);
	LPRDOFUNLogic pLogic = createLogic(pCalc);
	pLogic->m_intOrDouble.insert(m_intOrDouble);
	return pLogic;
}

LPRDOFUNLogic RDOFUNLogic::operator&& (CREF(LPRDOFUNLogic) pSecond)
{
	return generateLogic<rdo::runtime::RDOCalcAnd>(pSecond);
}

LPRDOFUNLogic RDOFUNLogic::operator|| (CREF(LPRDOFUNLogic) pSecond)
{
	return generateLogic<rdo::runtime::RDOCalcOr>(pSecond);
}

LPRDOFUNLogic RDOFUNLogic::operator_not(CREF(RDOSrcInfo::Position) position)
{
	return generateLogic<rdo::runtime::RDOCalcNot>(position);
}

void RDOFUNLogic::setSrcInfo(CREF(RDOParserSrcInfo) srcInfo)
{
	RDOParserSrcInfo::setSrcInfo(srcInfo);
	m_pExpression->setSrcInfo(srcInfo);
}

void RDOFUNLogic::setSrcPos(CREF(RDOSrcInfo::Position) position)
{
	RDOParserSrcInfo::setSrcPos(position);
	rdo::runtime::RDOSrcInfo srcInfo = m_pExpression->src_info();
	srcInfo.setSrcPos(position);
	m_pExpression->setSrcInfo(srcInfo);
}

void RDOFUNLogic::setSrcText(CREF(tstring) value)
{
	RDOParserSrcInfo::setSrcText(value);
	rdo::runtime::RDOSrcInfo srcInfo = m_pExpression->src_info();
	srcInfo.setSrcText(value);
	m_pExpression->setSrcInfo(srcInfo);
}

void RDOFUNLogic::setSrcPos(CREF(YYLTYPE) error_pos)
{
	RDOParserSrcInfo::setSrcPos(error_pos);
}

void RDOFUNLogic::setSrcPos(CREF(YYLTYPE) pos_begin, CREF(YYLTYPE) pos_end)
{
	RDOParserSrcInfo::setSrcPos(pos_begin, pos_end);
}

// --------------------------------------------------------------------------------
// -------------------- RDOFUNArithm
// --------------------------------------------------------------------------------
RDOFUNArithm::RDOFUNArithm(CREF(LPExpression) pExpression)
	: RDOFUNBase(pExpression)
{
	ASSERT(m_pExpression);
	setSrcInfo(m_pExpression->src_info());
}

RDOFUNArithm::~RDOFUNArithm()
{}

LPRDOFUNArithm RDOFUNArithm::generateByConst(CREF(LPRDOValue) pValue)
{
	ASSERT(pValue);
	ASSERT(pValue->constant());

	LPExpression pExpression = rdo::Factory<Expression>::create(pValue);
	ASSERT(pExpression);

	LPRDOFUNArithm pArithm = rdo::Factory<RDOFUNArithm>::create(pExpression);
	ASSERT(pArithm);

	return pArithm;
}

LPRDOFUNArithm RDOFUNArithm::generateByIdentificator(CREF(LPRDOValue) pValue)
{
	ASSERT(pValue);

	LPContext pContext = RDOParser::s_parser()->context();
	ASSERT(pContext);

	pContext = pContext->find(pValue);
	ASSERT(pContext);

	LPExpression pExpression = pContext->create(pValue);
	ASSERT(pExpression);

	LPRDOFUNArithm pArithm = rdo::Factory<RDOFUNArithm>::create(pExpression);
	ASSERT(pArithm);

	return pArithm;
}

LPRDOFUNArithm RDOFUNArithm::generateByIdentificator(CREF(LPRDOValue) pValue1, CREF(LPRDOValue) pValue2)
{
	ASSERT(pValue1);
	ASSERT(pValue2);

	LPContext pContext = RDOParser::s_parser()->context();
	ASSERT(pContext);

	pContext = pContext->find(pValue1);
	ASSERT(pContext);

	pContext = pContext->swch(pValue2);
	ASSERT(pContext);

	LPExpression pExpression = pContext->create(pValue2);
	ASSERT(pExpression);

	LPRDOFUNArithm pArithm = rdo::Factory<RDOFUNArithm>::create(pExpression);
	ASSERT(pArithm);

	return pArithm;
}

void RDOFUNArithm::castType(CREF(LPRDOFUNArithm) pSecond, CREF(tstring) error)
{
	try
	{
		expression()->typeInfo()->type()->type_cast(pSecond->expression()->typeInfo()->type(), pSecond->expression()->src_info(), expression()->src_info(), expression()->src_info());
	}
	catch (REF(RDOSyntaxException))
	{
		RDOParser::s_parser()->error().error(pSecond->src_info(), rdo::format(error.c_str(), typeInfo()->type()->name().c_str(), pSecond->typeInfo()->type()->name().c_str()));
	}
}

void RDOFUNArithm::castValue(CREF(LPRDOFUNArithm) pSecond, CREF(tstring) error)
{
	try
	{
		LPRDOType pType = expression()->typeInfo()->type()->type_cast(pSecond->expression()->typeInfo()->type(), pSecond->expression()->src_info(), expression()->src_info(), expression()->src_info());
		ASSERT(pType);
		LPRDOValue pConstant = pSecond->expression()->constant();
		if (pConstant && pConstant->defined())
		{
			pConstant = expression()->typeInfo()->type()->value_cast(pConstant, expression()->src_info(), pSecond->expression()->src_info());
			ASSERT(pConstant);
			pSecond->m_pExpression = rdo::Factory<Expression>::create(
				rdo::Factory<TypeInfo>::create(
					pType,
					expression()->src_info()
				),
				rdo::Factory<rdo::runtime::RDOCalcConst>::create(pConstant->value()),
				pConstant->src_info()
			);
			ASSERT(pSecond->m_pExpression);
		}
	}
	catch (REF(RDOSyntaxException))
	{
		RDOParser::s_parser()->error().error(pSecond->src_info(), rdo::format(error.c_str(), expression()->src_info().src_text().c_str(), pSecond->expression()->src_info().src_text().c_str()));
	}
}

template <class T>
rdo::runtime::LPRDOCalc RDOFUNArithm::generateCalc(CREF(rdo::runtime::RDOSrcInfo::Position) position, CREF(tstring) error)
{
	UNUSED(error);

	rdo::runtime::LPRDOCalc pCalc = rdo::runtime::RDOCalcUnaryBase::generateCalc<T>(position, m_pExpression->calc());
	ASSERT(pCalc);
	return pCalc;
}

template <class T>
rdo::runtime::LPRDOCalc RDOFUNArithm::generateCalc(CREF(LPRDOFUNArithm) pSecond, CREF(tstring) error)
{
	castType(pSecond, error);
	rdo::runtime::LPRDOCalc pCalc = rdo::runtime::RDOCalcBinaryBase::generateCalc<T>(m_pExpression->calc(), pSecond->m_pExpression->calc());
	ASSERT(pCalc);
	return pCalc;
}

template <class T>
LPRDOFUNArithm RDOFUNArithm::generateArithm(CREF(rdo::runtime::RDOSrcInfo::Position) position, CREF(tstring) error)
{
	rdo::runtime::LPRDOCalc pCalc = generateCalc<T>(position, error);
	ASSERT(pCalc);

	LPTypeInfo pType = getPreType(this);
	ASSERT(pType);

	LPExpression pExpression = rdo::Factory<Expression>::create(
		pType,
		pCalc,
		pCalc->srcInfo()
	);
	ASSERT(pExpression);

	LPRDOFUNArithm pArithm = rdo::Factory<RDOFUNArithm>::create(pExpression);
	ASSERT(pArithm);

	pArithm->m_intOrDouble.insert(m_intOrDouble);
	return pArithm;
}

template <class T>
LPRDOFUNArithm RDOFUNArithm::generateArithm(CREF(LPRDOFUNArithm) pSecond, CREF(tstring) error)
{
	rdo::runtime::LPRDOCalc pCalc = generateCalc<T>(pSecond, error);
	ASSERT(pCalc);

	LPTypeInfo pType = getPreType(pSecond);
	ASSERT(pType);

	LPExpression pExpression = rdo::Factory<Expression>::create(
		pType,
		pCalc,
		pCalc->srcInfo()
	);
	ASSERT(pExpression);

	LPRDOFUNArithm pArithm = rdo::Factory<RDOFUNArithm>::create(pExpression);
	ASSERT(pArithm);

	pArithm->m_intOrDouble.insert(m_intOrDouble, pSecond->m_intOrDouble);
	return pArithm;
}

template <class T>
LPRDOFUNLogic RDOFUNArithm::generateLogic(CREF(LPRDOFUNArithm) pSecond, CREF(tstring) error)
{
	rdo::runtime::LPRDOCalc pCalc = generateCalc<T>(pSecond, error);
	ASSERT(pCalc);

	LPExpression pExpression = rdo::Factory<Expression>::create(
		rdo::Factory<TypeInfo>::delegate<RDOType__bool>(pCalc->srcInfo()),
		pCalc,
		pCalc->srcInfo()
	);
	ASSERT(pExpression);

	LPRDOFUNLogic pLogic = rdo::Factory<RDOFUNLogic>::create(pExpression, false);
	ASSERT(pLogic);

	pLogic->setSrcInfo(pCalc->srcInfo());
	pLogic->m_intOrDouble.insert(m_intOrDouble, pSecond->m_intOrDouble);
	return pLogic;
}

rdo::runtime::RDOValue RDOFUNArithm::const_value() const
{
	rdo::runtime::LPRDOCalcConst pCalc = calc().object_dynamic_cast<rdo::runtime::RDOCalcConst>();
	ASSERT(pCalc);
	return pCalc->getValue();
}

LPTypeInfo RDOFUNArithm::getPreType(CREF(LPRDOFUNArithm) pSecond)
{
	if (typeID() == rdo::runtime::RDOType::t_unknow)
	{
		RDOParser::s_parser()->error().error(src_info(), rdo::format(_T("Внутренная ошибка парсера: неопределенное значение арифметического выражения: %s"), src_text().c_str()));
	}
	if (pSecond->typeID() == rdo::runtime::RDOType::t_unknow)
	{
		RDOParser::s_parser()->error().error(pSecond->src_info(), rdo::format(_T("Внутренная ошибка парсера: неопределенное значение арифметического выражения: %s"), pSecond->src_text().c_str()));
	}
	if (typeID() == rdo::runtime::RDOType::t_identificator)
	{
		rdo::runtime::LPRDOCalcConst pCalc = calc().object_dynamic_cast<rdo::runtime::RDOCalcConst>();
		ASSERT(pCalc);
		RDOParser::s_parser()->error().error(src_info(), rdo::format(_T("Неизвестный идентификатор: %s"), const_value().getIdentificator().c_str()));
	}
	if (pSecond->typeID() == rdo::runtime::RDOType::t_identificator)
	{
		RDOParser::s_parser()->error().error(pSecond->src_info(), rdo::format(_T("Неизвестный идентификатор: %s"), pSecond->const_value().getIdentificator().c_str()));
	}

	return typeInfo()->type_cast(pSecond->typeInfo(), src_info());
}

LPRDOFUNArithm RDOFUNArithm::operator+ (CREF(LPRDOFUNArithm) pSecond)
{
	return generateArithm<rdo::runtime::RDOCalcPlus>(pSecond, _T("Ну не могу я сложить %s и %s"));
}

LPRDOFUNArithm RDOFUNArithm::operator- (CREF(LPRDOFUNArithm) pSecond)
{
	return generateArithm<rdo::runtime::RDOCalcMinus>(pSecond, _T("Нельзя из %s вычесть %s"));
}

LPRDOFUNArithm RDOFUNArithm::operator* (CREF(LPRDOFUNArithm) pSecond)
{
	return generateArithm<rdo::runtime::RDOCalcMult>(pSecond, _T("Нельзя %s умножить на %s"));
}

LPRDOFUNArithm RDOFUNArithm::operator/ (CREF(LPRDOFUNArithm) pSecond)
{
	rdo::runtime::LPRDOCalc pCalc = generateCalc<rdo::runtime::RDOCalcDiv>(pSecond, _T("Нельзя %s разделить на %s"));
	ASSERT(pCalc);
	LPTypeInfo pType = getPreType(pSecond);
	ASSERT(pType);

	/// @todo перевод вещественного в целое при делении. А что делать с умножением и т.д. ?
	//! Ответ: с умножением надо делать тоже самое, только непонятно как
	if (pType->type()->typeID() == rdo::runtime::RDOType::t_int)
	{
		rdo::runtime::LPRDOCalc pNewCalcDiv = pCalc;
		pCalc = rdo::Factory<rdo::runtime::RDOCalcDoubleToIntByResult>::create(pNewCalcDiv);
		pCalc->setSrcInfo(pNewCalcDiv->srcInfo());
	}
	LPExpression pExpression = rdo::Factory<Expression>::create(
		pType,
		pCalc,
		pCalc->srcInfo()
	);
	ASSERT(pExpression);
	LPRDOFUNArithm pArithm = rdo::Factory<RDOFUNArithm>::create(pExpression);
	if (pType->type()->typeID() == rdo::runtime::RDOType::t_int)
	{
		rdo::runtime::LPRDOCalcDoubleToIntByResult pResult = pCalc.object_static_cast<rdo::runtime::RDOCalcDoubleToIntByResult>();
		pArithm->m_intOrDouble.push_back(pResult);
	}
	pArithm->m_intOrDouble.insert(m_intOrDouble, pSecond->m_intOrDouble);
	return pArithm;
}

LPRDOFUNArithm RDOFUNArithm::setEqual(CREF(LPRDOFUNArithm) pSecond)
{
	castValue(pSecond, _T("Ошибка присваивания %s = %s"));
	return this;
}

LPRDOFUNArithm RDOFUNArithm::uminus(CREF(rdo::runtime::RDOSrcInfo::Position) position)
{
	return generateArithm<rdo::runtime::RDOCalcUMinus>(position, _T("Нельзя взять унарный минус от %s"));
}

LPRDOFUNLogic RDOFUNArithm::operator< (CREF(LPRDOFUNArithm) pSecond)
{
	return generateLogic<rdo::runtime::RDOCalcIsLess>(pSecond, _T("Нельзя сравнивать %s и %s"));
}

LPRDOFUNLogic RDOFUNArithm::operator> (CREF(LPRDOFUNArithm) pSecond)
{
	return generateLogic<rdo::runtime::RDOCalcIsGreater>(pSecond, _T("Нельзя сравнивать %s и %s"));
}

LPRDOFUNLogic RDOFUNArithm::operator<= (CREF(LPRDOFUNArithm) pSecond)
{
	return generateLogic<rdo::runtime::RDOCalcIsLEQ>(pSecond, _T("Нельзя сравнивать %s и %s"));
}

LPRDOFUNLogic RDOFUNArithm::operator>= (CREF(LPRDOFUNArithm) pSecond)
{
	return generateLogic<rdo::runtime::RDOCalcIsGEQ>(pSecond, _T("Нельзя сравнивать %s и %s"));
}

LPRDOFUNLogic RDOFUNArithm::operator== (CREF(LPRDOFUNArithm) pSecond)
{
	castValue(pSecond, _T("Ошибка сравнения %s == %s"));
	return generateLogic<rdo::runtime::RDOCalcIsEqual>(pSecond, _T("Нельзя сравнивать %s и %s"));
}

LPRDOFUNLogic RDOFUNArithm::operator!= (CREF(LPRDOFUNArithm) pSecond)
{
	castValue(pSecond, _T("Ошибка сравнения %s <> %s"));
	return generateLogic<rdo::runtime::RDOCalcIsNotEqual>(pSecond, _T("Нельзя сравнивать %s и %s"));
}

void RDOFUNArithm::checkParamType(CREF(LPTypeInfo) pType)
{
	pType->type()->type_cast(typeInfo()->type(), src_info(), pType->src_info(), src_info());
	rdo::runtime::LPRDOCalcConst pConstCalc = calc().object_dynamic_cast<rdo::runtime::RDOCalcConst>();
	if (pConstCalc)
	{
		rdo::runtime::RDOValue value = pConstCalc->getValue();
		pType->value_cast(rdo::Factory<RDOValue>::create(value, src_info(), typeInfo()));
	}
}

void RDOFUNArithm::wrongVarInit(CREF(LPRDOValue) pParamName, CREF(tstring) paramName)
{
	LPRDOFUNFunction pFunction = RDOParser::s_parser()->getLastFUNFunction();
	ASSERT(pFunction);
	LPRDOParam pParam = pFunction->findParam(paramName);
	if (pParam)
	{
		RDOParser::s_parser()->error().error(pParamName->src_info(), rdo::format(_T("Функции не могут изменить свой параметр: %s"), paramName.c_str()));		
	}
	else
	{
		RDOParser::s_parser()->error().error(pParamName->src_info(), rdo::format(_T("Неизвестный идентификатор: %s"), paramName.c_str()));
	}
}

rdo::runtime::LPRDOCalc RDOFUNArithm::createCalc(CREF(LPTypeInfo) pForType)
{
	if (typeID() != rdo::runtime::RDOType::t_identificator)
	{
		if (!pForType)
		{
			return m_pExpression->calc();
		}
		if (pForType->type()->typeID() != rdo::runtime::RDOType::t_int)
		{
			if (pForType->type()->typeID() == rdo::runtime::RDOType::t_enum)
			{
				m_intOrDouble.roundCalc();
			}
			return m_pExpression->calc();
		}
		else
		{
			m_intOrDouble.roundCalc();
			rdo::runtime::LPRDOCalc pCalc = rdo::Factory<rdo::runtime::RDOCalcDoubleToInt>::create(m_pExpression->calc()->srcInfo().src_pos(), m_pExpression->calc());
			ASSERT(pCalc);
			pCalc->setSrcInfo(src_info());
			return pCalc;
		}
	}
	else if (typeID() == rdo::runtime::RDOType::t_identificator && !pForType)
	{
		RDOParser::s_parser()->error().error(src_info(), rdo::format(_T("Неизвестный идентификатор: %s"), const_value().getAsString().c_str()));
	}

	if (!pForType)
	{
		RDOParser::s_parser()->error().error(src_info(), _T("Неизвестный тип параметра"));
	}

	rdo::runtime::LPRDOCalc pNewCalc = rdo::Factory<rdo::runtime::RDOCalcConst>::create(pForType->value_cast(rdo::Factory<RDOValue>::create(src_info()))->value());
	pNewCalc->setSrcInfo(src_info());
	return pNewCalc;
}

void RDOFUNArithm::setSrcInfo(CREF(RDOParserSrcInfo) srcInfo)
{
	RDOParserSrcInfo::setSrcInfo(srcInfo);
	m_pExpression->setSrcInfo(srcInfo);
}

void RDOFUNArithm::setSrcPos(CREF(RDOSrcInfo::Position) position)
{
	RDOParserSrcInfo::setSrcPos(position);
	rdo::runtime::RDOSrcInfo srcInfo = m_pExpression->src_info();
	srcInfo.setSrcPos(position);
	m_pExpression->setSrcInfo(srcInfo);
}

void RDOFUNArithm::setSrcText(CREF(tstring) value)
{
	RDOParserSrcInfo::setSrcText(value);
	rdo::runtime::RDOSrcInfo srcInfo = m_pExpression->src_info();
	srcInfo.setSrcText(this->src_info().src_text());
	m_pExpression->setSrcInfo(srcInfo);
}

void RDOFUNArithm::setSrcInfo(CREF(RDOParserSrcInfo) begin, CREF(tstring) delim, CREF(RDOParserSrcInfo) end)
{
	RDOParserSrcInfo::setSrcInfo(begin, delim, end);
}

void RDOFUNArithm::setSrcPos(CREF(YYLTYPE) error_pos)
{
	RDOParserSrcInfo::setSrcPos(error_pos);
}

void RDOFUNArithm::setSrcPos(CREF(YYLTYPE) pos_begin, CREF(YYLTYPE) pos_end)
{
	RDOParserSrcInfo::setSrcPos(pos_begin, pos_end);
}

// --------------------------------------------------------------------------------
// -------------------- RDOFUNConstant
// --------------------------------------------------------------------------------
RDOFUNConstant::RDOFUNConstant(CREF(RDOParserSrcInfo) srcInfo, CREF(LPTypeInfo) pType, CREF(LPRDOValue) pDefault)
	: RDOParam(srcInfo, pType, pDefault)
{
	if (!getDefault() || !getDefault()->defined())
	{
		ASSERT(pType);
		RDOParser::s_parser()->error().error(pType->src_info(), _T("Константа должна иметь значение"));
	}

	m_number = RDOParser::s_parser()->getFUNCONST_id();
	RDOParser::s_parser()->insertFUNConstant(this);
}

RDOFUNConstant::~RDOFUNConstant()
{}

// --------------------------------------------------------------------------------
// -------------------- ArithmContainer
// --------------------------------------------------------------------------------
ArithmContainer::ArithmContainer()
	: RDOParserSrcInfo()
{}

ArithmContainer::~ArithmContainer()
{}

void ArithmContainer::addItem(CREF(LPRDOFUNArithm) pArithm)
{
	ASSERT(pArithm);
	m_arithmList.push_back(pArithm);
}

LPFunctionParamType ArithmContainer::getType() const
{
	FunctionParamType::ParamList paramList;

	BOOST_FOREACH(const LPRDOFUNArithm& pArithm, m_arithmList)
	{
		paramList.push_back(pArithm->typeInfo());
	}

	if (paramList.empty())
	{
		paramList.push_back(
			rdo::Factory<TypeInfo>::delegate<RDOType__void>(src_info())
		);
	}

	LPFunctionParamType pParamType = rdo::Factory<FunctionParamType>::create(paramList, src_info());
	ASSERT(pParamType);
	return pParamType;
}

// --------------------------------------------------------------------------------
// -------------------- RDOFUNParams
// --------------------------------------------------------------------------------
// Параметры, с которыми функция вызывается
// --------------------------------------------------------------------------------
RDOFUNParams::RDOFUNParams(CREF(LPArithmContainer) pArithmContainer)
	: RDOParserSrcInfo  (                )
	, m_pArithmContainer(pArithmContainer)
{
	ASSERT(m_pArithmContainer);
}

RDOFUNParams::~RDOFUNParams()
{}

rdo::runtime::LPRDOCalc RDOFUNParams::getCalc(ruint paramID, CREF(LPTypeInfo) pType)
{
	ASSERT(paramID < m_pArithmContainer->getContainer().size());
	rdo::runtime::LPRDOCalc pCalc = m_pArithmContainer->getContainer()[paramID]->createCalc(pType);
	ASSERT(pCalc);
	return pCalc;
}

LPExpression RDOFUNParams::createCallExpression(CREF(LPExpression) pExpression)
{
	ASSERT(pExpression);

	LPFunctionType pFunctionType = pExpression->typeInfo()->type().object_dynamic_cast<FunctionType>();
	ASSERT(pFunctionType);

	LPFunctionParamType pFunctionParamType = pFunctionType->paramType();
	ASSERT(pFunctionParamType);

	LPFunctionParamType pCallerParamType = m_pArithmContainer->getType();
	ASSERT(pCallerParamType);

	pFunctionParamType.object_parent_cast<RDOType>()->type_cast(
		pCallerParamType,
		pCallerParamType->src_info(),
		pFunctionParamType->src_info(),
		src_info()
	);

	rdo::runtime::LPRDOCalc pCalc = pExpression->calc();
	ASSERT(pCalc);

	rdo::runtime::LPRDOCalcFunctionCaller pFuncCall = rdo::Factory<rdo::runtime::RDOCalcFunctionCaller>::create(pCalc);
	ASSERT(pFuncCall);
	pFuncCall->setSrcInfo(src_info());

	ArithmContainer::Container::const_iterator arithmIt = m_pArithmContainer->getContainer().begin();
	BOOST_FOREACH(const LPTypeInfo& pFuncParam, pFunctionParamType->paramList())
	{
		if (pFuncParam->type()->typeID() != rdo::runtime::RDOType::t_void)
		{
			LPRDOFUNArithm pArithm = *arithmIt;
			ASSERT(pArithm);

			pArithm->checkParamType(pFuncParam);
			pFuncCall->addParameter(pFuncParam->type()->calc_cast(pArithm->createCalc(pFuncParam), pArithm->typeInfo()->type()));

			++arithmIt;
		}
	}

	LPExpression pResult = rdo::Factory<Expression>::create(
		pFunctionType->returnType(),
		pFuncCall,
		src_info()
	);
	ASSERT(pResult);

	return pResult;
}

LPRDOFUNArithm RDOFUNParams::createCall(CREF(tstring) funName)
{
	LPRDOFUNFunction pFunction = RDOParser::s_parser()->findFUNFunction(funName);
	if (!pFunction)
	{
		return createSeqCall(funName);
	}

	ruint nParams = pFunction->getParams().size();
	if (nParams != m_pArithmContainer->getContainer().size())
	{
		RDOParser::s_parser()->error().error(src_info(), rdo::format(_T("Неверное количество параметров функции: %s"), funName.c_str()));
	}

	rdo::runtime::LPRDOCalc pCalc = pFunction->getFunctionCalc();
	ASSERT(pCalc);
	pCalc = pFunction->getReturn()->getTypeInfo()->type()->calc_cast(pCalc, pFunction->getReturn()->getTypeInfo()->type());
	ASSERT(pCalc);
	rdo::runtime::LPRDOCalcFunctionCaller pFuncCall = rdo::Factory<rdo::runtime::RDOCalcFunctionCaller>::create(pCalc);
	pFuncCall->setSrcInfo(src_info());
	for (ruint i = 0; i < nParams; i++)
	{
		LPTypeInfo pFuncParam = pFunction->getParams()[i]->getTypeInfo();
		LPRDOFUNArithm pArithm = m_pArithmContainer->getContainer()[i];
		ASSERT(pArithm);
		pArithm->checkParamType(pFuncParam);
		pFuncCall->addParameter(pFuncParam->type()->calc_cast(pArithm->createCalc(pFuncParam), pArithm->typeInfo()->type()));
	}

	LPExpression pExpression = rdo::Factory<Expression>::create(
		pFunction->getReturn()->getTypeInfo(),
		pFuncCall,
		src_info()
	);
	ASSERT(pExpression);
	LPRDOFUNArithm pArithm = rdo::Factory<RDOFUNArithm>::create(pExpression);
	ASSERT(pArithm);
	pArithm->setSrcInfo(src_info());
	return pArithm;
}

LPRDOFUNArithm RDOFUNParams::createSeqCall(CREF(tstring) seqName)
{
	LPRDOFUNSequence pSequence = RDOParser::s_parser()->findFUNSequence(seqName);
	if (!pSequence)
	{
		RDOParser::s_parser()->error().error(m_funseqName, rdo::format(_T("Неопределенная функция или последовательность: %s"), seqName.c_str()));
	}
	LPRDOFUNParams pThis(this);
	ASSERT(pThis);
	LPRDOFUNArithm pArithm = pSequence->createCallCalc(pThis, src_info());
	ASSERT(pArithm);
	return pArithm;
}

// --------------------------------------------------------------------------------
// -------------------- Последовательности
// --------------------------------------------------------------------------------
// -------------------- RDOFUNSequence
// --------------------------------------------------------------------------------
RDOFUNSequence::RDOFUNSequence(CREF(LPRDOFUNSequenceHeader) pHeader, int seed)
	: RDOParserSrcInfo(pHeader->src_info())
	, m_pHeader       (pHeader            )
	, m_seed          (seed               )
{
	RDOParser::s_parser()->insertFUNSequence(this);
}

RDOFUNSequence::~RDOFUNSequence()
{}

void RDOFUNSequence::initResult()
{
	initCalcSrcInfo();
	m_pNextCalc->m_res_real = true;
	switch (m_pHeader->getTypeInfo()->type()->typeID())
	{
		case rdo::runtime::RDOType::t_int:
		{
			LPRDOTypeIntRange pRange = m_pHeader->getTypeInfo()->type().object_dynamic_cast<RDOTypeIntRange>();
			if (pRange)
			{
				m_pNextCalc->m_range = rdo::runtime::RDOCalcSeqNext::Range(pRange->range()->getMin()->value().getDouble(), pRange->range()->getMax()->value().getDouble());
			}
			break;
		}
		case rdo::runtime::RDOType::t_real:
		{
			m_pNextCalc->m_res_real = true;
			LPRDOTypeRealRange pRange = m_pHeader->getTypeInfo()->type().object_dynamic_cast<RDOTypeRealRange>();
			if (pRange)
			{
				m_pNextCalc->m_range = rdo::runtime::RDOCalcSeqNext::Range(pRange->range()->getMin()->value().getDouble(), pRange->range()->getMax()->value().getDouble());
			}
			break;
		}
		default:
			break;
	}
}

void RDOFUNSequence::initCalcSrcInfo()
{
	if (m_pInitCalc)
	{
		rdo::runtime::RDOSrcInfo srcInfo(m_pHeader->src_info());
		srcInfo.setSrcText(_T("Установка базы последовательности ") + m_pHeader->src_text());
		m_pInitCalc->setSrcInfo(srcInfo);
	}
	m_pNextCalc->setSrcInfo(m_pHeader->src_info());
}

// --------------------------------------------------------------------------------
// -------------------- Датчики случайных чисел
// --------------------------------------------------------------------------------
// -------------------- RDOFUNSequenceUniform
// --------------------------------------------------------------------------------
RDOFUNSequenceUniform::RDOFUNSequenceUniform(CREF(LPRDOFUNSequenceHeader) pHeader, int seed)
	: RDOFUNSequence(pHeader, seed)
{
	if (m_pHeader->getTypeInfo()->type()->typeID() == rdo::runtime::RDOType::t_enum)
	{
		RDOParser::s_parser()->error().error(src_info(), rdo::format(_T("Последовательность '%s' может возвращять значения только целого или вещественного типа"), src_text().c_str()));
	}
}

void RDOFUNSequenceUniform::createCalcs()
{
	PTR(rdo::runtime::RandGeneratorUniform) pGenerator = new rdo::runtime::RandGeneratorUniform();
	m_pInitCalc = rdo::Factory<rdo::runtime::RDOCalcSeqInit>::create(m_seed, pGenerator);
	RDOParser::s_parser()->runtime()->addInitCalc(m_pInitCalc);
	m_pNextCalc = rdo::Factory<rdo::runtime::RDOCalcSeqNextUniform>::create(pGenerator);
	initResult();
}

LPRDOFUNArithm RDOFUNSequenceUniform::createCallCalc(REF(LPRDOFUNParams) pParamList, CREF(RDOParserSrcInfo) seqSrcInfo) const
{
	if (pParamList->getParamList()->getContainer().size() != 2)
	{
		RDOParser::s_parser()->error().error(seqSrcInfo, rdo::format(_T("Для равномерного закона распределения '%s' нужно указать два параметра: минимальную и максимальную границы диапазона"), name().c_str()));
	}

	rdo::runtime::LPRDOCalcFunctionCaller pFuctionCall = rdo::Factory<rdo::runtime::RDOCalcFunctionCaller>::create(m_pNextCalc);
	ASSERT(pFuctionCall);

	LPTypeInfo              pType = rdo::Factory<TypeInfo>::delegate<RDOType__real>(RDOParserSrcInfo());
	rdo::runtime::LPRDOCalc pArg1 = pParamList->getCalc(0, pType);
	rdo::runtime::LPRDOCalc pArg2 = pParamList->getCalc(1, pType);

	pFuctionCall->addParameter(pArg1);
	pFuctionCall->addParameter(pArg2);

	LPExpression pExpression = rdo::Factory<Expression>::create(
		m_pHeader->getTypeInfo(),
		pFuctionCall,
		pParamList->src_info()
	);
	ASSERT(pExpression);
	LPRDOFUNArithm pArithm = rdo::Factory<RDOFUNArithm>::create(pExpression);
	ASSERT(pArithm);
	pArithm->setSrcInfo(seqSrcInfo);
	if (pArithm->typeID() == rdo::runtime::RDOType::t_enum)
	{
		RDOParser::s_parser()->error().error(src_info(), _T("Внутренняя ошибка парсера"));
	}
	return pArithm;
}

// --------------------------------------------------------------------------------
// -------------------- RDOFUNSequenceExponential
// --------------------------------------------------------------------------------
RDOFUNSequenceExponential::RDOFUNSequenceExponential(CREF(LPRDOFUNSequenceHeader) pHeader, int seed)
	: RDOFUNSequence(pHeader, seed)
{
	if (m_pHeader->getTypeInfo()->type()->typeID() != rdo::runtime::RDOType::t_int && m_pHeader->getTypeInfo()->type()->typeID() != rdo::runtime::RDOType::t_real)
	{
		RDOParser::s_parser()->error().error(src_info(), rdo::format(_T("Последовательность '%s' может возвращять значения только целого или вещественного типа"), src_text().c_str()));
	}
}

void RDOFUNSequenceExponential::createCalcs()
{
	PTR(rdo::runtime::RandGeneratorExponential) pGenerator = new rdo::runtime::RandGeneratorExponential();
	m_pInitCalc = rdo::Factory<rdo::runtime::RDOCalcSeqInit>::create(m_seed, pGenerator);
	RDOParser::s_parser()->runtime()->addInitCalc(m_pInitCalc);
	m_pNextCalc = rdo::Factory<rdo::runtime::RDOCalcSeqNextExponential>::create(pGenerator);
	initResult();
}

LPRDOFUNArithm RDOFUNSequenceExponential::createCallCalc(REF(LPRDOFUNParams) pParamList, CREF(RDOParserSrcInfo) seqSrcInfo) const
{
	if (pParamList->getParamList()->getContainer().size() != 1)
	{
		RDOParser::s_parser()->error().error(seqSrcInfo, rdo::format(_T("Для экспоненциального закона распределения '%s' нужно указать один параметр: математическое ожидание"), name().c_str()));
	}

	rdo::runtime::LPRDOCalcFunctionCaller pFuctionCall = rdo::Factory<rdo::runtime::RDOCalcFunctionCaller>::create(m_pNextCalc);
	ASSERT(pFuctionCall);

	LPTypeInfo              pType = rdo::Factory<TypeInfo>::delegate<RDOType__real>(RDOParserSrcInfo());
	rdo::runtime::LPRDOCalc pArg1 = pParamList->getCalc(0, pType);

	pFuctionCall->addParameter(pArg1);

	LPExpression pExpression = rdo::Factory<Expression>::create(
		m_pHeader->getTypeInfo(),
		pFuctionCall,
		pParamList->src_info()
	);
	ASSERT(pExpression);
	LPRDOFUNArithm pArithm = rdo::Factory<RDOFUNArithm>::create(pExpression);
	ASSERT(pArithm);
	pArithm->setSrcInfo(seqSrcInfo);
	if (pArithm->typeID() == rdo::runtime::RDOType::t_enum)
	{
		RDOParser::s_parser()->error().error(src_info(), _T("Внутренняя ошибка парсера"));
	}
	return pArithm;
}

// --------------------------------------------------------------------------------
// -------------------- RDOFUNSequenceNormal
// --------------------------------------------------------------------------------
RDOFUNSequenceNormal::RDOFUNSequenceNormal(CREF(LPRDOFUNSequenceHeader) pHeader, int seed)
	: RDOFUNSequence(pHeader, seed)
{
	if (m_pHeader->getTypeInfo()->type()->typeID() == rdo::runtime::RDOType::t_enum)
	{
		RDOParser::s_parser()->error().error(src_info(), rdo::format(_T("Последовательность '%s' может возвращять значения только целого или вещественного типа"), src_text().c_str()));
	}
}

void RDOFUNSequenceNormal::createCalcs()
{
	PTR(rdo::runtime::RandGeneratorNormal) pGenerator = new rdo::runtime::RandGeneratorNormal();
	m_pInitCalc = rdo::Factory<rdo::runtime::RDOCalcSeqInit>::create(m_seed, pGenerator);
	RDOParser::s_parser()->runtime()->addInitCalc(m_pInitCalc);
	m_pNextCalc = rdo::Factory<rdo::runtime::RDOCalcSeqNextNormal>::create(pGenerator);
	initResult();
}

LPRDOFUNArithm RDOFUNSequenceNormal::createCallCalc(REF(LPRDOFUNParams) pParamList, CREF(RDOParserSrcInfo) seqSrcInfo) const
{
	if (pParamList->getParamList()->getContainer().size() != 2)
	{
		RDOParser::s_parser()->error().error(seqSrcInfo, rdo::format(_T("Для нормального закона распределения '%s' нужно указать два параметра: математическое ожидание и среднее квадратическое отклонение"), name().c_str()));
	}

	rdo::runtime::LPRDOCalcFunctionCaller pFuctionCall = rdo::Factory<rdo::runtime::RDOCalcFunctionCaller>::create(m_pNextCalc);
	ASSERT(pFuctionCall);

	LPTypeInfo              pType = rdo::Factory<TypeInfo>::delegate<RDOType__real>(RDOParserSrcInfo());
	rdo::runtime::LPRDOCalc pArg1 = pParamList->getCalc(0, pType);
	rdo::runtime::LPRDOCalc pArg2 = pParamList->getCalc(1, pType);

	pFuctionCall->addParameter(pArg1);
	pFuctionCall->addParameter(pArg2);

	LPExpression pExpression = rdo::Factory<Expression>::create(
		m_pHeader->getTypeInfo(),
		pFuctionCall,
		pParamList->src_info()
	);
	ASSERT(pExpression);
	LPRDOFUNArithm pArithm = rdo::Factory<RDOFUNArithm>::create(pExpression);
	ASSERT(pArithm);
	pArithm->setSrcInfo(seqSrcInfo);
	if (pArithm->typeID() == rdo::runtime::RDOType::t_enum)
	{
		RDOParser::s_parser()->error().error(src_info(), _T("Внутренняя ошибка парсера"));
	}
	return pArithm;
}

// ----------------------------------------------------------------------------
// ---------- RDOFUNSequenceTriangular
// ----------------------------------------------------------------------------
RDOFUNSequenceTriangular::RDOFUNSequenceTriangular(CREF(LPRDOFUNSequenceHeader) pHeader, int seed)
	: RDOFUNSequence(pHeader, seed)
{
	if (m_pHeader->getTypeInfo()->type()->typeID() != rdo::runtime::RDOType::t_int && m_pHeader->getTypeInfo()->type()->typeID() != rdo::runtime::RDOType::t_real)
	{
		RDOParser::s_parser()->error().error(src_info(), rdo::format(_T("Последовательность '%s' может возвращять значения только целого или вещественного типа"), src_text().c_str()));
	}
}

void RDOFUNSequenceTriangular::createCalcs()
{
	PTR(rdo::runtime::RandGeneratorTriangular) pGenerator = new rdo::runtime::RandGeneratorTriangular();
	m_pInitCalc = rdo::Factory<rdo::runtime::RDOCalcSeqInit>::create(m_seed, pGenerator);
	RDOParser::s_parser()->runtime()->addInitCalc(m_pInitCalc);
	m_pNextCalc = rdo::Factory<rdo::runtime::RDOCalcSeqNextTriangular>::create(pGenerator);
	initResult();
}

LPRDOFUNArithm RDOFUNSequenceTriangular::createCallCalc(REF(LPRDOFUNParams) pParamList, CREF(RDOParserSrcInfo) seqSrcInfo) const
{
	if (pParamList->getParamList()->getContainer().size() != 3)
	{
		RDOParser::s_parser()->error().error(seqSrcInfo, rdo::format(_T("Для треугольного закона распределения '%s' нужно указать три параметра: левую границу, точку под высотой треугольника, правую границу"), name().c_str()));
	}

	rdo::runtime::LPRDOCalcFunctionCaller pFuctionCall = rdo::Factory<rdo::runtime::RDOCalcFunctionCaller>::create(m_pNextCalc);
	ASSERT(pFuctionCall);

	LPTypeInfo              pType = rdo::Factory<TypeInfo>::delegate<RDOType__real>(RDOParserSrcInfo());
	rdo::runtime::LPRDOCalc pArg1 = pParamList->getCalc(0, pType);
	rdo::runtime::LPRDOCalc pArg2 = pParamList->getCalc(1, pType);
	rdo::runtime::LPRDOCalc pArg3 = pParamList->getCalc(2, pType);

	pFuctionCall->addParameter(pArg1);
	pFuctionCall->addParameter(pArg2);
	pFuctionCall->addParameter(pArg3);

	LPExpression pExpression = rdo::Factory<Expression>::create(
		m_pHeader->getTypeInfo(),
		pFuctionCall,
		pParamList->src_info()
	);
	ASSERT(pExpression);
	LPRDOFUNArithm pArithm = rdo::Factory<RDOFUNArithm>::create(pExpression);
	ASSERT(pArithm);
	pArithm->setSrcInfo(seqSrcInfo);
	if (pArithm->typeID() == rdo::runtime::RDOType::t_enum)
	{
		RDOParser::s_parser()->error().error(src_info(), _T("Внутренняя ошибка парсера"));
	}
	return pArithm;
}

// ----------------------------------------------------------------------------
// ---------- Гистограмма
// ----------------------------------------------------------------------------
// ---------- RDOFUNSequenceByHist
// ----------------------------------------------------------------------------
RDOFUNSequenceByHist::RDOFUNSequenceByHist(CREF(LPRDOFUNSequenceByHistHeader) pHeader)
	: RDOFUNSequence(pHeader->getHeader(), pHeader->getSeed())
{}

LPRDOFUNArithm RDOFUNSequenceByHist::createCallCalc(REF(LPRDOFUNParams) pParamList, CREF(RDOParserSrcInfo) srcInfo) const
{
	if (pParamList->getParamList()->getContainer().size() != 0)
	{
		RDOParser::s_parser()->error().error(srcInfo, rdo::format(_T("Гистограмма '%s' должна вызываться без параметров"), name().c_str()));
	}

	rdo::runtime::LPRDOCalcFunctionCaller pFuctionCall = rdo::Factory<rdo::runtime::RDOCalcFunctionCaller>::create(m_pNextCalc);
	ASSERT(pFuctionCall);

	LPExpression pExpression = rdo::Factory<Expression>::create(
		m_pHeader->getTypeInfo(),
		pFuctionCall,
		pParamList->src_info()
	);
	ASSERT(pExpression);
	LPRDOFUNArithm pArithm = rdo::Factory<RDOFUNArithm>::create(pExpression);
	ASSERT(pArithm);
	pArithm->setSrcInfo(srcInfo);
	return pArithm;
}

// --------------------------------------------------------------------------------
// -------------------- RDOFUNSequenceByHistReal
// --------------------------------------------------------------------------------
RDOFUNSequenceByHistReal::RDOFUNSequenceByHistReal(CREF(LPRDOFUNSequenceByHistHeader) pHeader, CREF(LPRDOValue) pFrom, CREF(LPRDOValue) pTo, CREF(LPRDOValue) pFreq)
	: RDOFUNSequenceByHist(pHeader)
{
	addReal(pFrom, pTo, pFreq);
}

void RDOFUNSequenceByHistReal::addReal(CREF(LPRDOValue) pFrom, CREF(LPRDOValue) pTo, CREF(LPRDOValue) pFreq)
{
	ASSERT(pFrom);
	ASSERT(pTo  );
	ASSERT(pFreq);

	if (!m_to.empty() && pFrom->value() != m_to.back())
	{
		RDOParser::s_parser()->error().error(pFrom->src_info(), _T("Начало интервала должно совпадать с окончанием предыдущего"));
	}
	if (pFrom->value() > pTo->value())
	{
		RDOParser::s_parser()->error().error(pTo->src_info(), _T("Начало интервала должно быть меньше его конца"));
	}
	if (pFreq->value() == 0)
	{
		RDOParser::s_parser()->error().warning(pFreq->src_info(), _T("Относительная вероятность должна быть больше нуля"));
	}
	else if (pFreq->value() < 0)
	{
		RDOParser::s_parser()->error().error(pFreq->src_info(), _T("Относительная вероятность должна быть больше нуля"));
	}
	m_pHeader->getTypeInfo()->value_cast(pFrom);
	m_pHeader->getTypeInfo()->value_cast(pTo  );
	m_from.push_back(pFrom->value());
	m_to  .push_back(pTo  ->value());
	m_freq.push_back(pFreq->value());
}

void RDOFUNSequenceByHistReal::createCalcs()
{
	PTR(rdo::runtime::RandGeneratorByHistReal) pGenerator = new rdo::runtime::RandGeneratorByHistReal();
	int size = m_from.size();
	for (int i = 0; i < size; i++)
	{
		pGenerator->addValues(m_from[i].getDouble(), m_to[i].getDouble(), m_freq[i].getDouble());
	}

	m_pInitCalc = rdo::Factory<rdo::runtime::RDOCalcSeqInit>::create(m_seed, pGenerator);
	RDOParser::s_parser()->runtime()->addInitCalc(m_pInitCalc);
	m_pNextCalc = rdo::Factory<rdo::runtime::RDOCalcSeqNextByHist>::create(pGenerator);
	initResult();
}

// --------------------------------------------------------------------------------
// -------------------- RDOFUNSequenceByHistEnum
// --------------------------------------------------------------------------------
RDOFUNSequenceByHistEnum::RDOFUNSequenceByHistEnum(CREF(LPRDOFUNSequenceByHistHeader) pHeader, CREF(LPRDOValue) pValue, CREF(LPRDOValue) pFreq)
	: RDOFUNSequenceByHist(pHeader)
{
	addEnum(pValue, pFreq);
}

void RDOFUNSequenceByHistEnum::addEnum(CREF(LPRDOValue) pValue, CREF(LPRDOValue) pFreq)
{
	ASSERT(pValue);
	ASSERT(pFreq );

	if (pFreq->value() == 0)
	{
		RDOParser::s_parser()->error().warning(pFreq->src_info(), _T("Относительная вероятность должна быть больше нуля"));
	}
	else if (pFreq->value() < 0)
	{
		RDOParser::s_parser()->error().error(pFreq->src_info(), _T("Относительная вероятность должна быть больше нуля"));
	}
	m_pHeader->getTypeInfo()->value_cast(pValue);
	rdo::runtime::RDOValue enum_id = m_pHeader->getTypeInfo()->value_cast(pValue)->value();
	if (std::find(m_values.begin(), m_values.end(), enum_id) != m_values.end())
	{
		RDOParser::s_parser()->error().error(pValue->src_info(), rdo::format(_T("Перечислимое значение определено дважды: %s"), pValue->value().getIdentificator().c_str()));
	}
	m_values.push_back(enum_id);
	m_freq  .push_back(pFreq->value());
}

void RDOFUNSequenceByHistEnum::createCalcs()
{
	PTR(rdo::runtime::RandGeneratorByHistEnum) pGenerator = new rdo::runtime::RandGeneratorByHistEnum();
	ruint size = m_values.size();
	for (ruint i = 0; i < size; i++)
	{
		pGenerator->addValues(m_values[i], m_freq[i].getDouble());
	}
	m_pInitCalc = rdo::Factory<rdo::runtime::RDOCalcSeqInit>::create(m_seed, pGenerator);
	RDOParser::s_parser()->runtime()->addInitCalc(m_pInitCalc);
	m_pNextCalc = rdo::Factory<rdo::runtime::RDOCalcSeqNextByHist>::create(pGenerator);
	initCalcSrcInfo();
}

// --------------------------------------------------------------------------------
// -------------------- Перечень значений
// --------------------------------------------------------------------------------
// -------------------- RDOFUNSequenceEnumerative
// --------------------------------------------------------------------------------
LPRDOFUNArithm RDOFUNSequenceEnumerative::createCallCalc(REF(LPRDOFUNParams) pParamList, CREF(RDOParserSrcInfo) srcInfo) const
{
	if (pParamList->getParamList()->getContainer().size() != 0)
	{
		RDOParser::s_parser()->error().error(srcInfo, rdo::format(_T("Перечисление '%s' должно вызываться без параметров"), name().c_str()));
	}

	rdo::runtime::LPRDOCalcFunctionCaller pFuctionCall = rdo::Factory<rdo::runtime::RDOCalcFunctionCaller>::create(m_pNextCalc);
	ASSERT(pFuctionCall);

	LPExpression pExpression = rdo::Factory<Expression>::create(
		m_pHeader->getTypeInfo(),
		pFuctionCall,
		pParamList->src_info()
	);
	ASSERT(pExpression);
	LPRDOFUNArithm pArithm = rdo::Factory<RDOFUNArithm>::create(pExpression);
	ASSERT(pArithm);
	pArithm->setSrcInfo(srcInfo);
	return pArithm;
}

// --------------------------------------------------------------------------------
// -------------------- RDOFUNSequenceEnumerative
// --------------------------------------------------------------------------------
void RDOFUNSequenceEnumerative::createCalcs()
{
	PTR(rdo::runtime::RandGeneratorEnumerative) pGenerator = new rdo::runtime::RandGeneratorEnumerative();
	for (ruint i = 0; i < m_valueList.size(); i++)
	{
		pGenerator->addValue(m_valueList[i]->value());
	}
	m_pNextCalc = rdo::Factory<rdo::runtime::RDOCalcSeqNextByHist>::create(pGenerator);
	initCalcSrcInfo();
}

// --------------------------------------------------------------------------------
// -------------------- Функции
// --------------------------------------------------------------------------------
// -------------------- RDOFUNFunctionListElement
// --------------------------------------------------------------------------------
RDOFUNFunctionListElement::RDOFUNFunctionListElement(CREF(RDOParserSrcInfo) srcInfo)
	: RDOParserSrcInfo(srcInfo)
{}

RDOFUNFunctionListElement::~RDOFUNFunctionListElement()
{}

rdo::runtime::LPRDOCalcIsEqual RDOFUNFunctionListElement::createIsEqualCalc(CREF(LPTypeInfo) pRetType, CREF(rdo::runtime::LPRDOCalcFuncParam) pFuncParam, CREF(RDOParserSrcInfo) src_pos) const
{
	rdo::runtime::LPRDOCalcConst pCalcConst = createResultCalc(pRetType, src_pos);
	return rdo::Factory<rdo::runtime::RDOCalcIsEqual>::create(pFuncParam, pCalcConst);
}

// --------------------------------------------------------------------------------
// -------------------- RDOFUNFunctionListElementIdentif
// --------------------------------------------------------------------------------
RDOFUNFunctionListElementIdentif::RDOFUNFunctionListElementIdentif(CREF(RDOParserSrcInfo) srcInfo)
	: RDOFUNFunctionListElement(srcInfo)
{}

rdo::runtime::LPRDOCalcConst RDOFUNFunctionListElementIdentif::createResultCalc(CREF(LPTypeInfo) pRetType, CREF(RDOParserSrcInfo) src_pos) const
{
	rdo::runtime::LPRDOCalcConst pCalcConst = rdo::Factory<rdo::runtime::RDOCalcConst>::create(
		pRetType->value_cast(
			rdo::Factory<RDOValue>::create(RDOParserSrcInfo(src_pos.getPosAsYY(), src_text()))
		)->value()
	);
	ASSERT(pCalcConst);
	pCalcConst->setSrcInfo(src_pos);
	return pCalcConst;
}

// --------------------------------------------------------------------------------
// -------------------- RDOFUNFunctionListElementReal
// --------------------------------------------------------------------------------
RDOFUNFunctionListElementReal::RDOFUNFunctionListElementReal(CREF(YYLTYPE) position, double value)
	: RDOFUNFunctionListElement(RDOParserSrcInfo(position, rdo::format(_T("%f"), value)))
	, m_value(value)
{}

rdo::runtime::LPRDOCalcConst RDOFUNFunctionListElementReal::createResultCalc(CREF(LPTypeInfo) pRetType, CREF(RDOParserSrcInfo) src_pos) const
{
	LPTypeInfo pType = rdo::Factory<TypeInfo>::delegate<RDOType__real>(src_info());
	ASSERT(pType);
	rdo::runtime::LPRDOCalcConst pCalcConst = rdo::Factory<rdo::runtime::RDOCalcConst>::create(
		pRetType->value_cast(
			rdo::Factory<RDOValue>::create(m_value, src_pos, pType)
		)->value()
	);
	ASSERT(pCalcConst);
	pCalcConst->setSrcInfo(src_pos);
	return pCalcConst;
}

// --------------------------------------------------------------------------------
// -------------------- RDOFUNFunctionListElementInt
// --------------------------------------------------------------------------------
RDOFUNFunctionListElementInt::RDOFUNFunctionListElementInt(CREF(YYLTYPE) position, int value)
	: RDOFUNFunctionListElement(RDOParserSrcInfo(position, rdo::format(_T("%d"), value)))
	, m_value(value)
{}

rdo::runtime::LPRDOCalcConst RDOFUNFunctionListElementInt::createResultCalc(CREF(LPTypeInfo) pRetType, CREF(RDOParserSrcInfo) src_pos) const
{
	LPTypeInfo pType = rdo::Factory<TypeInfo>::delegate<RDOType__int>(src_info());
	ASSERT(pType);
	rdo::runtime::LPRDOCalcConst pCalcConst = rdo::Factory<rdo::runtime::RDOCalcConst>::create(
		pRetType->value_cast(
			rdo::Factory<RDOValue>::create(m_value, src_pos, pType)
		)->value()
	);
	ASSERT(pCalcConst);
	pCalcConst->setSrcInfo(src_pos);
	return pCalcConst;
}

// --------------------------------------------------------------------------------
// -------------------- RDOFUNFunctionListElementEq
// --------------------------------------------------------------------------------
RDOFUNFunctionListElementEq::RDOFUNFunctionListElementEq(CREF(YYLTYPE) position)
	: RDOFUNFunctionListElement(RDOParserSrcInfo(position, _T("=")))
{}

rdo::runtime::LPRDOCalcConst RDOFUNFunctionListElementEq::createResultCalc(CREF(LPTypeInfo) pRetType, CREF(RDOParserSrcInfo) src_pos) const
{
	UNUSED(pRetType);
	RDOParser::s_parser()->error().error(src_pos, _T("Внутренная ошибка парсера: RDOFUNFunctionListElementEq::createResultCalc"));
	NEVER_REACH_HERE;
	return NULL;
}

// --------------------------------------------------------------------------------
// -------------------- RDOFUNCalculateIf
// --------------------------------------------------------------------------------
RDOFUNCalculateIf::RDOFUNCalculateIf(CREF(LPRDOFUNLogic) pCondition, CREF(LPRDOFUNArithm) pAction)
	: m_pCondition(pCondition)
	, m_pAction   (pAction   )
{
	m_pAction->checkParamType(RDOParser::s_parser()->getLastFUNFunction()->getReturn()->getTypeInfo());
}

RDOFUNCalculateIf::~RDOFUNCalculateIf()
{}

// --------------------------------------------------------------------------------
// -------------------- RDOFUNFunction
// --------------------------------------------------------------------------------
RDOFUNFunction::RDOFUNFunction(CREF(RDOParserSrcInfo) srcInfo, CREF(LPRDOParam) pReturn)
	: Function (pReturn->getTypeInfo(), srcInfo)
	, m_pReturn(pReturn )
{
	init();
}

RDOFUNFunction::RDOFUNFunction(CREF(tstring) name, CREF(LPRDOParam) pReturn)
	: Function (pReturn->getTypeInfo(), RDOParserSrcInfo(name))
	, m_pReturn(pReturn)
{
	init();
}

RDOFUNFunction::~RDOFUNFunction()
{}

void RDOFUNFunction::init()
{
	RDOParser::s_parser()->insertFUNFunction(this);
	Function::pushContext();
}

void RDOFUNFunction::end()
{
	Function::popContext();
}

CREF(tstring) RDOFUNFunction::name() const
{
	return src_info().src_text();
}

void RDOFUNFunction::add(CREF(LPRDOFUNFunctionListElement) pParam)
{
	m_elementList.push_back(pParam); 
}

void RDOFUNFunction::add(CREF(LPRDOFUNCalculateIf) pCalculateIf)
{
	m_calculateIfList.push_back(pCalculateIf);
}

void RDOFUNFunction::createListCalc()
{
	if (!m_pReturn->getDefault()->defined())
	{
		RDOParser::s_parser()->error().error(m_pReturn->src_info(), rdo::format(_T("Функция '%s' должна иметь значение по умолчанию"), name().c_str()));
	}
	if (getParams().empty())
	{
		RDOParser::s_parser()->error().error(src_info(), rdo::format(_T("Функция '%s' должна иметь параметры, т.к. её тип list"), name().c_str()));
	}
	if (m_elementList.empty())
	{
		RDOParser::s_parser()->error().warning(src_info(), rdo::format(_T("Функция '%s' не содержит списка"), name().c_str()));
	}

	rdo::runtime::LPRDOCalcConst pCalcDefault = rdo::Factory<rdo::runtime::RDOCalcConst>::create(m_pReturn->getDefault()->value());
	ASSERT(pCalcDefault);
	pCalcDefault->setSrcInfo(m_pReturn->getDefault()->src_info());
	rdo::runtime::LPRDOFunListCalc pFunListCalc = rdo::Factory<rdo::runtime::RDOFunListCalc>::create(pCalcDefault);
	ASSERT(pFunListCalc);
	pFunListCalc->setSrcInfo(src_info());
	ElementList::const_iterator elem_it = m_elementList.begin();
	while (elem_it != m_elementList.end())
	{
		rdo::runtime::LPRDOCalc pCaseCalc = rdo::Factory<rdo::runtime::RDOCalcConst>::create(true);
		ASSERT(pCaseCalc);
		pCaseCalc->setSrcInfo((*elem_it)->src_info());
		int currParamNumber = 0;
		ParamList::const_iterator param_it = getParams().begin();
		while (param_it != getParams().end())
		{
			LPRDOParam pParam = *param_it;
			if (elem_it == m_elementList.end())
			{
				--elem_it;
				RDOParser::s_parser()->error().error((*elem_it)->src_info(), rdo::format(_T("Ожидается значение для параметра '%s'"), pParam->name().c_str()));
			}
			LPRDOFUNFunctionListElement pListElement = *elem_it;
			ASSERT(pListElement);
			if (pListElement->isEquivalence())
			{
				if ((param_it + 1) == getParams().end())
				{
					RDOParser::s_parser()->error().error(pListElement->src_info(), rdo::format(_T("Перед знаком равенства ожидается значение для параметра: %s"),pParam->name().c_str()));
				}
				else
				{
					tstring str = (*param_it)->src_text();
					++param_it;
					while (param_it != getParams().end())
					{
						str += _T(", ");
						str += (*param_it)->src_text();
						++param_it;
					}
					RDOParser::s_parser()->error().error(pListElement->src_info(), rdo::format(_T("Перед знаком равенства ожидаются значения для параметров: %s"), str.c_str()));
				}
			}
			rdo::runtime::LPRDOCalcFuncParam pFuncParam   = rdo::Factory<rdo::runtime::RDOCalcFuncParam>::create(currParamNumber, pParam->src_info());
			ASSERT(pFuncParam);
			rdo::runtime::LPRDOCalcIsEqual   pCompareCalc = pListElement->createIsEqualCalc(pParam->getTypeInfo(), pFuncParam, pListElement->src_info());
			ASSERT(pCompareCalc);
			rdo::runtime::LPRDOCalc          pAndCalc     = rdo::Factory<rdo::runtime::RDOCalcAnd>::create(pCaseCalc, pCompareCalc);
			ASSERT(pAndCalc);
			pCaseCalc = pAndCalc;
			++elem_it;
			++param_it;
			++currParamNumber;
		}
		if (elem_it == m_elementList.end())
		{
			--elem_it;
			--param_it;
			RDOParser::s_parser()->error().error((*elem_it)->src_info(), rdo::format(_T("После значения для параметра '%s' ожидается знак равенства"), (*param_it)->name().c_str()));
		}
		if (!(*elem_it)->isEquivalence())
		{
			RDOParser::s_parser()->error().error((*elem_it)->src_info(), rdo::format(_T("Слишком много значений, вместо '%s' ожидается знак равенства"), (*elem_it)->src_text().c_str()));
		}
		++elem_it;
		if (elem_it == m_elementList.end())
		{
			--elem_it;
			RDOParser::s_parser()->error().error((*elem_it)->src_info(), rdo::format(_T("После знака равенства ожидается значение функции '%s'"), name().c_str()));
		}
		rdo::runtime::LPRDOCalcConst pResultCalc = (*elem_it)->createResultCalc(m_pReturn->getTypeInfo(), (*elem_it)->src_info());
		pFunListCalc->addCase(pCaseCalc, pResultCalc);
		++elem_it;
	}
	setFunctionCalc(pFunListCalc);
}

void RDOFUNFunction::createTableCalc(CREF(YYLTYPE) elements_pos)
{
	ElementList::const_iterator it = m_elementList.begin();
	while (it != m_elementList.end())
	{
		if ((*it)->isEquivalence())
		{
			RDOParser::s_parser()->error().error((*it)->src_info(), _T("Символ '=' недопустим в табличной функции"));
		}
		++it;
	}
	int   param_cnt = getParams().size();
	ruint range     = 1;
	rdo::runtime::LPRDOCalc pCalc = rdo::Factory<rdo::runtime::RDOCalcConst>::create(0);
	ASSERT(pCalc);
	rdo::runtime::RDOSrcInfo srcInfo(src_info());
	srcInfo.setSrcText(_T("0"));
	pCalc->setSrcInfo(srcInfo);
	for (int currParam = 0; currParam < param_cnt; currParam++)
	{
		LPRDOParam pFunctionParam = getParams().at(currParam);
		ASSERT(pFunctionParam);
		rdo::runtime::LPRDOCalcFuncParam pFuncParam = rdo::Factory<rdo::runtime::RDOCalcFuncParam>::create(currParam, pFunctionParam->src_info());
		ASSERT(pFuncParam);
		rdo::runtime::LPRDOCalc pValue2 = pFuncParam;
		if (pFunctionParam->getTypeInfo()->type()->typeID() != rdo::runtime::RDOType::t_enum)
		{
			rdo::runtime::LPRDOCalcConst pCalcConst1 = rdo::Factory<rdo::runtime::RDOCalcConst>::create(1);
			pCalcConst1->setSrcInfo(pFunctionParam->src_info());
			pValue2 = rdo::Factory<rdo::runtime::RDOCalcMinus>::create(pValue2, pCalcConst1);
		}
		rdo::runtime::LPRDOCalcConst pCalcConst2 = rdo::Factory<rdo::runtime::RDOCalcConst>::create(range);
		ASSERT(pCalcConst2);
		pCalcConst2->setSrcInfo(pFunctionParam->src_info());
		rdo::runtime::LPRDOCalcMult pCalcMult = rdo::Factory<rdo::runtime::RDOCalcMultEnumSafe>::create(pCalcConst2, pValue2);
		rdo::runtime::LPRDOCalcPlus pCalcAdd  = rdo::Factory<rdo::runtime::RDOCalcPlusEnumSafe>::create(pCalcMult, pCalc);

		switch (pFunctionParam->getTypeInfo()->type()->typeID())
		{
		case rdo::runtime::RDOType::t_int:
			if (dynamic_cast<CPTR(RDOTypeIntRange)>(pFunctionParam->getTypeInfo()->type().get()))
			{
				LPRDOTypeIntRange pRange = pFunctionParam->getTypeInfo()->type().object_static_cast<RDOTypeIntRange>();
				if (pRange->range()->getMin()->value().getInt() != 1)
				{
					RDOParser::s_parser()->error().error(pRange->range()->src_info(), rdo::format(_T("Минимальное значение диапазона должно быть 1, текущий диапазон [%d..%d]"), pRange->range()->getMin()->value().getInt(), pRange->range()->getMax()->value().getInt()));
				}
				range *= pRange->range()->getMax()->value().getInt() - pRange->range()->getMin()->value().getInt() + 1;
			}
			else
			{
				RDOParser::s_parser()->error().error(pFunctionParam->getTypeInfo()->src_info(), _T("Для параметра табличной функции должен быть задан допустимый диапазон"));
			}
			break;
		case rdo::runtime::RDOType::t_enum:
			range *= pFunctionParam->getTypeInfo()->type().object_static_cast<RDOEnumType>()->getEnums()->getValues().size();
			break;
		default:
			RDOParser::s_parser()->error().error(pFunctionParam->getTypeInfo()->src_info(), _T("Параметр табличной функции может быть целого или перечислимого типа"));
		}
		pCalc = pCalcAdd;
	}

	if (m_elementList.size() != range)
	{
		RDOParser::s_parser()->error().error(elements_pos, rdo::format(_T("Неверное количество элементов табличной функции '%s': %d, требуется %d"), name().c_str(), m_elementList.size(), range));
	}

	rdo::runtime::LPRDOFuncTableCalc pFuncTableCalc = rdo::Factory<rdo::runtime::RDOFuncTableCalc>::create(pCalc);
	ASSERT(pFuncTableCalc);
	pFuncTableCalc->setSrcInfo(src_info());
	for (ruint currElem = 0; currElem < range; currElem++)
	{
		LPRDOFUNFunctionListElement pListElement = m_elementList.at(currElem);
		ASSERT(pListElement);
		rdo::runtime::LPRDOCalcConst  pResultCalc  = pListElement->createResultCalc(m_pReturn->getTypeInfo(), pListElement->src_info());
		ASSERT(pResultCalc);
		pFuncTableCalc->addResultCalc(pResultCalc);
	}
	setFunctionCalc(pFuncTableCalc);
}

void RDOFUNFunction::createAlgorithmicCalc()
{
	if (!m_returnFlag)
	{
		rdo::runtime::LPRDOCalcConst pCalcDefault;
		if (m_pReturn->getDefault()->defined())
		{
			LPRDOValue pDefault = m_pReturn->getTypeInfo()->value_cast(m_pReturn->getDefault());
			ASSERT(pDefault);
			pCalcDefault = rdo::Factory<rdo::runtime::RDOCalcConst>::create(pDefault->value());
		}
		else
		{
			//! Присвоить автоматическое значение по умолчанию, если оно не задано в явном виде
			pCalcDefault = rdo::Factory<rdo::runtime::RDOCalcConst>::create(m_pReturn->getTypeInfo()->type()->get_default());
			RDOParser::s_parser()->error().warning(src_info(), rdo::format(_T("Для функции '%s' неопределено значение по умолчанию"), name().c_str()));
		}
		ASSERT(pCalcDefault);
		pCalcDefault->setSrcInfo(m_pReturn->getTypeInfo()->src_info());
	}
}

CREF(LPRDOParam) RDOFUNFunction::getReturn() const
{
	return m_pReturn;
}

rbool RDOFUNFunction::getReturnFlag() const
{
	return m_returnFlag;
}

void RDOFUNFunction::setReturnFlag(rbool flag)
{
	m_returnFlag = flag;
}

rdo::runtime::LPRDOCalc RDOFUNFunction::getFunctionCalc() const
{
	return m_pFunctionCalc;
}

void RDOFUNFunction::setFunctionCalc(CREF(rdo::runtime::LPRDOCalc) pCalc)
{
	ASSERT(pCalc);
	m_pFunctionCalc = pCalc;
	if (m_pFunctionCalc->srcInfo().src_empty())
	{
		m_pFunctionCalc->setSrcInfo(src_info());
	}
}

// --------------------------------------------------------------------------------
// -------------------- RDOFUNGroup
// --------------------------------------------------------------------------------
RDOFUNGroup::RDOFUNGroup(CREF(RDOParserSrcInfo) res_info)
	: RDOParserSrcInfo(res_info)
{
	init(res_info);
}

RDOFUNGroup::~RDOFUNGroup()
{}

void RDOFUNGroup::init(CREF(RDOParserSrcInfo) res_info)
{
	m_pResType = RDOParser::s_parser()->findRTPResType(res_info.src_text());
	if (!m_pResType)
	{
		RDOParser::s_parser()->error().error(res_info, rdo::format(_T("Неизвестный тип ресурса: %s"), res_info.src_text().c_str()));
	}
	RDOParser::s_parser()->insertFUNGroup(this);
	RDOParser::s_parser()->contextStack()->push(this);
}

void RDOFUNGroup::end()
{
	RDOParser::s_parser()->contextStack()->pop<RDOFUNGroup>();
}

Context::FindResult RDOFUNGroup::onFindContext(CREF(LPRDOValue) pValue) const
{
	ASSERT(pValue);

	//! Ресурс внутри групповой функции
	if (getResType()->name() == pValue->value().getIdentificator())
	{
		LPExpression pExpression = rdo::Factory<Expression>::create(
			rdo::Factory<TypeInfo>::create(
				getResType(),
				pValue->src_info()
			),
			rdo::Factory<rdo::runtime::RDOCalcGetGroupFunctionResource>::create(),
			pValue->src_info()
		);
		ASSERT(pExpression);
		return Context::FindResult(const_cast<PTR(RDOFUNGroup)>(this), pExpression, pValue, getResType());
	}

	return Context::FindResult();
}

// --------------------------------------------------------------------------------
// -------------------- RDOFUNGroupLogic
// --------------------------------------------------------------------------------
RDOFUNGroupLogic::RDOFUNGroupLogic(FunGroupType funType, CREF(RDOParserSrcInfo) res_info)
	: RDOFUNGroup(res_info)
	, m_funType  (funType )
{}

LPRDOFUNLogic RDOFUNGroupLogic::createFunLogic(REF(LPRDOFUNLogic) pCondition)
{
	rdo::runtime::LPRDOFunCalcGroup pCalc;
	switch (m_funType)
	{
	case fgt_exist    : setSrcText(_T("Exist(")     + getResType()->name() + _T(": ") + pCondition->src_text() + _T(")")); pCalc = rdo::Factory<rdo::runtime::RDOFunCalcExist    >::create(getResType()->getNumber(), pCondition->getCalc()); break;
	case fgt_notexist : setSrcText(_T("NotExist(")  + getResType()->name() + _T(": ") + pCondition->src_text() + _T(")")); pCalc = rdo::Factory<rdo::runtime::RDOFunCalcNotExist >::create(getResType()->getNumber(), pCondition->getCalc()); break;
	case fgt_forall   : setSrcText(_T("ForAll(")    + getResType()->name() + _T(": ") + pCondition->src_text() + _T(")")); pCalc = rdo::Factory<rdo::runtime::RDOFunCalcForAll   >::create(getResType()->getNumber(), pCondition->getCalc()); break;
	case fgt_notforall: setSrcText(_T("NotForAll(") + getResType()->name() + _T(": ") + pCondition->src_text() + _T(")")); pCalc = rdo::Factory<rdo::runtime::RDOFunCalcNotForAll>::create(getResType()->getNumber(), pCondition->getCalc()); break;
	default: RDOParser::s_parser()->error().error(src_info(), _T("Внутренная ошибка: несуществующий тип функции"));
	}
	RDOParser::s_parser()->getFUNGroupStack().pop_back();
	end();

	LPExpression pExpression = rdo::Factory<Expression>::create(
		rdo::Factory<TypeInfo>::delegate<RDOType__bool>(pCalc->srcInfo()),
		pCalc,
		pCalc->srcInfo()
	);
	ASSERT(pExpression);

	LPRDOFUNLogic pLogic = rdo::Factory<RDOFUNLogic>::create(pExpression, false);
	ASSERT(pLogic);

	pLogic->setSrcInfo(src_info());
	return pLogic;
}

// --------------------------------------------------------------------------------
// -------------------- RDOFUNSelect
// --------------------------------------------------------------------------------
RDOFUNSelect::RDOFUNSelect(CREF(RDOParserSrcInfo) res_info)
	: RDOFUNGroup(RDOParserSrcInfo(res_info.src_text()))
{}

//! Сам Select как выборка по типу и условию
void RDOFUNSelect::initSelect(LPRDOFUNLogic pCondition)
{
	m_pCalcSelect = rdo::Factory<rdo::runtime::RDOFunCalcSelect>::create(getResType()->getRuntimeResType(), getResType()->getNumber(), pCondition->getCalc());
	m_pCalcSelect->setSrcInfo(pCondition->src_info());
}

//! Групповая функция над выборкой Select'а
LPRDOFUNLogic RDOFUNSelect::createFunSelectGroup(RDOFUNGroupLogic::FunGroupType funType, REF(LPRDOFUNLogic) pCondition)
{
	ASSERT(pCondition);
	rdo::runtime::LPRDOFunCalcSelectBase pCalc;
	switch (funType)
	{
	case RDOFUNGroupLogic::fgt_exist    : setSrcText(src_text() + _T(".Exist(")     + pCondition->src_text() + _T(")")); pCalc = rdo::Factory<rdo::runtime::RDOFunCalcSelectExist    >::create(m_pCalcSelect, pCondition->getCalc()); break;
	case RDOFUNGroupLogic::fgt_notexist : setSrcText(src_text() + _T(".NotExist(")  + pCondition->src_text() + _T(")")); pCalc = rdo::Factory<rdo::runtime::RDOFunCalcSelectNotExist >::create(m_pCalcSelect, pCondition->getCalc()); break;
	case RDOFUNGroupLogic::fgt_forall   : setSrcText(src_text() + _T(".ForAll(")    + pCondition->src_text() + _T(")")); pCalc = rdo::Factory<rdo::runtime::RDOFunCalcSelectForAll   >::create(m_pCalcSelect, pCondition->getCalc()); break;
	case RDOFUNGroupLogic::fgt_notforall: setSrcText(src_text() + _T(".NotForAll(") + pCondition->src_text() + _T(")")); pCalc = rdo::Factory<rdo::runtime::RDOFunCalcSelectNotForAll>::create(m_pCalcSelect, pCondition->getCalc()); break;
	default: RDOParser::s_parser()->error().error(pCondition->src_info(), _T("Внутренная ошибка: неизвестный метод для списка ресурсов"));
	}
	RDOParser::s_parser()->getFUNGroupStack().pop_back();
	end();

	LPExpression pExpression = rdo::Factory<Expression>::create(
		rdo::Factory<TypeInfo>::delegate<RDOType__bool>(pCalc->srcInfo()),
		pCalc,
		pCalc->srcInfo()
	);
	ASSERT(pExpression);

	LPRDOFUNLogic pLogic = rdo::Factory<RDOFUNLogic>::create(pExpression, false);
	ASSERT(pLogic);

	pLogic->setSrcInfo(src_info());
	return pLogic;
}

LPRDOFUNLogic RDOFUNSelect::createFunSelectEmpty(CREF(RDOParserSrcInfo) empty_info)
{
	setSrcText(src_text() + _T(".") + empty_info.src_text());
	RDOParser::s_parser()->getFUNGroupStack().pop_back();
	end();

	rdo::runtime::LPRDOCalc pCalc = rdo::Factory<rdo::runtime::RDOFunCalcSelectEmpty>::create(m_pCalcSelect);
	ASSERT(pCalc);
	pCalc->setSrcInfo(src_info());

	LPExpression pExpression = rdo::Factory<Expression>::create(
		rdo::Factory<TypeInfo>::delegate<RDOType__bool>(pCalc->srcInfo()),
		pCalc,
		pCalc->srcInfo()
	);
	ASSERT(pExpression);

	LPRDOFUNLogic pLogic = rdo::Factory<RDOFUNLogic>::create(pExpression, false);
	ASSERT(pLogic);

	pLogic->setSrcInfo(empty_info);
	return pLogic;
}

LPRDOFUNArithm RDOFUNSelect::createFunSelectSize(CREF(RDOParserSrcInfo) size_info)
{
	setSrcText(src_text() + _T(".") + size_info.src_text());
	RDOParser::s_parser()->getFUNGroupStack().pop_back();
	end();

	LPExpression pExpression = rdo::Factory<Expression>::create(
		rdo::Factory<TypeInfo>::delegate<RDOType__int>(size_info),
		rdo::Factory<rdo::runtime::RDOFunCalcSelectSize>::create(m_pCalcSelect),
		size_info
	);
	ASSERT(pExpression);

	LPRDOFUNArithm pArithm = rdo::Factory<RDOFUNArithm>::create(pExpression);
	ASSERT(pArithm);

	pArithm->setSrcInfo(size_info);
	return pArithm;
}

LPRDOFUNArithm RDOFUNSelect::createFunSelectArray(CREF(RDOParserSrcInfo) array_info)
{
	setSrcText(src_text() + _T(".") + array_info.src_text());
	RDOParser::s_parser()->getFUNGroupStack().pop_back();
	end();

	LPRDOArrayType pArrayType = rdo::Factory<RDOArrayType>::create(rdo::Factory<TypeInfo>::create(getResType(), array_info), array_info);

	LPExpression pExpression = rdo::Factory<Expression>::create(
		rdo::Factory<TypeInfo>::create(pArrayType, array_info),
		rdo::Factory<rdo::runtime::RDOFunCalcSelectArray>::create(m_pCalcSelect),
		array_info
	);
	ASSERT(pExpression);

	LPRDOFUNArithm pArithm = rdo::Factory<RDOFUNArithm>::create(pExpression);
	ASSERT(pArithm);

	pArithm->setSrcInfo(array_info);
	return pArithm;
}

CLOSE_RDO_PARSER_NAMESPACE
