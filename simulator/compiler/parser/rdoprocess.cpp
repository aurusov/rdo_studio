/*!
  \copyright (c) RDO-Team, 2012
  \file      rdoprocess.cpp
  \authors   Барс Александр
  \authors   Урусов Андрей (rdo@rk9.bmstu.ru)
  \authors   Лущан Дмитрий (dluschan@rk9.bmstu.ru)
  \date      21.02.2012
  \brief     Процессы
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

#include "simulator/runtime/process/rdoprocess.h"
#include "simulator/runtime/calc/calc_pattern.h"
// --------------------------------------------------------------------------------

OPEN_RDO_PARSER_NAMESPACE

int proc_rtp_lex(YYSTYPE* lpval, YYLTYPE* llocp, void* lexer)
{
	LEXER->m_lpval = lpval;
	LEXER->m_lploc = llocp;
	return LEXER->yylex();
}

void proc_rtp_error(YYLTYPE* /*llocp*/, void* /*lexer*/, const char* /*message*/)
{}

int proc_rss_lex(YYSTYPE* lpval, YYLTYPE* llocp, void* lexer)
{
	LEXER->m_lpval = lpval;
	LEXER->m_lploc = llocp;
	return LEXER->yylex();
}

void proc_rss_error(YYLTYPE* /*llocp*/, void* /*lexer*/, const char* /*message*/)
{}

int proc_opr_lex(YYSTYPE* lpval, YYLTYPE* llocp, void* lexer)
{
	LEXER->m_lpval = lpval;
	LEXER->m_lploc = llocp;
	return LEXER->yylex();
}

void proc_opr_error(YYLTYPE* /*llocp*/, void* /*lexer*/, const char* /*message*/)
{}

// -------------------- RDOPROCProcess
// --------------------------------------------------------------------------------
std::string RDOPROCProcess::s_name_prefix = "";
std::string RDOPROCProcess::s_name_sufix  = "s";

