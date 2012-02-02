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
#include "converter/smr2rdox/pch.h"
// ----------------------------------------------------------------------- INCLUDES
// ----------------------------------------------------------------------- SYNOPSIS
#include "converter/smr2rdox/rdofun.h"
#include "converter/smr2rdox/rdoparser.h"
#include "converter/smr2rdox/rdortp.h"
#include "converter/smr2rdox/rdorss.h"
#include "converter/smr2rdox/rdopat.h"
#include "converter/smr2rdox/rdofrm.h"
#include "converter/smr2rdox/rdodpt.h"
#include "converter/smr2rdox/rdoparser_lexer.h"
#include "converter/smr2rdox/rdo_type_range.h"
#include "simulator/runtime/rdo_random_distribution.h"
#include "simulator/runtime/rdo_runtime.h"
#include "simulator/runtime/rdoframe.h"
#include "simulator/runtime/calc/calc_process.h"
#include "simulator/runtime/calc/calc_pattern.h"
#include "simulator/runtime/calc/resource/calc_resource.h"
#include "simulator/runtime/calc/function/calc_function_system.h"
#include "simulator/runtime/calc/operation/calc_arithm.h"
#include "simulator/runtime/calc/procedural/calc_nop.h"
// --------------------------------------------------------------------------------

OPEN_RDO_CONVERTER_NAMESPACE

int cnv_funlex(PTR(YYSTYPE) lpval, PTR(YYLTYPE) llocp, PTR(void) lexer)
{
	LEXER->m_lpval = lpval;
	LEXER->m_lploc = llocp;
	return LEXER->yylex();
}

void cnv_funerror(PTR(char) message)
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
// -------------------- Набор макросов для генерации логических и арифметических выражений
// --------------------------------------------------------------------------------
#define CREATE_CALC(CALC, OPR) \
rdoRuntime::LPRDOCalcConst pConstCalc1 = m_pCalc.object_dynamic_cast<rdoRuntime::RDOCalcConst>(); \
rdoRuntime::LPRDOCalcConst pConstCalc2 = pSecond->m_pCalc.object_dynamic_cast<rdoRuntime::RDOCalcConst>(); \
rdoRuntime::LPRDOCalc pNewCalc; \
if (pConstCalc1 && pConstCalc2) \
{ \
	pNewCalc = rdo::Factory<rdoRuntime::RDOCalcConst>::create(pConstCalc1->calcValue(Converter::s_converter()->runtime()) OPR pConstCalc2->calcValue(Converter::s_converter()->runtime())); \
	pNewCalc->setSrcInfo(rdoRuntime::RDOCalc##CALC::getStaticSrcInfo(pConstCalc1, pConstCalc2)); \
} \
else \
{ \
	pNewCalc = rdo::Factory<rdoRuntime::RDOCalc##CALC>::create(m_pCalc, pSecond->m_pCalc); \
}

#define RETURN_LOGIC() \
LPRDOFUNLogic pLogic = rdo::Factory<RDOFUNLogic>::create(pNewCalc, false); \
pLogic->setSrcInfo(pNewCalc->srcInfo()); \
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
	Converter::s_converter()->error().error(pSecond->src_info(), rdo::format(ERROR, type()->name().c_str(), pSecond->type()->name().c_str())); \
}

#define GET_ARITHM_PRE_TYPE() \
LPRDOType pNewType = getPreType(pSecond);

#define GENERATE_ARITHM_CALC(CALC, OPR, ERROR) \
CAST_ARITHM_VALUE(OPR, ERROR) \
GET_ARITHM_PRE_TYPE() \
CREATE_CALC(CALC, OPR)

#define RETURN_ARITHM() \
LPRDOFUNArithm pArithm = rdo::Factory<RDOFUNArithm>::create(RDOValue(pNewType, pNewCalc->srcInfo()), pNewCalc); \
pArithm->m_intOrDouble.insert(m_intOrDouble, pSecond->m_intOrDouble); \
return pArithm;

#define GENERATE_ARITHM(CALC, OPR, ERROR) \
GENERATE_ARITHM_CALC(CALC, OPR, ERROR) \
RETURN_ARITHM()

#define GENERATE_LOGIC_FROM_ARITHM(CALC, OPR, ERROR) \
GENERATE_ARITHM_CALC(CALC, OPR, ERROR) \
RETURN_LOGIC()

// --------------------------------------------------------------------------------
// -------------------- RDOFUNLogic
// --------------------------------------------------------------------------------
RDOFUNLogic::RDOFUNLogic(CREF(LPRDOFUNArithm) pArithm)
	: RDOParserSrcInfo(pArithm->src_info())
{
	switch (pArithm->typeID())
	{
	case rdoRuntime::RDOType::t_bool: m_pCalc = pArithm->calc(); break;
	}
	if (!m_pCalc)
	{
		Converter::s_converter()->error().error(src_info(), rdo::format(_T("Ожидается логическое выражение, найдено: %s"), src_text().c_str()));
	}
}

