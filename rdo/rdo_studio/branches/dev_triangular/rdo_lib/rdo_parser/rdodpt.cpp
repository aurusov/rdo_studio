/*
 * copyright: (c) RDO-Team, 2010
 * filename : rdodpt.cpp
 * author   : Александ Барс, Урусов Андрей
 * date     : 
 * bref     : 
 * indent   : 4T
 */

// ====================================================================== PCH
#include "rdo_lib/rdo_parser/pch.h"
// ====================================================================== INCLUDES
// ====================================================================== SYNOPSIS
#include "rdo_lib/rdo_parser/rdodpt.h"
#include "rdo_lib/rdo_parser/rdoparser.h"
#include "rdo_lib/rdo_parser/rdoparser_lexer.h"
#include "rdo_lib/rdo_parser/rdorss.h"
#include "rdo_lib/rdo_parser/type/type.h"
#include "rdo_lib/rdo_runtime/rdo_rule.h"
#include "rdo_lib/rdo_runtime/rdo_operation.h"
#include "rdo_lib/rdo_runtime/rdo_keyboard.h"
#include "rdo_lib/rdo_runtime/rdoprocess.h"
#include "rdo_lib/rdo_runtime/rdo_logic_dptfree.h"
#include "rdo_lib/rdo_runtime/rdo_logic_dptsome.h"
#include "rdo_lib/rdo_runtime/rdo_logic_dptprior.h"
#include "rdo_lib/rdo_runtime/rdodptrtime.h"
#include "rdo_lib/rdo_runtime/rdo_dptsearch_activity.h"

#include "rdo_lib/rdo_mbuilder/rdo_resources.h"
// ===============================================================================

OPEN_RDO_PARSER_NAMESPACE

int dptlex(PTR(YYSTYPE) lpval, PTR(YYLTYPE) llocp, PTR(void) lexer)
{
	LEXER->m_lpval = lpval;
	LEXER->m_lploc = llocp;
	return LEXER->yylex();
}

void dpterror(PTR(char) mes)
{}

int proc_rtp_lex(PTR(YYSTYPE) lpval, PTR(YYLTYPE) llocp, PTR(void) lexer)
{
	LEXER->m_lpval = lpval;
	LEXER->m_lploc = llocp;
	return LEXER->yylex();
}

void proc_rtp_error(PTR(char) mes)
{}

int proc_rss_lex(PTR(YYSTYPE) lpval, PTR(YYLTYPE) llocp, PTR(void) lexer)
{
	LEXER->m_lpval = lpval;
	LEXER->m_lploc = llocp;
	return LEXER->yylex();
}

void proc_rss_error(PTR(char) mes)
{}

int proc_opr_lex(PTR(YYSTYPE) lpval, PTR(YYLTYPE) llocp, PTR(void) lexer)
{
	LEXER->m_lpval = lpval;
	LEXER->m_lploc = llocp;
	return LEXER->yylex();
}

void proc_opr_error(PTR(char) mes)
{}

// ----------------------------------------------------------------------------
// ---------- RDODPTActivity
// ----------------------------------------------------------------------------
RDODPTActivity::RDODPTActivity(CREF(RDOParserSrcInfo) src_info, CREF(RDOParserSrcInfo) pattern_src_info)
	: RDOParserSrcInfo(src_info)
	, m_currParam     (0       )
{
	RDOParser::s_parser()->checkActivityName(src_info);
	m_pPattern = RDOParser::s_parser()->findPATPattern(pattern_src_info.src_text());
	if (!m_pPattern)
	{
		RDOParser::s_parser()->error().error(pattern_src_info, rdo::format(_T("Не найден образец: %s"), pattern_src_info.src_text().c_str()));
	}
	RDOParser::s_parser()->contextStack()->push(this);
}

RDODPTActivity::~RDODPTActivity()
{}

LPContext RDODPTActivity::onFindContext(CREF(RDOValue) value) const
{
	LPContext pContext = m_pPattern->onFindContext(value);
	if (pContext)
	{
		return pContext;
	}
	return NULL;
}

