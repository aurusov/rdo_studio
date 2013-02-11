/*!
  \copyright (c) RDO-Team, 2011
  \file      rdopmd.cpp
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
#include "simulator/runtime/calc/calc_base.h"
#include "simulator/runtime/result/result.h"
#include "simulator/runtime/result/result_group.h"
#include "simulator/compiler/parser/rdopmd.h"
#include "simulator/compiler/parser/rdoparser.h"
#include "simulator/compiler/parser/rdorss.h"
#include "simulator/compiler/parser/rdortp.h"
#include "simulator/compiler/parser/rdoparser_lexer.h"
// --------------------------------------------------------------------------------

OPEN_RDO_PARSER_NAMESPACE

int pmdlex(PTR(YYSTYPE) lpval, PTR(YYLTYPE) llocp, PTR(void) lexer)
{
	LEXER->m_lpval = lpval;
	LEXER->m_lploc = llocp;
	return LEXER->yylex();
}

void pmderror(PTR(char) message)
{
	UNUSED(message);
}

// --------------------------------------------------------------------------------
// -------------------- RDOPMDResult
// --------------------------------------------------------------------------------
RDOPMDResult::RDOPMDResult(CREF(RDOParserSrcInfo) src_info)
	: RDOParserSrcInfo(src_info)
{}

RDOPMDResult::~RDOPMDResult()
{}

void RDOPMDResult::endOfCreation(CREF(LPIResult) pResult)
{
	ASSERT(pResult);

	LPContext pContext = RDOParser::s_parser()->context();
	ASSERT(pContext);

	LPRDOResultGroup pResultGroup = pContext->cast<RDOResultGroup>();
	if (!pResultGroup)
	{
		//! Показатель создаётся не в контексте группы
		//! Такое может быть из rdoproc_rss.y
		pResultGroup = RDOParser::s_parser()->findResultGroup(_T(""));
		if (!pResultGroup)
		{
			//! Нет даже группы по умолчанию
			pResultGroup = rdo::Factory<RDOResultGroup>::create();
			ASSERT(pResultGroup);
			pResultGroup->init(RDOParserSrcInfo());
		}
	}

	m_pResult = pResult;
	pResultGroup->append(this);

	LPITrace pTrace = m_pResult;
	if (pTrace)
	{
		pTrace->setTraceID(RDOParser::s_parser()->getPMD_id());
	}
}

// --------------------------------------------------------------------------------
// -------------------- ResultGroup
// --------------------------------------------------------------------------------
RDOResultGroup::RDOResultGroup()
{}

RDOResultGroup::~RDOResultGroup()
{}

void RDOResultGroup::init(CREF(RDOParserSrcInfo) src_info)
{
	setSrcInfo(src_info);

	LPRDOResultGroup pResultGroupFound = RDOParser::s_parser()->findResultGroup(name());
	if (pResultGroupFound)
	{
		RDOParser::s_parser()->error().push_only(src_info, rdo::format(_T("Группа показателей '%s' уже существует"), src_text().c_str()));
		RDOParser::s_parser()->error().push_only(pResultGroupFound->src_info(), _T("См. первое определение"));
		RDOParser::s_parser()->error().push_done();
	}
	RDOParser::s_parser()->insertResultGroup(this);
	m_pResultGroup = RF(rdo::runtime::RDOPMDResultGroup)::create(name());
	ASSERT(m_pResultGroup);
	RDOParser::s_parser()->runtime()->addRuntimeResult(m_pResultGroup);
}

CREF(tstring) RDOResultGroup::name() const
{
	return src_text();
}

CREF(LPIResultGroup) RDOResultGroup::getRuntime() const
{
	return m_pResultGroup;
}

void RDOResultGroup::append(CREF(LPRDOPMDResult) pResult)
{
	ASSERT(pResult);
	LPRDOPMDResult pResultFound = find(pResult->name());
	if (pResultFound)
	{
		RDOParser::s_parser()->error().push_only(pResult->src_info(), rdo::format(_T("Показатель '%s' уже существует"), pResult->src_text().c_str()));
		RDOParser::s_parser()->error().push_only(pResultFound->src_info(), _T("См. первое определение"));
		RDOParser::s_parser()->error().push_done();
	}
	m_resultList.push_back(pResult);
	m_pResultGroup->onAppend(pResult->getRuntime());
	RDOParser::s_parser()->runtime()->addRuntimeResult(pResult->getRuntime());
}

LPRDOPMDResult RDOResultGroup::find(CREF(tstring) resultName) const
{
	STL_FOR_ALL_CONST(m_resultList, it)
	{
		if ((*it)->name() == resultName)
		{
			return *it;
		}
	}
	return NULL;
}

// --------------------------------------------------------------------------------
// -------------------- RDOPMDWatchPar
// --------------------------------------------------------------------------------
RDOPMDWatchPar::RDOPMDWatchPar(CREF(RDOParserSrcInfo) src_info)
	: RDOPMDResult(src_info)
{}

RDOPMDWatchPar::~RDOPMDWatchPar()
{}

void RDOPMDWatchPar::init(rbool trace, CREF(RDOParserSrcInfo) res_src_info, CREF(RDOParserSrcInfo) par_src_info)
{
	LPRDORSSResource pResource = RDOParser::s_parser()->findRSSResource(res_src_info.src_text());
	if (!pResource)
	{
		RDOParser::s_parser()->error().error(res_src_info, rdo::format(_T("Ресурс '%s' не найден"), res_src_info.src_text().c_str()));
	}
/*
	if (!pResource->getType()->isPermanent())
	{
		RDOParser::s_parser()->error().push_only(res_src_info, _T("Наблюдать (watch_par) можно только за параметром постоянного ресурса"));
		RDOParser::s_parser()->error().push_only(pResource->getType()->src_info(), _T("См. тип ресурса"));
		RDOParser::s_parser()->error().push_done();
	}
*/
	LPRDORTPParam pParam = pResource->getType()->findRTPParam(par_src_info.src_text());
	if (!pParam)
	{
		RDOParser::s_parser()->error().push_only(par_src_info, rdo::format(_T("Параметр '%s' не найден"), par_src_info.src_text().c_str()));
		RDOParser::s_parser()->error().push_only(pResource->src_info(), _T("См. ресурс"));
		RDOParser::s_parser()->error().push_only(pResource->getType()->src_info(), _T("См. тип ресурса"));
		RDOParser::s_parser()->error().push_done();
	}
	rdo::runtime::RDOType::TypeID typeID = pParam->getTypeInfo()->type()->typeID();
	if (typeID != rdo::runtime::RDOType::t_int && typeID != rdo::runtime::RDOType::t_real)
	{
		RDOParser::s_parser()->error().push_only(par_src_info, _T("Наблюдать можно только за параметром целого или вещественного типа"));
		RDOParser::s_parser()->error().push_only(pParam->getTypeInfo()->src_info(), _T("См. тип параметра"));
		RDOParser::s_parser()->error().push_done();
	}
	endOfCreation(RF(rdo::runtime::RDOPMDWatchPar)::create(RDOParser::s_parser()->runtime(), src_text(), trace, res_src_info.src_text(), par_src_info.src_text(), pResource->getID(), pResource->getType()->getRTPParamNumber(par_src_info.src_text())));
}

