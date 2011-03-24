/*
 * copyright: (c) RDO-Team, 2010
 * filename : rdofun.cpp
 * author   : Александ Барс, Урусов Андрей
 * date     : 
 * bref     : 
 * indent   : 4T
 */

// ====================================================================== PCH
#include "rdo_lib/rdo_parser/pch.h"
// ====================================================================== INCLUDES
// ====================================================================== SYNOPSIS
#include "rdo_lib/rdo_parser/rdofun.h"
#include "rdo_lib/rdo_parser/rdoparser.h"
#include "rdo_lib/rdo_parser/rdortp.h"
#include "rdo_lib/rdo_parser/rdorss.h"
#include "rdo_lib/rdo_parser/rdopat.h"
#include "rdo_lib/rdo_parser/rdofrm.h"
#include "rdo_lib/rdo_parser/rdodpt.h"
#include "rdo_lib/rdo_parser/rdoparser_lexer.h"
#include "rdo_lib/rdo_parser/rdo_type_range.h"
#include "rdo_lib/rdo_runtime/rdo_random_distribution.h"
#include "rdo_lib/rdo_runtime/rdo_runtime.h"
#include "rdo_lib/rdo_runtime/rdoframe.h"
#include "rdo_lib/rdo_runtime/rdocalc.h"
// ===============================================================================

OPEN_RDO_PARSER_NAMESPACE

int funlex(PTR(YYSTYPE) lpval, PTR(YYLTYPE) llocp, PTR(void) lexer)
{
	LEXER->m_lpval = lpval;
	LEXER->m_lploc = llocp;
	return LEXER->yylex();
}

void funerror(PTR(char) mes)
{}

// ----------------------------------------------------------------------------
// ---------- RDOFUNDoubleToIntByResult
// ----------------------------------------------------------------------------
void RDOFUNDoubleToIntByResult::roundCalc()
{
	STL_FOR_ALL(m_intOrDouble, it)
	{
		(*it)->needRound();
	}
}