void RDODPTActivity::addParam(CREF(RDOValue) param)
{
	if (m_pPattern->m_paramList.size() <= m_currParam)
	{
		if (param.src_pos().m_first_line == src_pos().m_first_line)
		{
			RDOParser::s_parser()->error().push_only(param, rdo::format(_T("Слишком много параметров для образца '%s' при описании активности '%s'"), m_pPattern->name().c_str(), name().c_str()));
			RDOParser::s_parser()->error().push_only(m_pPattern->src_info(), _T("См. образец"));
			RDOParser::s_parser()->error().push_done();
		}
		else
		{
			RDOParser::s_parser()->error().error(param, _T("Имя активности должно заканчиваться двоеточием"));
		}
	}
	rdoRuntime::RDOValue val;
	LPRDOParam pPatternParam = m_pPattern->m_paramList.at(m_currParam);
	if (param->getAsString() == _T("*"))
	{
		if (!pPatternParam->getDefault().defined())
		{
			RDOParser::s_parser()->error().push_only(param, rdo::format(_T("Нет значения по-умолчанию для параметра '%s'"), pPatternParam->src_text().c_str()));
			RDOParser::s_parser()->error().push_only(pPatternParam->src_info(), rdo::format(_T("См. параметр '%s', тип '%s'"), pPatternParam->src_text().c_str(), pPatternParam->getTypeInfo()->src_info().src_text().c_str()));
			RDOParser::s_parser()->error().push_done();
		}
		val = pPatternParam->getDefault().value();
	}
	else
	{
		val = pPatternParam->getTypeInfo()->value_cast(param).value();
	}
	rdoRuntime::LPRDOCalc pCalc = rdo::Factory<rdoRuntime::RDOSetPatternParamCalc>::create(m_currParam, val);
	ASSERT(pCalc);
	pCalc->setSrcInfo(RDOParserSrcInfo(param.getPosAsYY(), rdo::format(_T("Параметр образца %s.%s = %s"), m_pPattern->name().c_str(), pPatternParam->name().c_str(), param->getAsString().c_str())));
	m_pActivity->addParamCalc(pCalc);
	m_currParam++;
}

void RDODPTActivity::endParam(CREF(YYLTYPE) param_pos)
{
	if (m_pPattern->m_paramList.size() > m_currParam)
	{
		LPRDOParam pPatternParam = m_pPattern->m_paramList.at(m_currParam);
		RDOParser::s_parser()->error().push_only(param_pos, rdo::format(_T("Указаны не все параметра образца '%s':"), m_pPattern->src_text().c_str()));
		for (ruint i = m_currParam; i < m_pPattern->m_paramList.size(); i++)
		{
			pPatternParam = m_pPattern->m_paramList.at(i);
			RDOParser::s_parser()->error().push_only(pPatternParam->src_info(), rdo::format(_T("Ожидаемый параметр '%s' имеет тип '%s'"), pPatternParam->name().c_str(), pPatternParam->getTypeInfo()->src_info().src_text().c_str()));
		}
		RDOParser::s_parser()->error().push_done();
	}
	if (m_pPattern->getType() == RDOPATPattern::PT_Keyboard)
	{
		LPIKeyboard pKeyboard = m_pActivity;
		ASSERT(pKeyboard);
		if (!pKeyboard->hasHotKey())
		{
			RDOParser::s_parser()->error().push_only(param_pos, _T("Для активности должна быть указана клавиша"));
			RDOParser::s_parser()->error().push_only(m_pPattern->src_info(), _T("См. образец"));
			RDOParser::s_parser()->error().push_done();
		}
	}
	RDOParser::s_parser()->contextStack()->pop();
}

rbool RDODPTActivity::setPrior(REF(LPRDOFUNArithm) pPrior)
{
	LPIPriority pPriorActivity = m_pActivity;
	if (pPriorActivity)
	{
		return pPriorActivity->setPrior(pPrior->createCalc());
	}
	return false;
}