// --------------------------------------------------------------------------------
// -------------------- RDOPMDWatchState
// --------------------------------------------------------------------------------
RDOPMDWatchState::RDOPMDWatchState(CREF(RDOParserSrcInfo) src_info)
	: RDOPMDResult(src_info)
{}

RDOPMDWatchState::~RDOPMDWatchState()
{}

void RDOPMDWatchState::init(rbool trace, LPRDOFUNLogic pLogic)
{
	endOfCreation(RF(rdo::runtime::RDOPMDWatchState)::create(RDOParser::s_parser()->runtime(), src_text(), trace, pLogic->getCalc()));
}

// --------------------------------------------------------------------------------
// -------------------- RDOPMDWatchTemp
// --------------------------------------------------------------------------------
RDOPMDWatchTemp::RDOPMDWatchTemp(CREF(RDOParserSrcInfo) src_info, CREF(RDOParserSrcInfo) res_type_src_info)
	: RDOPMDResult(src_info)
{
	LPRDORTPResType pResType = RDOParser::s_parser()->findRTPResType(res_type_src_info.src_text());
	if (!pResType)
	{
		RDOParser::s_parser()->error().error(res_type_src_info, rdo::format(_T("Тип ресурса '%s' не найден"), res_type_src_info.src_text().c_str()));
	}
	if (!pResType->isTemporary())
	{
		RDOParser::s_parser()->error().push_only(res_type_src_info, _T("Показатель собирает информацию по временным ресурсам (temporary)"));
		RDOParser::s_parser()->error().push_only(pResType->src_info(), _T("См. тип ресурса"));
		RDOParser::s_parser()->error().push_done();
	}
}

RDOPMDWatchTemp::~RDOPMDWatchTemp()
{}

