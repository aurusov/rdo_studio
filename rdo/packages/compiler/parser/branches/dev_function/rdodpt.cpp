/*!
  \copyright (c) RDO-Team, 2011
  \file      rdodpt.cpp
  \authors   Барс Александр
  \authors   Урусов Андрей (rdo@rk9.bmstu.ru)
  \date      11.06.2006
  \brief     Точки принятия решений
  \indent    4T
*/

// ---------------------------------------------------------------------------- PCH
#include "simulator/compiler/parser/pch.h"
// ----------------------------------------------------------------------- INCLUDES
// ----------------------------------------------------------------------- SYNOPSIS
#include "simulator/compiler/parser/rdodpt.h"
#include "simulator/compiler/parser/rdoparser.h"
#include "simulator/compiler/parser/rdoparser_lexer.h"
#include "simulator/compiler/parser/rdorss.h"
#include "simulator/compiler/parser/type/type.h"

#include "simulator/compiler/mbuilder/rdo_resources.h"

#include "simulator/runtime/rdo_rule.h"
#include "simulator/runtime/rdo_keyboard.h"
#include "simulator/runtime/rdo_logic_dptsome.h"
#include "simulator/runtime/rdo_logic_dptprior.h"
#include "simulator/runtime/rdodptrtime.h"
#include "simulator/runtime/rdo_dptsearch_activity.h"
#include "simulator/runtime/calc/calc_pattern.h"
// --------------------------------------------------------------------------------

OPEN_RDO_PARSER_NAMESPACE

int dptlex(PTR(YYSTYPE) lpval, PTR(YYLTYPE) llocp, PTR(void) lexer)
{
	LEXER->m_lpval = lpval;
	LEXER->m_lploc = llocp;
	return LEXER->yylex();
}

void dpterror(PTR(char) message)
{
	UNUSED(message);
}

// --------------------------------------------------------------------------------
// -------------------- RDODPTActivity
// --------------------------------------------------------------------------------
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
	RDOParser::s_parser()->insertDPTActivity(this);
}

RDODPTActivity::~RDODPTActivity()
{}

Context::FindResult RDODPTActivity::onFindContext(CREF(LPRDOValue) pValue) const
{
	ASSERT(pValue);
	Context::FindResult result = m_pPattern->onFindContext(pValue);
	if (result.m_pContext)
	{
		return result;
	}
	return Context::FindResult();
}