// ----------------------------------------------------------------------------
// ---------- RDODPTActivityHotKey
// ----------------------------------------------------------------------------
RDODPTActivityHotKey::RDODPTActivityHotKey(LPIBaseOperationContainer pDPT, CREF(RDOParserSrcInfo) src_info, CREF(RDOParserSrcInfo) pattern_src_info)
	: RDODPTActivity(src_info, pattern_src_info)
{
	switch (pattern()->getType())
	{
	case RDOPATPattern::PT_Rule:
		m_pActivity = pattern()->getPatRuntime<rdoRuntime::RDOPatternRule>()->createActivity(pDPT, RDOParser::s_parser()->runtime(), name());
		break;

	case RDOPATPattern::PT_Operation:
		m_pActivity = pattern()->getPatRuntime<rdoRuntime::RDOPatternOperation>()->createActivity(pDPT, RDOParser::s_parser()->runtime(), name());
		break;

	case RDOPATPattern::PT_Keyboard:
		m_pActivity = pattern()->getPatRuntime<rdoRuntime::RDOPatternKeyboard>()->createActivity(pDPT, RDOParser::s_parser()->runtime(), name());
		break;

	default:
		RDOParser::s_parser()->error().push_only(this->src_info(), _T("Неизвестный тип образца"));
		RDOParser::s_parser()->error().push_only(pattern()->src_info(), _T("См. образец"));
		RDOParser::s_parser()->error().push_done();
	}
}

void RDODPTActivityHotKey::addHotKey(CREF(tstring) hotKey, CREF(YYLTYPE) hotkey_pos)
{
	if (pattern()->getType() != RDOPATPattern::PT_Keyboard)
	{
		RDOParser::s_parser()->error().push_only(hotkey_pos, _T("Горячие клавиши используются только в клавиатурных операциях"));
		RDOParser::s_parser()->error().push_only(pattern()->src_info(), _T("См. образец"));
		RDOParser::s_parser()->error().push_done();
	}
	LPIKeyboard pKeyboard = m_pActivity;
	ASSERT(pKeyboard);
	switch (pKeyboard->addHotKey(RDOParser::s_parser()->runtime(), hotKey))
	{
	case rdoRuntime::RDOKeyboard::addhk_ok:
		break;

	case rdoRuntime::RDOKeyboard::addhk_already:
		RDOParser::s_parser()->error().error(hotkey_pos, rdo::format(_T("Для активности '%s' клавиша уже назначена"), src_text().c_str()));
		break;

	case rdoRuntime::RDOKeyboard::addhk_notfound:
		RDOParser::s_parser()->error().error(hotkey_pos, rdo::format(_T("Неизвестная клавиша: %s"), hotKey.c_str()));
		break;

	case rdoRuntime::RDOKeyboard::addhk_dont:
		RDOParser::s_parser()->error().push_only(src_info(), rdo::format(_T("Операция '%s' не является клавиатурной"), src_text().c_str()));
		RDOParser::s_parser()->error().push_only(pattern()->src_info(), _T("См. образец"));
		RDOParser::s_parser()->error().push_done();
		break;

	default:
		RDOParser::s_parser()->error().error(src_info(), _T("Внутренная ошибка: RDOOPROperation::addHotKey"));
	}
}

// ----------------------------------------------------------------------------
// ---------- RDODPTFreeActivity
// ----------------------------------------------------------------------------
RDODPTFreeActivity::RDODPTFreeActivity(LPIBaseOperationContainer pDPT, CREF(RDOParserSrcInfo) src_info, CREF(RDOParserSrcInfo) pattern_src_info)
	: RDODPTActivityHotKey(pDPT, src_info, pattern_src_info)
{
	RDOParser::s_parser()->insertDPTFreeActivity(this);
}

// ----------------------------------------------------------------------------
// ---------- RDODPTFree
// ----------------------------------------------------------------------------
RDODPTFree::RDODPTFree(CREF(RDOParserSrcInfo) src_info)
	: RDOLogicActivity<rdoRuntime::RDODPTFree, RDODPTFreeActivity>(src_info)
{
	RDOParser::s_parser()->checkDPTName(this->src_info());
	m_pRuntimeLogic = F(rdoRuntime::RDODPTFree)::create(RDOParser::s_parser()->runtime());
	ASSERT(m_pRuntimeLogic);
	m_pRuntimeLogic->init(RDOParser::s_parser()->runtime());
	RDOParser::s_parser()->insertDPTFree(this);
	RDOParser::s_parser()->contextStack()->push(this);
}

void RDODPTFree::end()
{
	RDOParser::s_parser()->contextStack()->pop();
}

LPContext RDODPTFree::onFindContext(CREF(RDOValue) value) const
{
	//! Поиск не нужен, добавлен для порядка, чтобы контекст активности был на стеке после контекста точки
	return NULL;
}