RDOFUNLogic::RDOFUNLogic(CREF(rdoRuntime::LPRDOCalc) pCalc, rbool hideWarning)
	: RDOParserSrcInfo(     )
	, m_pCalc         (pCalc)
{
	if (m_pCalc)
	{
		rdoRuntime::RDOSrcInfo srcInfo(m_pCalc->srcInfo());
		srcInfo.setSrcFileType(src_filetype());
		m_pCalc->setSrcInfo(srcInfo);
	}
	if (!hideWarning)
	{
		rdoRuntime::LPRDOCalcConst pConstCalc = m_pCalc.object_dynamic_cast<rdoRuntime::RDOCalcConst>();
		if (pConstCalc)
		{
			if (pConstCalc->calcValue(Converter::s_converter()->runtime()).getAsBool())
			{
				Converter::s_converter()->error().warning(pConstCalc->srcInfo(), rdo::format(_T("Логическое выражение всегда истинно: %s"), pConstCalc->srcInfo().src_text().c_str()));
			}
			else
			{
				Converter::s_converter()->error().warning(pConstCalc->srcInfo(), rdo::format(_T("Логическое выражение всегда ложно: %s"), pConstCalc->srcInfo().src_text().c_str()));
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
	rdoRuntime::LPRDOCalc pNewCalc = rdo::Factory<rdoRuntime::RDOCalcNot>::create(m_pCalc->srcInfo().src_pos(), m_pCalc);
	LPRDOFUNLogic         pLogic   = rdo::Factory<RDOFUNLogic>::create(pNewCalc, false);
	pLogic->setSrcInfo(pNewCalc->srcInfo());
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
		rdoRuntime::RDOSrcInfo srcInfo(m_pCalc->srcInfo());
		srcInfo.setSrcPos(position);
		m_pCalc->setSrcInfo(srcInfo);
	}
}

void RDOFUNLogic::setSrcText(CREF(tstring) value)
{
	RDOParserSrcInfo::setSrcText(value);
	if (m_pCalc)
	{
		rdoRuntime::RDOSrcInfo srcInfo(m_pCalc->srcInfo());
		srcInfo.setSrcText(value);
		m_pCalc->setSrcInfo(srcInfo);
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

// --------------------------------------------------------------------------------
// -------------------- RDOFUNArithm
// --------------------------------------------------------------------------------
RDOFUNArithm::RDOFUNArithm(CREF(RDOValue) value, CREF(rdoRuntime::LPRDOCalc) pCalc)
	: m_value(value)
	, m_pCalc(pCalc)
{
	setSrcInfo(m_value.src_info());
}

RDOFUNArithm::RDOFUNArithm(CREF(RDOValue) value)
{
	init(value);
}

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
		m_value = value;
		m_pCalc = rdo::Factory<rdoRuntime::RDOCalcConst>::create(m_value.value());
		m_pCalc->setSrcInfo(src_info());
		return;
	}

	if (value->getIdentificator() == _T("Time_now") || value->getIdentificator() == _T("time_now") || value->getIdentificator() == _T("Системное_время") || value->getIdentificator() == _T("системное_время"))
	{
		m_value = rdo::Factory<RDOType__real>::create().object_parent_cast<RDOType>();
		m_pCalc = rdo::Factory<rdoRuntime::RDOCalcGetTimeNow>::create();
		m_pCalc->setSrcInfo(src_info());
		return;
	}
	else if (value->getIdentificator() == _T("Terminate_counter") || value->getIdentificator() == _T("terminate_counter"))
	{
		m_value = rdo::Factory<RDOType__int>::create().object_parent_cast<RDOType>();
		m_pCalc = rdo::Factory<rdoRuntime::RDOCalcGetTermNow>::create();
		m_pCalc->setSrcInfo(src_info());
		return;
	}
	else if (value->getIdentificator() == _T("Seconds") || value->getIdentificator() == _T("seconds"))
	{
		m_value = rdo::Factory<RDOType__real>::create().object_parent_cast<RDOType>();
		m_pCalc = rdo::Factory<rdoRuntime::RDOCalcGetSeconds>::create();
		m_pCalc->setSrcInfo(src_info());
		return;
	}

	//! Возможно, что это значение перечислимого типа, только одно и тоже значение может встречаться в разных
	//! перечислимых типах, поэтому какой именно из них выбрать - вопрос
	{ErrorBlockMonicker errorBlockMonicker;
		CREF(Converter::PreCastTypeList) typeList = Converter::s_converter()->getPreCastTypeList();
		STL_FOR_ALL_CONST(typeList, it)
		{
			RDOValue try_cast_value = (*it)->value_cast(value);
			if (try_cast_value.defined())
			{
				m_value = value;
				return;
			}
		}
	}

	//! Ищем параметр релевантного ресурса
	if (Converter::s_converter()->getFileToParse() == rdoModelObjectsConvertor::PAT_IN)
	{
		LPRDOPATPattern pPattern = Converter::s_converter()->getLastPATPattern();
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
	switch (Converter::s_converter()->getFileToParse())
	{
	case rdoModelObjectsConvertor::PAT_IN: pFunctionParam = Converter::s_converter()->getLastPATPattern ()->findPATPatternParam (value->getIdentificator()); break;
	case rdoModelObjectsConvertor::FUN_IN: pFunctionParam = Converter::s_converter()->getLastFUNFunction()->findFUNFunctionParam(value->getIdentificator()); break;
	}

	//! Ищем константы по имени
	LPRDOFUNConstant pConstant = Converter::s_converter()->findFUNConstant(value->getIdentificator());

	if (pConstant && pFunctionParam)
	{
		Converter::s_converter()->error().error(src_info(), rdo::format(_T("Имя параметра образца совпадает с именем константы: %s"), value->getIdentificator().c_str()));
	}

	if (pConstant)
	{
		m_value = pConstant->getType()->type();
		m_pCalc = rdo::Factory<rdoRuntime::RDOCalcGetConst>::create(pConstant->getNumber());
		m_pCalc->setSrcInfo(src_info());
		return;
	}

	//! Ищем последовательность по имени
	LPRDOFUNSequence pSequence = Converter::s_converter()->findFUNSequence(value->getIdentificator());
	if (pSequence && pFunctionParam)
	{
		Converter::s_converter()->error().error(src_info(), rdo::format(_T("Имя параметра образца совпадает с именем последовательности: %s"), value->getIdentificator().c_str()));
	}

	if (pSequence)
	{
		LPRDOFUNParams pParams = rdo::Factory<RDOFUNParams>::create();
		ASSERT(pParams);
		LPRDOFUNArithm pArithm = pParams->createSeqCall(value->getIdentificator());
		ASSERT(pArithm);
		pArithm->setSrcInfo(src_info());
		m_value = pArithm->value();
		m_pCalc = pArithm->m_pCalc;
		return;
	}

	if (pFunctionParam)
	{
		//! Это параметр
		m_value = pFunctionParam->getType()->type();
		switch (Converter::s_converter()->getFileToParse())
		{
		case rdoModelObjectsConvertor::PAT_IN: m_pCalc = rdo::Factory<rdoRuntime::RDOCalcPatParam> ::create(Converter::s_converter()->getLastPATPattern ()->findPATPatternParamNum (value->getIdentificator())); break;
		case rdoModelObjectsConvertor::FUN_IN: m_pCalc = rdo::Factory<rdoRuntime::RDOCalcFuncParam>::create(Converter::s_converter()->getLastFUNFunction()->findFUNFunctionParamNum(value->getIdentificator()), pFunctionParam->src_info()); break;
		}
		if (m_pCalc)
		{
			m_pCalc->setSrcInfo(src_info());
		}
		return;
	}

	Converter::s_converter()->error().error(value.src_info(), rdo::format(_T("Неизвестный идентификатор: %s"), value->getIdentificator().c_str()));
}

void RDOFUNArithm::init(CREF(RDOValue) resName, CREF(RDOValue) parName)
{
	setSrcInfo(resName.src_info(), _T("."), parName.src_info());
	LPRDORSSResource pResource = Converter::s_converter()->findRSSResource(resName->getIdentificator()); 
	if (pResource)
	{
		//! Это ресурс с закладки RSS
		ruint parNumb = pResource->getType()->getRTPParamNumber(parName->getIdentificator());
		if (parNumb == RDORTPResType::UNDEFINED_PARAM)
		{
			Converter::s_converter()->error().error(parName.src_info(), rdo::format(_T("Неизвестный параметр ресурса: %s"), parName->getIdentificator().c_str()));
		}
		if (pResource->getType()->isPermanent())
		{
			m_pCalc = rdo::Factory<rdoRuntime::RDOCalcNop>::create();
		}
		else if (pResource->getType()->isTemporary() && Converter::s_converter()->getFileToParse() == rdoModelObjectsConvertor::FRM_IN)
		{
			m_pCalc = rdo::Factory<rdoRuntime::RDOCalcNop>::create();
		}
		else
		{
			Converter::s_converter()->error().error(resName.src_info(), rdo::format(_T("Нельзя использовать временный ресурс: %s"), resName->getIdentificator().c_str()));
		}
		m_pCalc->setSrcInfo(src_info());
		m_value = pResource->getType()->findRTPParam(parName->getIdentificator())->getType()->type();
		return;
	}
	//! Это не ресурс, но возможно, ресурс внутри групповой функции
	else if (!Converter::s_converter()->getFUNGroupStack().empty() && Converter::s_converter()->getFUNGroupStack().back()->getResType()->name() == resName->getIdentificator())
	{
		//! Это ресурс внутри групповой функции
		LPRDOFUNGroup pFUNGroup = Converter::s_converter()->getFUNGroupStack().back();
		ruint         parNumb   = pFUNGroup->getResType()->getRTPParamNumber(parName->getIdentificator());
		if (parNumb == RDORTPResType::UNDEFINED_PARAM)
		{
			Converter::s_converter()->error().error(parName.src_info(), rdo::format(_T("Неизвестный параметр ресурса: %s"), parName->getIdentificator().c_str()));
		}
		m_pCalc = rdo::Factory<rdoRuntime::RDOCalcNop>::create();
		m_pCalc->setSrcInfo(src_info());
		m_value = pFUNGroup->getResType()->findRTPParam(parName->getIdentificator())->getType()->type();
		return;
	}
	else
	{
		//! Возможно, это релевантный ресурс
		switch (Converter::s_converter()->getFileToParse())
		{
		case rdoModelObjectsConvertor::PAT_IN:
			if (Converter::s_converter()->getLastPATPattern() && Converter::s_converter()->getLastPATPattern()->findRelevantResource(resName->getIdentificator()))
			{
				//! Это релевантный ресурс где-то в паттерне (with_min-common-choice, $Time, $Body)
				LPRDOPATPattern       pPattern          = Converter::s_converter()->getLastPATPattern();
				LPRDORelevantResource pRelevantResource = pPattern->findRelevantResource(resName->getIdentificator());
				if (!pPattern->m_pCurrRelRes)
				{
					//! Внутри with_min-common-choice или $Time
					if (pRelevantResource->m_statusBegin == rdoRuntime::RDOResource::CS_NonExist || pRelevantResource->m_statusBegin == rdoRuntime::RDOResource::CS_Create)
					{
						Converter::s_converter()->error().error(resName.src_info(), rdo::format(_T("Релевантный ресурс не может быть использован, т.к. он еще не существует: %s"), pRelevantResource->name().c_str()));
					}
				}
				else
				{
					//! Внутри $Body
					//! Проверяем использование неинициализированного рел.ресурса (pRelevantResource) в Choice from другом рел.ресурсе (pPattern->m_pCurrRelRes)
					if (pPattern->m_pCurrRelRes->isChoiceFromState())
					{
						if (!pRelevantResource->m_alreadyHaveConverter && !pRelevantResource->isDirect())
						{
							Converter::s_converter()->error().error(resName.src_info(), rdo::format(_T("Релевантный ресурс не определен: %s. Его нельзя использовать в условиях выбора других ресурсов до его собственного Choice from"), pRelevantResource->name().c_str()));
						}
						if (pRelevantResource->m_statusBegin == rdoRuntime::RDOResource::CS_NonExist)
						{
							Converter::s_converter()->error().error(resName.src_info(), rdo::format(_T("Релевантный ресурс в начале операции не существует (NonExist): %s"), pRelevantResource->name().c_str()));
						}
						if (pRelevantResource->m_statusBegin == rdoRuntime::RDOResource::CS_Create)
						{
							Converter::s_converter()->error().error(resName.src_info(), rdo::format(_T("Сразу после создания (Create) релевантный ресурс '%s' можно использовать только в конверторах, но не в условии выбора"), pRelevantResource->name().c_str()));
						}
					}
					//! Проверяем использование временного рел.ресурса внутри конвертора другого рел.ресурса
					if (pRelevantResource->getType()->isTemporary())
					{
						//! В конверторе начала
						if (pPattern->m_pCurrRelRes->m_currentState == RDORelevantResource::convertBegin)
						{
							if (pRelevantResource->m_statusBegin == rdoRuntime::RDOResource::CS_Create && !pRelevantResource->m_alreadyHaveConverter)
							{
								Converter::s_converter()->error().error(resName.src_info(), rdo::format(_T("Релевантный ресурс нельзя использовать до его создания (Create): %s"), pRelevantResource->name().c_str()));
							}
							if (pRelevantResource->m_statusBegin == rdoRuntime::RDOResource::CS_Erase && pRelevantResource->m_alreadyHaveConverter)
							{
								Converter::s_converter()->error().error(resName.src_info(), rdo::format(_T("Релевантный ресурс нельзя использовать после удаления (Erase): %s"), pRelevantResource->name().c_str()));
							}
							if (pRelevantResource->m_statusBegin == rdoRuntime::RDOResource::CS_NonExist)
							{
								Converter::s_converter()->error().error(resName.src_info(), rdo::format(_T("Релевантный ресурс не существует в этом конверторе (NonExist): %s"), pRelevantResource->name().c_str()));
							}
						}
						//! В конверторе конца
						if (pPattern->m_pCurrRelRes->m_currentState == RDORelevantResource::convertEnd)
						{
							if (pRelevantResource->m_statusEnd == rdoRuntime::RDOResource::CS_Create && !pRelevantResource->m_alreadyHaveConverter)
							{
								Converter::s_converter()->error().error(resName.src_info(), rdo::format(_T("Релевантный ресурс нельзя использовать до его создания (Create): %s"), pRelevantResource->name().c_str()));
							}
							if (pRelevantResource->m_statusEnd == rdoRuntime::RDOResource::CS_Erase && pRelevantResource->m_alreadyHaveConverter)
							{
								Converter::s_converter()->error().error(resName.src_info(), rdo::format(_T("Релевантный ресурс нельзя использовать после удаления (Erase): %s"), pRelevantResource->name().c_str()));
							}
							if (pRelevantResource->m_statusEnd == rdoRuntime::RDOResource::CS_NonExist)
							{
								Converter::s_converter()->error().error(resName.src_info(), rdo::format(_T("Релевантный ресурс не существует в этом конверторе (NonExist): %s"), pRelevantResource->name().c_str()));
							}
						}
					}
					//! Проверяем использование еще не инициализированного (только для Create) параметра рел. ресурса в его же конверторе
					LPRDORTPParam pParam = pPattern->m_pCurrRelRes->getType()->findRTPParam(parName->getIdentificator());
					if (pParam && pPattern->m_pCurrRelRes->name() == resName->getIdentificator())
					{
						//! В конверторе начала
						if (pPattern->m_pCurrRelRes->m_currentState == RDORelevantResource::convertBegin && pPattern->m_pCurrRelRes->m_statusBegin == rdoRuntime::RDOResource::CS_Create)
						{
							if (!pPattern->m_pCurrRelRes->getParamSetList().find(parName->getIdentificator()))
							{
								if (!pParam->getDefault().defined())
								{
									Converter::s_converter()->error().error(parName.src_info(), rdo::format(_T("Параметр '%s' еще не определен, ему необходимо присвоить значение в текущем конверторе или указать значение по-умолчанию в типе ресурса"), parName->getIdentificator().c_str()));
								}
							}
						}
						//! В конверторе конца
						if (pPattern->m_pCurrRelRes->m_currentState == RDORelevantResource::convertEnd && pPattern->m_pCurrRelRes->m_statusEnd == rdoRuntime::RDOResource::CS_Create)
						{
							if (!pPattern->m_pCurrRelRes->getParamSetList().find(parName->getIdentificator()))
							{
								if (!pPattern->m_pCurrRelRes->getParamSetList().find(parName->getIdentificator()))
								{
									if (!pParam->getDefault().defined())
									{
										Converter::s_converter()->error().error(parName.src_info(), rdo::format(_T("Параметр '%s' еще не определен, ему необходимо присвоить значение в текущем конверторе или указать значение по-умолчанию в типе ресурса"), parName->getIdentificator().c_str()));
									}
								}
							}
						}
					}
				}
				ruint parNumb = pRelevantResource->getType()->getRTPParamNumber(parName->getIdentificator());
				if (parNumb == RDORTPResType::UNDEFINED_PARAM)
				{
					Converter::s_converter()->error().error(parName.src_info(), rdo::format(_T("Неизвестный параметр ресурса: %s"), parName->getIdentificator().c_str()));
				}
				m_pCalc = rdo::Factory<rdoRuntime::RDOCalcNop>::create();
				m_pCalc->setSrcInfo(src_info());
				m_value = pRelevantResource->getType()->findRTPParam(parName->getIdentificator())->getType()->type();
				return;
			}
			break;

		case rdoModelObjectsConvertor::DPT_IN:
			if (Converter::s_converter()->isCurrentDPTSearch() && Converter::s_converter()->getLastDPTSearch()->getLastActivity())
			{
				LPRDOPATPattern pPattern = Converter::s_converter()->getLastDPTSearch()->getLastActivity()->pattern();
				if (pPattern && pPattern->findRelevantResource(resName->getIdentificator())) {
					//! Это ресурс, который используется в DPT (condition, term_condition, evaluate_by, value before, value after)
					LPRDORelevantResource pRelevantResource = pPattern->findRelevantResource(resName->getIdentificator());
					ruint                 parNumb           = pRelevantResource->getType()->getRTPParamNumber(parName->getIdentificator());
					if (parNumb == RDORTPResType::UNDEFINED_PARAM)
					{
						Converter::s_converter()->error().error(parName.src_info(), rdo::format(_T("Неизвестный параметр ресурса: %s"), parName->getIdentificator().c_str()));
					}
					m_pCalc = rdo::Factory<rdoRuntime::RDOCalcNop>::create();
					m_pCalc->setSrcInfo(src_info());
					m_value = pRelevantResource->getType()->findRTPParam(parName->getIdentificator())->getType()->type();
					return;
				}
			}
			if (Converter::s_converter()->isCurrentDPTPrior() && Converter::s_converter()->getLastDPTPrior()->getLastActivity())
			{
				LPRDOPATPattern pPattern = Converter::s_converter()->getLastDPTPrior()->getLastActivity()->pattern();
				if (pPattern && pPattern->findRelevantResource(resName->getIdentificator())) {
					//! Это ресурс, который используется в выражении приоритета активности DPTPrior
					LPRDORelevantResource pRelevantResource = pPattern->findRelevantResource(resName->getIdentificator());
					ruint                 parNumb           = pRelevantResource->getType()->getRTPParamNumber(parName->getIdentificator());
					if (parNumb == RDORTPResType::UNDEFINED_PARAM)
					{
						Converter::s_converter()->error().error(parName.src_info(), rdo::format(_T("Неизвестный параметр ресурса: %s"), parName->getIdentificator().c_str()));
					}
					m_pCalc = rdo::Factory<rdoRuntime::RDOCalcNop>::create();
					m_pCalc->setSrcInfo(src_info());
					m_value = pRelevantResource->getType()->findRTPParam(parName->getIdentificator())->getType()->type();
					return;
				}
			}
			break;

		case rdoModelObjectsConvertor::FRM_IN:
			if (Converter::s_converter()->getLastFRMFrame() && Converter::s_converter()->getLastFRMFrame()->frame()->getLastShow() && Converter::s_converter()->getLastFRMFrame()->frame()->getLastShow()->isShowIf())
			{
				m_pCalc = rdo::Factory<rdoRuntime::RDOCalcGetUnknowResParam>::create(resName->getIdentificator(), parName->getIdentificator());
				m_pCalc->setSrcInfo(src_info());
				return;
			}
			break;
		}
	}
	Converter::s_converter()->error().error(resName.src_info(), rdo::format(_T("Неизвестный ресурс: %s"), resName->getIdentificator().c_str()));
}

RDOFUNArithm::CastResult RDOFUNArithm::beforeCastValue(LPRDOFUNArithm pSecond)
{
	if (typeID() == rdoRuntime::RDOType::t_enum && pSecond->typeID() == rdoRuntime::RDOType::t_identificator)
	{
		pSecond->m_value = RDOValue(
			type()->value_cast(
				pSecond->value(),
				pSecond->src_info(),
				src_info()
			).value(),
			enumType(),
			pSecond->m_value.src_info()
		);
		pSecond->m_pCalc = rdo::Factory<rdoRuntime::RDOCalcConst>::create(pSecond->m_value.value());
		pSecond->m_pCalc->setSrcInfo(pSecond->src_info());
		return CR_DONE;
	}
	else if (typeID() == rdoRuntime::RDOType::t_identificator && pSecond->typeID() == rdoRuntime::RDOType::t_enum)
	{
		m_value = RDOValue(
			pSecond->type()->value_cast(
				value(),
				src_info(),
				pSecond->src_info()
			).value(),
			pSecond->enumType(),
			m_value.src_info()
		);
		m_pCalc = rdo::Factory<rdoRuntime::RDOCalcConst>::create(m_value.value());
		m_pCalc->setSrcInfo(src_info());
		return CR_DONE;
	}
	return CR_CONTINUE;
}

LPRDOType RDOFUNArithm::getPreType(CREF(LPRDOFUNArithm) pSecond)
{
	if (typeID() == rdoRuntime::RDOType::t_unknow)
	{
		Converter::s_converter()->error().error(src_info(), rdo::format(_T("Внутренная ошибка парсера: неопределенное значение арифметического выражения: %s"), src_text().c_str()));
	}
	if (pSecond->typeID() == rdoRuntime::RDOType::t_unknow)
	{
		Converter::s_converter()->error().error(pSecond->src_info(), rdo::format(_T("Внутренная ошибка парсера: неопределенное значение арифметического выражения: %s"), pSecond->src_text().c_str()));
	}
	if (typeID() == rdoRuntime::RDOType::t_identificator)
	{
		Converter::s_converter()->error().error(src_info(), rdo::format(_T("Неизвестный идентификатор: %s"), value()->getIdentificator().c_str()));
	}
	if (pSecond->typeID() == rdoRuntime::RDOType::t_identificator)
	{
		Converter::s_converter()->error().error(pSecond->src_info(), rdo::format(_T("Неизвестный идентификатор: %s"), pSecond->value()->getIdentificator().c_str()));
	}

	/// @todo смущают два одинаковых src_info(), проверить и доказать правильность
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
	/// @todo перевод вещественного в целое при делении. А что делать с умножением и т.д. ?
	//! Ответ: с умножением надо делать тоже самое, только непонятно как
	if (pNewType->type()->typeID() == rdoRuntime::RDOType::t_int)
	{
		rdoRuntime::LPRDOCalc pNewCalcDiv = pNewCalc;
		pNewCalc = rdo::Factory<rdoRuntime::RDOCalcDoubleToIntByResult>::create(pNewCalcDiv);
		pNewCalc->setSrcInfo(pNewCalcDiv->srcInfo());
	}
	LPRDOFUNArithm pArithm = rdo::Factory<RDOFUNArithm>::create(RDOValue(pNewType, pNewCalc->srcInfo()), pNewCalc);
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
		rdoRuntime::RDOValue value = pConstCalc->calcValue(Converter::s_converter()->runtime());
		pType->value_cast(RDOValue(value, type(), src_info()));
	}
}

rdoRuntime::LPRDOCalc RDOFUNArithm::createCalc(CREF(LPRDOTypeParam) pForType)
{
	if (typeID() != rdoRuntime::RDOType::t_identificator)
	{
		if (!pForType)
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
			rdoRuntime::LPRDOCalc pNewCalc = rdo::Factory<rdoRuntime::RDOCalcDoubleToInt>::create(m_pCalc->srcInfo().src_pos(), m_pCalc);
			pNewCalc->setSrcInfo(src_info());
			return pNewCalc;
		}
	}
	else if (typeID() == rdoRuntime::RDOType::t_identificator && !pForType)
	{
		Converter::s_converter()->error().error(src_info(), rdo::format(_T("Неизвестный идентификатор: %s"), value()->getAsString().c_str()));
	}

	if (!pForType)
	{
		Converter::s_converter()->error().error(src_info(), _T("Неизвестный тип параметра"));
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
		rdoRuntime::RDOSrcInfo srcInfo(m_pCalc->srcInfo());
		srcInfo.setSrcPos(position);
		m_pCalc->setSrcInfo(srcInfo);
	}
}

void RDOFUNArithm::setSrcText(CREF(tstring) value)
{
	RDOParserSrcInfo::setSrcText(value);
	if (m_pCalc)
	{
		rdoRuntime::RDOSrcInfo srcInfo(m_pCalc->srcInfo());
		srcInfo.setSrcText(src_info().src_text());
		m_pCalc->setSrcInfo(srcInfo);
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

// --------------------------------------------------------------------------------
// -------------------- RDOFUNConstant
// --------------------------------------------------------------------------------
RDOFUNConstant::RDOFUNConstant(CREF(RDOParserSrcInfo) src_info, CREF(LPRDOTypeParam) pType, CREF(RDOValue) defaultValue)
	: RDOParam(src_info, pType, defaultValue)
{
	if (!getDefault().defined())
	{
		ASSERT(pType);
		Converter::s_converter()->error().error(pType->src_info(), _T("Константа должна иметь значение"));
	}

	m_number = Converter::s_converter()->getFUNCONST_id();
	Converter::s_converter()->insertFUNConstant(this);
}

RDOFUNConstant::~RDOFUNConstant()
{}

// --------------------------------------------------------------------------------
// -------------------- RDOFUNParams
// --------------------------------------------------------------------------------
// Параметры, с которыми функция вызывается
// --------------------------------------------------------------------------------
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
	LPRDOFUNFunction pFunction = Converter::s_converter()->findFUNFunction(funName);
	if (!pFunction)
	{
		return createSeqCall(funName);
	}

	ruint nParams = pFunction->getParams().size();
	if (nParams != m_paramList.size())
	{
		Converter::s_converter()->error().error(src_info(), rdo::format(_T("Неверное количество параметров функции: %s"), funName.c_str()));
	}

	rdoRuntime::LPRDOCalcFunctionCaller pFuncCall = rdo::Factory<rdoRuntime::RDOCalcFunctionCaller>::create(pFunction->getFunctionCalc());
	pFunction->insertPostLinked(pFuncCall);
	pFuncCall->setSrcInfo(src_info());
	for (ruint i = 0; i < nParams; i++)
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
	LPRDOFUNSequence pSequence = Converter::s_converter()->findFUNSequence(seqName);
	if (!pSequence)
	{
		Converter::s_converter()->error().error(m_funseqName, rdo::format(_T("Неопределенная функция или последовательность: %s"), seqName.c_str()));
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
	Converter::s_converter()->insertFUNSequence(this);
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
				m_pNextCalc->m_range = rdoRuntime::RDOCalcSeqNext::Range(pRange->range()->getMin().value().getDouble(), pRange->range()->getMax().value().getDouble());
			}
			break;
		}
		case rdoRuntime::RDOType::t_real:
		{
			m_pNextCalc->m_res_real = true;
			LPRDOTypeRealRange pRange = m_pHeader->getType()->type().object_dynamic_cast<RDOTypeRealRange>();
			if (pRange)
			{
				m_pNextCalc->m_range = rdoRuntime::RDOCalcSeqNext::Range(pRange->range()->getMin().value().getDouble(), pRange->range()->getMax().value().getDouble());
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
		srcInfo.setSrcText(_T("Установка базы последовательности ") + m_pHeader->src_info().src_text());
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
	if (m_pHeader->getType()->type()->typeID() == rdoRuntime::RDOType::t_enum)
	{
		Converter::s_converter()->error().error(src_info(), _T("Последовательность типа uniform не может возвращять значения перечислимого типа"));
	}
}

void RDOFUNSequenceUniform::createCalcs()
{
	PTR(rdoRuntime::RandGeneratorUniform) pGenerator = new rdoRuntime::RandGeneratorUniform();
	m_pInitCalc = rdo::Factory<rdoRuntime::RDOCalcSeqInit>::create(m_seed, pGenerator);
	Converter::s_converter()->runtime()->addInitCalc(m_pInitCalc);
	m_pNextCalc = rdo::Factory<rdoRuntime::RDOCalcSeqNextUniform>::create(pGenerator);
	initResult();
}

LPRDOFUNArithm RDOFUNSequenceUniform::createCallCalc(REF(LPRDOFUNParams) pParamList, CREF(RDOParserSrcInfo) seq_src_info) const
{
	if (pParamList->getParamList().size() != 2)
	{
		Converter::s_converter()->error().error(seq_src_info, rdo::format(_T("Для равномерного закона распределения '%s' нужно указать два параметра: минимальную и максимальную границы диапазона"), name().c_str()));
	}

	rdoRuntime::LPRDOCalcFunctionCaller pFuctionCall = rdo::Factory<rdoRuntime::RDOCalcFunctionCaller>::create(m_pNextCalc);
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
		Converter::s_converter()->error().error(src_info(), _T("Внутренняя ошибка парсера"));
	}
	return pArithm;
}

// --------------------------------------------------------------------------------
// -------------------- RDOFUNSequenceExponential
// --------------------------------------------------------------------------------
RDOFUNSequenceExponential::RDOFUNSequenceExponential(CREF(LPRDOFUNSequenceHeader) pHeader, int seed)
	: RDOFUNSequence(pHeader, seed)
{
	if (m_pHeader->getType()->type()->typeID() != rdoRuntime::RDOType::t_int && m_pHeader->getType()->type()->typeID() != rdoRuntime::RDOType::t_real)
	{
		Converter::s_converter()->error().error(src_info(), rdo::format(_T("Последовательность '%s' может возвращять значения только целого или вещественного типа"), src_text().c_str()));
	}
}

void RDOFUNSequenceExponential::createCalcs()
{
	PTR(rdoRuntime::RandGeneratorExponential) pGenerator = new rdoRuntime::RandGeneratorExponential();
	m_pInitCalc = rdo::Factory<rdoRuntime::RDOCalcSeqInit>::create(m_seed, pGenerator);
	Converter::s_converter()->runtime()->addInitCalc(m_pInitCalc);
	m_pNextCalc = rdo::Factory<rdoRuntime::RDOCalcSeqNextExponential>::create(pGenerator);
	initResult();
}

LPRDOFUNArithm RDOFUNSequenceExponential::createCallCalc(REF(LPRDOFUNParams) pParamList, CREF(RDOParserSrcInfo) seq_src_info) const
{
	if (pParamList->getParamList().size() != 1)
	{
		Converter::s_converter()->error().error(seq_src_info, rdo::format(_T("Для экспоненциального закона распределения '%s' нужно указать один параметр: математическое ожидание"), name().c_str()));
	}

	rdoRuntime::LPRDOCalcFunctionCaller pFuctionCall = rdo::Factory<rdoRuntime::RDOCalcFunctionCaller>::create(m_pNextCalc);
	ASSERT(pFuctionCall);

	LPRDOTypeParam        pType = rdo::Factory<RDOTypeParam>::create(rdo::Factory<RDOType__real>::create(), RDOParserSrcInfo());
	rdoRuntime::LPRDOCalc pArg1 = pParamList->getCalc(0, pType);

	pFuctionCall->addParameter(pArg1);

	LPRDOFUNArithm pArithm = rdo::Factory<RDOFUNArithm>::create(RDOValue(m_pHeader->getType()->type(), pParamList->src_pos()), pFuctionCall);
	ASSERT(pArithm);
	pArithm->setSrcInfo(seq_src_info);
	if (pArithm->typeID() == rdoRuntime::RDOType::t_enum)
	{
		Converter::s_converter()->error().error(src_info(), _T("Внутренняя ошибка парсера"));
	}
	return pArithm;
}

// --------------------------------------------------------------------------------
// -------------------- RDOFUNSequenceNormal
// --------------------------------------------------------------------------------
RDOFUNSequenceNormal::RDOFUNSequenceNormal(CREF(LPRDOFUNSequenceHeader) pHeader, int seed)
	: RDOFUNSequence(pHeader, seed)
{
	if (m_pHeader->getType()->type()->typeID() == rdoRuntime::RDOType::t_enum)
	{
		Converter::s_converter()->error().error(src_info(), _T("Последовательность типа normal не может возвращять значения перечислимого типа"));
	}
}

void RDOFUNSequenceNormal::createCalcs()
{
	PTR(rdoRuntime::RandGeneratorNormal) pGenerator = new rdoRuntime::RandGeneratorNormal();
	m_pInitCalc = rdo::Factory<rdoRuntime::RDOCalcSeqInit>::create(m_seed, pGenerator);
	Converter::s_converter()->runtime()->addInitCalc(m_pInitCalc);
	m_pNextCalc = rdo::Factory<rdoRuntime::RDOCalcSeqNextNormal>::create(pGenerator);
	initResult();
}

LPRDOFUNArithm RDOFUNSequenceNormal::createCallCalc(REF(LPRDOFUNParams) pParamList, CREF(RDOParserSrcInfo) seq_src_info) const
{
	if (pParamList->getParamList().size() != 2)
	{
		Converter::s_converter()->error().error(seq_src_info, rdo::format(_T("Для нормального закона распределения '%s' нужно указать два параметра: математическое ожидание и среднее квадратическое отклонение"), name().c_str()));
	}

	rdoRuntime::LPRDOCalcFunctionCaller pFuctionCall = rdo::Factory<rdoRuntime::RDOCalcFunctionCaller>::create(m_pNextCalc);
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
		Converter::s_converter()->error().error(src_info(), _T("Внутренняя ошибка парсера"));
	}
	return pArithm;
}

// --------------------------------------------------------------------------------
// -------------------- Гистограмма
// --------------------------------------------------------------------------------
// -------------------- RDOFUNSequenceByHist
// --------------------------------------------------------------------------------
RDOFUNSequenceByHist::RDOFUNSequenceByHist(CREF(LPRDOFUNSequenceByHistHeader) pHeader)
	: RDOFUNSequence(pHeader->getHeader(), pHeader->getSeed())
{}

LPRDOFUNArithm RDOFUNSequenceByHist::createCallCalc(REF(LPRDOFUNParams) pParamList, CREF(RDOParserSrcInfo) src_info) const
{
	if (pParamList->getParamList().size() != 0)
	{
		Converter::s_converter()->error().error(src_info, rdo::format(_T("Гистограмма '%s' должна вызываться без параметров"), name().c_str()));
	}

	rdoRuntime::LPRDOCalcFunctionCaller pFuctionCall = rdo::Factory<rdoRuntime::RDOCalcFunctionCaller>::create(m_pNextCalc);
	ASSERT(pFuctionCall);

	LPRDOFUNArithm pArithm = rdo::Factory<RDOFUNArithm>::create(RDOValue(m_pHeader->getType()->type(), pParamList->src_pos()), pFuctionCall);
	ASSERT(pArithm);
	pArithm->setSrcInfo(src_info);
	return pArithm;
}

// --------------------------------------------------------------------------------
// -------------------- RDOFUNSequenceByHistReal
// --------------------------------------------------------------------------------
RDOFUNSequenceByHistReal::RDOFUNSequenceByHistReal(CREF(LPRDOFUNSequenceByHistHeader) pHeader, CREF(RDOValue) from, CREF(RDOValue) to, CREF(RDOValue) freq)
	: RDOFUNSequenceByHist(pHeader)
{
	addReal(from, to, freq);
}

void RDOFUNSequenceByHistReal::addReal(CREF(RDOValue) from, CREF(RDOValue) to, CREF(RDOValue) freq)
{
	if (!m_to.empty() && from.value() != m_to.back())
	{
		Converter::s_converter()->error().error(from, _T("Начало интервала должно совпадать с окончанием предыдущего"));
	}
	if (from.value() > to.value())
	{
		Converter::s_converter()->error().error(to, _T("Начало интервала должно быть меньше его конца"));
	}
	if (freq.value() == 0)
	{
		Converter::s_converter()->error().warning(freq, _T("Относительная вероятность должна быть больше нуля"));
	}
	else if (freq.value() < 0)
	{
		Converter::s_converter()->error().error(freq, _T("Относительная вероятность должна быть больше нуля"));
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
	Converter::s_converter()->runtime()->addInitCalc(m_pInitCalc);
	m_pNextCalc = rdo::Factory<rdoRuntime::RDOCalcSeqNextByHist>::create(pGenerator);
	initResult();
}

// --------------------------------------------------------------------------------
// -------------------- RDOFUNSequenceByHistEnum
// --------------------------------------------------------------------------------
RDOFUNSequenceByHistEnum::RDOFUNSequenceByHistEnum(CREF(LPRDOFUNSequenceByHistHeader) pHeader, CREF(RDOValue) value, CREF(RDOValue) freq)
	: RDOFUNSequenceByHist(pHeader)
{
	addEnum(value, freq);
}

void RDOFUNSequenceByHistEnum::addEnum(CREF(RDOValue) value, CREF(RDOValue) freq)
{
	if (freq.value() == 0)
	{
		Converter::s_converter()->error().warning(freq, _T("Относительная вероятность должна быть больше нуля"));
	}
	else if (freq.value() < 0)
	{
		Converter::s_converter()->error().error(freq, _T("Относительная вероятность должна быть больше нуля"));
	}
	m_pHeader->getType()->value_cast(value);
	rdoRuntime::RDOValue enum_id = m_pHeader->getType()->value_cast(value).value();
	if (std::find(m_values.begin(), m_values.end(), enum_id) != m_values.end())
	{
		Converter::s_converter()->error().error(value, rdo::format(_T("Перечислимое значение определено дважды: %s"), value->getIdentificator().c_str()));
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
	Converter::s_converter()->runtime()->addInitCalc(m_pInitCalc);
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
	if (pParamList->getParamList().size() != 0)
	{
		Converter::s_converter()->error().error(src_info, rdo::format(_T("Перечисление '%s' должно вызываться без параметров"), name().c_str()));
	}

	rdoRuntime::LPRDOCalcFunctionCaller pFuctionCall = rdo::Factory<rdoRuntime::RDOCalcFunctionCaller>::create(m_pNextCalc);
	ASSERT(pFuctionCall);

	LPRDOFUNArithm pArithm = rdo::Factory<RDOFUNArithm>::create(RDOValue(m_pHeader->getType()->type(), pParamList->src_pos()), pFuctionCall);
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
		pGenerator->addValue(m_valueList[i].value());
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

rdoRuntime::LPRDOCalcIsEqual RDOFUNFunctionListElement::createIsEqualCalc(CREF(LPRDOTypeParam) pRetType, CREF(rdoRuntime::LPRDOCalcFuncParam) pFuncParam, CREF(RDOParserSrcInfo) src_pos) const
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

rdoRuntime::LPRDOCalcConst RDOFUNFunctionListElementIdentif::createResultCalc(CREF(LPRDOTypeParam) pRetType, CREF(RDOParserSrcInfo) src_pos) const
{
	rdoRuntime::LPRDOCalcConst pCalcConst = rdo::Factory<rdoRuntime::RDOCalcConst>::create(pRetType->value_cast(RDOValue(RDOParserSrcInfo(src_pos.getPosAsYY(), src_text()))).value());
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

rdoRuntime::LPRDOCalcConst RDOFUNFunctionListElementReal::createResultCalc(CREF(LPRDOTypeParam) pRetType, CREF(RDOParserSrcInfo) src_pos) const
{
	rdoRuntime::LPRDOCalcConst pCalcConst = rdo::Factory<rdoRuntime::RDOCalcConst>::create(pRetType->value_cast(RDOValue(m_value, rdo::Factory<RDOType__real>::create(), src_pos)).value());
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

rdoRuntime::LPRDOCalcConst RDOFUNFunctionListElementInt::createResultCalc(CREF(LPRDOTypeParam) pRetType, CREF(RDOParserSrcInfo) src_pos) const
{
	rdoRuntime::LPRDOCalcConst pCalcConst = rdo::Factory<rdoRuntime::RDOCalcConst>::create(pRetType->value_cast(RDOValue(m_value, rdo::Factory<RDOType__int>::create(), src_pos)).value());
	pCalcConst->setSrcInfo(src_pos);
	return pCalcConst;
}

// --------------------------------------------------------------------------------
// -------------------- RDOFUNFunctionListElementEq
// --------------------------------------------------------------------------------
RDOFUNFunctionListElementEq::RDOFUNFunctionListElementEq(CREF(YYLTYPE) position)
	: RDOFUNFunctionListElement(RDOParserSrcInfo(position, _T("=")))
{}

rdoRuntime::LPRDOCalcConst RDOFUNFunctionListElementEq::createResultCalc(CREF(LPRDOTypeParam) pRetType, CREF(RDOParserSrcInfo) src_pos) const
{
	UNUSED(pRetType);

	Converter::s_converter()->error().error(src_pos, _T("Внутренная ошибка парсера: RDOFUNFunctionListElementEq::createResultCalc"));
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
	m_pAction->checkParamType(Converter::s_converter()->getLastFUNFunction()->getReturn()->getType());
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
	Converter::s_converter()->insertFUNFunction(this);
}

RDOFUNFunction::RDOFUNFunction(CREF(tstring) name, CREF(LPRDOParam) pReturn)
	: RDOParserSrcInfo(name   )
	, m_pReturn       (pReturn)
{
	Converter::s_converter()->insertFUNFunction(this);
}

RDOFUNFunction::~RDOFUNFunction()
{}

void RDOFUNFunction::setFunctionCalc(CREF(rdoRuntime::LPRDOFunCalc) pCalc)
{
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
		Converter::s_converter()->error().push_only(pParam->src_info(), rdo::format(_T("Параметр уже существует: %s"), pParam->name().c_str()));
		Converter::s_converter()->error().push_only(pParamPrev->src_info(), _T("См. первое определение"));
		Converter::s_converter()->error().push_done();
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
		Converter::s_converter()->error().error(m_pReturn->src_info(), rdo::format(_T("Функция '%s' должна иметь значение по-умолчанию"), name().c_str()));
	}
	if (m_paramList.empty())
	{
		Converter::s_converter()->error().error(src_info(), rdo::format(_T("Функция '%s' должна иметь параметры, т.к. её тип list"), name().c_str()));
	}
	if (m_elementList.empty())
	{
		Converter::s_converter()->error().warning(src_info(), rdo::format(_T("Функция '%s' не содержит списка"), name().c_str()));
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
				Converter::s_converter()->error().error((*elem_it)->src_info(), rdo::format(_T("Ожидается значение для параметра '%s'"), pParam->name().c_str()));
			}
			LPRDOFUNFunctionListElement pListElement = *elem_it;
			ASSERT(pListElement);
			if (pListElement->isEquivalence())
			{
				if ((param_it + 1) == m_paramList.end())
				{
					Converter::s_converter()->error().error(pListElement->src_info(), rdo::format(_T("Перед знаком равенства ожидается значение для параметра: %s"),pParam->name().c_str()));
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
					Converter::s_converter()->error().error(pListElement->src_info(), rdo::format(_T("Перед знаком равенства ожидаются значения для параметров: %s"), str.c_str()));
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
			Converter::s_converter()->error().error((*elem_it)->src_info(), rdo::format(_T("После значения для параметра '%s' ожидается знак равенства"), (*param_it)->name().c_str()));
		}
		if (!(*elem_it)->isEquivalence())
		{
			Converter::s_converter()->error().error((*elem_it)->src_info(), rdo::format(_T("Слишком много значений, вместо '%s' ожидается знак равенства"), (*elem_it)->src_text().c_str()));
		}
		++elem_it;
		if (elem_it == m_elementList.end())
		{
			--elem_it;
			Converter::s_converter()->error().error((*elem_it)->src_info(), rdo::format(_T("После знака равенства ожидается значение функции '%s'"), name().c_str()));
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
			Converter::s_converter()->error().error((*it)->src_info(), _T("Символ '=' недопустим в табличной функции"));
		}
		++it;
	}
	ruint param_cnt = m_paramList.size();
	ruint range     = 1;
	rdoRuntime::LPRDOCalc pCalc = rdo::Factory<rdoRuntime::RDOCalcConst>::create(0);
	ASSERT(pCalc);
	rdoRuntime::RDOSrcInfo srcInfo(src_info());
	srcInfo.setSrcText(_T("0"));
	pCalc->setSrcInfo(srcInfo);
	for (ruint currParam = 0; currParam < param_cnt; currParam++)
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

		switch (pFunctionParam->getType()->type()->typeID())
		{
		case rdoRuntime::RDOType::t_int:
			if (dynamic_cast<PTR(RDOTypeIntRange)>(pFunctionParam->getType()->type().get()))
			{
				LPRDOTypeIntRange pRange = pFunctionParam->getType()->type().object_static_cast<RDOTypeIntRange>();
				if (pRange->range()->getMin().value().getInt() != 1)
				{
					Converter::s_converter()->error().error(pRange->range()->src_info(), rdo::format(_T("Минимальное значение диапазона должно быть 1, текущий диапазон [%d..%d]"), pRange->range()->getMin().value().getInt(), pRange->range()->getMax().value().getInt()));
				}
				range *= pRange->range()->getMax().value().getInt() - pRange->range()->getMin().value().getInt() + 1;
			}
			else
			{
				Converter::s_converter()->error().error(pFunctionParam->getType()->src_info(), _T("Для параметра табличной функции должен быть задан допустимый диапазон"));
			}
			break;
		case rdoRuntime::RDOType::t_enum:
			range *= pFunctionParam->getType()->type().object_static_cast<RDOEnumType>()->getEnums()->getValues().size();
			break;
		default:
			Converter::s_converter()->error().error(pFunctionParam->getType()->src_info(), _T("Параметр табличной функции может быть целого или перечислимого типа"));
		}
		pCalc = pCalcAdd;
	}

	if (m_elementList.size() != range)
	{
		Converter::s_converter()->error().error(elements_pos, rdo::format(_T("Неверное количество элементов табличной функции '%s': %d, требуется %d"), name().c_str(), m_elementList.size(), range));
	}

	rdoRuntime::LPRDOFuncTableCalc pFuncTableCalc = rdo::Factory<rdoRuntime::RDOFuncTableCalc>::create(pCalc);
	ASSERT(pFuncTableCalc);
	pFuncTableCalc->setSrcInfo(src_info());
	for (ruint currElem = 0; currElem < range; currElem++)
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
	rdoRuntime::LPRDOFunAlgorithmicCalc pFunAlgorithmicCalc = rdo::Factory<rdoRuntime::RDOFunAlgorithmicCalc>::create();
	ASSERT(pFunAlgorithmicCalc);

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
			Converter::s_converter()->error().warning(m_calculateIfList[i]->getCondition()->src_info(), rdo::format(_T("Условие не используется: %s"), m_calculateIfList[i]->getCondition()->src_text().c_str()));
			Converter::s_converter()->error().warning(pCondition->srcInfo(), rdo::format(_T("Последнее рабочее условие функции: %s"), pCondition->srcInfo().src_text().c_str()));
		}
		else if (!pConditionLast || pConditionLast->calcValue(Converter::s_converter()->runtime()).getAsBool())
		{
			//! Игнорируем чистые false-условия предыдущей проверкой
			pFunAlgorithmicCalc->addCalcIf(pLogicCalc, m_calculateIfList[i]->getAction()->createCalc(m_pReturn->getType()));
			cnt++;
		}
		if (!defaultFlag && pConditionLast && pConditionLast->calcValue(Converter::s_converter()->runtime()).getAsBool())
		{
			trueConst   = true;
			defaultFlag = true;
			pCondition  = pConditionLast;
		}
	}
	if (!cnt)
	{
		Converter::s_converter()->error().warning(src_info(), rdo::format(_T("Отсутствует тело функции '%s'"), name().c_str()));
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
		Converter::s_converter()->error().warning(src_info(), rdo::format(_T("Для функции '%s' неопределено значение по-умолчанию"), name().c_str()));
	}
	setFunctionCalc(pFunAlgorithmicCalc);
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
	m_pResType = Converter::s_converter()->findRTPResType(res_info.src_text());
	if (!m_pResType)
	{
		Converter::s_converter()->error().error(res_info, rdo::format(_T("Неизвестный тип ресурса: %s"), res_info.src_text().c_str()));
	}
	Converter::s_converter()->insertFUNGroup(this);
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
	rdoRuntime::LPRDOFunCalcGroup calc;
	switch (m_funType)
	{
	case fgt_exist    : setSrcText(_T("Exist(")     + getResType()->name() + _T(": ") + pCondition->src_text() + _T(")")); calc = rdo::Factory<rdoRuntime::RDOFunCalcExist    >::create(getResType()->getNumber(), pCondition->getCalc()); break;
	case fgt_notexist : setSrcText(_T("NotExist(")  + getResType()->name() + _T(": ") + pCondition->src_text() + _T(")")); calc = rdo::Factory<rdoRuntime::RDOFunCalcNotExist >::create(getResType()->getNumber(), pCondition->getCalc()); break;
	case fgt_forall   : setSrcText(_T("ForAll(")    + getResType()->name() + _T(": ") + pCondition->src_text() + _T(")")); calc = rdo::Factory<rdoRuntime::RDOFunCalcForAll   >::create(getResType()->getNumber(), pCondition->getCalc()); break;
	case fgt_notforall: setSrcText(_T("NotForAll(") + getResType()->name() + _T(": ") + pCondition->src_text() + _T(")")); calc = rdo::Factory<rdoRuntime::RDOFunCalcNotForAll>::create(getResType()->getNumber(), pCondition->getCalc()); break;
	default: Converter::s_converter()->error().error(src_info(), _T("Внутренная ошибка: несуществующий тип функции"));
	}
	Converter::s_converter()->getFUNGroupStack().pop_back();
	LPRDOFUNLogic pLogic = rdo::Factory<RDOFUNLogic>::create(calc, false);
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
	rdoRuntime::LPRDOFunCalcSelectBase calc;
	switch (funType)
	{
	case RDOFUNGroupLogic::fgt_exist    : setSrcText(src_text() + _T(".Exist(")     + pCondition->src_text() + _T(")")); calc = rdo::Factory<rdoRuntime::RDOFunCalcSelectExist    >::create(m_pCalcSelect, pCondition->getCalc()); break;
	case RDOFUNGroupLogic::fgt_notexist : setSrcText(src_text() + _T(".NotExist(")  + pCondition->src_text() + _T(")")); calc = rdo::Factory<rdoRuntime::RDOFunCalcSelectNotExist >::create(m_pCalcSelect, pCondition->getCalc()); break;
	case RDOFUNGroupLogic::fgt_forall   : setSrcText(src_text() + _T(".ForAll(")    + pCondition->src_text() + _T(")")); calc = rdo::Factory<rdoRuntime::RDOFunCalcSelectForAll   >::create(m_pCalcSelect, pCondition->getCalc()); break;
	case RDOFUNGroupLogic::fgt_notforall: setSrcText(src_text() + _T(".NotForAll(") + pCondition->src_text() + _T(")")); calc = rdo::Factory<rdoRuntime::RDOFunCalcSelectNotForAll>::create(m_pCalcSelect, pCondition->getCalc()); break;
	default: Converter::s_converter()->error().error(pCondition->src_info(), _T("Внутренная ошибка: неизвестный метод для списка ресурсов"));
	}
	Converter::s_converter()->getFUNGroupStack().pop_back();
	LPRDOFUNLogic pLogic = rdo::Factory<RDOFUNLogic>::create(calc, false);
	pLogic->setSrcInfo(src_info());
	return pLogic;
}

LPRDOFUNLogic RDOFUNSelect::createFunSelectEmpty(CREF(RDOParserSrcInfo) empty_info)
{
	setSrcText(src_text() + _T(".") + empty_info.src_text());
	Converter::s_converter()->getFUNGroupStack().pop_back();
	LPRDOFUNLogic pLogic = rdo::Factory<RDOFUNLogic>::create(rdo::Factory<rdoRuntime::RDOFunCalcSelectEmpty>::create(m_pCalcSelect), false);
	pLogic->setSrcInfo(empty_info);
	return pLogic;
}

LPRDOFUNArithm RDOFUNSelect::createFunSelectSize(CREF(RDOParserSrcInfo) size_info)
{
	setSrcText(src_text() + _T(".") + size_info.src_text());
	Converter::s_converter()->getFUNGroupStack().pop_back();
	LPRDOFUNArithm pArithm = rdo::Factory<RDOFUNArithm>::create(RDOValue(rdo::Factory<RDOType__int>::create(), size_info), rdo::Factory<rdoRuntime::RDOFunCalcSelectSize>::create(m_pCalcSelect));
	pArithm->setSrcInfo(size_info);
	return pArithm;
}

CLOSE_RDO_CONVERTER_NAMESPACE