RDOPROCProcess::RDOPROCProcess(const RDOParserSrcInfo& info, const std::string& name, LPRDORTPResType transactType)
	: RDOParserSrcInfo(info        )
	, m_closed        (false       )
	, m_name          (name        )
	, m_transactType  (transactType)
{
	RDOParser::s_parser()->insertPROCProcess(this);
	m_pRuntime = RF(rdo::runtime::RDOPROCProcess)::create(info.src_text(), RDOParser::s_parser()->runtime());
	ASSERT(m_pRuntime);
	m_pRuntime.query_cast<ILogic>()->init(RDOParser::s_parser()->runtime());
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

bool RDOPROCProcess::checkTransactType(const std::string& name) const
{
	return (name == m_transactType->name());
}

Context::FindResult RDOPROCProcess::onFindContext(const std::string& method, const Context::Params& params, const RDOParserSrcInfo& srcInfo) const
{
	return FindResult();
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
RDOPROCGenerate::RDOPROCGenerate(const LPRDOPROCProcess& pProcess, const std::string& name, const rdo::runtime::LPRDOCalc& pTimeCalc, const rdo::runtime::LPRDOCalc& pCreateAndGoOnTransactCalc)
	: RDOPROCOperator(pProcess, name)
{
	m_pRuntime = RF(rdo::runtime::RDOPROCGenerate)::create(RDOParser::s_parser()->getLastPROCProcess()->getRunTime(), pTimeCalc, pCreateAndGoOnTransactCalc);
	ASSERT(m_pRuntime);
}

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
	LPRDORSSResource pResource = RDOParser::s_parser()->findRSSResource(m_resourceName);
	if (pResource)
	{
		const std::string res_name = pResource->name();
		//! Получили список всех ресурсов
		rdo::compiler::mbuilder::RDOResourceList rssList(RDOParser::s_parser());
		//! Создадим тип ресурса
		rdo::compiler::mbuilder::RDOResType rtp = rssList[res_name].getType();
		//! "длина_очереди"
		const std::string rtp_param_name = rdo::runtime::RDOPROCQueue::getQueueParamName();
		m_parserForRuntime.Id_res = pResource->getID();
		m_parserForRuntime.Id_param = rtp.m_params[rtp_param_name].id();
	}
	else
	{
		RDOParser::s_parser()->error().error(RDOParserSrcInfo(), rdo::format("Внутренняя ошибка RDOPROCQueue: не нашли parser-ресурс '%s'", m_resourceName.c_str()));
	}
	m_pRuntime = RF(rdo::runtime::RDOPROCQueue)::create(RDOParser::s_parser()->getLastPROCProcess()->getRunTime(), m_parserForRuntime);
	ASSERT(m_pRuntime);
}

void RDOPROCQueue::setResource(const std::string& name)
{
	ASSERT(!name.empty());
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
	LPRDORSSResource pResource = RDOParser::s_parser()->findRSSResource(m_resourceName);
	if (pResource)
	{
		const std::string res_name = pResource->name();
		//! Получили список всех ресурсов
		rdo::compiler::mbuilder::RDOResourceList rssList(RDOParser::s_parser());
		//! Создадим тип ресурса
		rdo::compiler::mbuilder::RDOResType rtp = rssList[res_name].getType();
		//! "длина_очереди"
		const std::string rtp_param_name = rdo::runtime::RDOPROCDepart::getDepartParamName();
		m_parserForRuntime.Id_res = pResource->getID();
		m_parserForRuntime.Id_param = rtp.m_params[rtp_param_name].id();
	}
	else
	{
		RDOParser::s_parser()->error().error(RDOParserSrcInfo(), rdo::format("Внутренняя ошибка RDOPROCQueue: не нашли parser-ресурс '%s'", m_resourceName.c_str()));
	}
	m_pRuntime = RF(rdo::runtime::RDOPROCDepart)::create(RDOParser::s_parser()->getLastPROCProcess()->getRunTime(), m_parserForRuntime);
	ASSERT(m_pRuntime);
}

void RDOPROCDepart::setResource(const std::string& name)
{
	ASSERT(!name.empty());
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
	for (const auto& resource: m_resourceList)
	{
		LPRDORSSResource pResource = RDOParser::s_parser()->findRSSResource(resource);
		if (pResource)
		{
			const std::string res_name = pResource->name();
			// Получили список всех ресурсов
			rdo::compiler::mbuilder::RDOResourceList rssList(RDOParser::s_parser());
			// Создадим тип ресурса
			rdo::compiler::mbuilder::RDOResType rtp = rssList[res_name].getType();
			// "Состояние"
			const std::string rtp_param_name = rdo::runtime::RDOPROCBlockForSeize::getStateParamName();
			// проверим его на наличие перечислимого параметра
			if (!rtp.m_params[rtp_param_name].exist())
			{
				RDOParser::s_parser()->error().error(rtp.src_info(), rdo::format("У типа ресурса '%s' нет параметра перечислимого типа '%s'", rtp.name().c_str(), rtp_param_name.c_str()));
			}
			rdo::runtime::parser_for_Seize bbb;
			bbb.Id_res   = pResource->getID();
			bbb.Id_param = rtp.m_params[rtp_param_name].id();
			m_parserForRuntime.push_back(bbb);
		}
		else
		{
			RDOParser::s_parser()->error().error(RDOParserSrcInfo(), rdo::format("Внутренняя ошибка RDOPROCSeize: не нашли parser-ресурс '%s'", resource.c_str()));
		}
	}

	if (!m_parserForRuntime.empty())
	{
		m_pRuntime = RF(rdo::runtime::RDOPROCSeize)::create(RDOParser::s_parser()->getLastPROCProcess()->getRunTime(), m_parserForRuntime);
		ASSERT(m_pRuntime);
	}
	else
	{
		RDOParser::s_parser()->error().error(RDOParserSrcInfo(), "Внутренняя ошибка: блок Seize не содержит ресурсов");
	}
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
	for (const auto& resource: m_resourceList)
	{
		LPRDORSSResource pResource = RDOParser::s_parser()->findRSSResource(resource);
		if (pResource)
		{
			const std::string res_name = pResource->name();
			//! Получили список всех ресурсов
			rdo::compiler::mbuilder::RDOResourceList rssList(RDOParser::s_parser());
			//! Создадим тип ресурса
			rdo::compiler::mbuilder::RDOResType rtp = rssList[res_name].getType();
			//! "Состояние"
			const std::string rtp_param_name = rdo::runtime::RDOPROCBlockForSeize::getStateParamName();
			//! проверим его на наличие перечислимого параметра
			if (!rtp.m_params[rtp_param_name].exist())
			{
				RDOParser::s_parser()->error().error(rtp.src_info(), rdo::format("У типа ресурса '%s' нет параметра перечислимого типа '%s'", rtp.name().c_str(), rtp_param_name.c_str()));
			}
			rdo::runtime::parser_for_Seize bbb;
			bbb.Id_res   = pResource->getID();
			bbb.Id_param = rtp.m_params[rtp_param_name].id();
			m_parserForRuntime.push_back(bbb);
		}
		else
		{
			RDOParser::s_parser()->error().error(RDOParserSrcInfo(), rdo::format("Внутренняя ошибка RDOPROCRelease: не нашли parser-ресурс '%s'", resource.c_str()));
		}
	}

	if (!m_parserForRuntime.empty())
	{
		m_pRuntime = RF(rdo::runtime::RDOPROCRelease)::create(RDOParser::s_parser()->getLastPROCProcess()->getRunTime(), m_parserForRuntime);
		ASSERT(m_pRuntime);
	}
	else
	{
		RDOParser::s_parser()->error().error(RDOParserSrcInfo(), "Внутренняя ошибка: блок Release не содержит ресурсов");
	}
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
	m_pRuntime = RF(rdo::runtime::RDOPROCAdvance)::create(RDOParser::s_parser()->getLastPROCProcess()->getRunTime(), pTimeCalc);
	ASSERT(m_pRuntime);
}

// --------------------------------------------------------------------------------
// -------------------- RDOPROCTerminate
// --------------------------------------------------------------------------------
RDOPROCTerminate::RDOPROCTerminate(const LPRDOPROCProcess& pProcess, const std::string& name, const rdo::runtime::LPRDOCalc& pCalc)
	: RDOPROCOperator(pProcess, name)
{
	m_pRuntime = RF(rdo::runtime::RDOPROCTerminate)::create(RDOParser::s_parser()->getLastPROCProcess()->getRunTime(), pCalc);
	ASSERT(m_pRuntime);
}

// --------------------------------------------------------------------------------
// -------------------- RDOPROCAssign
// --------------------------------------------------------------------------------
RDOPROCAssign::RDOPROCAssign(const LPRDOPROCProcess& pProcess, const std::string& name, const rdo::runtime::LPRDOCalc& pCalc)
	: RDOPROCOperator(pProcess, name)
{
	m_pRuntime = RF(rdo::runtime::RDOPROCAssign)::create(RDOParser::s_parser()->getLastPROCProcess()->getRunTime(), pCalc);
	ASSERT(m_pRuntime);
}

CLOSE_RDO_PARSER_NAMESPACE
