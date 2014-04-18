/*!
  \copyright (c) RDO-Team, 2011
  \file      rdodpt.cpp
  \authors   Барс Александр
  \authors   Урусов Андрей (rdo@rk9.bmstu.ru)
  \date      
  \brief     
  \indent    4T
*/

// ---------------------------------------------------------------------------- PCH
#include "converter/smr2rdox/pch.h"
// ----------------------------------------------------------------------- INCLUDES
#include <boost/format.hpp>
#include <boost/algorithm/string/join.hpp>
#include <boost/range/algorithm_ext/insert.hpp>
// ----------------------------------------------------------------------- SYNOPSIS
#include "converter/smr2rdox/rdodpt.h"
#include "converter/smr2rdox/rdoopr.h"
#include "converter/smr2rdox/rdoparser.h"
#include "converter/smr2rdox/rdoparser_lexer.h"
#include "converter/smr2rdox/rdorss.h"
#include "converter/smr2rdox/rdo_type.h"
#include "converter/smr2rdox/runtime/rdo_logic_dptfree.h"
#include "converter/smr2rdox/update/update.h"
#include "simulator/runtime/keyboard.h"
#include "simulator/runtime/rdo_rule.h"
#include "simulator/runtime/rdo_operation.h"
#include "simulator/runtime/rdo_keyboard.h"
#include "simulator/runtime/process/rdoprocess.h"
#include "simulator/runtime/rdo_logic_dptsome.h"
#include "simulator/runtime/rdo_logic_dptprior.h"
#include "simulator/runtime/rdodptrtime.h"
#include "simulator/runtime/rdo_dptsearch_activity.h"
#include "simulator/runtime/calc/calc_pattern.h"
// --------------------------------------------------------------------------------

OPEN_RDO_CONVERTER_SMR2RDOX_NAMESPACE

int cnv_dptlex(YYSTYPE* lpval, YYLTYPE* llocp, void* lexer)
{
	LEXER->m_lpval = lpval;
	LEXER->m_lploc = llocp;
	return LEXER->yylex();
}

void cnv_dpterror(const char* /*message*/)
{}

int cnv_proc_rtp_lex(YYSTYPE* lpval, YYLTYPE* llocp, void* lexer)
{
	LEXER->m_lpval = lpval;
	LEXER->m_lploc = llocp;
	return LEXER->yylex();
}

void cnv_proc_rtp_error(const char* /*message*/)
{}

int cnv_proc_rss_lex(YYSTYPE* lpval, YYLTYPE* llocp, void* lexer)
{
	LEXER->m_lpval = lpval;
	LEXER->m_lploc = llocp;
	return LEXER->yylex();
}

void cnv_proc_rss_error(const char* /*message*/)
{}

int cnv_proc_opr_lex(YYSTYPE* lpval, YYLTYPE* llocp, void* lexer)
{
	LEXER->m_lpval = lpval;
	LEXER->m_lploc = llocp;
	return LEXER->yylex();
}

void cnv_proc_opr_error(const char* /*message*/)
{}

// --------------------------------------------------------------------------------
// -------------------- RDODPTActivity
// --------------------------------------------------------------------------------
RDODPTActivity::RDODPTActivity(const RDOParserSrcInfo& src_info, const RDOParserSrcInfo& pattern_src_info)
	: RDOParserSrcInfo(src_info)
	, m_currParam     (0       )
{
	Converter::s_converter()->checkActivityName(src_info);
	m_pPattern = Converter::s_converter()->findPATPattern(pattern_src_info.src_text());
	if (!m_pPattern)
	{
		Converter::s_converter()->error().error(pattern_src_info, rdo::format("Не найден образец: %s", pattern_src_info.src_text().c_str()));
	}
}

RDODPTActivity::~RDODPTActivity()
{}

