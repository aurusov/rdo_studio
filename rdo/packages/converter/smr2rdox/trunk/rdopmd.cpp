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
#include "converter/smr2rdox/pch.h"
// ----------------------------------------------------------------------- INCLUDES
// ----------------------------------------------------------------------- SYNOPSIS
#include "simulator/runtime/calc/calc_base.h"
#include "converter/smr2rdox/rdopmd.h"
#include "converter/smr2rdox/rdoparser.h"
#include "converter/smr2rdox/rdorss.h"
#include "converter/smr2rdox/rdortp.h"
#include "converter/smr2rdox/rdoparser_lexer.h"
// --------------------------------------------------------------------------------

OPEN_RDO_CONVERTER_NAMESPACE

int cnv_pmdlex(PTR(YYSTYPE) lpval, PTR(YYLTYPE) llocp, PTR(void) lexer)
{
	LEXER->m_lpval = lpval;
	LEXER->m_lploc = llocp;
	return LEXER->yylex();
}

void cnv_pmderror(PTR(char) message)
{
	UNUSED(message);
}

// --------------------------------------------------------------------------------
// -------------------- RDOPMDResult
// --------------------------------------------------------------------------------
RDOPMDResult::RDOPMDResult(CREF(RDOParserSrcInfo) src_info)
	: RDOParserSrcInfo(src_info)
{
	LPRDOPMDResult pResult = Converter::s_converter()->findPMDResult(src_text());
	if (pResult)
	{
		Converter::s_converter()->error().push_only(this->src_info(), rdo::format(_T("Показатель '%s' уже существует"), src_text().c_str()));
		Converter::s_converter()->error().push_only(pResult->src_info(), _T("См. первое определение"));
		Converter::s_converter()->error().push_done();
	}
}

RDOPMDResult::~RDOPMDResult()
{}

void RDOPMDResult::endOfCreation(CREF(LPIResult) pResult)
{
	m_pResult = pResult;
	LPITrace trace = m_pResult;
	if (trace)
	{
		trace->setTraceID(Converter::s_converter()->getPMD_id());
	}
	Converter::s_converter()->insertPMDResult(this);
	/// @todo перенести в конструктор rdo::runtime::RDOPMDResult
	Converter::s_converter()->runtime()->addRuntimeResult(m_pResult);
}

// --------------------------------------------------------------------------------
// -------------------- RDOPMDWatchPar
// --------------------------------------------------------------------------------
RDOPMDWatchPar::RDOPMDWatchPar(CREF(RDOParserSrcInfo) src_info, rbool trace, CREF(RDOParserSrcInfo) res_src_info, CREF(RDOParserSrcInfo) par_src_info)
	: RDOPMDResult(src_info)
{
	LPRDORSSResource pResource = Converter::s_converter()->findRSSResource(res_src_info.src_text());
	if (!pResource)
	{
		Converter::s_converter()->error().error(res_src_info, rdo::format(_T("Ресурс '%s' не найден"), res_src_info.src_text().c_str()));
	}
/*
	if (!pResource->getType()->isPermanent())
	{
		Converter::s_converter()->error().push_only(res_src_info, _T("Наблюдать (watch_par) можно только за параметром постоянного ресурса"));
		Converter::s_converter()->error().push_only(pResource->getType()->src_info(), _T("См. тип ресурса"));
		Converter::s_converter()->error().push_done();
	}
*/
	LPRDORTPParam pParam = pResource->getType()->findRTPParam(par_src_info.src_text());
	if (!pParam)
	{
		Converter::s_converter()->error().push_only(par_src_info, rdo::format(_T("Параметр '%s' не найден"), par_src_info.src_text().c_str()));
		Converter::s_converter()->error().push_only(pResource->src_info(), _T("См. ресурс"));
		Converter::s_converter()->error().push_only(pResource->getType()->src_info(), _T("См. тип ресурса"));
		Converter::s_converter()->error().push_done();
	}
	rdo::runtime::RDOType::TypeID typeID = pParam->getType()->type()->typeID();
	if (typeID != rdo::runtime::RDOType::t_int && typeID != rdo::runtime::RDOType::t_real)
	{
		Converter::s_converter()->error().push_only(par_src_info, _T("Наблюдать можно только за параметром целого или вещественного типа"));
		Converter::s_converter()->error().push_only(pParam->getType()->src_info(), _T("См. тип параметра"));
		Converter::s_converter()->error().push_done();
	}
	endOfCreation(RF(rdo::runtime::RDOPMDWatchPar)::create(Converter::s_converter()->runtime(), src_text(), trace, res_src_info.src_text(), par_src_info.src_text(), pResource->getID(), pResource->getType()->getRTPParamNumber(par_src_info.src_text())));
}

// --------------------------------------------------------------------------------
// -------------------- RDOPMDWatchState
// --------------------------------------------------------------------------------
RDOPMDWatchState::RDOPMDWatchState(CREF(RDOParserSrcInfo) src_info, rbool trace, LPRDOFUNLogic pLogic)
	: RDOPMDResult(src_info)
{
	endOfCreation(RF(rdo::runtime::RDOPMDWatchState)::create(Converter::s_converter()->runtime(), src_text(), trace, pLogic->getCalc()));
}