void RDODPTActivity::addParam(CREF(LPRDOValue) pParam)
{
	ASSERT(pParam);

	if (m_pPattern->m_paramList.size() <= m_currParam)
	{
		if (pParam->src_pos().m_first_line == src_pos().m_first_line)
		{
			RDOParser::s_parser()->error().push_only(pParam->src_pos(), rdo::format(_T("Слишком много параметров для образца '%s' при описании активности '%s'"), m_pPattern->name().c_str(), name().c_str()));
			RDOParser::s_parser()->error().push_only(m_pPattern->src_info(), _T("См. образец"));
			RDOParser::s_parser()->error().push_done();
		}
		else
		{
			RDOParser::s_parser()->error().error(pParam->src_pos(), _T("Имя активности должно заканчиваться двоеточием"));
		}
	}
	rdo::runtime::RDOValue val;
	LPRDOParam pPatternParam = m_pPattern->m_paramList.at(m_currParam);
	if (pParam->value().getAsString() == _T("*"))
	{
		if (!pPatternParam->getDefault()->defined())
		{
			RDOParser::s_parser()->error().push_only(pParam->src_pos(), rdo::format(_T("Нет значения по умолчанию для параметра '%s'"), pPatternParam->src_text().c_str()));
			RDOParser::s_parser()->error().push_only(pPatternParam->src_info(), rdo::format(_T("См. параметр '%s', тип '%s'"), pPatternParam->src_text().c_str(), pPatternParam->getTypeInfo()->src_info().src_text().c_str()));
			RDOParser::s_parser()->error().push_done();
		}
		val = pPatternParam->getDefault()->value();
	}
	else
	{
		val = pPatternParam->getTypeInfo()->value_cast(pParam)->value();
	}

	rdo::runtime::LPRDOCalc pSetParamCalc = rdo::Factory<rdo::runtime::RDOSetPatternParamCalc>::create(
		m_currParam,
		rdo::Factory<rdo::runtime::RDOCalcConst>::create(val)
	);
	ASSERT(pSetParamCalc);
	pSetParamCalc->setSrcInfo(RDOParserSrcInfo(pParam->getPosAsYY(), rdo::format(_T("Параметр образца %s.%s = %s"), m_pPattern->name().c_str(), pPatternParam->name().c_str(), pParam->value().getAsString().c_str())));
	m_pActivity->addParamCalc(pSetParamCalc);
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
	RDOParser::s_parser()->contextStack()->pop_safe<RDODPTActivity>();
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

// --------------------------------------------------------------------------------
// -------------------- RDODPTActivityHotKey
// --------------------------------------------------------------------------------
RDODPTActivityHotKey::RDODPTActivityHotKey(LPIBaseOperationContainer pDPT, CREF(RDOParserSrcInfo) src_info, CREF(RDOParserSrcInfo) pattern_src_info)
	: RDODPTActivity(src_info, pattern_src_info)
{
	switch (pattern()->getType())
	{
	case RDOPATPattern::PT_Rule:
		m_pActivity = pattern()->getPatRuntime<rdo::runtime::RDOPatternRule>()->createActivity(pDPT, RDOParser::s_parser()->runtime(), name());
		break;

	case RDOPATPattern::PT_Operation:
		m_pActivity = pattern()->getPatRuntime<rdo::runtime::RDOPatternOperation>()->createActivity(pDPT, RDOParser::s_parser()->runtime(), name());
		break;

	case RDOPATPattern::PT_Keyboard:
		m_pActivity = pattern()->getPatRuntime<rdo::runtime::RDOPatternKeyboard>()->createActivity(pDPT, RDOParser::s_parser()->runtime(), name());
		break;

	default:
		RDOParser::s_parser()->error().push_only(this->src_info(), _T("Неизвестный тип образца"));
		RDOParser::s_parser()->error().push_only(pattern()->src_info(), _T("См. образец"));
		RDOParser::s_parser()->error().push_done();
	}
}

RDODPTActivityHotKey::~RDODPTActivityHotKey()
{}

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
	case rdo::runtime::RDOKeyboard::addhk_ok:
		break;

	case rdo::runtime::RDOKeyboard::addhk_already:
		RDOParser::s_parser()->error().error(hotkey_pos, rdo::format(_T("Для активности '%s' клавиша уже назначена"), src_text().c_str()));
		break;

	case rdo::runtime::RDOKeyboard::addhk_notfound:
		RDOParser::s_parser()->error().error(hotkey_pos, rdo::format(_T("Неизвестная клавиша: %s"), hotKey.c_str()));
		break;

	case rdo::runtime::RDOKeyboard::addhk_dont:
		RDOParser::s_parser()->error().push_only(src_info(), rdo::format(_T("Операция '%s' не является клавиатурной"), src_text().c_str()));
		RDOParser::s_parser()->error().push_only(pattern()->src_info(), _T("См. образец"));
		RDOParser::s_parser()->error().push_done();
		break;

	default:
		RDOParser::s_parser()->error().error(src_info(), _T("Внутренная ошибка: RDOOPROperation::addHotKey"));
	}
}

// --------------------------------------------------------------------------------
// -------------------- RDODPTSomeActivity
// --------------------------------------------------------------------------------
RDODPTSomeActivity::RDODPTSomeActivity(LPIBaseOperationContainer pDPT, CREF(RDOParserSrcInfo) src_info, CREF(RDOParserSrcInfo) pattern_src_info)
	: RDODPTActivityHotKey(pDPT, src_info, pattern_src_info)
{}

RDODPTSomeActivity::~RDODPTSomeActivity()
{}

// --------------------------------------------------------------------------------
// -------------------- RDODPTPriorActivity
// --------------------------------------------------------------------------------
RDODPTPriorActivity::RDODPTPriorActivity(LPIBaseOperationContainer pDPT, CREF(RDOParserSrcInfo) src_info, CREF(RDOParserSrcInfo) pattern_src_info)
	: RDODPTActivityHotKey(pDPT, src_info, pattern_src_info)
{}

RDODPTPriorActivity::~RDODPTPriorActivity()
{}

// --------------------------------------------------------------------------------
// -------------------- RDODPTSome
// --------------------------------------------------------------------------------
RDODPTSome::RDODPTSome(CREF(RDOParserSrcInfo) src_info, LPILogic pParent)
	: RDOLogic<rdo::runtime::RDODPTSome, RDODPTSomeActivity>(src_info)
{
	RDOParser::s_parser()->checkDPTName(this->src_info());
	m_pRuntimeLogic = RF(rdo::runtime::RDODPTSome)::create(RDOParser::s_parser()->runtime(), pParent);
	ASSERT(m_pRuntimeLogic);
	m_pRuntimeLogic->init(RDOParser::s_parser()->runtime());
	RDOParser::s_parser()->insertDPTSome(this);
	RDOParser::s_parser()->contextStack()->push(this);
}

RDODPTSome::~RDODPTSome()
{}

Context::FindResult RDODPTSome::onFindContext(CREF(LPRDOValue) pValue) const
{
	UNUSED(pValue);

	//! Поиск не нужен, добавлен для порядка, чтобы контекст активности был на стеке после контекста точки
	return Context::FindResult();
}

