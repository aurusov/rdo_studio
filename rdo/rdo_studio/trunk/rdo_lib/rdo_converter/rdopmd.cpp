/*
 * copyright: (c) RDO-Team, 2011
 * file     : rdopmd.cpp
 * author   : Барс Александр, Урусов Андрей
 * date     : 
 * brief    : 
 * indent   : 4T
 */

// ====================================================================== PCH
#include "rdo_lib/rdo_converter/pch.h"
// ====================================================================== INCLUDES
// ====================================================================== SYNOPSIS
#include "rdo_lib/rdo_runtime/rdocalc.h"
#include "rdo_lib/rdo_converter/rdopmd.h"
#include "rdo_lib/rdo_converter/rdoparser.h"
#include "rdo_lib/rdo_converter/rdorss.h"
#include "rdo_lib/rdo_converter/rdortp.h"
#include "rdo_lib/rdo_converter/rdoparser_lexer.h"
// ===============================================================================

OPEN_RDO_CONVERTER_NAMESPACE

int cnv_pmdlex(PTR(YYSTYPE) lpval, PTR(YYLTYPE) llocp, PTR(void) lexer)
{
	LEXER->m_lpval = lpval;
	LEXER->m_lploc = llocp;
	return LEXER->yylex();
}

void cnv_pmderror(PTR(char) mes)
{}

// ----------------------------------------------------------------------------
// ---------- RDOPMDPokaz
// ----------------------------------------------------------------------------
RDOPMDPokaz::RDOPMDPokaz(CREF(RDOParserSrcInfo) src_info)
	: RDOParserSrcInfo(src_info)
{
	LPRDOPMDPokaz pPokaz = Converter::s_converter()->findPMDPokaz(src_text());
	if (pPokaz)
	{
		Converter::s_converter()->error().push_only(this->src_info(), rdo::format(_T("Показатель '%s' уже существует"), src_text().c_str()));
		Converter::s_converter()->error().push_only(pPokaz->src_info(), _T("См. первое определение"));
		Converter::s_converter()->error().push_done();
	}
}

RDOPMDPokaz::~RDOPMDPokaz()
{}

void RDOPMDPokaz::endOfCreation(CREF(LPIPokaz) pPokaz)
{
	m_pPokaz = pPokaz;
	LPITrace trace = m_pPokaz;
	if (trace)
	{
		trace->setTraceID(Converter::s_converter()->getPMD_id());
	}
	Converter::s_converter()->insertPMDPokaz(this);
	/// @todo перенести в конструктор rdoRuntime::RDOPMDPokaz
	Converter::s_converter()->runtime()->addRuntimePokaz(m_pPokaz);
}

// ----------------------------------------------------------------------------
// ---------- RDOPMDWatchPar
// ----------------------------------------------------------------------------
RDOPMDWatchPar::RDOPMDWatchPar(CREF(RDOParserSrcInfo) src_info, rbool trace, CREF(RDOParserSrcInfo) res_src_info, CREF(RDOParserSrcInfo) par_src_info)
	: RDOPMDPokaz(src_info)
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
	rdoRuntime::RDOType::TypeID typeID = pParam->getType()->type()->typeID();
	if (typeID != rdoRuntime::RDOType::t_int && typeID != rdoRuntime::RDOType::t_real)
	{
		Converter::s_converter()->error().push_only(par_src_info, _T("Наблюдать можно только за параметром целого или вещественного типа"));
		Converter::s_converter()->error().push_only(pParam->getType()->src_info(), _T("См. тип параметра"));
		Converter::s_converter()->error().push_done();
	}
	endOfCreation(F(rdoRuntime::RDOPMDWatchPar)::create(Converter::s_converter()->runtime(), src_text(), trace, res_src_info.src_text(), par_src_info.src_text(), pResource->getID(), pResource->getType()->getRTPParamNumber(par_src_info.src_text())));
}

// ----------------------------------------------------------------------------
// ---------- RDOPMDWatchState
// ----------------------------------------------------------------------------
RDOPMDWatchState::RDOPMDWatchState(CREF(RDOParserSrcInfo) src_info, rbool trace, LPRDOFUNLogic pLogic)
	: RDOPMDPokaz(src_info)
{
	endOfCreation(F(rdoRuntime::RDOPMDWatchState)::create(Converter::s_converter()->runtime(), src_text(), trace, pLogic->getCalc()));
}