// ----------------------------------------------------------------------------
// ---------- RDODPTSomeActivity
// ----------------------------------------------------------------------------
RDODPTSomeActivity::RDODPTSomeActivity(LPIBaseOperationContainer pDPT, CREF(RDOParserSrcInfo) src_info, CREF(RDOParserSrcInfo) pattern_src_info)
	: RDODPTActivityHotKey(pDPT, src_info, pattern_src_info)
{}

// ----------------------------------------------------------------------------
// ---------- RDODPTPriorActivity
// ----------------------------------------------------------------------------
RDODPTPriorActivity::RDODPTPriorActivity(LPIBaseOperationContainer pDPT, CREF(RDOParserSrcInfo) src_info, CREF(RDOParserSrcInfo) pattern_src_info)
	: RDODPTActivityHotKey(pDPT, src_info, pattern_src_info)
{}

// ----------------------------------------------------------------------------
// ---------- RDODPTSome
// ----------------------------------------------------------------------------
RDODPTSome::RDODPTSome(CREF(RDOParserSrcInfo) src_info, LPILogic pParent)
	: RDOLogicActivity<rdoRuntime::RDODPTSome, RDODPTSomeActivity>(src_info)
{
	RDOParser::s_parser()->checkDPTName(this->src_info());
	m_pRuntimeLogic = F(rdoRuntime::RDODPTSome)::create(RDOParser::s_parser()->runtime(), pParent);
	ASSERT(m_pRuntimeLogic);
	m_pRuntimeLogic->init(RDOParser::s_parser()->runtime());
	RDOParser::s_parser()->insertDPTSome(this);
	RDOParser::s_parser()->contextStack()->push(this);
}

LPContext RDODPTSome::onFindContext(CREF(RDOValue) value) const
{
	//! Поиск не нужен, добавлен для порядка, чтобы контекст активности был на стеке после контекста точки
	return NULL;
}

void RDODPTSome::end()
{
	if (getConditon())
	{
		m_pRuntimeLogic->setCondition(getConditon()->getCalc());
	}
	RDOParser::s_parser()->contextStack()->pop();
}

// ----------------------------------------------------------------------------
// ---------- RDODPTPrior
// ----------------------------------------------------------------------------
RDODPTPrior::RDODPTPrior(CREF(RDOParserSrcInfo) src_info, LPILogic pParent)
	: RDOLogicActivity<rdoRuntime::RDODPTPrior, RDODPTPriorActivity>(src_info)
{
	RDOParser::s_parser()->checkDPTName(this->src_info());
	m_pRuntimeLogic = F(rdoRuntime::RDODPTPrior)::create(RDOParser::s_parser()->runtime(), pParent);
	ASSERT(m_pRuntimeLogic);
	m_pRuntimeLogic->init(RDOParser::s_parser()->runtime());
	RDOParser::s_parser()->insertDPTPrior(this);
	RDOParser::s_parser()->contextStack()->push(this);
}

LPContext RDODPTPrior::onFindContext(CREF(RDOValue) value) const
{
	//! Поиск не нужен, добавлен для порядка, чтобы контекст активности был на стеке после контекста точки
	return NULL;
}

void RDODPTPrior::end()
{
	if (getConditon())
	{
		m_pRuntimeLogic->setCondition(getConditon()->getCalc());
	}
	RDOParser::s_parser()->contextStack()->pop();
}

// ----------------------------------------------------------------------------
// ---------- RDODPTSearchActivity
// ----------------------------------------------------------------------------
RDODPTSearchActivity::RDODPTSearchActivity(LPIBaseOperationContainer pDPT, CREF(RDOParserSrcInfo) src_info, CREF(RDOParserSrcInfo) pattern_src_info)
	: RDODPTActivity(src_info, pattern_src_info   )
	, m_value       (IDPTSearchActivity::vt_before)
{
	if (pattern()->getType() != RDOPATPattern::PT_Rule)
	{
		RDOParser::s_parser()->error().push_only(this->src_info(), _T("Только продукционные правила могут быть использованы в точке принятия решений типа search"));
		RDOParser::s_parser()->error().push_only(pattern()->src_info(), _T("См. образец"));
		RDOParser::s_parser()->error().push_done();
	}
	for (RDOPATPattern::RelResList::const_iterator it = pattern()->rel_res_begin(); it != pattern()->rel_res_end(); ++it)
	{
		if (((*it)->m_statusBegin == rdoRuntime::RDOResource::CS_Create) || ((*it)->m_statusBegin == rdoRuntime::RDOResource::CS_Erase))
		{
			RDOParser::s_parser()->error().push_only(this->src_info(), rdo::format(_T("В продукционном правиле '%s' нельзя создавать или удалять ресурсы, т.к. оно используется в точке типа search"), src_text().c_str()));
			RDOParser::s_parser()->error().push_only(pattern()->src_info(), _T("См. образец"));
			RDOParser::s_parser()->error().push_only((*it)->src_info(), _T("См. релевантный ресурс"));
			RDOParser::s_parser()->error().push_done();
		}
	}
	m_pActivity = F(rdoRuntime::RDORule)::create(RDOParser::s_parser()->runtime(), pattern()->getPatRuntime<rdoRuntime::RDOPatternRule>(), true, name());
	ASSERT(m_pActivity);
}