void RDODPTActivity::addParam(const LPRDOValue& pParam)
{
	ASSERT(pParam);

	if (m_pPattern->m_paramList.size() <= m_currParam)
	{
		if (pParam->src_pos().m_first_line == src_pos().m_first_line)
		{
			if (dynamic_cast<RDOOPROperation*>(this))
			{
				Converter::s_converter()->error().push_only(pParam->src_info(), rdo::format("Слишком много параметров для образца '%s' при описании операции '%s'", m_pPattern->name().c_str(), name().c_str()));
			}
			else
			{
				Converter::s_converter()->error().push_only(pParam->src_info(), rdo::format("Слишком много параметров для образца '%s' при описании активности '%s'", m_pPattern->name().c_str(), name().c_str()));
			}
			Converter::s_converter()->error().push_only(m_pPattern->src_info(), "См. образец");
			Converter::s_converter()->error().push_done();
		}
		else
		{
			if (dynamic_cast<RDOOPROperation*>(this))
			{
				Converter::s_converter()->error().error(pParam->src_info(), "Имя операции должно заканчиваться двоеточием");
			}
			else
			{
				Converter::s_converter()->error().error(pParam->src_info(), "Имя активности должно заканчиваться двоеточием");
			}
		}
	}
	rdo::runtime::RDOValue val;
	LPRDOParam pPatternParam = m_pPattern->m_paramList.at(m_currParam);
	if (pParam->value().getAsString() == "*")
	{
		if (!pPatternParam->getDefault()->defined())
		{
			Converter::s_converter()->error().push_only(pParam->src_pos(), rdo::format("Нет значения по умолчанию для параметра '%s'", pPatternParam->src_text().c_str()));
			Converter::s_converter()->error().push_only(pPatternParam->src_info(), rdo::format("См. параметр '%s', тип '%s'", pPatternParam->src_text().c_str(), pPatternParam->getType()->src_info().src_text().c_str()));
			Converter::s_converter()->error().push_done();
		}
		val = pPatternParam->getDefault()->value();
	}
	else
	{
		val = pPatternParam->getType()->value_cast(pParam)->value();
	}

	m_paramValuesAsString.push_back(pParam->src_text());

	m_currParam++;
}

void RDODPTActivity::endParam(const YYLTYPE& param_pos)
{
	if (m_pPattern->m_paramList.size() > m_currParam)
	{
		LPRDOParam pPatternParam = m_pPattern->m_paramList.at(m_currParam);
		Converter::s_converter()->error().push_only(param_pos, rdo::format("Указаны не все параметра образца '%s':", m_pPattern->src_text().c_str()));
		for (std::size_t i = m_currParam; i < m_pPattern->m_paramList.size(); i++)
		{
			pPatternParam = m_pPattern->m_paramList.at(i);
			Converter::s_converter()->error().push_only(pPatternParam->src_info(), rdo::format("Ожидаемый параметр '%s' имеет тип '%s'", pPatternParam->name().c_str(), pPatternParam->getType()->src_text().c_str()));
		}
		Converter::s_converter()->error().push_done();
	}
	if (m_pPattern->getType() == RDOPATPattern::PT_Keyboard)
	{
		if (!static_cast<RDODPTActivityHotKey*>(this)->hasHotKey())
		{
			if (dynamic_cast<RDOOPROperation*>(this))
			{
				Converter::s_converter()->error().push_only(param_pos, "Для клавиатурной операции должна быть указана клавиша");
			}
			else
			{
				Converter::s_converter()->error().push_only(param_pos, "Для активности должна быть указана клавиша");
			}
			Converter::s_converter()->error().push_only(m_pPattern->src_info(), "См. образец");
			Converter::s_converter()->error().push_done();
		}
	}

	planningInsertIntoSMR();
}