// --------------------------------------------------------------------------------
// -------------------- RDOPMDWatchQuant
// --------------------------------------------------------------------------------
RDOPMDWatchQuant::RDOPMDWatchQuant(CREF(RDOParserSrcInfo) src_info, CREF(RDOParserSrcInfo) res_type_src_info)
	: RDOPMDWatchTemp(src_info, res_type_src_info)
{}

RDOPMDWatchQuant::~RDOPMDWatchQuant()
{}

void RDOPMDWatchQuant::init(rbool trace, CREF(RDOParserSrcInfo) res_type_src_info)
{
	m_pGroupLogic = rdo::Factory<RDOFUNGroupLogic>::create(RDOFUNGroupLogic::fgt_unknow, RDOParserSrcInfo(res_type_src_info.src_text()));
	ASSERT(m_pGroupLogic);
	endOfCreation(RF(rdo::runtime::RDOPMDWatchQuant)::create(RDOParser::s_parser()->runtime(), src_text(), trace, res_type_src_info.src_text(), m_pGroupLogic->getResType()->getNumber()));
}

void RDOPMDWatchQuant::setLogic(REF(LPRDOFUNLogic) pLogic)
{
	LPIResultWatchQuant pQuant = m_pResult;
	ASSERT(pQuant);
	pQuant->setLogicCalc(pLogic->getCalc());
	m_pGroupLogic->end();
	RDOParser::s_parser()->getFUNGroupStack().pop_back();
}

void RDOPMDWatchQuant::setLogicNoCheck()
{
	LPIResultWatchQuant pQuant = m_pResult;
	ASSERT(pQuant);
	pQuant->setLogicCalc(rdo::Factory<rdo::runtime::RDOCalcConst>::create(1));
	m_pGroupLogic->end();
	RDOParser::s_parser()->getFUNGroupStack().pop_back();
}

// --------------------------------------------------------------------------------
// -------------------- RDOPMDWatchValue
// --------------------------------------------------------------------------------
RDOPMDWatchValue::RDOPMDWatchValue(CREF(RDOParserSrcInfo) src_info, CREF(RDOParserSrcInfo) res_type_src_info)
	: RDOPMDWatchTemp(src_info, res_type_src_info)
{}

RDOPMDWatchValue::~RDOPMDWatchValue()
{}

void RDOPMDWatchValue::init(rbool trace, CREF(RDOParserSrcInfo) res_type_src_info)
{
	m_pGroupLogic = rdo::Factory<RDOFUNGroupLogic>::create(RDOFUNGroupLogic::fgt_unknow, RDOParserSrcInfo(res_type_src_info.src_text()));
	ASSERT(m_pGroupLogic);
	endOfCreation(RF(rdo::runtime::RDOPMDWatchValue)::create(RDOParser::s_parser()->runtime(), src_text(), trace, res_type_src_info.src_text(), m_pGroupLogic->getResType()->getNumber()));
}

void RDOPMDWatchValue::setLogic(REF(LPRDOFUNLogic) pLogic, REF(LPRDOFUNArithm) pArithm)
{
	LPIResultWatchValue pWatch = m_pResult;
	ASSERT(pWatch);
	pWatch->setLogicCalc (pLogic->getCalc()    );
	pWatch->setArithmCalc(pArithm->createCalc());
	m_pGroupLogic->end();
	RDOParser::s_parser()->getFUNGroupStack().pop_back();
}

void RDOPMDWatchValue::setLogicNoCheck(REF(LPRDOFUNArithm) pArithm)
{
	LPIResultWatchValue pWatch = m_pResult;
	ASSERT(pWatch);
	pWatch->setLogicCalc (rdo::Factory<rdo::runtime::RDOCalcConst>::create(1));
	pWatch->setArithmCalc(pArithm->createCalc());
	m_pGroupLogic->end();
	RDOParser::s_parser()->getFUNGroupStack().pop_back();
}

// --------------------------------------------------------------------------------
// -------------------- RDOPMDGetValue
// --------------------------------------------------------------------------------
RDOPMDGetValue::RDOPMDGetValue(CREF(RDOParserSrcInfo) src_info)
	: RDOPMDResult(src_info)
{}

RDOPMDGetValue::~RDOPMDGetValue()
{}

void RDOPMDGetValue::init(LPRDOFUNArithm pArithm)
{
	endOfCreation(RF(rdo::runtime::RDOPMDGetValue)::create(RDOParser::s_parser()->runtime(), src_text(), pArithm->createCalc()));
}

CLOSE_RDO_PARSER_NAMESPACE