// ----------------------------------------------------------------------------
// ---------- Набор макросов для генерации логических и арифметических выражений
// ----------------------------------------------------------------------------
#define CREATE_CALC(CALC, OPR) \
rdoRuntime::LPRDOCalcConst pConstCalc1 = m_pCalc.object_dynamic_cast<rdoRuntime::RDOCalcConst>(); \
rdoRuntime::LPRDOCalcConst pConstCalc2 = pSecond->m_pCalc.object_dynamic_cast<rdoRuntime::RDOCalcConst>(); \
rdoRuntime::LPRDOCalc pNewCalc; \
if (pConstCalc1 && pConstCalc2) \
{ \
	pNewCalc = rdo::Factory<rdoRuntime::RDOCalcConst>::create(pConstCalc1->calcValue(RDOParser::s_parser()->runtime()) OPR pConstCalc2->calcValue(RDOParser::s_parser()->runtime())); \
	pNewCalc->setSrcInfo(rdoRuntime::RDOCalc##CALC::getStaticSrcInfo(pConstCalc1, pConstCalc2)); \
} \
else \
{ \
	pNewCalc = rdo::Factory<rdoRuntime::RDOCalc##CALC>::create(m_pCalc, pSecond->m_pCalc); \
}

#define RETURN_LOGIC() \
LPRDOFUNLogic pLogic = rdo::Factory<RDOFUNLogic>::create(pNewCalc, false); \
pLogic->setSrcInfo(pNewCalc->src_info()); \
pLogic->m_intOrDouble.insert(m_intOrDouble, pSecond->m_intOrDouble); \
return pLogic;

#define GENERATE_LOGIC(CALC, OPR) \
CREATE_CALC(CALC, OPR); \
RETURN_LOGIC();

#define CAST_ARITHM_VALUE(OPR, ERROR) \
try \
{ \
	if (beforeCastValue(pSecond) == CR_CONTINUE) \
	{ \
		value().value() OPR pSecond->value().value(); \
	} \
} \
catch (REF(rdoRuntime::RDOValueException)) \
{ \
	RDOParser::s_parser()->error().error(pSecond->src_info(), rdo::format(ERROR, type()->name().c_str(), pSecond->type()->name().c_str())); \
}

#define GET_ARITHM_PRE_TYPE() \
LPRDOType pNewType = getPreType(pSecond);

#define GENERATE_ARITHM_CALC(CALC, OPR, ERROR) \
CAST_ARITHM_VALUE(OPR, ERROR) \
GET_ARITHM_PRE_TYPE() \
CREATE_CALC(CALC, OPR)

#define RETURN_ARITHM() \
LPRDOFUNArithm pArithm = rdo::Factory<RDOFUNArithm>::create(RDOValue(pNewType, pNewCalc->src_info()), pNewCalc); \
pArithm->m_intOrDouble.insert(m_intOrDouble, pSecond->m_intOrDouble); \
return pArithm;

#define GENERATE_ARITHM(CALC, OPR, ERROR) \
GENERATE_ARITHM_CALC(CALC, OPR, ERROR) \
RETURN_ARITHM()

#define GENERATE_LOGIC_FROM_ARITHM(CALC, OPR, ERROR) \
GENERATE_ARITHM_CALC(CALC, OPR, ERROR) \
RETURN_LOGIC()

// ----------------------------------------------------------------------------
// ---------- RDOFUNLogic
// ----------------------------------------------------------------------------
RDOFUNLogic::RDOFUNLogic(CREF(LPRDOFUNArithm) pArithm)
	: RDOParserSrcInfo(pArithm->src_info())
{
	switch (pArithm->typeID())
	{
	case rdoRuntime::RDOType::t_bool: m_pCalc = pArithm->calc(); break;
	}
	if (!m_pCalc)
	{
		RDOParser::s_parser()->error().error(src_info(), rdo::format(_T("Ожидается логическое выражение, найдено: %s"), src_text().c_str()));
	}
}

RDOFUNLogic::RDOFUNLogic(CREF(rdoRuntime::LPRDOCalc) pCalc, rbool hideWarning)
	: RDOParserSrcInfo(     )
	, m_pCalc         (pCalc)
{
	if (m_pCalc)
	{
		m_pCalc->setSrcFileType(src_filetype());
	}
	if (!hideWarning)
	{
		rdoRuntime::LPRDOCalcConst pConstCalc = m_pCalc.object_dynamic_cast<rdoRuntime::RDOCalcConst>();
		if (pConstCalc)
		{
			if (pConstCalc->calcValue(RDOParser::s_parser()->runtime()).getAsBool())
			{
				RDOParser::s_parser()->error().warning(pConstCalc->src_info(), rdo::format(_T("Логическое выражение всегда истинно: %s"), pConstCalc->src_text().c_str()));
			}
			else
			{
				RDOParser::s_parser()->error().warning(pConstCalc->src_info(), rdo::format(_T("Логическое выражение всегда ложно: %s"), pConstCalc->src_text().c_str()));
			}
		}
	}
}

RDOFUNLogic::~RDOFUNLogic()
{}

rdoRuntime::LPRDOCalc RDOFUNLogic::getCalc(rdoRuntime::RDOType::TypeID id)
{
	if (id != rdoRuntime::RDOType::t_real)
	{
		m_intOrDouble.roundCalc();
	}
	return m_pCalc;
}

LPRDOFUNLogic RDOFUNLogic::operator&& (CREF(LPRDOFUNLogic) pSecond)
{
	GENERATE_LOGIC(And, &&);
}

LPRDOFUNLogic RDOFUNLogic::operator|| (CREF(LPRDOFUNLogic) pSecond)
{
	GENERATE_LOGIC(Or, ||);
}

LPRDOFUNLogic RDOFUNLogic::operator_not()
{
	rdoRuntime::LPRDOCalc pNewCalc = rdo::Factory<rdoRuntime::RDOCalcNot>::create(m_pCalc);
	LPRDOFUNLogic         pLogic   = rdo::Factory<RDOFUNLogic>::create(pNewCalc, false);
	pLogic->setSrcInfo(pNewCalc->src_info());
	pLogic->m_intOrDouble.insert(m_intOrDouble);
	return pLogic;
}

void RDOFUNLogic::setSrcInfo(CREF(RDOParserSrcInfo) src_info)
{
	RDOParserSrcInfo::setSrcInfo(src_info);
	if (m_pCalc)
	{
		m_pCalc->setSrcInfo(src_info);
	}
}

void RDOFUNLogic::setSrcPos(CREF(RDOSrcInfo::Position) position)
{
	RDOParserSrcInfo::setSrcPos(position);
	if (m_pCalc)
	{
		m_pCalc->setSrcPos(position);
	}
}

void RDOFUNLogic::setSrcText(CREF(tstring) value)
{
	RDOParserSrcInfo::setSrcText(value);
	if (m_pCalc)
	{
		m_pCalc->setSrcText(value);
	}
}

void RDOFUNLogic::setSrcPos(CREF(YYLTYPE) error_pos)
{
	RDOParserSrcInfo::setSrcPos(error_pos);
}

void RDOFUNLogic::setSrcPos(CREF(YYLTYPE) pos_begin, CREF(YYLTYPE) pos_end)
{
	RDOParserSrcInfo::setSrcPos(pos_begin, pos_end);
}

// ----------------------------------------------------------------------------
// ---------- RDOFUNArithm
// ----------------------------------------------------------------------------
RDOFUNArithm::RDOFUNArithm(CREF(LPExpression) pExpression)
	: m_pExpression(pExpression)
{
	ASSERT(m_pExpression);
	setSrcInfo(m_pExpression->src_info());
}

//RDOFUNArithm::RDOFUNArithm(CREF(RDOValue) value, CREF(rdoRuntime::LPRDOCalc) pCalc)
//	: m_value(value)
//	, m_pCalc(pCalc)
//{
//	setSrcInfo(m_value.src_info());
//}
//
//RDOFUNArithm::RDOFUNArithm(CREF(RDOValue) value)
//{
//	init(value);
//}

RDOFUNArithm::RDOFUNArithm(CREF(RDOValue) resName, CREF(RDOValue) parName)
{
	init(resName, parName);
}

RDOFUNArithm::~RDOFUNArithm()
{}

void RDOFUNArithm::init(CREF(RDOValue) value)
{
	setSrcInfo(value.src_info());

	if (value.constant())
	{
		m_pExpression = rdo::Factory<Expression>::create(
			value.type(),
			rdo::Factory<rdoRuntime::RDOCalcConst>::create(value.value()),
			value.src_info()
		);
		return;
	}

	if (value->getIdentificator() == _T("Time_now") || value->getIdentificator() == _T("time_now") || value->getIdentificator() == _T("Системное_время") || value->getIdentificator() == _T("системное_время"))
	{
		m_pExpression = rdo::Factory<Expression>::create(
			rdo::Factory<RDOType__real>::create(),
			rdo::Factory<rdoRuntime::RDOCalcGetTimeNow>::create(),
			value.src_info()
		);
		return;
	}
	else if (value->getIdentificator() == _T("Terminate_counter") || value->getIdentificator() == _T("terminate_counter"))
	{
		m_pExpression = rdo::Factory<Expression>::create(
			rdo::Factory<RDOType__int>::create(),
			rdo::Factory<rdoRuntime::RDOCalcGetTermNow>::create(),
			value.src_info()
		);
		return;
	}
	else if (value->getIdentificator() == _T("Seconds") || value->getIdentificator() == _T("seconds"))
	{
		m_pExpression = rdo::Factory<Expression>::create(
			rdo::Factory<RDOType__real>::create(),
			rdo::Factory<rdoRuntime::RDOCalcGetSeconds>::create(),
			value.src_info()
		);
		return;
	}

	//! Возможно, что это значение перечислимого типа, только одно и тоже значение может встречаться в разных
	//! перечислимых типах, поэтому какой именно из них выбрать - вопрос
	{ErrorBlockMonicker errorBlockMonicker;
		CREF(RDOParser::PreCastTypeList) typeList = RDOParser::s_parser()->getPreCastTypeList();
		STL_FOR_ALL_CONST(typeList, it)
		{
			RDOValue try_cast_value = (*it)->value_cast(value);
			if (try_cast_value.defined())
			{
				m_identificator = value;
				return;
			}
		}
	}

	//! Ищем параметр релевантного ресурса
	if (RDOParser::s_parser()->getFileToParse() == rdoModelObjects::PAT)
	{
		LPRDOPATPattern pPattern = RDOParser::s_parser()->getLastPATPattern();
		if (pPattern && pPattern->m_pCurrRelRes)
		{
			LPRDORTPParam pParam = pPattern->m_pCurrRelRes->getType()->findRTPParam(value->getIdentificator());
			if (pParam)
			{
				RDOValue paramName(pPattern->m_pCurrRelRes->m_bodySrcInfo);
				init(paramName, value);
				return;
			}
		}
	}

	//! Ищем параметры паттерна или функции по имени
	LPRDOParam pFunctionParam;
	switch (RDOParser::s_parser()->getFileToParse())
	{
	case rdoModelObjects::PAT:
	case rdoModelObjects::EVN: pFunctionParam = RDOParser::s_parser()->getLastPATPattern ()->findPATPatternParam (value->getIdentificator()); break;
	case rdoModelObjects::FUN: pFunctionParam = RDOParser::s_parser()->getLastFUNFunction()->findFUNFunctionParam(value->getIdentificator()); break;
	}

	//! Ищем константы по имени
	LPRDOFUNConstant pConstant = RDOParser::s_parser()->findFUNConstant(value->getIdentificator());

	if (pConstant && pFunctionParam)
	{
		RDOParser::s_parser()->error().error(src_info(), rdo::format(_T("Имя параметра образца совпадает с именем константы: %s"), value->getIdentificator().c_str()));
	}

	if (pConstant)
	{
		m_pExpression = rdo::Factory<Expression>::create(
			pConstant->getType()->type(),
			rdo::Factory<rdoRuntime::RDOCalcGetConst>::create(pConstant->getNumber()),
			value.src_info()
		);
		return;
	}

	//! Ищем последовательность по имени
	LPRDOFUNSequence pSequence = RDOParser::s_parser()->findFUNSequence(value->getIdentificator());
	if (pSequence && pFunctionParam)
	{
		RDOParser::s_parser()->error().error(src_info(), rdo::format(_T("Имя параметра образца совпадает с именем последовательности: %s"), value->getIdentificator().c_str()));
	}

	if (pSequence)
	{
		LPRDOFUNParams pParams = rdo::Factory<RDOFUNParams>::create();
		ASSERT(pParams);
		LPRDOFUNArithm pArithm = pParams->createSeqCall(value->getIdentificator());
		ASSERT(pArithm);
		pArithm->setSrcInfo(src_info());
		m_pExpression = rdo::Factory<Expression>::create(
			pArithm->type(),
			pArithm->calc(),
			value.src_info()
		);
		return;
	}

	if (pFunctionParam)
	{
		//! Это параметр
		rdoRuntime::LPRDOCalc pCalc;
		switch (RDOParser::s_parser()->getFileToParse())
		{
		case rdoModelObjects::PAT:
		case rdoModelObjects::EVN: pCalc = rdo::Factory<rdoRuntime::RDOCalcPatParam> ::create(RDOParser::s_parser()->getLastPATPattern ()->findPATPatternParamNum (value->getIdentificator())); break;
		case rdoModelObjects::FUN: pCalc = rdo::Factory<rdoRuntime::RDOCalcFuncParam>::create(RDOParser::s_parser()->getLastFUNFunction()->findFUNFunctionParamNum(value->getIdentificator()), pFunctionParam->src_info()); break;
		}
		ASSERT(pCalc);
		m_pExpression = rdo::Factory<Expression>::create(
			pFunctionParam->getType()->type(),
			pCalc,
			value.src_info()
		);
		return;
	}

	//Ищем локальную переменную
	LPContext pContext = RDOParser::s_parser()->context();
	ASSERT(pContext);
	LPContextMemory pContextMemory = pContext->cast<ContextMemory>();
	if (pContextMemory)
	{
		LPLocalVariableListStack pLocalVariableListStack = pContextMemory->getLocalMemory();
		ASSERT(pLocalVariableListStack);
		LPLocalVariable pLocalVariable = pLocalVariableListStack->findLocalVariable(value->getIdentificator());
		if (pLocalVariable)
		{
			m_pExpression = rdo::Factory<Expression>::create(
				pLocalVariable->getArithm()->type(),
				rdo::Factory<rdoRuntime::RDOCalcGetLocalVariable>::create(pLocalVariable->getValue()->getIdentificator()),
				value.src_info()
			);
			return;
		}
	}

	RDOParser::s_parser()->error().error(value.src_info(), rdo::format(_T("Неизвестный идентификатор: %s"), value->getIdentificator().c_str()));
}

void RDOFUNArithm::init(CREF(RDOValue) resName, CREF(RDOValue) parName)
{
	setSrcInfo(resName.src_info(), _T("."), parName.src_info());
	//! Это не ресурс, но возможно, ресурс внутри групповой функции
	if (!RDOParser::s_parser()->getFUNGroupStack().empty() && RDOParser::s_parser()->getFUNGroupStack().back()->getResType()->name() == resName->getIdentificator())
	{
		////! Это ресурс внутри групповой функции
		//LPRDOFUNGroup pFUNGroup = RDOParser::s_parser()->getFUNGroupStack().back();
		//ruint         parNumb   = pFUNGroup->getResType()->getRTPParamNumber(parName->getIdentificator());
		//if (parNumb == RDORTPResType::UNDEFINED_PARAM)
		//{
		//	RDOParser::s_parser()->error().error(parName.src_info(), rdo::format(_T("Неизвестный параметр ресурса: %s"), parName->getIdentificator().c_str()));
		//}
		//m_pCalc = rdo::Factory<rdoRuntime::RDOCalcGetGroupResParam>::create(parNumb);
		//m_pCalc->setSrcInfo(src_info());
		//m_value = pFUNGroup->getResType()->findRTPParam(parName->getIdentificator())->getType()->type();
		//return;
	}

	RDOParser::s_parser()->error().error(resName.src_info(), rdo::format(_T("Неизвестный ресурс: %s"), resName->getIdentificator().c_str()));
}

RDOFUNArithm::CastResult RDOFUNArithm::beforeCastValue(LPRDOFUNArithm pSecond)
{
	if (typeID() == rdoRuntime::RDOType::t_enum && pSecond->typeID() == rdoRuntime::RDOType::t_identificator)
	{
		//pSecond->m_value = RDOValue(
		//	type()->value_cast(
		//		pSecond->value(),
		//		pSecond->src_info(),
		//		src_info()
		//	).value(),
		//	enumType(),
		//	pSecond->m_value.src_info()
		//);
		//pSecond->m_pCalc = rdo::Factory<rdoRuntime::RDOCalcConst>::create(pSecond->m_value.value());
		//pSecond->m_pCalc->setSrcInfo(pSecond->src_info());
		return CR_DONE;
	}
	else if (typeID() == rdoRuntime::RDOType::t_identificator && pSecond->typeID() == rdoRuntime::RDOType::t_enum)
	{
		//m_value = RDOValue(
		//	pSecond->type()->value_cast(
		//		value(),
		//		src_info(),
		//		pSecond->src_info()
		//	).value(),
		//	pSecond->enumType(),
		//	m_value.src_info()
		//);
		//m_pCalc = rdo::Factory<rdoRuntime::RDOCalcConst>::create(m_value.value());
		//m_pCalc->setSrcInfo(src_info());
		return CR_DONE;
	}
	return CR_CONTINUE;
}

rdoRuntime::RDOValue RDOFUNArithm::const_value() const
{
	rdoRuntime::LPRDOCalcConst pCalc = calc().object_dynamic_cast<rdoRuntime::RDOCalcConst>();
	ASSERT(pCalc);
	return pCalc->getValue();
}

LPRDOType RDOFUNArithm::getPreType(CREF(LPRDOFUNArithm) pSecond)
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

	//! TODO: смущают два одинаковых src_info(), проверить и доказать правильность
	return type()->type_cast(pSecond->type(), pSecond->src_info(), src_info(), src_info());
}