void RDODPTActivity::planningInsertIntoSMR() const
{
	if (pattern()->getType() != RDOPATPattern::PT_IE)
		return;

	const std::string planning_time = boost::str(boost::format("time_now + %s")
		% pattern()->time->calc()->srcInfo().src_text());

	std::vector<std::string> planning_params;
	planning_params.push_back(planning_time);
	boost::range::insert(planning_params, planning_params.end(), m_paramValuesAsString);

	const std::string planning = boost::str(boost::format("%s.planning(%s)\r\n")
		% pattern()->name()
		% boost::algorithm::join(planning_params, ", "));

	LPDocUpdate pPlanningInsertIntoSMR = rdo::Factory<UpdateInsert>::create(
		IDocUpdate::Position::POSITION_END,
		planning,
		IDocument::SMR);
	Converter::s_converter()->insertDocUpdate(pPlanningInsertIntoSMR);
}

bool RDODPTActivity::setPrior(LPRDOFUNArithm& /*pPrior*/)
{
	return false;
}

// --------------------------------------------------------------------------------
// -------------------- RDODPTActivityHotKey
// --------------------------------------------------------------------------------
RDODPTActivityHotKey::RDODPTActivityHotKey(LPIBaseOperationContainer pDPT, const RDOParserSrcInfo& src_info, const RDOParserSrcInfo& pattern_src_info)
	: RDODPTActivity(src_info, pattern_src_info)
{
	switch (pattern()->getType())
	{
	case RDOPATPattern::PT_IE       :
	case RDOPATPattern::PT_Rule     :
	case RDOPATPattern::PT_Operation:
	case RDOPATPattern::PT_Keyboard :
		break;

	default:
		Converter::s_converter()->error().push_only(this->src_info(), "Неизвестный тип образца");
		Converter::s_converter()->error().push_only(pattern()->src_info(), "См. образец");
		Converter::s_converter()->error().push_done();
	}
}

IKeyboard::AddHotKeyResult RDODPTActivityHotKey::addHotKey(const std::string& hotKey)
{
	rdo::runtime::RDOHotKey::KeyCode scanCode = Converter::s_converter()->runtime()->hotkey().toolkit().codeFromString(hotKey);
	if (scanCode == rdo::runtime::RDOHotKey::UNDEFINED_KEY)
	{
		return IKeyboard::addhk_notfound;
	}
	if (std::find(m_scanCodeList.begin(), m_scanCodeList.end(), scanCode) != m_scanCodeList.end())
	{
		return IKeyboard::addhk_already;
	}
	m_scanCodeList.push_back(scanCode);
	return IKeyboard::addhk_ok;
}

bool RDODPTActivityHotKey::hasHotKey() const
{
	for (const auto key: m_scanCodeList)
	{
		if (key != VK_SHIFT && key != VK_CONTROL)
			return true;
	}
	return false;
}

void RDODPTActivityHotKey::addHotKey(const std::string& hotKey, const YYLTYPE& hotkey_pos)
{
	if (pattern()->getType() != RDOPATPattern::PT_Keyboard)
	{
		Converter::s_converter()->error().push_only(hotkey_pos, "Горячие клавиши используются только в клавиатурных операциях");
		Converter::s_converter()->error().push_only(pattern()->src_info(), "См. образец");
		Converter::s_converter()->error().push_done();
	}

	switch (addHotKey(hotKey))
	{
	case rdo::runtime::RDOKeyboard::addhk_ok:
		break;

	case rdo::runtime::RDOKeyboard::addhk_already:
		if (dynamic_cast<RDOOPROperation*>(this))
		{
			Converter::s_converter()->error().error(hotkey_pos, rdo::format("Для операции '%s' клавиша уже назначена", src_text().c_str()));
		}
		else
		{
			Converter::s_converter()->error().error(hotkey_pos, rdo::format("Для активности '%s' клавиша уже назначена", src_text().c_str()));
		}
		break;

	case rdo::runtime::RDOKeyboard::addhk_notfound:
		Converter::s_converter()->error().error(hotkey_pos, rdo::format("Неизвестная клавиша: %s", hotKey.c_str()));
		break;

	case rdo::runtime::RDOKeyboard::addhk_dont:
		Converter::s_converter()->error().push_only(src_info(), rdo::format("Операция '%s' не является клавиатурной", src_text().c_str()));
		Converter::s_converter()->error().push_only(pattern()->src_info(), "См. образец");
		Converter::s_converter()->error().push_done();
		break;

	default:
		Converter::s_converter()->error().error(src_info(), "Внутренная ошибка: RDODPTActivityHotKey::addHotKey");
	}
}