void RDODPTSearchActivity::setValue(IDPTSearchActivity::ValueTime value, CREF(LPRDOFUNArithm) pRuleCost)
{
	m_value     = value;
	m_pRuleCost = pRuleCost;
}

// ----------------------------------------------------------------------------
// ---------- RDODPTSearch
// ----------------------------------------------------------------------------
RDODPTSearch::RDODPTSearch(CREF(RDOParserSrcInfo) src_info, rdoRuntime::RDODPTSearchTrace::DPT_TraceFlag trace, LPILogic pParent)
	: RDOLogicActivity<rdoRuntime::RDODPTSearchRuntime, RDODPTSearchActivity>(src_info)
	, m_trace  (trace  )
	, m_closed (false  )
	, m_pParent(pParent)
{
	RDOParser::s_parser()->checkDPTName   (this->src_info());
	RDOParser::s_parser()->insertDPTSearch(this);
	RDOParser::s_parser()->contextStack()->push(this);
}

LPContext RDODPTSearch::onFindContext(CREF(RDOValue) value) const
{
	//! Поиск не нужен, добавлен для порядка, чтобы контекст активности был на стеке после контекста точки
	return NULL;
}

void RDODPTSearch::end()
{
	rdoRuntime::LPRDOCalc pCalcCondition = m_pConditon     ? m_pConditon->getCalc()     : rdo::Factory<rdoRuntime::RDOCalcConst>::create(1);
	rdoRuntime::LPRDOCalc pCalcTerminate = m_pTermConditon ? m_pTermConditon->getCalc() : rdo::Factory<rdoRuntime::RDOCalcConst>::create(1);

	m_pRuntimeLogic = F(rdoRuntime::RDODPTSearchRuntime)::create(RDOParser::s_parser()->runtime(),
		m_pParent,
		pCalcCondition,
		pCalcTerminate,
		m_pEvalBy->createCalc(),
		m_compTops,
		m_trace);
	ASSERT(m_pRuntimeLogic);
	m_pRuntimeLogic->init(RDOParser::s_parser()->runtime());

	int size = getActivities().size();
	for (int i = 0; i < size; i++)
	{
		LPRDODPTSearchActivity pSearchActivity = getActivities().at(i);
		ASSERT(pSearchActivity);
		LPIDPTSearchActivity pActivity = F(rdoRuntime::RDODPTSearchActivity)::create(
			pSearchActivity->activity(),
			pSearchActivity->getValue(),
			pSearchActivity->getRuleCost()->createCalc()
		);
		ASSERT(pActivity);
		LPIDPTSearchLogic pSearchLogic = m_pRuntimeLogic;
		ASSERT(pSearchLogic);
		pSearchLogic->addActivity(pActivity);
	}
	m_closed = true;
	RDOParser::s_parser()->contextStack()->pop();
}

// ----------------------------------------------------------------------------
// ---------- RDOPROCProcess
// ----------------------------------------------------------------------------
tstring RDOPROCProcess::s_name_prefix = _T("");
tstring RDOPROCProcess::s_name_sufix  = _T("s");