// --------------------------------------------------------------------------------
// -------------------- RDOPMDWatchTemp
// --------------------------------------------------------------------------------
RDOPMDWatchTemp::RDOPMDWatchTemp(CREF(RDOParserSrcInfo) src_info, CREF(RDOParserSrcInfo) res_type_src_info)
	: RDOPMDResult(src_info)
{
	LPRDORTPResType pResType = Converter::s_converter()->findRTPResType(res_type_src_info.src_text());
	if (!pResType)
	{
		Converter::s_converter()->error().error(res_type_src_info, rdo::format(_T("Тип ресурса '%s' не найден"), res_type_src_info.src_text().c_str()));
	}
	if (!pResType->isTemporary())
	{
		Converter::s_converter()->error().push_only(res_type_src_info, _T("Показатель собирает информацию по временным ресурсам (temporary)"));
		Converter::s_converter()->error().push_only(pResType->src_info(), _T("См. тип ресурса"));
		Converter::s_converter()->error().push_done();
	}
}

// --------------------------------------------------------------------------------
// -------------------- RDOPMDWatchQuant
// --------------------------------------------------------------------------------
RDOPMDWatchQuant::RDOPMDWatchQuant(CREF(RDOParserSrcInfo) src_info, rbool trace, CREF(RDOParserSrcInfo) res_type_src_info)
	: RDOPMDWatchTemp(src_info, res_type_src_info)
{
	LPRDOFUNGroupLogic pGroupLogic = rdo::Factory<RDOFUNGroupLogic>::create(RDOFUNGroupLogic::fgt_unknow, RDOParserSrcInfo(res_type_src_info.src_text()));
	ASSERT(pGroupLogic);
	endOfCreation(RF(rdo::runtime::RDOPMDWatchQuant)::create(Converter::s_converter()->runtime(), src_text(), trace, res_type_src_info.src_text(), pGroupLogic->getResType()->getNumber()));
}

void RDOPMDWatchQuant::setLogic(REF(LPRDOFUNLogic) pLogic)
{
	LPIResultWatchQuant pQuant = m_pResult;
	ASSERT(pQuant);
	pQuant->setLogicCalc(pLogic->getCalc());
	Converter::s_converter()->getFUNGroupStack().pop_back();
}

void RDOPMDWatchQuant::setLogicNoCheck()
{
	LPIResultWatchQuant pQuant = m_pResult;
	ASSERT(pQuant);
	pQuant->setLogicCalc(rdo::Factory<rdo::runtime::RDOCalcConst>::create(1));
	Converter::s_converter()->getFUNGroupStack().pop_back();
}

// --------------------------------------------------------------------------------
// -------------------- RDOPMDWatchValue
// --------------------------------------------------------------------------------
RDOPMDWatchValue::RDOPMDWatchValue(CREF(RDOParserSrcInfo) src_info, rbool trace, CREF(RDOParserSrcInfo) res_type_src_info)
	: RDOPMDWatchTemp(src_info, res_type_src_info)
{
	LPRDOFUNGroupLogic pGroupLogic = rdo::Factory<RDOFUNGroupLogic>::create(RDOFUNGroupLogic::fgt_unknow, RDOParserSrcInfo(res_type_src_info.src_text()));
	ASSERT(pGroupLogic);
	endOfCreation(RF(rdo::runtime::RDOPMDWatchValue)::create(Converter::s_converter()->runtime(), src_text(), trace, res_type_src_info.src_text(), pGroupLogic->getResType()->getNumber()));
}

void RDOPMDWatchValue::setLogic(REF(LPRDOFUNLogic) pLogic, REF(LPRDOFUNArithm) pArithm)
{
	LPIResultWatchValue pWatch = m_pResult;
	ASSERT(pWatch);
	pWatch->setLogicCalc (pLogic->getCalc()    );
	pWatch->setArithmCalc(pArithm->createCalc());
	Converter::s_converter()->getFUNGroupStack().pop_back();
}

void RDOPMDWatchValue::setLogicNoCheck(REF(LPRDOFUNArithm) pArithm)
{
	LPIResultWatchValue pWatch = m_pResult;
	ASSERT(pWatch);
	pWatch->setLogicCalc (rdo::Factory<rdo::runtime::RDOCalcConst>::create(1));
	pWatch->setArithmCalc(pArithm->createCalc());
	Converter::s_converter()->getFUNGroupStack().pop_back();
}

// --------------------------------------------------------------------------------
// -------------------- RDOPMDGetValue
// --------------------------------------------------------------------------------
RDOPMDGetValue::RDOPMDGetValue(CREF(RDOParserSrcInfo) src_info, LPRDOFUNArithm pArithm)
	: RDOPMDResult(src_info)
{
	endOfCreation(RF(rdo::runtime::RDOPMDGetValue)::create(Converter::s_converter()->runtime(), src_text(), pArithm->createCalc()));
}

CLOSE_RDO_CONVERTER_NAMESPACE