// --------------------------------------------------------------------------------
// -------------------- RDODPTFreeActivity
// --------------------------------------------------------------------------------
RDODPTFreeActivity::RDODPTFreeActivity(LPIBaseOperationContainer pDPT, const RDOParserSrcInfo& src_info, const RDOParserSrcInfo& pattern_src_info)
	: RDODPTActivityHotKey(pDPT, src_info, pattern_src_info)
{
	Converter::s_converter()->insertDPTFreeActivity(this);
}

// --------------------------------------------------------------------------------
// -------------------- RDODPTFree
// --------------------------------------------------------------------------------
RDODPTFree::RDODPTFree(const RDOParserSrcInfo& src_info)
	: RDOLogicActivity<rdo::runtime::RDODPTFree, RDODPTFreeActivity>(src_info)
{
	Converter::s_converter()->checkDPTName(this->src_info());
	m_pRuntimeLogic = RF(rdo::runtime::RDODPTFree)::create(Converter::s_converter()->runtime());
	ASSERT(m_pRuntimeLogic);
	m_pRuntimeLogic->init(Converter::s_converter()->runtime());
	Converter::s_converter()->insertDPTFree(this);
}

// --------------------------------------------------------------------------------
// -------------------- RDODPTSomeActivity
// --------------------------------------------------------------------------------
RDODPTSomeActivity::RDODPTSomeActivity(LPIBaseOperationContainer pDPT, const RDOParserSrcInfo& src_info, const RDOParserSrcInfo& pattern_src_info)
	: RDODPTActivityHotKey(pDPT, src_info, pattern_src_info)
{}

// --------------------------------------------------------------------------------
// -------------------- RDODPTPriorActivity
// --------------------------------------------------------------------------------
RDODPTPriorActivity::RDODPTPriorActivity(LPIBaseOperationContainer pDPT, const RDOParserSrcInfo& src_info, const RDOParserSrcInfo& pattern_src_info)
	: RDODPTActivityHotKey(pDPT, src_info, pattern_src_info)
{}

// --------------------------------------------------------------------------------
// -------------------- RDODPTSome
// --------------------------------------------------------------------------------
RDODPTSome::RDODPTSome(const RDOParserSrcInfo& src_info, LPILogic pParent)
	: RDOLogicActivity<rdo::runtime::RDODPTSome, RDODPTSomeActivity>(src_info)
{
	Converter::s_converter()->checkDPTName(this->src_info());
	m_pRuntimeLogic = RF(rdo::runtime::RDODPTSome)::create(Converter::s_converter()->runtime(), pParent);
	ASSERT(m_pRuntimeLogic);
	m_pRuntimeLogic->init(Converter::s_converter()->runtime());
	Converter::s_converter()->insertDPTSome(this);
}

void RDODPTSome::end()
{
	if (getConditon())
	{
		m_pRuntimeLogic->setCondition(getConditon()->getCalc());
	}
}

// --------------------------------------------------------------------------------
// -------------------- RDODPTPrior
// --------------------------------------------------------------------------------
RDODPTPrior::RDODPTPrior(const RDOParserSrcInfo& src_info, LPILogic pParent)
	: RDOLogicActivity<rdo::runtime::RDODPTPrior, RDODPTPriorActivity>(src_info)
{
	Converter::s_converter()->checkDPTName(this->src_info());
	m_pRuntimeLogic = RF(rdo::runtime::RDODPTPrior)::create(Converter::s_converter()->runtime(), pParent);
	ASSERT(m_pRuntimeLogic);
	m_pRuntimeLogic->init(Converter::s_converter()->runtime());
	Converter::s_converter()->insertDPTPrior(this);
}