RDOPROCProcess::RDOPROCProcess(CREF(RDOParserSrcInfo) info, CREF(tstring) name, LPRDORTPResType transactType)
	: RDOParserSrcInfo(info        )
	, m_closed        (false       )
	, m_name          (name        )
	, m_transactType  (transactType)
{
	RDOParser::s_parser()->insertPROCProcess(this);
	m_pRuntime = F(rdoRuntime::RDOPROCProcess)::create(info.src_text(), RDOParser::s_parser()->runtime());
	ASSERT(m_pRuntime);
	m_pRuntime.query_cast<ILogic>()->init(RDOParser::s_parser()->runtime());
}

rbool RDOPROCProcess::setPrior(REF(LPRDOFUNArithm) pPrior)
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

void RDOPROCProcess::insertBlock(CREF(LPRDOPROCOperator) pBlock)
{
	ASSERT(pBlock);
	m_blockList.push_back(pBlock);
}

void RDOPROCProcess::insertChild(REF(LPRDOPROCProcess) pProcess)
{
	ASSERT(pProcess);
	m_childProcessList.push_back(pProcess);
	pProcess->m_pParentProcess = this;
}

rbool RDOPROCProcess::checkTransactType(CREF(tstring) name) const
{
	return (name == m_transactType->name());
}

// ----------------------------------------------------------------------------
// ---------- RDOPROCOperator
// ----------------------------------------------------------------------------
RDOPROCOperator::RDOPROCOperator(CREF(LPRDOPROCProcess) pProcess, CREF(tstring) name)
	: m_name    (name    )
	, m_pProcess(pProcess)
{
	ASSERT(pProcess);
	m_pProcess->insertBlock(this);
}

RDOPROCOperator::~RDOPROCOperator()
{}

// ----------------------------------------------------------------------------
// ---------- RDOPROCGenerate
// ----------------------------------------------------------------------------
RDOPROCGenerate::RDOPROCGenerate(CREF(LPRDOPROCProcess) pProcess, CREF(tstring) name, CREF(rdoRuntime::LPRDOCalc) pTimeCalc)
	: RDOPROCOperator(pProcess, name)
{
	m_pRuntime = F(rdoRuntime::RDOPROCGenerate)::create(RDOParser::s_parser()->getLastPROCProcess()->getRunTime(), pTimeCalc);
	ASSERT(m_pRuntime);
}

// ----------------------------------------------------------------------------
// ---------- RDOPROCBlockForQueue
// ----------------------------------------------------------------------------
RDOPROCBlockForQueue::RDOPROCBlockForQueue(CREF(LPRDOPROCProcess) pProcess, CREF(tstring) name)
	: RDOPROCOperator(pProcess, name)
{}

// ----------------------------------------------------------------------------
// ---------- RDOPROCQueue
// ----------------------------------------------------------------------------
RDOPROCQueue::RDOPROCQueue(CREF(LPRDOPROCProcess) pProcess, CREF(tstring) name)
	: RDOPROCBlockForQueue(pProcess, name)
{}

void RDOPROCQueue::createRuntime()
{
	LPRDORSSResource pResource = RDOParser::s_parser()->findRSSResource(m_resourceName);
	if (pResource)
	{
		tstring res_name = pResource->name();
		//! Получили список всех ресурсов
		rdoMBuilder::RDOResourceList rssList(RDOParser::s_parser());
		//! Создадим тип ресурса
		rdoMBuilder::RDOResType rtp = rssList[res_name].getType();
		//! "длина_очереди"
		tstring rtp_param_name      = rdoRuntime::RDOPROCQueue::getQueueParamName();
		m_parserForRuntime.Id_res   = pResource->getID();
		m_parserForRuntime.Id_param = rtp.m_params[rtp_param_name].id();
	}
	else
	{
		RDOParser::s_parser()->error().error(RDOParserSrcInfo(), rdo::format(_T("Внутренняя ошибка RDOPROCQueue: не нашли parser-ресурс '%s'"), m_resourceName.c_str()));
	}
	m_pRuntime = F(rdoRuntime::RDOPROCQueue)::create(RDOParser::s_parser()->getLastPROCProcess()->getRunTime(), m_parserForRuntime);
	ASSERT(m_pRuntime);
}

void RDOPROCQueue::setResource(CREF(tstring) name)
{
	ASSERT(!name.empty()         );
	ASSERT(m_resourceName.empty());

	m_resourceName = name;
}