// --------------------------------------------------------------------------------
// -------------------- RDODPTPrior
// --------------------------------------------------------------------------------
RDODPTPrior::RDODPTPrior(CREF(RDOParserSrcInfo) src_info, LPILogic pParent)
	: RDOLogic<rdo::runtime::RDODPTPrior, RDODPTPriorActivity>(src_info)
{
	RDOParser::s_parser()->checkDPTName(this->src_info());
	m_pRuntimeLogic = RF(rdo::runtime::RDODPTPrior)::create(RDOParser::s_parser()->runtime(), pParent);
	ASSERT(m_pRuntimeLogic);
	m_pRuntimeLogic->init(RDOParser::s_parser()->runtime());
	RDOParser::s_parser()->insertDPTPrior(this);
	RDOParser::s_parser()->contextStack()->push(this);
}

RDODPTPrior::~RDODPTPrior()
{}

Context::FindResult RDODPTPrior::onFindContext(CREF(LPRDOValue) pValue) const
{
	UNUSED(pValue);

	//! Поиск не нужен, добавлен для порядка, чтобы контекст активности был на стеке после контекста точки
	return Context::FindResult();
}

// --------------------------------------------------------------------------------
// -------------------- RDODPTSearchActivity
// --------------------------------------------------------------------------------
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
		if (((*it)->m_statusBegin == rdo::runtime::RDOResource::CS_Create) || ((*it)->m_statusBegin == rdo::runtime::RDOResource::CS_Erase))
		{
			RDOParser::s_parser()->error().push_only(this->src_info(), rdo::format(_T("В продукционном правиле '%s' нельзя создавать или удалять ресурсы, т.к. оно используется в точке типа search"), src_text().c_str()));
			RDOParser::s_parser()->error().push_only(pattern()->src_info(), _T("См. образец"));
			RDOParser::s_parser()->error().push_only((*it)->src_info(), _T("См. релевантный ресурс"));
			RDOParser::s_parser()->error().push_done();
		}
	}
	m_pActivity = RF(rdo::runtime::RDORule)::create(RDOParser::s_parser()->runtime(), pattern()->getPatRuntime<rdo::runtime::RDOPatternRule>(), true, name());
	ASSERT(m_pActivity);
}

RDODPTSearchActivity::~RDODPTSearchActivity()
{}

void RDODPTSearchActivity::setValue(IDPTSearchActivity::ValueTime value, CREF(LPRDOFUNArithm) pRuleCost)
{
	m_value     = value;
	m_pRuleCost = pRuleCost;
}

// --------------------------------------------------------------------------------
// -------------------- RDODPTSearch
// --------------------------------------------------------------------------------
RDODPTSearch::RDODPTSearch(CREF(RDOParserSrcInfo) src_info, rdo::runtime::RDODPTSearchTrace::DPT_TraceFlag trace, LPILogic pParent)
	: RDOLogic<rdo::runtime::RDODPTSearchRuntime, RDODPTSearchActivity>(src_info)
	, m_pParent(pParent)
	, m_closed (false  )
	, m_trace  (trace  )
{
	RDOParser::s_parser()->checkDPTName   (this->src_info());
	RDOParser::s_parser()->insertDPTSearch(this);
	RDOParser::s_parser()->contextStack()->push(this);
}

RDODPTSearch::~RDODPTSearch()
{}

Context::FindResult RDODPTSearch::onFindContext(CREF(LPRDOValue) pValue) const
{
	UNUSED(pValue);

	//! Поиск не нужен, добавлен для порядка, чтобы контекст активности был на стеке после контекста точки
	return Context::FindResult();
}

void RDODPTSearch::end()
{
	rdo::runtime::LPRDOCalc pCalcCondition = m_pConditon     ? m_pConditon->getCalc()     : rdo::Factory<rdo::runtime::RDOCalcConst>::create(1).object_parent_cast<rdo::runtime::RDOCalc>();
	rdo::runtime::LPRDOCalc pCalcTerminate = m_pTermConditon ? m_pTermConditon->getCalc() : rdo::Factory<rdo::runtime::RDOCalcConst>::create(1).object_parent_cast<rdo::runtime::RDOCalc>();

	m_pRuntimeLogic = RF(rdo::runtime::RDODPTSearchRuntime)::create(RDOParser::s_parser()->runtime(),
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
		LPIDPTSearchActivity pActivity = RF(rdo::runtime::RDODPTSearchActivity)::create(
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

	//! @todo проверить, можно ли избавиться от повторения этой строки. Уже есть в RDOLogicBase::end()
	RDOParser::s_parser()->contextStack()->pop();
}

CLOSE_RDO_PARSER_NAMESPACE