void RDODPTPrior::end()
{
	if (getConditon())
	{
		m_pRuntimeLogic->setCondition(getConditon()->getCalc());
	}
}

// --------------------------------------------------------------------------------
// -------------------- RDODPTSearchActivity
// --------------------------------------------------------------------------------
RDODPTSearchActivity::RDODPTSearchActivity(LPIBaseOperationContainer pDPT, const RDOParserSrcInfo& src_info, const RDOParserSrcInfo& pattern_src_info)
	: RDODPTActivity(src_info, pattern_src_info   )
	, m_value       (IDPTSearchActivity::vt_before)
{
	if (pattern()->getType() != RDOPATPattern::PT_Rule)
	{
		Converter::s_converter()->error().push_only(this->src_info(), "Только продукционные правила могут быть использованы в точке принятия решений типа search");
		Converter::s_converter()->error().push_only(pattern()->src_info(), "См. образец");
		Converter::s_converter()->error().push_done();
	}
	for (RDOPATPattern::RelResList::const_iterator it = pattern()->rel_res_begin(); it != pattern()->rel_res_end(); ++it)
	{
		if (((*it)->m_statusBegin == rdo::runtime::RDOResource::CS_Create) || ((*it)->m_statusBegin == rdo::runtime::RDOResource::CS_Erase))
		{
			Converter::s_converter()->error().push_only(this->src_info(), rdo::format("В продукционном правиле '%s' нельзя создавать или удалять ресурсы, т.к. оно используется в точке типа search", src_text().c_str()));
			Converter::s_converter()->error().push_only(pattern()->src_info(), "См. образец");
			Converter::s_converter()->error().push_only((*it)->src_info(), "См. релевантный ресурс");
			Converter::s_converter()->error().push_done();
		}
	}
}

void RDODPTSearchActivity::setValue(IDPTSearchActivity::ValueTime value, const LPRDOFUNArithm& pRuleCost, const YYLTYPE& param_pos)
{
	endParam(param_pos);
	m_value     = value;
	m_pRuleCost = pRuleCost;
}

// --------------------------------------------------------------------------------
// -------------------- RDODPTSearch
// --------------------------------------------------------------------------------
RDODPTSearch::RDODPTSearch(const RDOParserSrcInfo& src_info, rdo::runtime::RDODPTSearchTrace::DPT_TraceFlag trace, LPILogic pParent)
	: RDOLogicActivity<rdo::runtime::RDODPTSearchRuntime, RDODPTSearchActivity>(src_info)
	, m_pParent(pParent)
	, m_closed (false  )
	, m_trace  (trace  )
{
	Converter::s_converter()->checkDPTName   (this->src_info());
	Converter::s_converter()->insertDPTSearch(this);
}

void RDODPTSearch::end()
{
	rdo::runtime::LPRDOCalc pCalcCondition = m_pConditon     ? m_pConditon->getCalc()     : rdo::Factory<rdo::runtime::RDOCalcConst>::create(1).object_parent_cast<rdo::runtime::RDOCalc>();
	rdo::runtime::LPRDOCalc pCalcTerminate = m_pTermConditon ? m_pTermConditon->getCalc() : rdo::Factory<rdo::runtime::RDOCalcConst>::create(1).object_parent_cast<rdo::runtime::RDOCalc>();

	m_pRuntimeLogic = RF(rdo::runtime::RDODPTSearchRuntime)::create(Converter::s_converter()->runtime(),
		m_pParent,
		pCalcCondition,
		pCalcTerminate,
		m_pEvalBy->createCalc(),
		m_compTops,
		m_trace);
	ASSERT(m_pRuntimeLogic);
	m_pRuntimeLogic->init(Converter::s_converter()->runtime());

	int size = getActivities().size();
	for (int i = 0; i < size; i++)
	{
		LPRDODPTSearchActivity pSearchActivity = getActivities().at(i);
		ASSERT(pSearchActivity);
		LPIDPTSearchLogic pSearchLogic = m_pRuntimeLogic;
		ASSERT(pSearchLogic);
	}
	m_closed = true;
}