// ----------------------------------------------------------------------------
// ---------- RDOPROCDepart
// ----------------------------------------------------------------------------
RDOPROCDepart::RDOPROCDepart(CREF(LPRDOPROCProcess) pProcess, CREF(tstring) name)
	: RDOPROCBlockForQueue(pProcess, name)
{}

void RDOPROCDepart::createRuntime()
{
	LPRDORSSResource pResource = RDOParser::s_parser()->findRSSResource(m_resourceName);
	if (pResource)
	{
		tstring res_name = pResource->name();
		//! Получили список всех ресурсов
		rdoMBuilder::RDOResourceList rssList(RDOParser::s_parser());
		//! Создадим тип ресурса
		rdoMBuilder::RDOResType rtp = rssList[res_name].getType();
		//! "длина_очереди"
		tstring rtp_param_name      = rdoRuntime::RDOPROCDepart::getDepartParamName();
		m_parserForRuntime.Id_res   = pResource->getID();
		m_parserForRuntime.Id_param = rtp.m_params[rtp_param_name].id(); 
	}	
	else
	{
		RDOParser::s_parser()->error().error(RDOParserSrcInfo(), rdo::format(_T("Внутренняя ошибка RDOPROCQueue: не нашли parser-ресурс '%s'"), m_resourceName.c_str()));
	}
	m_pRuntime = F(rdoRuntime::RDOPROCDepart)::create(RDOParser::s_parser()->getLastPROCProcess()->getRunTime(), m_parserForRuntime);
	ASSERT(m_pRuntime);
}

void RDOPROCDepart::setResource(CREF(tstring) name)
{
	ASSERT(!name.empty()         );
	ASSERT(m_resourceName.empty());

	m_resourceName = name;
}

// ----------------------------------------------------------------------------
// ---------- RDOPROCBlockForSeize
// ----------------------------------------------------------------------------
RDOPROCBlockForSeize::RDOPROCBlockForSeize(CREF(LPRDOPROCProcess) pProcess, CREF(tstring) name)
	: RDOPROCOperator(pProcess, name)
{}

// ----------------------------------------------------------------------------
// ---------- RDOPROCSeize
// ----------------------------------------------------------------------------
RDOPROCSeize::RDOPROCSeize(CREF(LPRDOPROCProcess) pProcess, CREF(tstring) name)
	: RDOPROCBlockForSeize(pProcess, name)
{}

void RDOPROCSeize::createRuntime()
{
	STL_FOR_ALL_CONST(m_resourceList, it)
	{
		LPRDORSSResource pResource = RDOParser::s_parser()->findRSSResource((*it));
		if (pResource)
		{
			tstring res_name = pResource->name();
			//! Получили список всех ресурсов
			rdoMBuilder::RDOResourceList rssList(RDOParser::s_parser());
			//! Создадим тип ресурса
			rdoMBuilder::RDOResType rtp = rssList[res_name].getType();
			//! "Состояние"
			tstring rtp_param_name = rdoRuntime::RDOPROCBlockForSeize::getStateParamName();
			//! проверим его на наличие перечислимого параметра
			if (!rtp.m_params[rtp_param_name].exist())
			{
				RDOParser::s_parser()->error().error(rtp.src_info(), rdo::format(_T("У типа ресурса '%s' нет параметра перечислимого типа '%s'"), rtp.name().c_str(), rtp_param_name.c_str()));
			}
			rdoRuntime::parser_for_Seize bbb;
			bbb.Id_res   = pResource->getID();
			bbb.Id_param = rtp.m_params[rtp_param_name].id(); 
			m_parserForRuntime.push_back(bbb);
		}
		else
		{
			RDOParser::s_parser()->error().error(RDOParserSrcInfo(), rdo::format(_T("Внутренняя ошибка RDOPROCSeize: не нашли parser-ресурс '%s'"), it->c_str()));
		}
	}

	if (!m_parserForRuntime.empty())
	{
		m_pRuntime = F(rdoRuntime::RDOPROCSeize)::create(RDOParser::s_parser()->getLastPROCProcess()->getRunTime(), m_parserForRuntime);
		ASSERT(m_pRuntime);
	}
	else
	{
		RDOParser::s_parser()->error().error(RDOParserSrcInfo(), _T("Внутренняя ошибка: блок Seize не содержит ресурсов"));
	}
}

void RDOPROCSeize::addResource(CREF(tstring) name)
{
	ASSERT(!name.empty());
	m_resourceList.push_back(name);
}