LPRDOFUNArithm RDOFUNArithm::operator+ (CREF(LPRDOFUNArithm) pSecond)
{
	GENERATE_ARITHM(Plus, +, _T("Ну не могу я сложить %s и %s"));
}

LPRDOFUNArithm RDOFUNArithm::operator- (CREF(LPRDOFUNArithm) pSecond)
{
	GENERATE_ARITHM(Minus, -, _T("Нельзя из %s вычесть %s"));
}

LPRDOFUNArithm RDOFUNArithm::operator* (CREF(LPRDOFUNArithm) pSecond)
{
	GENERATE_ARITHM(Mult, *, _T("Нельзя %s умножить на %s"));
}

LPRDOFUNArithm RDOFUNArithm::operator/ (CREF(LPRDOFUNArithm) pSecond)
{
	GENERATE_ARITHM_CALC(Div, /, _T("Нельзя %s разделить на %s"));
	//! TODO: перевод вещественного в целое при делении. А что делать с умножением и т.д. ?
	//! Ответ: с умножением надо делать тоже самое, только непонятно как
	if (pNewType->type()->typeID() == rdoRuntime::RDOType::t_int)
	{
		rdoRuntime::LPRDOCalc pNewCalc_div = pNewCalc;
		pNewCalc = rdo::Factory<rdoRuntime::RDOCalcDoubleToIntByResult>::create(pNewCalc_div);
		pNewCalc->setSrcInfo(pNewCalc_div->src_info());
	}
	LPRDOFUNArithm pArithm = rdo::Factory<RDOFUNArithm>::create(RDOValue(pNewType, pNewCalc->src_info()), pNewCalc);
	if (pNewType->type()->typeID() == rdoRuntime::RDOType::t_int)
	{
		rdoRuntime::LPRDOCalcDoubleToIntByResult pResult = pNewCalc.object_static_cast<rdoRuntime::RDOCalcDoubleToIntByResult>();
		pArithm->m_intOrDouble.push_back(pResult);
	}
	pArithm->m_intOrDouble.insert(m_intOrDouble, pSecond->m_intOrDouble);
	return pArithm;
}

LPRDOFUNLogic RDOFUNArithm::operator< (CREF(LPRDOFUNArithm) pSecond)
{
	GENERATE_LOGIC_FROM_ARITHM(IsLess, <, _T("Нельзя сравнивать %s и %s"));
}

LPRDOFUNLogic RDOFUNArithm::operator> (CREF(LPRDOFUNArithm) pSecond)
{
	GENERATE_LOGIC_FROM_ARITHM(IsGreater, >, _T("Нельзя сравнивать %s и %s"));
}

LPRDOFUNLogic RDOFUNArithm::operator<= (CREF(LPRDOFUNArithm) pSecond)
{
	GENERATE_LOGIC_FROM_ARITHM(IsLEQ, <=, _T("Нельзя сравнивать %s и %s"));
}

LPRDOFUNLogic RDOFUNArithm::operator>= (CREF(LPRDOFUNArithm) pSecond)
{
	GENERATE_LOGIC_FROM_ARITHM(IsGEQ, >, _T("Нельзя сравнивать %s и %s"));
}