// --------------------------------------------------------------------------------
// -------------------- RDOPROCProcess
// --------------------------------------------------------------------------------
std::string RDOPROCProcess::s_name_prefix = "";
std::string RDOPROCProcess::s_name_sufix  = "s";

RDOPROCProcess::RDOPROCProcess(const RDOParserSrcInfo& info)
	: RDOParserSrcInfo(info )
	, m_closed        (false)
{
	Converter::s_converter()->insertPROCProcess(this);
	m_pRuntime = RF(rdo::runtime::RDOPROCProcess)::create(info.src_text(), Converter::s_converter()->runtime());
	ASSERT(m_pRuntime);
	m_pRuntime.query_cast<ILogic>()->init(Converter::s_converter()->runtime());
}

bool RDOPROCProcess::setPrior(LPRDOFUNArithm& pPrior)
{
	LPILogic pRuntimeLogic = getRunTime();
	LPIPriority pPriority = pRuntimeLogic.query_cast<IPriority>();
	if (pPriority)
	{
		return pPriority->setPrior(pPrior->createCalc());
	}
	return false;
}

void RDOPROCProcess::end()
{
	m_closed = true;
	if (getConditon())
	{
		getRunTime()->setCondition(getConditon()->getCalc());
	}
}

void RDOPROCProcess::insertBlock(const LPRDOPROCOperator& pBlock)
{
	ASSERT(pBlock);
	m_blockList.push_back(pBlock);
}

void RDOPROCProcess::insertChild(LPRDOPROCProcess& pProcess)
{
	ASSERT(pProcess);
	m_childProcessList.push_back(pProcess);
	pProcess->m_pParentProcess = this;
}

// --------------------------------------------------------------------------------
// -------------------- RDOPROCOperator
// --------------------------------------------------------------------------------
RDOPROCOperator::RDOPROCOperator(const LPRDOPROCProcess& pProcess, const std::string& name)
	: m_name    (name    )
	, m_pProcess(pProcess)
{
	ASSERT(pProcess);
	m_pProcess->insertBlock(this);
}

RDOPROCOperator::~RDOPROCOperator()
{}

// --------------------------------------------------------------------------------
// -------------------- RDOPROCGenerate
// --------------------------------------------------------------------------------
RDOPROCGenerate::RDOPROCGenerate(const LPRDOPROCProcess& pProcess, const std::string& name, const rdo::runtime::LPRDOCalc& /*pTimeCalc*/)
	: RDOPROCOperator(pProcess, name)
{}

// --------------------------------------------------------------------------------
// -------------------- RDOPROCBlockForQueue
// --------------------------------------------------------------------------------
RDOPROCBlockForQueue::RDOPROCBlockForQueue(const LPRDOPROCProcess& pProcess, const std::string& name)
	: RDOPROCOperator(pProcess, name)
{}

// --------------------------------------------------------------------------------
// -------------------- RDOPROCQueue
// --------------------------------------------------------------------------------
RDOPROCQueue::RDOPROCQueue(const LPRDOPROCProcess& pProcess, const std::string& name)
	: RDOPROCBlockForQueue(pProcess, name)
{}

void RDOPROCQueue::createRuntime()
{
   NEVER_REACH_HERE;
}

void RDOPROCQueue::setResource(const std::string& name)
{
	ASSERT(!name.empty()         );
	ASSERT(m_resourceName.empty());

	m_resourceName = name;
}

