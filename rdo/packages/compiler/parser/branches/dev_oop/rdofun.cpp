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
// ----------------------------------------------------------------------- SYNOPSIS
#include "simulator/compiler/parser/rdofun.h"
#include "simulator/compiler/parser/rdoparser.h"
#include "simulator/compiler/parser/rdortp.h"
#include "simulator/compiler/parser/rdorss.h"
#include "simulator/compiler/parser/rdopat.h"
#include "simulator/compiler/parser/rdofrm.h"
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
RDOFUNBase::RDOFUNBase(CREF(RDOParserSrcInfo) src_info)
	: RDOParserSrcInfo(src_info)
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
	if (pArithm->typeID() != rdoRuntime::RDOType::t_bool)
	{
		RDOParser::s_parser()->error().error(src_info(), rdo::format(_T("Ожидается логическое выражение, найдено: %s"), src_text().c_str()));
	}
}

RDOFUNLogic::RDOFUNLogic(CREF(LPExpression) pExpression, rbool hideWarning)
	: RDOFUNBase(pExpression)
{
	rdoRuntime::RDOSrcInfo src_info = m_pExpression->src_info();
	src_info.setSrcFileType(src_filetype());
	m_pExpression->setSrcInfo(src_info);
	if (!hideWarning)
	{
		rdoRuntime::LPRDOCalcConst pConstCalc = m_pExpression->calc().object_dynamic_cast<rdoRuntime::RDOCalcConst>();
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

LPRDOFUNLogic RDOFUNLogic::generateTrue(CREF(RDOParserSrcInfo) src_info)
{
	LPExpression pExpression = rdo::Factory<Expression>::create(
		rdo::Factory<TypeInfo>::create(
			rdo::Factory<RDOType__bool>::create(),
			src_info
		),
		rdo::Factory<rdoRuntime::RDOCalcConst>::create(true),
		src_info
	);
	ASSERT(pExpression);

	LPRDOFUNLogic pLogic = rdo::Factory<RDOFUNLogic>::create(pExpression, false);
	ASSERT(pLogic);

	return pLogic;
}

rdoRuntime::LPRDOCalc RDOFUNLogic::getCalc(rdoRuntime::RDOType::TypeID id)
{
	if (id != rdoRuntime::RDOType::t_real)
	{
		m_intOrDouble.roundCalc();
	}
	return m_pExpression->calc();
}

LPRDOFUNLogic RDOFUNLogic::createLogic(CREF(rdoRuntime::LPRDOCalc) pCalc)
{
	ASSERT(pCalc);

	LPExpression pExpression = rdo::Factory<Expression>::create(
		rdo::Factory<TypeInfo>::create(
			rdo::Factory<RDOType__bool>::create(),
			pCalc->srcInfo()
		),
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

	rdoRuntime::LPRDOCalc pCalc = rdoRuntime::RDOCalcBinaryBase::generateCalc<T>(m_pExpression->calc(), pSecond->m_pExpression->calc());
	ASSERT(pCalc);
	LPRDOFUNLogic pLogic = createLogic(pCalc);
	pLogic->m_intOrDouble.insert(m_intOrDouble, pSecond->m_intOrDouble);
	return pLogic;
}

template <class T>
LPRDOFUNLogic RDOFUNLogic::generateLogic(CREF(RDOSrcInfo::Position) position)
{
	rdoRuntime::LPRDOCalc pCalc = rdoRuntime::RDOCalcUnaryBase::generateCalc<T>(position, m_pExpression->calc());
	ASSERT(pCalc);
	LPRDOFUNLogic pLogic = createLogic(pCalc);
	pLogic->m_intOrDouble.insert(m_intOrDouble);
	return pLogic;
}

LPRDOFUNLogic RDOFUNLogic::operator&& (CREF(LPRDOFUNLogic) pSecond)
{
	return generateLogic<rdoRuntime::RDOCalcAnd>(pSecond);
}

LPRDOFUNLogic RDOFUNLogic::operator|| (CREF(LPRDOFUNLogic) pSecond)
{
	return generateLogic<rdoRuntime::RDOCalcOr>(pSecond);
}

LPRDOFUNLogic RDOFUNLogic::operator_not(CREF(RDOSrcInfo::Position) position)
{
	return generateLogic<rdoRuntime::RDOCalcNot>(position);
}

void RDOFUNLogic::setSrcInfo(CREF(RDOParserSrcInfo) src_info)
{
	RDOParserSrcInfo::setSrcInfo(src_info);
	m_pExpression->setSrcInfo(src_info);
}

void RDOFUNLogic::setSrcPos(CREF(RDOSrcInfo::Position) position)
{
	RDOParserSrcInfo::setSrcPos(position);
	rdoRuntime::RDOSrcInfo src_info = m_pExpression->src_info();
	src_info.setSrcPos(position);
	m_pExpression->setSrcInfo(src_info);
}

void RDOFUNLogic::setSrcText(CREF(tstring) value)
{
	RDOParserSrcInfo::setSrcText(value);
	rdoRuntime::RDOSrcInfo src_info = m_pExpression->src_info();
	src_info.setSrcText(value);
	m_pExpression->setSrcInfo(src_info);
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
				rdo::Factory<rdoRuntime::RDOCalcConst>::create(pConstant->value()),
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
rdoRuntime::LPRDOCalc RDOFUNArithm::generateCalc(CREF(rdoRuntime::RDOSrcInfo::Position) position, CREF(tstring) error)
{
	UNUSED(error);

	rdoRuntime::LPRDOCalc pCalc = rdoRuntime::RDOCalcUnaryBase::generateCalc<T>(position, m_pExpression->calc());
	ASSERT(pCalc);
	return pCalc;
}

template <class T>
rdoRuntime::LPRDOCalc RDOFUNArithm::generateCalc(CREF(LPRDOFUNArithm) pSecond, CREF(tstring) error)
{
	castType(pSecond, error);
	rdoRuntime::LPRDOCalc pCalc = rdoRuntime::RDOCalcBinaryBase::generateCalc<T>(m_pExpression->calc(), pSecond->m_pExpression->calc());
	ASSERT(pCalc);
	return pCalc;
}

template <class T>
LPRDOFUNArithm RDOFUNArithm::generateArithm(CREF(rdoRuntime::RDOSrcInfo::Position) position, CREF(tstring) error)
{
	rdoRuntime::LPRDOCalc pCalc = generateCalc<T>(position, error);
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
	rdoRuntime::LPRDOCalc pCalc = generateCalc<T>(pSecond, error);
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
	rdoRuntime::LPRDOCalc pCalc = generateCalc<T>(pSecond, error);
	ASSERT(pCalc);

	LPExpression pExpression = rdo::Factory<Expression>::create(
		rdo::Factory<TypeInfo>::create(
			rdo::Factory<RDOType__bool>::create(),
			pCalc->srcInfo()
		),
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

rdoRuntime::RDOValue RDOFUNArithm::const_value() const
{
	rdoRuntime::LPRDOCalcConst pCalc = calc().object_dynamic_cast<rdoRuntime::RDOCalcConst>();
	ASSERT(pCalc);
	return pCalc->getValue();
}

LPTypeInfo RDOFUNArithm::getPreType(CREF(LPRDOFUNArithm) pSecond)
{
	if (typeID() == rdoRuntime::RDOType::t_unknow)
	{
		RDOParser::s_parser()->error().error(src_info(), rdo::format(_T("Внутренная ошибка парсера: неопределенное значение арифметического выражения: %s"), src_text().c_str()));
	}
	if (pSecond->typeID() == rdoRuntime::RDOType::t_unknow)
	{
		RDOParser::s_parser()->error().error(pSecond->src_info(), rdo::format(_T("Внутренная ошибка парсера: неопределенное значение арифметического выражения: %s"), pSecond->src_text().c_str()));
	}
	if (typeID() == rdoRuntime::RDOType::t_identificator)
	{
		rdoRuntime::LPRDOCalcConst pCalc = calc().object_dynamic_cast<rdoRuntime::RDOCalcConst>();
		ASSERT(pCalc);
		RDOParser::s_parser()->error().error(src_info(), rdo::format(_T("Неизвестный идентификатор: %s"), const_value().getIdentificator().c_str()));
	}
	if (pSecond->typeID() == rdoRuntime::RDOType::t_identificator)
	{
		RDOParser::s_parser()->error().error(pSecond->src_info(), rdo::format(_T("Неизвестный идентификатор: %s"), pSecond->const_value().getIdentificator().c_str()));
	}

	return typeInfo()->type_cast(pSecond->typeInfo(), src_info());
}

LPRDOFUNArithm RDOFUNArithm::operator+ (CREF(LPRDOFUNArithm) pSecond)
{
	return generateArithm<rdoRuntime::RDOCalcPlus>(pSecond, _T("Ну не могу я сложить %s и %s"));
}

LPRDOFUNArithm RDOFUNArithm::operator- (CREF(LPRDOFUNArithm) pSecond)
{
	return generateArithm<rdoRuntime::RDOCalcMinus>(pSecond, _T("Нельзя из %s вычесть %s"));
}

LPRDOFUNArithm RDOFUNArithm::operator* (CREF(LPRDOFUNArithm) pSecond)
{
	return generateArithm<rdoRuntime::RDOCalcMult>(pSecond, _T("Нельзя %s умножить на %s"));
}

LPRDOFUNArithm RDOFUNArithm::operator/ (CREF(LPRDOFUNArithm) pSecond)
{
	rdoRuntime::LPRDOCalc pCalc = generateCalc<rdoRuntime::RDOCalcDiv>(pSecond, _T("Нельзя %s разделить на %s"));
	ASSERT(pCalc);
	LPTypeInfo pType = getPreType(pSecond);
	ASSERT(pType);

	/// @todo перевод вещественного в целое при делении. А что делать с умножением и т.д. ?
	//! Ответ: с умножением надо делать тоже самое, только непонятно как
	if (pType->type()->typeID() == rdoRuntime::RDOType::t_int)
	{
		rdoRuntime::LPRDOCalc pNewCalcDiv = pCalc;
		pCalc = rdo::Factory<rdoRuntime::RDOCalcDoubleToIntByResult>::create(pNewCalcDiv);
		pCalc->setSrcInfo(pNewCalcDiv->srcInfo());
	}
	LPExpression pExpression = rdo::Factory<Expression>::create(
		pType,
		pCalc,
		pCalc->srcInfo()
	);
	ASSERT(pExpression);
	LPRDOFUNArithm pArithm = rdo::Factory<RDOFUNArithm>::create(pExpression);
	if (pType->type()->typeID() == rdoRuntime::RDOType::t_int)
	{
		rdoRuntime::LPRDOCalcDoubleToIntByResult pResult = pCalc.object_static_cast<rdoRuntime::RDOCalcDoubleToIntByResult>();
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

LPRDOFUNArithm RDOFUNArithm::uminus(CREF(rdoRuntime::RDOSrcInfo::Position) position)
{
	return generateArithm<rdoRuntime::RDOCalcUMinus>(position, _T("Нельзя взять унарный минус от %s"));
}

LPRDOFUNLogic RDOFUNArithm::operator< (CREF(LPRDOFUNArithm) pSecond)
{
	return generateLogic<rdoRuntime::RDOCalcIsLess>(pSecond, _T("Нельзя сравнивать %s и %s"));
}

LPRDOFUNLogic RDOFUNArithm::operator> (CREF(LPRDOFUNArithm) pSecond)
{
	return generateLogic<rdoRuntime::RDOCalcIsGreater>(pSecond, _T("Нельзя сравнивать %s и %s"));
}

LPRDOFUNLogic RDOFUNArithm::operator<= (CREF(LPRDOFUNArithm) pSecond)
{
	return generateLogic<rdoRuntime::RDOCalcIsLEQ>(pSecond, _T("Нельзя сравнивать %s и %s"));
}

LPRDOFUNLogic RDOFUNArithm::operator>= (CREF(LPRDOFUNArithm) pSecond)
{
	return generateLogic<rdoRuntime::RDOCalcIsGEQ>(pSecond, _T("Нельзя сравнивать %s и %s"));
}

LPRDOFUNLogic RDOFUNArithm::operator== (CREF(LPRDOFUNArithm) pSecond)
{
	castValue(pSecond, _T("Ошибка сравнения %s == %s"));
	return generateLogic<rdoRuntime::RDOCalcIsEqual>(pSecond, _T("Нельзя сравнивать %s и %s"));
}

LPRDOFUNLogic RDOFUNArithm::operator!= (CREF(LPRDOFUNArithm) pSecond)
{
	castValue(pSecond, _T("Ошибка сравнения %s <> %s"));
	return generateLogic<rdoRuntime::RDOCalcIsNotEqual>(pSecond, _T("Нельзя сравнивать %s и %s"));
}

void RDOFUNArithm::checkParamType(CREF(LPTypeInfo) pType)
{
	pType->type()->type_cast(typeInfo()->type(), src_info(), pType->src_info(), src_info());
	rdoRuntime::LPRDOCalcConst pConstCalc = calc().object_dynamic_cast<rdoRuntime::RDOCalcConst>();
	if (pConstCalc)
	{
		rdoRuntime::RDOValue value = pConstCalc->getValue();
		pType->value_cast(rdo::Factory<RDOValue>::create(value, src_info(), typeInfo()));
	}
}

void RDOFUNArithm::wrongVarInit(CREF(LPRDOValue) pParamName,CREF(tstring) paramName)
{
	LPRDOFUNFunction pFunction = RDOParser::s_parser()->getLastFUNFunction();
	ASSERT(pFunction);
	LPRDOParam pParam = pFunction->findFUNFunctionParam(paramName);
	if(pParam)
	{
		RDOParser::s_parser()->error().error(pParamName->src_info(), rdo::format(_T("Функции не могут изменить свой параметр: %s"), paramName.c_str()));		
	}
	else
	{
		RDOParser::s_parser()->error().error(pParamName->src_info(), rdo::format(_T("Неизвестный идентификатор: %s"), paramName.c_str()));
	}
}
rdoRuntime::LPRDOCalc RDOFUNArithm::createCalc(CREF(LPTypeInfo) pForType)
{
	if (typeID() != rdoRuntime::RDOType::t_identificator)
	{
		if (!pForType)
		{
			return m_pExpression->calc();
		}
		if (pForType->type()->typeID() != rdoRuntime::RDOType::t_int)
		{
			if (pForType->type()->typeID() == rdoRuntime::RDOType::t_enum)
			{
				m_intOrDouble.roundCalc();
			}
			return m_pExpression->calc();
		}
		else
		{
			m_intOrDouble.roundCalc();
			rdoRuntime::LPRDOCalc pCalc = rdo::Factory<rdoRuntime::RDOCalcDoubleToInt>::create(m_pExpression->calc()->srcInfo().src_pos(), m_pExpression->calc());
			ASSERT(pCalc);
			pCalc->setSrcInfo(src_info());
			return pCalc;
		}
	}
	else if (typeID() == rdoRuntime::RDOType::t_identificator && !pForType)
	{
		RDOParser::s_parser()->error().error(src_info(), rdo::format(_T("Неизвестный идентификатор: %s"), const_value().getAsString().c_str()));
	}

	if (!pForType)
	{
		RDOParser::s_parser()->error().error(src_info(), _T("Неизвестный тип параметра"));
	}

	rdoRuntime::LPRDOCalc pNewCalc = rdo::Factory<rdoRuntime::RDOCalcConst>::create(pForType->value_cast(rdo::Factory<RDOValue>::create(src_info()))->value());
	pNewCalc->setSrcInfo(src_info());
	return pNewCalc;
}

void RDOFUNArithm::setSrcInfo(CREF(RDOParserSrcInfo) src_info)
{
	RDOParserSrcInfo::setSrcInfo(src_info);
	m_pExpression->setSrcInfo(src_info);
}

void RDOFUNArithm::setSrcPos(CREF(RDOSrcInfo::Position) position)
{
	RDOParserSrcInfo::setSrcPos(position);
	rdoRuntime::RDOSrcInfo src_info = m_pExpression->src_info();
	src_info.setSrcPos(position);
	m_pExpression->setSrcInfo(src_info);
}

void RDOFUNArithm::setSrcText(CREF(tstring) value)
{
	RDOParserSrcInfo::setSrcText(value);
	rdoRuntime::RDOSrcInfo src_info = m_pExpression->src_info();
	src_info.setSrcText(this->src_info().src_text());
	m_pExpression->setSrcInfo(src_info);
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
RDOFUNConstant::RDOFUNConstant(CREF(RDOParserSrcInfo) src_info, CREF(LPTypeInfo) pType, CREF(LPRDOValue) pDefault)
	: RDOParam(src_info, pType, pDefault)
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

rdoRuntime::LPRDOCalc RDOFUNParams::getCalc(ruint paramID, CREF(LPTypeInfo) pType)
{
	ASSERT(paramID < m_pArithmContainer->getContainer().size());
	rdoRuntime::LPRDOCalc pCalc = m_pArithmContainer->getContainer()[paramID]->createCalc(pType);
	ASSERT(pCalc);
	return pCalc;
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

	rdoRuntime::LPRDOCalc pCalc = pFunction->getFunctionCalc().object_parent_cast<rdoRuntime::RDOCalc>();
	ASSERT(pCalc);
	pCalc = pFunction->getReturn()->getTypeInfo()->type()->calc_cast(pCalc, pFunction->getReturn()->getTypeInfo()->type());
	ASSERT(pCalc);
	rdoRuntime::LPRDOCalcFunctionCaller pFuncCall = rdo::Factory<rdoRuntime::RDOCalcFunctionCaller>::create(pCalc);
	pFunction->insertPostLinked(pFuncCall);
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
		case rdoRuntime::RDOType::t_int:
		{
			LPRDOTypeIntRange pRange = m_pHeader->getTypeInfo()->type().object_dynamic_cast<RDOTypeIntRange>();
			if (pRange)
			{
				m_pNextCalc->m_range = rdoRuntime::RDOCalcSeqNext::Range(pRange->range()->getMin()->value().getDouble(), pRange->range()->getMax()->value().getDouble());
			}
			break;
		}
		case rdoRuntime::RDOType::t_real:
		{
			m_pNextCalc->m_res_real = true;
			LPRDOTypeRealRange pRange = m_pHeader->getTypeInfo()->type().object_dynamic_cast<RDOTypeRealRange>();
			if (pRange)
			{
				m_pNextCalc->m_range = rdoRuntime::RDOCalcSeqNext::Range(pRange->range()->getMin()->value().getDouble(), pRange->range()->getMax()->value().getDouble());
			}
			break;
		}
	}
}

void RDOFUNSequence::initCalcSrcInfo()
{
	if (m_pInitCalc)
	{
		rdoRuntime::RDOSrcInfo srcInfo(m_pHeader->src_info());
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
	if (m_pHeader->getTypeInfo()->type()->typeID() == rdoRuntime::RDOType::t_enum)
	{
		RDOParser::s_parser()->error().error(src_info(), rdo::format(_T("Последовательность '%s' может возвращять значения только целого или вещественного типа"), src_text().c_str()));
	}
}

void RDOFUNSequenceUniform::createCalcs()
{
	PTR(rdoRuntime::RandGeneratorUniform) pGenerator = new rdoRuntime::RandGeneratorUniform();
	m_pInitCalc = rdo::Factory<rdoRuntime::RDOCalcSeqInit>::create(m_seed, pGenerator);
	RDOParser::s_parser()->runtime()->addInitCalc(m_pInitCalc);
	m_pNextCalc = rdo::Factory<rdoRuntime::RDOCalcSeqNextUniform>::create(pGenerator);
	initResult();
}

LPRDOFUNArithm RDOFUNSequenceUniform::createCallCalc(REF(LPRDOFUNParams) pParamList, CREF(RDOParserSrcInfo) seq_src_info) const
{
	if (pParamList->getParamList()->getContainer().size() != 2)
	{
		RDOParser::s_parser()->error().error(seq_src_info, rdo::format(_T("Для равномерного закона распределения '%s' нужно указать два параметра: минимальную и максимальную границы диапазона"), name().c_str()));
	}

	rdoRuntime::LPRDOCalcFunctionCaller pFuctionCall = rdo::Factory<rdoRuntime::RDOCalcFunctionCaller>::create(m_pNextCalc);
	ASSERT(pFuctionCall);

	LPTypeInfo            pType = rdo::Factory<TypeInfo>::create(rdo::Factory<RDOType__real>::create(), RDOParserSrcInfo());
	rdoRuntime::LPRDOCalc pArg1 = pParamList->getCalc(0, pType);
	rdoRuntime::LPRDOCalc pArg2 = pParamList->getCalc(1, pType);

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
	pArithm->setSrcInfo(seq_src_info);
	if (pArithm->typeID() == rdoRuntime::RDOType::t_enum)
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
	if (m_pHeader->getTypeInfo()->type()->typeID() != rdoRuntime::RDOType::t_int && m_pHeader->getTypeInfo()->type()->typeID() != rdoRuntime::RDOType::t_real)
	{
		RDOParser::s_parser()->error().error(src_info(), rdo::format(_T("Последовательность '%s' может возвращять значения только целого или вещественного типа"), src_text().c_str()));
	}
}

void RDOFUNSequenceExponential::createCalcs()
{
	PTR(rdoRuntime::RandGeneratorExponential) pGenerator = new rdoRuntime::RandGeneratorExponential();
	m_pInitCalc = rdo::Factory<rdoRuntime::RDOCalcSeqInit>::create(m_seed, pGenerator);
	RDOParser::s_parser()->runtime()->addInitCalc(m_pInitCalc);
	m_pNextCalc = rdo::Factory<rdoRuntime::RDOCalcSeqNextExponential>::create(pGenerator);
	initResult();
}

LPRDOFUNArithm RDOFUNSequenceExponential::createCallCalc(REF(LPRDOFUNParams) pParamList, CREF(RDOParserSrcInfo) seq_src_info) const
{
	if (pParamList->getParamList()->getContainer().size() != 1)
	{
		RDOParser::s_parser()->error().error(seq_src_info, rdo::format(_T("Для экспоненциального закона распределения '%s' нужно указать один параметр: математическое ожидание"), name().c_str()));
	}

	rdoRuntime::LPRDOCalcFunctionCaller pFuctionCall = rdo::Factory<rdoRuntime::RDOCalcFunctionCaller>::create(m_pNextCalc);
	ASSERT(pFuctionCall);

	LPTypeInfo            pType = rdo::Factory<TypeInfo>::create(rdo::Factory<RDOType__real>::create(), RDOParserSrcInfo());
	rdoRuntime::LPRDOCalc pArg1 = pParamList->getCalc(0, pType);

	pFuctionCall->addParameter(pArg1);

	LPExpression pExpression = rdo::Factory<Expression>::create(
		m_pHeader->getTypeInfo(),
		pFuctionCall,
		pParamList->src_info()
	);
	ASSERT(pExpression);
	LPRDOFUNArithm pArithm = rdo::Factory<RDOFUNArithm>::create(pExpression);
	ASSERT(pArithm);
	pArithm->setSrcInfo(seq_src_info);
	if (pArithm->typeID() == rdoRuntime::RDOType::t_enum)
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
	if (m_pHeader->getTypeInfo()->type()->typeID() == rdoRuntime::RDOType::t_enum)
	{
		RDOParser::s_parser()->error().error(src_info(), rdo::format(_T("Последовательность '%s' может возвращять значения только целого или вещественного типа"), src_text().c_str()));
	}
}

void RDOFUNSequenceNormal::createCalcs()
{
	PTR(rdoRuntime::RandGeneratorNormal) pGenerator = new rdoRuntime::RandGeneratorNormal();
	m_pInitCalc = rdo::Factory<rdoRuntime::RDOCalcSeqInit>::create(m_seed, pGenerator);
	RDOParser::s_parser()->runtime()->addInitCalc(m_pInitCalc);
	m_pNextCalc = rdo::Factory<rdoRuntime::RDOCalcSeqNextNormal>::create(pGenerator);
	initResult();
}

LPRDOFUNArithm RDOFUNSequenceNormal::createCallCalc(REF(LPRDOFUNParams) pParamList, CREF(RDOParserSrcInfo) seq_src_info) const
{
	if (pParamList->getParamList()->getContainer().size() != 2)
	{
		RDOParser::s_parser()->error().error(seq_src_info, rdo::format(_T("Для нормального закона распределения '%s' нужно указать два параметра: математическое ожидание и среднее квадратическое отклонение"), name().c_str()));
	}

	rdoRuntime::LPRDOCalcFunctionCaller pFuctionCall = rdo::Factory<rdoRuntime::RDOCalcFunctionCaller>::create(m_pNextCalc);
	ASSERT(pFuctionCall);

	LPTypeInfo            pType = rdo::Factory<TypeInfo>::create(rdo::Factory<RDOType__real>::create(), RDOParserSrcInfo());
	rdoRuntime::LPRDOCalc pArg1 = pParamList->getCalc(0, pType);
	rdoRuntime::LPRDOCalc pArg2 = pParamList->getCalc(1, pType);

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
	pArithm->setSrcInfo(seq_src_info);
	if (pArithm->typeID() == rdoRuntime::RDOType::t_enum)
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
	if (m_pHeader->getTypeInfo()->type()->typeID() != rdoRuntime::RDOType::t_int && m_pHeader->getTypeInfo()->type()->typeID() != rdoRuntime::RDOType::t_real)
	{
		RDOParser::s_parser()->error().error(src_info(), rdo::format(_T("Последовательность '%s' может возвращять значения только целого или вещественного типа"), src_text().c_str()));
	}
}

void RDOFUNSequenceTriangular::createCalcs()
{
	PTR(rdoRuntime::RandGeneratorTriangular) pGenerator = new rdoRuntime::RandGeneratorTriangular();
	m_pInitCalc = rdo::Factory<rdoRuntime::RDOCalcSeqInit>::create(m_seed, pGenerator);
	RDOParser::s_parser()->runtime()->addInitCalc(m_pInitCalc);
	m_pNextCalc = rdo::Factory<rdoRuntime::RDOCalcSeqNextTriangular>::create(pGenerator);
	initResult();
}

LPRDOFUNArithm RDOFUNSequenceTriangular::createCallCalc(REF(LPRDOFUNParams) pParamList, CREF(RDOParserSrcInfo) seq_src_info) const
{
	if (pParamList->getParamList()->getContainer().size() != 3)
	{
		RDOParser::s_parser()->error().error(seq_src_info, rdo::format(_T("Для треугольного закона распределения '%s' нужно указать три параметра: левую границу, точку под высотой треугольника, правую границу"), name().c_str()));
	}

	rdoRuntime::LPRDOCalcFunctionCaller pFuctionCall = rdo::Factory<rdoRuntime::RDOCalcFunctionCaller>::create(m_pNextCalc);
	ASSERT(pFuctionCall);

	LPTypeInfo            pType = rdo::Factory<TypeInfo>::create(rdo::Factory<RDOType__real>::create(), RDOParserSrcInfo());
	rdoRuntime::LPRDOCalc pArg1 = pParamList->getCalc(0, pType);
	rdoRuntime::LPRDOCalc pArg2 = pParamList->getCalc(1, pType);
	rdoRuntime::LPRDOCalc pArg3 = pParamList->getCalc(2, pType);

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
	pArithm->setSrcInfo(seq_src_info);
	if (pArithm->typeID() == rdoRuntime::RDOType::t_enum)
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

LPRDOFUNArithm RDOFUNSequenceByHist::createCallCalc(REF(LPRDOFUNParams) pParamList, CREF(RDOParserSrcInfo) src_info) const
{
	if (pParamList->getParamList()->getContainer().size() != 0)
	{
		RDOParser::s_parser()->error().error(src_info, rdo::format(_T("Гистограмма '%s' должна вызываться без параметров"), name().c_str()));
	}

	rdoRuntime::LPRDOCalcFunctionCaller pFuctionCall = rdo::Factory<rdoRuntime::RDOCalcFunctionCaller>::create(m_pNextCalc);
	ASSERT(pFuctionCall);

	LPExpression pExpression = rdo::Factory<Expression>::create(
		m_pHeader->getTypeInfo(),
		pFuctionCall,
		pParamList->src_info()
	);
	ASSERT(pExpression);
	LPRDOFUNArithm pArithm = rdo::Factory<RDOFUNArithm>::create(pExpression);
	ASSERT(pArithm);
	pArithm->setSrcInfo(src_info);
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
	PTR(rdoRuntime::RandGeneratorByHistReal) pGenerator = new rdoRuntime::RandGeneratorByHistReal();
	int size = m_from.size();
	for (int i = 0; i < size; i++)
	{
		pGenerator->addValues(m_from[i].getDouble(), m_to[i].getDouble(), m_freq[i].getDouble());
	}

	m_pInitCalc = rdo::Factory<rdoRuntime::RDOCalcSeqInit>::create(m_seed, pGenerator);
	RDOParser::s_parser()->runtime()->addInitCalc(m_pInitCalc);
	m_pNextCalc = rdo::Factory<rdoRuntime::RDOCalcSeqNextByHist>::create(pGenerator);
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
	rdoRuntime::RDOValue enum_id = m_pHeader->getTypeInfo()->value_cast(pValue)->value();
	if (std::find(m_values.begin(), m_values.end(), enum_id) != m_values.end())
	{
		RDOParser::s_parser()->error().error(pValue->src_info(), rdo::format(_T("Перечислимое значение определено дважды: %s"), pValue->value().getIdentificator().c_str()));
	}
	m_values.push_back(enum_id);
	m_freq  .push_back(pFreq->value());
}

void RDOFUNSequenceByHistEnum::createCalcs()
{
	PTR(rdoRuntime::RandGeneratorByHistEnum) pGenerator = new rdoRuntime::RandGeneratorByHistEnum();
	ruint size = m_values.size();
	for (ruint i = 0; i < size; i++)
	{
		pGenerator->addValues(m_values[i], m_freq[i].getDouble());
	}
	m_pInitCalc = rdo::Factory<rdoRuntime::RDOCalcSeqInit>::create(m_seed, pGenerator);
	RDOParser::s_parser()->runtime()->addInitCalc(m_pInitCalc);
	m_pNextCalc = rdo::Factory<rdoRuntime::RDOCalcSeqNextByHist>::create(pGenerator);
	initCalcSrcInfo();
}

// --------------------------------------------------------------------------------
// -------------------- Перечень значений
// --------------------------------------------------------------------------------
// -------------------- RDOFUNSequenceEnumerative
// --------------------------------------------------------------------------------
LPRDOFUNArithm RDOFUNSequenceEnumerative::createCallCalc(REF(LPRDOFUNParams) pParamList, CREF(RDOParserSrcInfo) src_info) const
{
	if (pParamList->getParamList()->getContainer().size() != 0)
	{
		RDOParser::s_parser()->error().error(src_info, rdo::format(_T("Перечисление '%s' должно вызываться без параметров"), name().c_str()));
	}

	rdoRuntime::LPRDOCalcFunctionCaller pFuctionCall = rdo::Factory<rdoRuntime::RDOCalcFunctionCaller>::create(m_pNextCalc);
	ASSERT(pFuctionCall);

	LPExpression pExpression = rdo::Factory<Expression>::create(
		m_pHeader->getTypeInfo(),
		pFuctionCall,
		pParamList->src_info()
	);
	ASSERT(pExpression);
	LPRDOFUNArithm pArithm = rdo::Factory<RDOFUNArithm>::create(pExpression);
	ASSERT(pArithm);
	pArithm->setSrcInfo(src_info);
	return pArithm;
}

// --------------------------------------------------------------------------------
// -------------------- RDOFUNSequenceEnumerative
// --------------------------------------------------------------------------------
void RDOFUNSequenceEnumerative::createCalcs()
{
	PTR(rdoRuntime::RandGeneratorEnumerative) pGenerator = new rdoRuntime::RandGeneratorEnumerative();
	for (ruint i = 0; i < m_valueList.size(); i++)
	{
		pGenerator->addValue(m_valueList[i]->value());
	}
	m_pNextCalc = rdo::Factory<rdoRuntime::RDOCalcSeqNextByHist>::create(pGenerator);
	initCalcSrcInfo();
}

// --------------------------------------------------------------------------------
// -------------------- Функции
// --------------------------------------------------------------------------------
// -------------------- RDOFUNFunctionListElement
// --------------------------------------------------------------------------------
RDOFUNFunctionListElement::RDOFUNFunctionListElement(CREF(RDOParserSrcInfo) src_info)
	: RDOParserSrcInfo(src_info)
{}

RDOFUNFunctionListElement::~RDOFUNFunctionListElement()
{}

rdoRuntime::LPRDOCalcIsEqual RDOFUNFunctionListElement::createIsEqualCalc(CREF(LPTypeInfo) pRetType, CREF(rdoRuntime::LPRDOCalcFuncParam) pFuncParam, CREF(RDOParserSrcInfo) src_pos) const
{
	rdoRuntime::LPRDOCalcConst pCalcConst = createResultCalc(pRetType, src_pos);
	return rdo::Factory<rdoRuntime::RDOCalcIsEqual>::create(pFuncParam, pCalcConst);
}

// --------------------------------------------------------------------------------
// -------------------- RDOFUNFunctionListElementIdentif
// --------------------------------------------------------------------------------
RDOFUNFunctionListElementIdentif::RDOFUNFunctionListElementIdentif(CREF(RDOParserSrcInfo) src_info)
	: RDOFUNFunctionListElement(src_info)
{}

rdoRuntime::LPRDOCalcConst RDOFUNFunctionListElementIdentif::createResultCalc(CREF(LPTypeInfo) pRetType, CREF(RDOParserSrcInfo) src_pos) const
{
	rdoRuntime::LPRDOCalcConst pCalcConst = rdo::Factory<rdoRuntime::RDOCalcConst>::create(
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

rdoRuntime::LPRDOCalcConst RDOFUNFunctionListElementReal::createResultCalc(CREF(LPTypeInfo) pRetType, CREF(RDOParserSrcInfo) src_pos) const
{
	LPTypeInfo pType = rdo::Factory<TypeInfo>::create(rdo::Factory<RDOType__real>::create(), src_info());
	ASSERT(pType);
	rdoRuntime::LPRDOCalcConst pCalcConst = rdo::Factory<rdoRuntime::RDOCalcConst>::create(
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

rdoRuntime::LPRDOCalcConst RDOFUNFunctionListElementInt::createResultCalc(CREF(LPTypeInfo) pRetType, CREF(RDOParserSrcInfo) src_pos) const
{
	LPTypeInfo pType = rdo::Factory<TypeInfo>::create(rdo::Factory<RDOType__int>::create(), src_info());
	ASSERT(pType);
	rdoRuntime::LPRDOCalcConst pCalcConst = rdo::Factory<rdoRuntime::RDOCalcConst>::create(
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

rdoRuntime::LPRDOCalcConst RDOFUNFunctionListElementEq::createResultCalc(CREF(LPTypeInfo) pRetType, CREF(RDOParserSrcInfo) src_pos) const
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
RDOFUNFunction::RDOFUNFunction(CREF(RDOParserSrcInfo) src_info, CREF(LPRDOParam) pReturn)
	: RDOParserSrcInfo(src_info)
	, m_pReturn       (pReturn )
{
	init();
}

RDOFUNFunction::RDOFUNFunction(CREF(tstring) name, CREF(LPRDOParam) pReturn)
	: RDOParserSrcInfo(name   )
	, m_pReturn       (pReturn)
{
	init();
}

void RDOFUNFunction::init()
{
	RDOParser::s_parser()->insertFUNFunction(this);
	RDOParser::s_parser()->contextStack()->push(this);

	m_pContextMemory = rdo::Factory<ContextMemory>::create();
	ASSERT(m_pContextMemory);
	RDOParser::s_parser()->contextStack()->push(m_pContextMemory);

	ContextMemory::push();
}

void RDOFUNFunction::end()
{
	ContextMemory::pop();
	RDOParser::s_parser()->contextStack()->pop();
	RDOParser::s_parser()->contextStack()->pop();
}

Context::FindResult RDOFUNFunction::onFindContext(CREF(LPRDOValue) pValue) const
{
	ASSERT(pValue);

	Context::FindResult result = m_pContextMemory->onFindContext(pValue);
	if (result.m_pContext)
	{
		return result;
	}

	//! Параметры
	LPRDOParam pParam = findFUNFunctionParam(pValue->value().getIdentificator());
	if (pParam)
	{
		rdoRuntime::RDOType::TypeID typeID = pParam->getTypeInfo()->type()->typeID();
		if (typeID == rdoRuntime::RDOType::t_identificator)
		{
			RDOParser::s_parser()->error().push_only(
				pValue->src_info()
				, rdo::format(_T("Тип параметра '%s' определён неверно")
					, pValue->src_info().src_text().c_str()
				)
			);
			RDOParser::s_parser()->error().push_only(pParam->getTypeInfo()->src_info(), _T("См. описание типа"));
			RDOParser::s_parser()->error().push_done();

		}LPExpression pExpression = rdo::Factory<Expression>::create(
			pParam->getTypeInfo(),
			rdo::Factory<rdoRuntime::RDOCalcFuncParam>::create(findFUNFunctionParamNum(pValue->value().getIdentificator()), pParam->src_info()),
			pValue->src_info()
		);
		ASSERT(pExpression);
		return Context::FindResult(const_cast<PTR(RDOFUNFunction)>(this), pExpression, pValue, pParam);
	}

	return Context::FindResult();
}

RDOFUNFunction::~RDOFUNFunction()
{}

void RDOFUNFunction::setFunctionCalc(CREF(rdoRuntime::LPRDOFunCalc) pCalc)
{
	ASSERT(pCalc);
	m_pFunctionCalc = pCalc;
	if (m_pFunctionCalc->srcInfo().src_empty())
	{
		m_pFunctionCalc->setSrcInfo(src_info());
	}
	STL_FOR_ALL(m_postLinkedList, it)
	{
		(*it)->setFunctionCalc(getFunctionCalc());
	}
}

LPRDOParam RDOFUNFunction::findFUNFunctionParam(CREF(tstring) paramName) const 
{
	ParamList::const_iterator it = std::find_if(m_paramList.begin(), m_paramList.end(), compareName<RDOParam>(paramName));
	return it != m_paramList.end() ? *it : LPRDOParam(NULL);
}

int RDOFUNFunction::findFUNFunctionParamNum(CREF(tstring) paramName) const
{
	ParamList::const_iterator it = std::find_if(m_paramList.begin(), m_paramList.end(), compareName<RDOParam>(paramName));
	return it != m_paramList.end() ? it - m_paramList.begin() : -1;
}

void RDOFUNFunction::add(CREF(LPRDOParam) pParam)
{ 
	LPRDOParam pParamPrev = findFUNFunctionParam(pParam->name());
	if (pParamPrev)
	{
		RDOParser::s_parser()->error().push_only(pParam->src_info(), rdo::format(_T("Параметр уже существует: %s"), pParam->name().c_str()));
		RDOParser::s_parser()->error().push_only(pParamPrev->src_info(), _T("См. первое определение"));
		RDOParser::s_parser()->error().push_done();
	}
	m_paramList.push_back(pParam); 
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
		RDOParser::s_parser()->error().error(m_pReturn->src_info(), rdo::format(_T("Функция '%s' должна иметь значение по-умолчанию"), name().c_str()));
	}
	if (m_paramList.empty())
	{
		RDOParser::s_parser()->error().error(src_info(), rdo::format(_T("Функция '%s' должна иметь параметры, т.к. её тип list"), name().c_str()));
	}
	if (m_elementList.empty())
	{
		RDOParser::s_parser()->error().warning(src_info(), rdo::format(_T("Функция '%s' не содержит списка"), name().c_str()));
	}

	rdoRuntime::LPRDOCalcConst pCalcDefault = rdo::Factory<rdoRuntime::RDOCalcConst>::create(m_pReturn->getDefault()->value());
	ASSERT(pCalcDefault);
	pCalcDefault->setSrcInfo(m_pReturn->getDefault()->src_info());
	rdoRuntime::LPRDOFunListCalc pFunListCalc = rdo::Factory<rdoRuntime::RDOFunListCalc>::create(pCalcDefault);
	ASSERT(pFunListCalc);
	pFunListCalc->setSrcInfo(src_info());
	ElementList::const_iterator elem_it = m_elementList.begin();
	while (elem_it != m_elementList.end())
	{
		rdoRuntime::LPRDOCalc pCaseCalc = rdo::Factory<rdoRuntime::RDOCalcConst>::create(true);
		ASSERT(pCaseCalc);
		pCaseCalc->setSrcInfo((*elem_it)->src_info());
		int currParamNumber = 0;
		ParamList::const_iterator param_it = m_paramList.begin();
		while (param_it != m_paramList.end())
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
				if ((param_it + 1) == m_paramList.end())
				{
					RDOParser::s_parser()->error().error(pListElement->src_info(), rdo::format(_T("Перед знаком равенства ожидается значение для параметра: %s"),pParam->name().c_str()));
				}
				else
				{
					tstring str = (*param_it)->src_text();
					++param_it;
					while (param_it != m_paramList.end())
					{
						str += _T(", ");
						str += (*param_it)->src_text();
						++param_it;
					}
					RDOParser::s_parser()->error().error(pListElement->src_info(), rdo::format(_T("Перед знаком равенства ожидаются значения для параметров: %s"), str.c_str()));
				}
			}
			rdoRuntime::LPRDOCalcFuncParam pFuncParam   = rdo::Factory<rdoRuntime::RDOCalcFuncParam>::create(currParamNumber, pParam->src_info());
			ASSERT(pFuncParam);
			rdoRuntime::LPRDOCalcIsEqual   pCompareCalc = pListElement->createIsEqualCalc(pParam->getTypeInfo(), pFuncParam, pListElement->src_info());
			ASSERT(pCompareCalc);
			rdoRuntime::LPRDOCalc          pAndCalc     = rdo::Factory<rdoRuntime::RDOCalcAnd>::create(pCaseCalc, pCompareCalc);
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
		rdoRuntime::LPRDOCalcConst pResultCalc = (*elem_it)->createResultCalc(m_pReturn->getTypeInfo(), (*elem_it)->src_info());
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
	int   param_cnt = m_paramList.size();
	ruint range     = 1;
	rdoRuntime::LPRDOCalc pCalc = rdo::Factory<rdoRuntime::RDOCalcConst>::create(0);
	ASSERT(pCalc);
	rdoRuntime::RDOSrcInfo srcInfo(src_info());
	srcInfo.setSrcText(_T("0"));
	pCalc->setSrcInfo(srcInfo);
	for (int currParam = 0; currParam < param_cnt; currParam++)
	{
		LPRDOParam pFunctionParam = m_paramList.at(currParam);
		ASSERT(pFunctionParam);
		rdoRuntime::LPRDOCalcFuncParam pFuncParam = rdo::Factory<rdoRuntime::RDOCalcFuncParam>::create(currParam, pFunctionParam->src_info());
		ASSERT(pFuncParam);
		rdoRuntime::LPRDOCalc pValue2 = pFuncParam;
		if (pFunctionParam->getTypeInfo()->type()->typeID() != rdoRuntime::RDOType::t_enum)
		{
			rdoRuntime::LPRDOCalcConst pCalcConst1 = rdo::Factory<rdoRuntime::RDOCalcConst>::create(1);
			pCalcConst1->setSrcInfo(pFunctionParam->src_info());
			pValue2 = rdo::Factory<rdoRuntime::RDOCalcMinus>::create(pValue2, pCalcConst1);
		}
		rdoRuntime::LPRDOCalcConst pCalcConst2 = rdo::Factory<rdoRuntime::RDOCalcConst>::create(range);
		ASSERT(pCalcConst2);
		pCalcConst2->setSrcInfo(pFunctionParam->src_info());
		rdoRuntime::LPRDOCalcMult pCalcMult = rdo::Factory<rdoRuntime::RDOCalcMultEnumSafe>::create(pCalcConst2, pValue2);
		rdoRuntime::LPRDOCalcPlus pCalcAdd  = rdo::Factory<rdoRuntime::RDOCalcPlusEnumSafe>::create(pCalcMult, pCalc);

		switch (pFunctionParam->getTypeInfo()->type()->typeID())
		{
		case rdoRuntime::RDOType::t_int:
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
		case rdoRuntime::RDOType::t_enum:
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

	rdoRuntime::LPRDOFuncTableCalc pFuncTableCalc = rdo::Factory<rdoRuntime::RDOFuncTableCalc>::create(pCalc);
	ASSERT(pFuncTableCalc);
	pFuncTableCalc->setSrcInfo(src_info());
	for (ruint currElem = 0; currElem < range; currElem++)
	{
		LPRDOFUNFunctionListElement pListElement = m_elementList.at(currElem);
		ASSERT(pListElement);
		rdoRuntime::LPRDOCalcConst  pResultCalc  = pListElement->createResultCalc(m_pReturn->getTypeInfo(), pListElement->src_info());
		ASSERT(pResultCalc);
		pFuncTableCalc->addResultCalc(pResultCalc);
	}
	setFunctionCalc(pFuncTableCalc);
}

void RDOFUNFunction::createAlgorithmicCalc(CREF(RDOParserSrcInfo) body_src_info)
{
	if (!m_returnFlag)
	{
		RDOParser::s_parser()->error().warning(body_src_info, rdo::format(_T("Не горантированно возвращение значания функции")));
		if (m_pReturn->getDefault()->defined())
		{
			LPRDOValue pDefault = m_pReturn->getTypeInfo()->value_cast(m_pReturn->getDefault());
			ASSERT(pDefault);
			rdoRuntime::LPRDOCalcConst pCalcDefault = rdo::Factory<rdoRuntime::RDOCalcConst>::create(pDefault->value());
			ASSERT(pCalcDefault);
			pCalcDefault->setSrcInfo(m_pReturn->getTypeInfo()->src_info());
			m_pFunctionCalc->addRetCalc(pCalcDefault);
		}
		else
		{
			//! Присвоить автоматическое значение по-умолчанию, если оно не задано в явном виде
			rdoRuntime::LPRDOCalcConst pCalcDefault = rdo::Factory<rdoRuntime::RDOCalcConst>::create(m_pReturn->getDefault()->value());
			ASSERT(pCalcDefault);
			pCalcDefault->setSrcInfo(m_pReturn->getTypeInfo()->src_info());
			m_pFunctionCalc->addRetCalc(pCalcDefault);
			RDOParser::s_parser()->error().warning(src_info(), rdo::format(_T("Для функции '%s' неопределено значение по-умолчанию"), name().c_str()));
		}
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
	RDOParser::s_parser()->contextStack()->pop();
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
			rdo::Factory<rdoRuntime::RDOCalcGetGroupFunctionResource>::create(),
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
	rdoRuntime::LPRDOFunCalcGroup pCalc;
	switch (m_funType)
	{
	case fgt_exist    : setSrcText(_T("Exist(")     + getResType()->name() + _T(": ") + pCondition->src_text() + _T(")")); pCalc = rdo::Factory<rdoRuntime::RDOFunCalcExist    >::create(getResType()->getNumber(), pCondition->getCalc()); break;
	case fgt_notexist : setSrcText(_T("NotExist(")  + getResType()->name() + _T(": ") + pCondition->src_text() + _T(")")); pCalc = rdo::Factory<rdoRuntime::RDOFunCalcNotExist >::create(getResType()->getNumber(), pCondition->getCalc()); break;
	case fgt_forall   : setSrcText(_T("ForAll(")    + getResType()->name() + _T(": ") + pCondition->src_text() + _T(")")); pCalc = rdo::Factory<rdoRuntime::RDOFunCalcForAll   >::create(getResType()->getNumber(), pCondition->getCalc()); break;
	case fgt_notforall: setSrcText(_T("NotForAll(") + getResType()->name() + _T(": ") + pCondition->src_text() + _T(")")); pCalc = rdo::Factory<rdoRuntime::RDOFunCalcNotForAll>::create(getResType()->getNumber(), pCondition->getCalc()); break;
	default: RDOParser::s_parser()->error().error(src_info(), _T("Внутренная ошибка: несуществующий тип функции"));
	}
	RDOParser::s_parser()->getFUNGroupStack().pop_back();
	end();

	LPExpression pExpression = rdo::Factory<Expression>::create(
		rdo::Factory<TypeInfo>::create(
			rdo::Factory<RDOType__bool>::create(),
			pCalc->srcInfo()
		),
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
	m_pCalcSelect = rdo::Factory<rdoRuntime::RDOFunCalcSelect>::create(getResType()->getNumber(), pCondition->getCalc());
	m_pCalcSelect->setSrcInfo(pCondition->src_info());
}

//! Групповая функция над выборкой Select'а
LPRDOFUNLogic RDOFUNSelect::createFunSelectGroup(RDOFUNGroupLogic::FunGroupType funType, REF(LPRDOFUNLogic) pCondition)
{
	ASSERT(pCondition);
	rdoRuntime::LPRDOFunCalcSelectBase pCalc;
	switch (funType)
	{
	case RDOFUNGroupLogic::fgt_exist    : setSrcText(src_text() + _T(".Exist(")     + pCondition->src_text() + _T(")")); pCalc = rdo::Factory<rdoRuntime::RDOFunCalcSelectExist    >::create(m_pCalcSelect, pCondition->getCalc()); break;
	case RDOFUNGroupLogic::fgt_notexist : setSrcText(src_text() + _T(".NotExist(")  + pCondition->src_text() + _T(")")); pCalc = rdo::Factory<rdoRuntime::RDOFunCalcSelectNotExist >::create(m_pCalcSelect, pCondition->getCalc()); break;
	case RDOFUNGroupLogic::fgt_forall   : setSrcText(src_text() + _T(".ForAll(")    + pCondition->src_text() + _T(")")); pCalc = rdo::Factory<rdoRuntime::RDOFunCalcSelectForAll   >::create(m_pCalcSelect, pCondition->getCalc()); break;
	case RDOFUNGroupLogic::fgt_notforall: setSrcText(src_text() + _T(".NotForAll(") + pCondition->src_text() + _T(")")); pCalc = rdo::Factory<rdoRuntime::RDOFunCalcSelectNotForAll>::create(m_pCalcSelect, pCondition->getCalc()); break;
	default: RDOParser::s_parser()->error().error(pCondition->src_info(), _T("Внутренная ошибка: неизвестный метод для списка ресурсов"));
	}
	RDOParser::s_parser()->getFUNGroupStack().pop_back();
	end();

	LPExpression pExpression = rdo::Factory<Expression>::create(
		rdo::Factory<TypeInfo>::create(
			rdo::Factory<RDOType__bool>::create(),
			pCalc->srcInfo()
		),
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

	rdoRuntime::LPRDOCalc pCalc = rdo::Factory<rdoRuntime::RDOFunCalcSelectEmpty>::create(m_pCalcSelect);
	ASSERT(pCalc);
	pCalc->setSrcInfo(src_info());

	LPExpression pExpression = rdo::Factory<Expression>::create(
		rdo::Factory<TypeInfo>::create(
			rdo::Factory<RDOType__bool>::create(),
			pCalc->srcInfo()
		),
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
		rdo::Factory<TypeInfo>::create(
			rdo::Factory<RDOType__int>::create(),
			size_info
		),
		rdo::Factory<rdoRuntime::RDOFunCalcSelectSize>::create(m_pCalcSelect),
		size_info
	);
	ASSERT(pExpression);

	LPRDOFUNArithm pArithm = rdo::Factory<RDOFUNArithm>::create(pExpression);
	ASSERT(pArithm);

	pArithm->setSrcInfo(size_info);
	return pArithm;
}

CLOSE_RDO_PARSER_NAMESPACE