LPRDOFUNLogic RDOFUNArithm::operator== (CREF(LPRDOFUNArithm) pSecond)
{
	GENERATE_LOGIC_FROM_ARITHM(IsEqual, ==, _T("Нельзя сравнивать %s и %s"));
}

LPRDOFUNLogic RDOFUNArithm::operator!= (CREF(LPRDOFUNArithm) pSecond)
{
	GENERATE_LOGIC_FROM_ARITHM(IsNotEqual, !=, _T("Нельзя сравнивать %s и %s"));
}

void RDOFUNArithm::checkParamType(CREF(LPRDOTypeParam) pType)
{
	pType->type()->type_cast(type(), src_info(), pType->src_info(), src_info());
	rdoRuntime::LPRDOCalcConst pConstCalc = calc().object_dynamic_cast<rdoRuntime::RDOCalcConst>();
	if (pConstCalc)
	{
		rdoRuntime::RDOValue value = pConstCalc->calcValue(RDOParser::s_parser()->runtime());
		pType->value_cast(RDOValue(value, type(), src_info()));
	}
}

rdoRuntime::LPRDOCalc RDOFUNArithm::createCalc(CREF(LPRDOTypeParam) pForType)
{
	if (typeID() != rdoRuntime::RDOType::t_identificator)
	{
		if (pForType == NULL)
		{
			return m_pCalc;
		}
		if (pForType->type()->typeID() != rdoRuntime::RDOType::t_int)
		{
			if (pForType->type()->typeID() == rdoRuntime::RDOType::t_enum)
			{
				m_intOrDouble.roundCalc();
			}
			return m_pCalc;
		}
		else
		{
			m_intOrDouble.roundCalc();
			rdoRuntime::LPRDOCalc pNewCalc = rdo::Factory<rdoRuntime::RDOCalcDoubleToInt>::create(m_pCalc);
			pNewCalc->setSrcInfo(src_info());
			return pNewCalc;
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

	rdoRuntime::LPRDOCalc pNewCalc = rdo::Factory<rdoRuntime::RDOCalcConst>::create(pForType->value_cast(RDOValue(src_info())).value());
	pNewCalc->setSrcInfo(src_info());
	return pNewCalc;
}

void RDOFUNArithm::setSrcInfo(CREF(RDOParserSrcInfo) src_info)
{
	RDOParserSrcInfo::setSrcInfo(src_info);
	if (m_pCalc)
	{
		m_pCalc->setSrcInfo(src_info);
	}
}

void RDOFUNArithm::setSrcPos(CREF(RDOSrcInfo::Position) position)
{
	RDOParserSrcInfo::setSrcPos(position);
	if (m_pCalc)
	{
		m_pCalc->setSrcPos(position);
	}
}

void RDOFUNArithm::setSrcText(CREF(tstring) value)
{
	RDOParserSrcInfo::setSrcText(value);
	if (m_pCalc)
	{
		m_pCalc->setSrcText(src_info().src_text());
	}
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

// ----------------------------------------------------------------------------
// ---------- RDOFUNConstant
// ----------------------------------------------------------------------------
RDOFUNConstant::RDOFUNConstant(CREF(RDOParserSrcInfo) src_info, CREF(LPRDOTypeParam) pType, CREF(RDOValue) default)
	: RDOParam(src_info, pType, default)
{
	if (!getDefault().defined())
	{
		ASSERT(pType);
		RDOParser::s_parser()->error().error(pType->src_info(), _T("Константа должна иметь значение"));
	}

	m_number = RDOParser::s_parser()->getFUNCONST_id();
	RDOParser::s_parser()->insertFUNConstant(this);
}

RDOFUNConstant::~RDOFUNConstant()
{}

// ----------------------------------------------------------------------------
// ---------- RDOFUNParams
// ----------------------------------------------------------------------------
// Параметры, с которыми функция вызывается
// ----------------------------------------------------------------------------
RDOFUNParams::RDOFUNParams()
	: RDOParserSrcInfo()
{}

RDOFUNParams::~RDOFUNParams()
{}

rdoRuntime::LPRDOCalc RDOFUNParams::getCalc(ruint paramID, CREF(LPRDOTypeParam) pType)
{
	ASSERT(paramID < m_paramList.size());
	rdoRuntime::LPRDOCalc pCalc = m_paramList[paramID]->createCalc(pType);
	ASSERT(pCalc);
	return pCalc;
}

void RDOFUNParams::addParameter(CREF(LPRDOFUNArithm) pParam)
{
	ASSERT(pParam);
	m_paramList.push_back(pParam);
}

LPRDOFUNArithm RDOFUNParams::createCall(CREF(tstring) funName)
{
	LPRDOFUNFunction pFunction = RDOParser::s_parser()->findFUNFunction(funName);
	if (!pFunction)
	{
		return createSeqCall(funName);
	}

	int nParams = pFunction->getParams().size();
	if (nParams != m_paramList.size())
	{
		RDOParser::s_parser()->error().error(src_info(), rdo::format(_T("Неверное количество параметров функции: %s"), funName.c_str()));
	}

	rdoRuntime::LPRDOCalcFunctionCall pFuncCall = rdo::Factory<rdoRuntime::RDOCalcFunctionCall>::create(pFunction->getFunctionCalc());
	pFunction->insertPostLinked(pFuncCall);
	pFuncCall->setSrcInfo(src_info());
	for (int i = 0; i < nParams; i++)
	{
		LPRDOTypeParam pFuncParam = pFunction->getParams()[i]->getType();
		LPRDOFUNArithm pArithm = m_paramList[i];
		ASSERT(pArithm);
		pArithm->checkParamType(pFuncParam);
		pFuncCall->addParameter(pFuncParam->type()->calc_cast(pArithm->createCalc(pFuncParam), pArithm->type()));
	}

	LPRDOFUNArithm pArithm = rdo::Factory<RDOFUNArithm>::create(RDOValue(pFunction->getReturn()->getType()->type(), src_pos()), pFuncCall);
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
	LPRDOFUNArithm pArithm = pSequence->createCallCalc(LPRDOFUNParams(this), src_info());
	ASSERT(pArithm);
	return pArithm;
}

// ----------------------------------------------------------------------------
// ---------- Последовательности
// ----------------------------------------------------------------------------
// ---------- RDOFUNSequence
// ----------------------------------------------------------------------------
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
	switch (m_pHeader->getType()->type()->typeID())
	{
		case rdoRuntime::RDOType::t_int:
		{
			LPRDOTypeIntRange pRange = m_pHeader->getType()->type().object_dynamic_cast<RDOTypeIntRange>();
			if (pRange)
			{
				m_pNextCalc->m_diap     = true;
				m_pNextCalc->m_diap_min = pRange->range()->getMin().value().getDouble();
				m_pNextCalc->m_diap_max = pRange->range()->getMax().value().getDouble();
			}
			break;
		}
		case rdoRuntime::RDOType::t_real:
		{
			m_pNextCalc->m_res_real = true;
			LPRDOTypeRealRange pRange = m_pHeader->getType()->type().object_dynamic_cast<RDOTypeRealRange>();
			if (pRange)
			{
				m_pNextCalc->m_diap     = true;
				m_pNextCalc->m_diap_min = pRange->range()->getMin().value().getDouble();
				m_pNextCalc->m_diap_max = pRange->range()->getMax().value().getDouble();
			}
			break;
		}
	}
}

void RDOFUNSequence::initCalcSrcInfo()
{
	if (m_pInitCalc)
	{
		m_pInitCalc->setSrcInfo(m_pHeader->src_info());
		m_pInitCalc->setSrcText(_T("Установка базы последовательности ") + m_pInitCalc->src_text());
	}
	m_pNextCalc->setSrcInfo(m_pHeader->src_info());
}

// ----------------------------------------------------------------------------
// ---------- Датчики случайных чисел
// ----------------------------------------------------------------------------
// ---------- RDOFUNSequenceUniform
// ----------------------------------------------------------------------------
RDOFUNSequenceUniform::RDOFUNSequenceUniform(CREF(LPRDOFUNSequenceHeader) pHeader, int seed)
	: RDOFUNSequence(pHeader, seed)
{
	if (m_pHeader->getType()->type()->typeID() == rdoRuntime::RDOType::t_enum)
	{
		RDOParser::s_parser()->error().error(src_info(), _T("Последовательность типа uniform не может возвращять значения перечислимого типа"));
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
	if (pParamList->getParamList().size() != 2)
	{
		RDOParser::s_parser()->error().error(seq_src_info, rdo::format(_T("Для равномерного закона распределения '%s' нужно указать два параметра: минимальную и максимальную границы диапазона"), name().c_str()));
	}

	rdoRuntime::LPRDOCalcFunctionCall pFuctionCall = rdo::Factory<rdoRuntime::RDOCalcFunctionCall>::create(m_pNextCalc);
	ASSERT(pFuctionCall);

	LPRDOTypeParam        pType = rdo::Factory<RDOTypeParam>::create(rdo::Factory<RDOType__real>::create(), RDOParserSrcInfo());
	rdoRuntime::LPRDOCalc pArg1 = pParamList->getCalc(0, pType);
	rdoRuntime::LPRDOCalc pArg2 = pParamList->getCalc(1, pType);

	pFuctionCall->addParameter(pArg1);
	pFuctionCall->addParameter(pArg2);

	LPRDOFUNArithm pArithm = rdo::Factory<RDOFUNArithm>::create(RDOValue(m_pHeader->getType()->type(), pParamList->src_pos()), pFuctionCall);
	ASSERT(pArithm);
	pArithm->setSrcInfo(seq_src_info);
	if (pArithm->typeID() == rdoRuntime::RDOType::t_enum)
	{
		RDOParser::s_parser()->error().error(src_info(), _T("Внутренняя ошибка парсера"));
	}
	return pArithm;
}

// ----------------------------------------------------------------------------
// ---------- RDOFUNSequenceExponential
// ----------------------------------------------------------------------------
RDOFUNSequenceExponential::RDOFUNSequenceExponential(CREF(LPRDOFUNSequenceHeader) pHeader, int seed)
	: RDOFUNSequence(pHeader, seed)
{
	if (m_pHeader->getType()->type()->typeID() != rdoRuntime::RDOType::t_int && m_pHeader->getType()->type()->typeID() != rdoRuntime::RDOType::t_real)
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
	if (pParamList->getParamList().size() != 1)
	{
		RDOParser::s_parser()->error().error(seq_src_info, rdo::format(_T("Для экспоненциального закона распределения '%s' нужно указать один параметр: математическое ожидание"), name().c_str()));
	}

	rdoRuntime::LPRDOCalcFunctionCall pFuctionCall = rdo::Factory<rdoRuntime::RDOCalcFunctionCall>::create(m_pNextCalc);
	ASSERT(pFuctionCall);

	LPRDOTypeParam        pType = rdo::Factory<RDOTypeParam>::create(rdo::Factory<RDOType__real>::create(), RDOParserSrcInfo());
	rdoRuntime::LPRDOCalc pArg1 = pParamList->getCalc(0, pType);

	pFuctionCall->addParameter(pArg1);

	LPRDOFUNArithm pArithm = rdo::Factory<RDOFUNArithm>::create(RDOValue(m_pHeader->getType()->type(), pParamList->src_pos()), pFuctionCall);
	ASSERT(pArithm);
	pArithm->setSrcInfo(seq_src_info);
	if (pArithm->typeID() == rdoRuntime::RDOType::t_enum)
	{
		RDOParser::s_parser()->error().error(src_info(), _T("Внутренняя ошибка парсера"));
	}
	return pArithm;
}

// ----------------------------------------------------------------------------
// ---------- RDOFUNSequenceNormal
// ----------------------------------------------------------------------------
RDOFUNSequenceNormal::RDOFUNSequenceNormal(CREF(LPRDOFUNSequenceHeader) pHeader, int seed)
	: RDOFUNSequence(pHeader, seed)
{
	if (m_pHeader->getType()->type()->typeID() == rdoRuntime::RDOType::t_enum)
	{
		RDOParser::s_parser()->error().error(src_info(), _T("Последовательность типа normal не может возвращять значения перечислимого типа"));
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
	if (pParamList->getParamList().size() != 2)
	{
		RDOParser::s_parser()->error().error(seq_src_info, rdo::format(_T("Для нормального закона распределения '%s' нужно указать два параметра: математическое ожидание и среднее квадратическое отклонение"), name().c_str()));
	}

	rdoRuntime::LPRDOCalcFunctionCall pFuctionCall = rdo::Factory<rdoRuntime::RDOCalcFunctionCall>::create(m_pNextCalc);
	ASSERT(pFuctionCall);

	LPRDOTypeParam        pType = rdo::Factory<RDOTypeParam>::create(rdo::Factory<RDOType__real>::create(), RDOParserSrcInfo());
	rdoRuntime::LPRDOCalc pArg1 = pParamList->getCalc(0, pType);
	rdoRuntime::LPRDOCalc pArg2 = pParamList->getCalc(1, pType);

	pFuctionCall->addParameter(pArg1);
	pFuctionCall->addParameter(pArg2);

	LPRDOFUNArithm pArithm = rdo::Factory<RDOFUNArithm>::create(RDOValue(m_pHeader->getType()->type(), pParamList->src_pos()), pFuctionCall);
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
	if (pParamList->getParamList().size() != 0)
	{
		RDOParser::s_parser()->error().error(src_info, rdo::format(_T("Гистограмма '%s' должна вызываться без параметров"), name().c_str()));
	}

	rdoRuntime::LPRDOCalcFunctionCall pFuctionCall = rdo::Factory<rdoRuntime::RDOCalcFunctionCall>::create(m_pNextCalc);
	ASSERT(pFuctionCall);

	LPRDOFUNArithm pArithm = rdo::Factory<RDOFUNArithm>::create(RDOValue(m_pHeader->getType()->type(), pParamList->src_pos()), pFuctionCall);
	ASSERT(pArithm);
	pArithm->setSrcInfo(src_info);
	return pArithm;
}

// ----------------------------------------------------------------------------
// ---------- RDOFUNSequenceByHistReal
// ----------------------------------------------------------------------------
RDOFUNSequenceByHistReal::RDOFUNSequenceByHistReal(CREF(LPRDOFUNSequenceByHistHeader) pHeader, CREF(RDOValue) from, CREF(RDOValue) to, CREF(RDOValue) freq)
	: RDOFUNSequenceByHist(pHeader)
{
	addReal(from, to, freq);
}

void RDOFUNSequenceByHistReal::addReal(CREF(RDOValue) from, CREF(RDOValue) to, CREF(RDOValue) freq)
{
	if (!m_to.empty() && from.value() != m_to.back())
	{
		RDOParser::s_parser()->error().error(from, _T("Начало интервала должно совпадать с окончанием предыдущего"));
	}
	if (from.value() > to.value())
	{
		RDOParser::s_parser()->error().error(to, _T("Начало интервала должно быть меньше его конца"));
	}
	if (freq.value() == 0)
	{
		RDOParser::s_parser()->error().warning(freq, _T("Относительная вероятность должна быть больше нуля"));
	}
	else if (freq.value() < 0)
	{
		RDOParser::s_parser()->error().error(freq, _T("Относительная вероятность должна быть больше нуля"));
	}
	m_pHeader->getType()->value_cast(from);
	m_pHeader->getType()->value_cast(to  );
	m_from.push_back(from.value());
	m_to  .push_back(to  .value());
	m_freq.push_back(freq.value());
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

// ----------------------------------------------------------------------------
// ---------- RDOFUNSequenceByHistEnum
// ----------------------------------------------------------------------------
RDOFUNSequenceByHistEnum::RDOFUNSequenceByHistEnum(CREF(LPRDOFUNSequenceByHistHeader) pHeader, CREF(RDOValue) value, CREF(RDOValue) freq)
	: RDOFUNSequenceByHist(pHeader)
{
	addEnum(value, freq);
}

void RDOFUNSequenceByHistEnum::addEnum(CREF(RDOValue) value, CREF(RDOValue) freq)
{
	if (freq.value() == 0)
	{
		RDOParser::s_parser()->error().warning(freq, _T("Относительная вероятность должна быть больше нуля"));
	}
	else if (freq.value() < 0)
	{
		RDOParser::s_parser()->error().error(freq, _T("Относительная вероятность должна быть больше нуля"));
	}
	m_pHeader->getType()->value_cast(value);
	rdoRuntime::RDOValue enum_id = m_pHeader->getType()->value_cast(value).value();
	if (std::find(m_values.begin(), m_values.end(), enum_id) != m_values.end())
	{
		RDOParser::s_parser()->error().error(value, rdo::format(_T("Перечислимое значение определено дважды: %s"), value->getIdentificator().c_str()));
	}
	m_values.push_back(enum_id);
	m_freq  .push_back(freq.value());
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

// ----------------------------------------------------------------------------
// ---------- Перечень значений
// ----------------------------------------------------------------------------
// ---------- RDOFUNSequenceEnumerative
// ----------------------------------------------------------------------------
LPRDOFUNArithm RDOFUNSequenceEnumerative::createCallCalc(REF(LPRDOFUNParams) pParamList, CREF(RDOParserSrcInfo) src_info) const
{
	if (pParamList->getParamList().size() != 0)
	{
		RDOParser::s_parser()->error().error(src_info, rdo::format(_T("Перечисление '%s' должно вызываться без параметров"), name().c_str()));
	}

	rdoRuntime::LPRDOCalcFunctionCall pFuctionCall = rdo::Factory<rdoRuntime::RDOCalcFunctionCall>::create(m_pNextCalc);
	ASSERT(pFuctionCall);

	LPRDOFUNArithm pArithm = rdo::Factory<RDOFUNArithm>::create(RDOValue(m_pHeader->getType()->type(), pParamList->src_pos()), pFuctionCall);
	ASSERT(pArithm);
	pArithm->setSrcInfo(src_info);
	return pArithm;
}

// ----------------------------------------------------------------------------
// ---------- RDOFUNSequenceEnumerative
// ----------------------------------------------------------------------------
void RDOFUNSequenceEnumerative::createCalcs()
{
	PTR(rdoRuntime::RandGeneratorEnumerative) pGenerator = new rdoRuntime::RandGeneratorEnumerative();
	for (ruint i = 0; i < m_valueList.size(); i++)
	{
		pGenerator->addValue(m_valueList[i].value());
	}
	m_pNextCalc = rdo::Factory<rdoRuntime::RDOCalcSeqNextByHist>::create(pGenerator);
	initCalcSrcInfo();
}

// ----------------------------------------------------------------------------
// ---------- Функции
// ----------------------------------------------------------------------------
// ---------- RDOFUNFunctionListElement
// ----------------------------------------------------------------------------
RDOFUNFunctionListElement::RDOFUNFunctionListElement(CREF(RDOParserSrcInfo) src_info)
	: RDOParserSrcInfo(src_info)
{}

RDOFUNFunctionListElement::~RDOFUNFunctionListElement()
{}

rdoRuntime::LPRDOCalcIsEqual RDOFUNFunctionListElement::createIsEqualCalc(CREF(LPRDOTypeParam) pRetType, CREF(rdoRuntime::LPRDOCalcFuncParam) pFuncParam, CREF(RDOParserSrcInfo) src_pos) const
{
	rdoRuntime::LPRDOCalcConst pCalcConst = createResultCalc(pRetType, src_pos);
	return rdo::Factory<rdoRuntime::RDOCalcIsEqual>::create(pFuncParam, pCalcConst);
}

// ----------------------------------------------------------------------------
// ---------- RDOFUNFunctionListElementIdentif
// ----------------------------------------------------------------------------
RDOFUNFunctionListElementIdentif::RDOFUNFunctionListElementIdentif(CREF(RDOParserSrcInfo) src_info)
	: RDOFUNFunctionListElement(src_info)
{}

rdoRuntime::LPRDOCalcConst RDOFUNFunctionListElementIdentif::createResultCalc(CREF(LPRDOTypeParam) pRetType, CREF(RDOParserSrcInfo) src_pos) const
{
	rdoRuntime::LPRDOCalcConst pCalcConst = rdo::Factory<rdoRuntime::RDOCalcConst>::create(pRetType->value_cast(RDOValue(RDOParserSrcInfo(src_pos.getPosAsYY(), src_text()))).value());
	pCalcConst->setSrcInfo(src_pos);
	return pCalcConst;
}

// ----------------------------------------------------------------------------
// ---------- RDOFUNFunctionListElementReal
// ----------------------------------------------------------------------------
RDOFUNFunctionListElementReal::RDOFUNFunctionListElementReal(CREF(YYLTYPE) position, double value)
	: RDOFUNFunctionListElement(RDOParserSrcInfo(position, rdo::format(_T("%f"), value)))
	, m_value(value)
{}

rdoRuntime::LPRDOCalcConst RDOFUNFunctionListElementReal::createResultCalc(CREF(LPRDOTypeParam) pRetType, CREF(RDOParserSrcInfo) src_pos) const
{
	rdoRuntime::LPRDOCalcConst pCalcConst = rdo::Factory<rdoRuntime::RDOCalcConst>::create(pRetType->value_cast(RDOValue(m_value, rdo::Factory<RDOType__real>::create(), src_pos)).value());
	pCalcConst->setSrcInfo(src_pos);
	return pCalcConst;
}

// ----------------------------------------------------------------------------
// ---------- RDOFUNFunctionListElementInt
// ----------------------------------------------------------------------------
RDOFUNFunctionListElementInt::RDOFUNFunctionListElementInt(CREF(YYLTYPE) position, int value)
	: RDOFUNFunctionListElement(RDOParserSrcInfo(position, rdo::format(_T("%d"), value)))
	, m_value(value)
{}

rdoRuntime::LPRDOCalcConst RDOFUNFunctionListElementInt::createResultCalc(CREF(LPRDOTypeParam) pRetType, CREF(RDOParserSrcInfo) src_pos) const
{
	rdoRuntime::LPRDOCalcConst pCalcConst = rdo::Factory<rdoRuntime::RDOCalcConst>::create(pRetType->value_cast(RDOValue(m_value, rdo::Factory<RDOType__int>::create(), src_pos)).value());
	pCalcConst->setSrcInfo(src_pos);
	return pCalcConst;
}

// ----------------------------------------------------------------------------
// ---------- RDOFUNFunctionListElementEq
// ----------------------------------------------------------------------------
RDOFUNFunctionListElementEq::RDOFUNFunctionListElementEq(CREF(YYLTYPE) position)
	: RDOFUNFunctionListElement(RDOParserSrcInfo(position, _T("=")))
{}

rdoRuntime::LPRDOCalcConst RDOFUNFunctionListElementEq::createResultCalc(CREF(LPRDOTypeParam) pRetType, CREF(RDOParserSrcInfo) src_pos) const
{
	RDOParser::s_parser()->error().error(src_pos, _T("Внутренная ошибка парсера: RDOFUNFunctionListElementEq::createResultCalc"));
	NEVER_REACH_HERE;
	return NULL;
}

// ----------------------------------------------------------------------------
// ---------- RDOFUNCalculateIf
// ----------------------------------------------------------------------------
RDOFUNCalculateIf::RDOFUNCalculateIf(CREF(LPRDOFUNLogic) pCondition, CREF(LPRDOFUNArithm) pAction)
	: m_pCondition(pCondition)
	, m_pAction   (pAction   )
{
	m_pAction->checkParamType(RDOParser::s_parser()->getLastFUNFunction()->getReturn()->getType());
}

RDOFUNCalculateIf::~RDOFUNCalculateIf()
{}

// ----------------------------------------------------------------------------
// ---------- RDOFUNFunction
// ----------------------------------------------------------------------------
RDOFUNFunction::RDOFUNFunction(CREF(RDOParserSrcInfo) src_info, CREF(LPRDOParam) pReturn)
	: RDOParserSrcInfo(src_info)
	, m_pReturn       (pReturn )
{
	RDOParser::s_parser()->insertFUNFunction(this);
}

RDOFUNFunction::RDOFUNFunction(CREF(tstring) name, CREF(LPRDOParam) pReturn)
	: RDOParserSrcInfo(name   )
	, m_pReturn       (pReturn)
{
	RDOParser::s_parser()->insertFUNFunction(this);
}

RDOFUNFunction::~RDOFUNFunction()
{}

void RDOFUNFunction::setFunctionCalc(CREF(rdoRuntime::LPRDOFunCalc) pCalc)
{
	m_pFunctionCalc = pCalc;
	if (m_pFunctionCalc->src_empty())
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
	return it != m_paramList.end() ? *it : NULL;
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
	if (!m_pReturn->getDefault().defined())
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

	rdoRuntime::LPRDOCalcConst pCalcDefault = rdo::Factory<rdoRuntime::RDOCalcConst>::create(m_pReturn->getDefault().value());
	ASSERT(pCalcDefault);
	pCalcDefault->setSrcInfo(m_pReturn->getDefault().src_info());
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
			rdoRuntime::LPRDOCalcIsEqual   pCompareCalc = pListElement->createIsEqualCalc(pParam->getType(), pFuncParam, pListElement->src_info());
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
		rdoRuntime::LPRDOCalcConst pResultCalc = (*elem_it)->createResultCalc(m_pReturn->getType(), (*elem_it)->src_info());
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
	int param_cnt = m_paramList.size();
	int range     = 1;
	rdoRuntime::LPRDOCalc pCalc = rdo::Factory<rdoRuntime::RDOCalcConst>::create(0);
	ASSERT(pCalc);
	pCalc->setSrcInfo(src_info());
	pCalc->setSrcText(_T("0"));
	for (int currParam = 0; currParam < param_cnt; currParam++)
	{
		LPRDOParam pFunctionParam = m_paramList.at(currParam);
		ASSERT(pFunctionParam);
		rdoRuntime::LPRDOCalcFuncParam pFuncParam = rdo::Factory<rdoRuntime::RDOCalcFuncParam>::create(currParam, pFunctionParam->src_info());
		ASSERT(pFuncParam);
		rdoRuntime::LPRDOCalc pValue2 = pFuncParam;
		if (pFunctionParam->getType()->type()->typeID() != rdoRuntime::RDOType::t_enum)
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

		rbool found = false;
		switch (pFunctionParam->getType()->type()->typeID())
		{
		case rdoRuntime::RDOType::t_int:
			if (dynamic_cast<PTR(RDOTypeIntRange)>(pFunctionParam->getType()->type().get()))
			{
				LPRDOTypeIntRange pRange = pFunctionParam->getType()->type().object_static_cast<RDOTypeIntRange>();
				if (pRange->range()->getMin().value().getInt() != 1)
				{
					RDOParser::s_parser()->error().error(pRange->range()->src_info(), rdo::format(_T("Минимальное значение диапазона должно быть 1, текущий диапазон [%d..%d]"), pRange->range()->getMin().value().getInt(), pRange->range()->getMax().value().getInt()));
				}
				range *= pRange->range()->getMax().value().getInt() - pRange->range()->getMin().value().getInt() + 1;
			}
			else
			{
				RDOParser::s_parser()->error().error(pFunctionParam->getType()->src_info(), _T("Для параметра табличной функции должен быть задан допустимый диапазон"));
			}
			break;
		case rdoRuntime::RDOType::t_enum:
			range *= pFunctionParam->getType()->type().object_static_cast<RDOEnumType>()->getEnums()->getValues().size();
			break;
		default:
			RDOParser::s_parser()->error().error(pFunctionParam->getType()->src_info(), _T("Параметр табличной функции может быть целого или перечислимого типа"));
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
	for (int currElem = 0; currElem < range; currElem++)
	{
		LPRDOFUNFunctionListElement pListElement = m_elementList.at(currElem);
		ASSERT(pListElement);
		rdoRuntime::LPRDOCalcConst  pResultCalc  = pListElement->createResultCalc(m_pReturn->getType(), pListElement->src_info());
		ASSERT(pResultCalc);
		pFuncTableCalc->addResultCalc(pResultCalc);
	}
	setFunctionCalc(pFuncTableCalc);
}

void RDOFUNFunction::createAlgorithmicCalc(CREF(RDOParserSrcInfo) /* body_src_info */)
{
	rdoRuntime::LPRDOFunAlgorithmicCalc pFunAlgorithmicCalc;
	//! Фильтр на функцию
	switch (m_pReturn->getType()->type()->typeID())
	{
		case rdoRuntime::RDOType::t_int:
		{
			LPRDOTypeIntRange pRange = m_pReturn->getType()->type().object_dynamic_cast<RDOTypeIntRange>();
			if (pRange)
			{
				pFunAlgorithmicCalc = rdo::Factory<rdoRuntime::RDOFunAlgorithmicDiapCalc>::create(pRange->range()->getMin().value(), pRange->range()->getMax().value());
				ASSERT(pFunAlgorithmicCalc);
			}
			break;
		}
		case rdoRuntime::RDOType::t_real:
		{
			LPRDOTypeRealRange pRange = m_pReturn->getType()->type().object_dynamic_cast<RDOTypeRealRange>();
			if (pRange)
			{
				pFunAlgorithmicCalc = rdo::Factory<rdoRuntime::RDOFunAlgorithmicDiapCalc>::create(pRange->range()->getMin().value(), pRange->range()->getMax().value());
				ASSERT(pFunAlgorithmicCalc);
			}
			break;
		}
	}
	if (!pFunAlgorithmicCalc)
	{
		pFunAlgorithmicCalc = rdo::Factory<rdoRuntime::RDOFunAlgorithmicCalc>::create();
		ASSERT(pFunAlgorithmicCalc);
	}
	pFunAlgorithmicCalc->setSrcInfo(src_info());
	rbool defaultFlag = false;
	rbool trueConst   = false;
	rdoRuntime::LPRDOCalcConst pCondition;
	int size = m_calculateIfList.size();
	int cnt  = 0;
	for (int i = 0; i < size; i++)
	{
		rdoRuntime::LPRDOCalc pLogicCalc = m_calculateIfList[i]->getCondition()->getCalc(m_pReturn->getType()->type()->typeID());
		ASSERT(pLogicCalc);
		rdoRuntime::LPRDOCalcConst pConditionLast = pLogicCalc.object_dynamic_cast<rdoRuntime::RDOCalcConst>();
		if (trueConst)
		{
			RDOParser::s_parser()->error().warning(m_calculateIfList[i]->getCondition()->src_info(), rdo::format(_T("Условие не используется: %s"), m_calculateIfList[i]->getCondition()->src_text().c_str()));
			RDOParser::s_parser()->error().warning(pCondition->src_info(), rdo::format(_T("Последнее рабочее условие функции: %s"), pCondition->src_text().c_str()));
		}
		else if (!pConditionLast || pConditionLast->calcValue(RDOParser::s_parser()->runtime()).getAsBool())
		{
			//! Игнорируем чистые false-условия предыдущей проверкой
			pFunAlgorithmicCalc->addCalcIf(pLogicCalc, m_calculateIfList[i]->getAction()->createCalc(m_pReturn->getType()));
			cnt++;
		}
		if (!defaultFlag && pConditionLast && pConditionLast->calcValue(RDOParser::s_parser()->runtime()).getAsBool())
		{
			trueConst   = true;
			defaultFlag = true;
			pCondition  = pConditionLast;
		}
	}
	if (!cnt)
	{
		RDOParser::s_parser()->error().warning(src_info(), rdo::format(_T("Отсутствует тело функции '%s'"), name().c_str()));
	}
	if (!trueConst)
	{
		if (m_pReturn->getDefault().defined())
		{
			rdoRuntime::LPRDOCalcConst pCalcCondition = rdo::Factory<rdoRuntime::RDOCalcConst>::create(1);
			rdoRuntime::LPRDOCalcConst pCalcAction    = rdo::Factory<rdoRuntime::RDOCalcConst>::create(m_pReturn->getType()->value_cast(m_pReturn->getDefault()).value());
			ASSERT(pCalcCondition);
			ASSERT(pCalcAction   );
			pCalcCondition->setSrcInfo(m_pReturn->getType()->src_info());
			pCalcAction->setSrcInfo(m_pReturn->getType()->src_info());
			pFunAlgorithmicCalc->addCalcIf(pCalcCondition, pCalcAction);
			defaultFlag = true;
		}
	}
	if (!defaultFlag)
	{
		//! Присвоить автоматическое значение по-умолчанию, если оно не задано в явном виде
		rdoRuntime::LPRDOCalcConst pCalcCondition = rdo::Factory<rdoRuntime::RDOCalcConst>::create(1);
		rdoRuntime::LPRDOCalcConst pCalcAction    = rdo::Factory<rdoRuntime::RDOCalcConst>::create(m_pReturn->getType()->type()->get_default().value());
		ASSERT(pCalcCondition);
		ASSERT(pCalcAction   );
		pCalcCondition->setSrcInfo(m_pReturn->getType()->src_info());
		pCalcAction->setSrcInfo(m_pReturn->getType()->src_info());
		pFunAlgorithmicCalc->addCalcIf(pCalcCondition, pCalcAction);
		RDOParser::s_parser()->error().warning(src_info(), rdo::format(_T("Для функции '%s' неопределено значение по-умолчанию"), name().c_str()));
	}
	setFunctionCalc(pFunAlgorithmicCalc);
}

// ----------------------------------------------------------------------------
// ---------- RDOFUNGroup
// ----------------------------------------------------------------------------
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
}

// ----------------------------------------------------------------------------
// ---------- RDOFUNGroupLogic
// ----------------------------------------------------------------------------
RDOFUNGroupLogic::RDOFUNGroupLogic(FunGroupType funType, CREF(RDOParserSrcInfo) res_info)
	: RDOFUNGroup(res_info)
	, m_funType  (funType )
{}

LPRDOFUNLogic RDOFUNGroupLogic::createFunLogic(REF(LPRDOFUNLogic) pCondition)
{
	rdoRuntime::LPRDOFunCalcGroup calc;
	switch (m_funType)
	{
	case fgt_exist    : setSrcText(_T("Exist(")     + getResType()->name() + _T(": ") + pCondition->src_text() + _T(")")); calc = rdo::Factory<rdoRuntime::RDOFunCalcExist    >::create(getResType()->getNumber(), pCondition->getCalc()); break;
	case fgt_notexist : setSrcText(_T("NotExist(")  + getResType()->name() + _T(": ") + pCondition->src_text() + _T(")")); calc = rdo::Factory<rdoRuntime::RDOFunCalcNotExist >::create(getResType()->getNumber(), pCondition->getCalc()); break;
	case fgt_forall   : setSrcText(_T("ForAll(")    + getResType()->name() + _T(": ") + pCondition->src_text() + _T(")")); calc = rdo::Factory<rdoRuntime::RDOFunCalcForAll   >::create(getResType()->getNumber(), pCondition->getCalc()); break;
	case fgt_notforall: setSrcText(_T("NotForAll(") + getResType()->name() + _T(": ") + pCondition->src_text() + _T(")")); calc = rdo::Factory<rdoRuntime::RDOFunCalcNotForAll>::create(getResType()->getNumber(), pCondition->getCalc()); break;
	default: RDOParser::s_parser()->error().error(src_info(), _T("Внутренная ошибка: несуществующий тип функции"));
	}
	RDOParser::s_parser()->getFUNGroupStack().pop_back();
	LPRDOFUNLogic pLogic = rdo::Factory<RDOFUNLogic>::create(calc, false);
	pLogic->setSrcInfo(src_info());
	return pLogic;
}

// ----------------------------------------------------------------------------
// ---------- RDOFUNSelect
// ----------------------------------------------------------------------------
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
	rdoRuntime::LPRDOFunCalcSelectBase calc;
	switch (funType)
	{
	case RDOFUNGroupLogic::fgt_exist    : setSrcText(src_text() + _T(".Exist(")     + pCondition->src_text() + _T(")")); calc = rdo::Factory<rdoRuntime::RDOFunCalcSelectExist    >::create(m_pCalcSelect, pCondition->getCalc()); break;
	case RDOFUNGroupLogic::fgt_notexist : setSrcText(src_text() + _T(".NotExist(")  + pCondition->src_text() + _T(")")); calc = rdo::Factory<rdoRuntime::RDOFunCalcSelectNotExist >::create(m_pCalcSelect, pCondition->getCalc()); break;
	case RDOFUNGroupLogic::fgt_forall   : setSrcText(src_text() + _T(".ForAll(")    + pCondition->src_text() + _T(")")); calc = rdo::Factory<rdoRuntime::RDOFunCalcSelectForAll   >::create(m_pCalcSelect, pCondition->getCalc()); break;
	case RDOFUNGroupLogic::fgt_notforall: setSrcText(src_text() + _T(".NotForAll(") + pCondition->src_text() + _T(")")); calc = rdo::Factory<rdoRuntime::RDOFunCalcSelectNotForAll>::create(m_pCalcSelect, pCondition->getCalc()); break;
	default: RDOParser::s_parser()->error().error(pCondition->src_info(), _T("Внутренная ошибка: неизвестный метод для списка ресурсов"));
	}
	RDOParser::s_parser()->getFUNGroupStack().pop_back();
	LPRDOFUNLogic pLogic = rdo::Factory<RDOFUNLogic>::create(calc, false);
	pLogic->setSrcInfo(src_info());
	return pLogic;
}

LPRDOFUNLogic RDOFUNSelect::createFunSelectEmpty(CREF(RDOParserSrcInfo) empty_info)
{
	setSrcText(src_text() + _T(".") + empty_info.src_text());
	RDOParser::s_parser()->getFUNGroupStack().pop_back();
	LPRDOFUNLogic pLogic = rdo::Factory<RDOFUNLogic>::create(rdo::Factory<rdoRuntime::RDOFunCalcSelectEmpty>::create(m_pCalcSelect), false);
	pLogic->setSrcInfo(empty_info);
	return pLogic;
}

LPRDOFUNArithm RDOFUNSelect::createFunSelectSize(CREF(RDOParserSrcInfo) size_info)
{
	setSrcText(src_text() + _T(".") + size_info.src_text());
	RDOParser::s_parser()->getFUNGroupStack().pop_back();
	LPRDOFUNArithm pArithm = rdo::Factory<RDOFUNArithm>::create(RDOValue(rdo::Factory<RDOType__int>::create(), size_info), rdo::Factory<rdoRuntime::RDOFunCalcSelectSize>::create(m_pCalcSelect));
	pArithm->setSrcInfo(size_info);
	return pArithm;
}

CLOSE_RDO_PARSER_NAMESPACE