// --------------------------------------------------------------------------------
// -------------------- RDOPROCDepart
// --------------------------------------------------------------------------------
RDOPROCDepart::RDOPROCDepart(const LPRDOPROCProcess& pProcess, const std::string& name)
	: RDOPROCBlockForQueue(pProcess, name)
{}

void RDOPROCDepart::createRuntime()
{
   NEVER_REACH_HERE;
}

void RDOPROCDepart::setResource(const std::string& name)
{
	ASSERT(!name.empty()         );
	ASSERT(m_resourceName.empty());

	m_resourceName = name;
}

// --------------------------------------------------------------------------------
// -------------------- RDOPROCBlockForSeize
// --------------------------------------------------------------------------------
RDOPROCBlockForSeize::RDOPROCBlockForSeize(const LPRDOPROCProcess& pProcess, const std::string& name)
	: RDOPROCOperator(pProcess, name)
{}

// --------------------------------------------------------------------------------
// -------------------- RDOPROCSeize
// --------------------------------------------------------------------------------
RDOPROCSeize::RDOPROCSeize(const LPRDOPROCProcess& pProcess, const std::string& name)
	: RDOPROCBlockForSeize(pProcess, name)
{}

void RDOPROCSeize::createRuntime()
{
   NEVER_REACH_HERE;
}

void RDOPROCSeize::addResource(const std::string& name)
{
	ASSERT(!name.empty());
	m_resourceList.push_back(name);
}

// --------------------------------------------------------------------------------
// -------------------- RDOPROCRelease
// --------------------------------------------------------------------------------
RDOPROCRelease::RDOPROCRelease(const LPRDOPROCProcess& pProcess, const std::string& name)
	: RDOPROCBlockForSeize(pProcess, name)
{}

void RDOPROCRelease::createRuntime()
{
   NEVER_REACH_HERE;
}

void RDOPROCRelease::addResource(const std::string& name)
{
	ASSERT(!name.empty());
	m_resourceList.push_back(name);
}

// --------------------------------------------------------------------------------
// -------------------- RDOPROCAdvance
// --------------------------------------------------------------------------------
RDOPROCAdvance::RDOPROCAdvance(const LPRDOPROCProcess& pProcess, const std::string& name, const rdo::runtime::LPRDOCalc& pTimeCalc)
	: RDOPROCOperator(pProcess, name)
{
	m_pRuntime = RF(rdo::runtime::RDOPROCAdvance)::create(Converter::s_converter()->getLastPROCProcess()->getRunTime(), pTimeCalc);
	ASSERT(m_pRuntime);
}

// --------------------------------------------------------------------------------
// -------------------- RDOPROCTerminate
// --------------------------------------------------------------------------------
RDOPROCTerminate::RDOPROCTerminate(const LPRDOPROCProcess& pProcess, const std::string& name, const rdo::runtime::LPRDOCalc& pCalc)
	: RDOPROCOperator(pProcess, name)
{
	m_pRuntime = RF(rdo::runtime::RDOPROCTerminate)::create(Converter::s_converter()->getLastPROCProcess()->getRunTime(), pCalc);
	ASSERT(m_pRuntime);
}

// --------------------------------------------------------------------------------
// -------------------- RDOPROCAssign
// --------------------------------------------------------------------------------
RDOPROCAssign::RDOPROCAssign(const LPRDOPROCProcess& pProcess, const std::string& name, const rdo::runtime::LPRDOCalc& pValue)
	: RDOPROCOperator(pProcess, name)
{
	m_pRuntime = RF(rdo::runtime::RDOPROCAssign)::create(Converter::s_converter()->getLastPROCProcess()->getRunTime(), pValue);
	ASSERT(m_pRuntime);
}

CLOSE_RDO_CONVERTER_SMR2RDOX_NAMESPACE