// ----------------------------------------------------------------------------
// ---------- RDOPMDWatchTemp
// ----------------------------------------------------------------------------
RDOPMDWatchTemp::RDOPMDWatchTemp(CREF(RDOParserSrcInfo) src_info, CREF(RDOParserSrcInfo) res_type_src_info)
	: RDOPMDPokaz(src_info)
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

// ----------------------------------------------------------------------------
// ---------- RDOPMDWatchQuant
// ----------------------------------------------------------------------------
RDOPMDWatchQuant::RDOPMDWatchQuant(CREF(RDOParserSrcInfo) src_info, rbool trace, CREF(RDOParserSrcInfo) res_type_src_info)
	: RDOPMDWatchTemp(src_info, res_type_src_info)
{
	LPRDOFUNGroupLogic pGroupLogic = rdo::Factory<RDOFUNGroupLogic>::create(RDOFUNGroupLogic::fgt_unknow, RDOParserSrcInfo(res_type_src_info.src_text()));
	ASSERT(pGroupLogic);
	endOfCreation(F(rdoRuntime::RDOPMDWatchQuant)::create(Converter::s_converter()->runtime(), src_text(), trace, res_type_src_info.src_text(), pGroupLogic->getResType()->getNumber()));
}

void RDOPMDWatchQuant::setLogic(REF(LPRDOFUNLogic) pLogic)
{
	LPIPokazWatchQuant pQuant = m_pPokaz;
	ASSERT(pQuant);
	pQuant->setLogicCalc(pLogic->getCalc());
	Converter::s_converter()->getFUNGroupStack().pop_back();
}

void RDOPMDWatchQuant::setLogicNoCheck()
{
	LPIPokazWatchQuant pQuant = m_pPokaz;
	ASSERT(pQuant);
	pQuant->setLogicCalc(rdo::Factory<rdoRuntime::RDOCalcConst>::create(1));
	Converter::s_converter()->getFUNGroupStack().pop_back();
}

// ----------------------------------------------------------------------------
// ---------- RDOPMDWatchValue
// ----------------------------------------------------------------------------
RDOPMDWatchValue::RDOPMDWatchValue(CREF(RDOParserSrcInfo) src_info, rbool trace, CREF(RDOParserSrcInfo) res_type_src_info)
	: RDOPMDWatchTemp(src_info, res_type_src_info)
{
	LPRDOFUNGroupLogic pGroupLogic = rdo::Factory<RDOFUNGroupLogic>::create(RDOFUNGroupLogic::fgt_unknow, RDOParserSrcInfo(res_type_src_info.src_text()));
	ASSERT(pGroupLogic);
	endOfCreation(F(rdoRuntime::RDOPMDWatchValue)::create(Converter::s_converter()->runtime(), src_text(), trace, res_type_src_info.src_text(), pGroupLogic->getResType()->getNumber()));
}

void RDOPMDWatchValue::setLogic(REF(LPRDOFUNLogic) pLogic, REF(LPRDOFUNArithm) pArithm)
{
	LPIPokazWatchValue pWatch = m_pPokaz;
	ASSERT(pWatch);
	pWatch->setLogicCalc (pLogic->getCalc()    );
	pWatch->setArithmCalc(pArithm->createCalc());
	Converter::s_converter()->getFUNGroupStack().pop_back();
}

void RDOPMDWatchValue::setLogicNoCheck(REF(LPRDOFUNArithm) pArithm)
{
	LPIPokazWatchValue pWatch = m_pPokaz;
	ASSERT(pWatch);
	pWatch->setLogicCalc (rdo::Factory<rdoRuntime::RDOCalcConst>::create(1));
	pWatch->setArithmCalc(pArithm->createCalc());
	Converter::s_converter()->getFUNGroupStack().pop_back();
}

// ----------------------------------------------------------------------------
// ---------- RDOPMDGetValue
// ----------------------------------------------------------------------------
RDOPMDGetValue::RDOPMDGetValue(CREF(RDOParserSrcInfo) src_info, LPRDOFUNArithm pArithm)
	: RDOPMDPokaz(src_info)
{
	endOfCreation(F(rdoRuntime::RDOPMDGetValue)::create(Converter::s_converter()->runtime(), src_text(), pArithm->createCalc()));
}

CLOSE_RDO_CONVERTER_NAMESPACE