// ----------------------------------------------------------------------------
// ---------- RDOPROCRelease
// ----------------------------------------------------------------------------
RDOPROCRelease::RDOPROCRelease(CREF(LPRDOPROCProcess) pProcess, CREF(tstring) name)
	: RDOPROCBlockForSeize(pProcess, name)
{}

void RDOPROCRelease::createRuntime()
{
	STL_FOR_ALL_CONST(m_resourceList, it)
	{
		LPRDORSSResource pResource = RDOParser::s_parser()->findRSSResource((*it));
		if (pResource)
		{
			tstring res_name = pResource->name();
			//! Получили список всех ресурсов
			rdoMBuilder::RDOResourceList rssList(RDOParser::s_parser());
			//! Создадим тип ресурса
			rdoMBuilder::RDOResType rtp = rssList[res_name].getType();
			//! "Состояние"
			tstring rtp_param_name = rdoRuntime::RDOPROCBlockForSeize::getStateParamName();
			//! проверим его на наличие перечислимого параметра
			if (!rtp.m_params[rtp_param_name].exist())
			{
				RDOParser::s_parser()->error().error(rtp.src_info(), rdo::format(_T("У типа ресурса '%s' нет параметра перечислимого типа '%s'"), rtp.name().c_str(), rtp_param_name.c_str()));
			}
			rdoRuntime::parser_for_Seize bbb;
			bbb.Id_res   = pResource->getID();
			bbb.Id_param = rtp.m_params[rtp_param_name].id(); 
			m_parserForRuntime.push_back(bbb);
		}
		else
		{
			RDOParser::s_parser()->error().error(RDOParserSrcInfo(), rdo::format(_T("Внутренняя ошибка RDOPROCRelease: не нашли parser-ресурс '%s'"), it->c_str()));
		}
	}

	if (!m_parserForRuntime.empty())
	{
		m_pRuntime = F(rdoRuntime::RDOPROCRelease)::create(RDOParser::s_parser()->getLastPROCProcess()->getRunTime(), m_parserForRuntime);
		ASSERT(m_pRuntime);
	}
	else
	{
		RDOParser::s_parser()->error().error(RDOParserSrcInfo(), _T("Внутренняя ошибка: блок Release не содержит ресурсов"));
	}
}

void RDOPROCRelease::addResource(CREF(tstring) name)
{
	ASSERT(!name.empty());
	m_resourceList.push_back(name);
}

// ----------------------------------------------------------------------------
// ---------- RDOPROCAdvance
// ----------------------------------------------------------------------------
RDOPROCAdvance::RDOPROCAdvance(CREF(LPRDOPROCProcess) pProcess, CREF(tstring) name, CREF(rdoRuntime::LPRDOCalc) pTimeCalc)
	: RDOPROCOperator(pProcess, name)
{
	m_pRuntime = F(rdoRuntime::RDOPROCAdvance)::create(RDOParser::s_parser()->getLastPROCProcess()->getRunTime(), pTimeCalc);
	ASSERT(m_pRuntime);
}

// ----------------------------------------------------------------------------
// ---------- RDOPROCTerminate
// ----------------------------------------------------------------------------
RDOPROCTerminate::RDOPROCTerminate(CREF(LPRDOPROCProcess) pProcess, CREF(tstring) name, CREF(ruint) term)
	: RDOPROCOperator(pProcess, name)
{
	m_pRuntime = F(rdoRuntime::RDOPROCTerminate)::create(RDOParser::s_parser()->getLastPROCProcess()->getRunTime(), term);
	ASSERT(m_pRuntime);
}

// ----------------------------------------------------------------------------
// ---------- RDOPROCAssign
// ----------------------------------------------------------------------------
RDOPROCAssign::RDOPROCAssign(CREF(LPRDOPROCProcess) pProcess, CREF(tstring) name, CREF(rdoRuntime::LPRDOCalc) pValue, int resID, int paramID)
	: RDOPROCOperator(pProcess, name)
{
	m_pRuntime = F(rdoRuntime::RDOPROCAssign)::create(RDOParser::s_parser()->getLastPROCProcess()->getRunTime(), pValue, resID, paramID);
	ASSERT(m_pRuntime);
}

CLOSE_RDO_PARSER_NAMESPACE
