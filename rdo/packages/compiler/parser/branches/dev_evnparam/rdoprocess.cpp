/*!
  \copyright (c) RDO-Team, 2012
  \file      rdoprocess.cpp
  \authors   ���� ���������
  \authors   ������ ������ (rdo@rk9.bmstu.ru)
  \authors   ����� ������� (dluschan@rk9.bmstu.ru)
  \date      21.02.2012
  \brief     ��������
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

int proc_rtp_lex(PTR(YYSTYPE) lpval, PTR(YYLTYPE) llocp, PTR(void) lexer)
{
	LEXER->m_lpval = lpval;
	LEXER->m_lploc = llocp;
	return LEXER->yylex();
}

void proc_rtp_error(PTR(char) message)
{
	UNUSED(message);
}

int proc_rss_lex(PTR(YYSTYPE) lpval, PTR(YYLTYPE) llocp, PTR(void) lexer)
{
	LEXER->m_lpval = lpval;
	LEXER->m_lploc = llocp;
	return LEXER->yylex();
}

void proc_rss_error(PTR(char) message)
{
	UNUSED(message);
}

int proc_opr_lex(PTR(YYSTYPE) lpval, PTR(YYLTYPE) llocp, PTR(void) lexer)
{
	LEXER->m_lpval = lpval;
	LEXER->m_lploc = llocp;
	return LEXER->yylex();
}

void proc_opr_error(PTR(char) message)
{
	UNUSED(message);
}

// -------------------- RDOPROCProcess
// --------------------------------------------------------------------------------
tstring RDOPROCProcess::s_name_prefix = _T("");
tstring RDOPROCProcess::s_name_sufix  = _T("s");

RDOPROCProcess::RDOPROCProcess(CREF(RDOParserSrcInfo) info, CREF(tstring) name, LPRDORTPResType transactType)
	: RDOParserSrcInfo(info        )
	, m_closed        (false       )
	, m_name          (name        )
	, m_transactType  (transactType)
{
	RDOParser::s_parser()->insertPROCProcess(this);
	m_pRuntime = RF(rdoRuntime::RDOPROCProcess)::create(info.src_text(), RDOParser::s_parser()->runtime());
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

// --------------------------------------------------------------------------------
// -------------------- RDOPROCOperator
// --------------------------------------------------------------------------------
RDOPROCOperator::RDOPROCOperator(CREF(LPRDOPROCProcess) pProcess, CREF(tstring) name)
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
RDOPROCGenerate::RDOPROCGenerate(CREF(LPRDOPROCProcess) pProcess, CREF(tstring) name, CREF(rdoRuntime::LPRDOCalc) pTimeCalc, CREF(rdoRuntime::LPRDOCalc) pCreateAndGoOnTransactCalc)
	: RDOPROCOperator(pProcess, name)
{
	m_pRuntime = RF(rdoRuntime::RDOPROCGenerate)::create(RDOParser::s_parser()->getLastPROCProcess()->getRunTime(), pTimeCalc, pCreateAndGoOnTransactCalc);
	ASSERT(m_pRuntime);
}

// --------------------------------------------------------------------------------
// -------------------- RDOPROCBlockForQueue
// --------------------------------------------------------------------------------
RDOPROCBlockForQueue::RDOPROCBlockForQueue(CREF(LPRDOPROCProcess) pProcess, CREF(tstring) name)
	: RDOPROCOperator(pProcess, name)
{}

// --------------------------------------------------------------------------------
// -------------------- RDOPROCQueue
// --------------------------------------------------------------------------------
RDOPROCQueue::RDOPROCQueue(CREF(LPRDOPROCProcess) pProcess, CREF(tstring) name)
	: RDOPROCBlockForQueue(pProcess, name)
{}

void RDOPROCQueue::createRuntime()
{
	LPRDORSSResource pResource = RDOParser::s_parser()->findRSSResource(m_resourceName);
	if (pResource)
	{
		tstring res_name = pResource->name();
		//! �������� ������ ���� ��������
		rdoMBuilder::RDOResourceList rssList(RDOParser::s_parser());
		//! �������� ��� �������
		rdoMBuilder::RDOResType rtp = rssList[res_name].getType();
		//! "�����_�������"
		tstring rtp_param_name      = rdoRuntime::RDOPROCQueue::getQueueParamName();
		m_parserForRuntime.Id_res   = pResource->getID();
		m_parserForRuntime.Id_param = rtp.m_params[rtp_param_name].id();
	}
	else
	{
		RDOParser::s_parser()->error().error(RDOParserSrcInfo(), rdo::format(_T("���������� ������ RDOPROCQueue: �� ����� parser-������ '%s'"), m_resourceName.c_str()));
	}
	m_pRuntime = RF(rdoRuntime::RDOPROCQueue)::create(RDOParser::s_parser()->getLastPROCProcess()->getRunTime(), m_parserForRuntime);
	ASSERT(m_pRuntime);
}

void RDOPROCQueue::setResource(CREF(tstring) name)
{
	ASSERT(!name.empty()         );
	ASSERT(m_resourceName.empty());

	m_resourceName = name;
}

// --------------------------------------------------------------------------------
// -------------------- RDOPROCDepart
// --------------------------------------------------------------------------------
RDOPROCDepart::RDOPROCDepart(CREF(LPRDOPROCProcess) pProcess, CREF(tstring) name)
	: RDOPROCBlockForQueue(pProcess, name)
{}

void RDOPROCDepart::createRuntime()
{
	LPRDORSSResource pResource = RDOParser::s_parser()->findRSSResource(m_resourceName);
	if (pResource)
	{
		tstring res_name = pResource->name();
		//! �������� ������ ���� ��������
		rdoMBuilder::RDOResourceList rssList(RDOParser::s_parser());
		//! �������� ��� �������
		rdoMBuilder::RDOResType rtp = rssList[res_name].getType();
		//! "�����_�������"
		tstring rtp_param_name      = rdoRuntime::RDOPROCDepart::getDepartParamName();
		m_parserForRuntime.Id_res   = pResource->getID();
		m_parserForRuntime.Id_param = rtp.m_params[rtp_param_name].id(); 
	}	
	else
	{
		RDOParser::s_parser()->error().error(RDOParserSrcInfo(), rdo::format(_T("���������� ������ RDOPROCQueue: �� ����� parser-������ '%s'"), m_resourceName.c_str()));
	}
	m_pRuntime = RF(rdoRuntime::RDOPROCDepart)::create(RDOParser::s_parser()->getLastPROCProcess()->getRunTime(), m_parserForRuntime);
	ASSERT(m_pRuntime);
}

void RDOPROCDepart::setResource(CREF(tstring) name)
{
	ASSERT(!name.empty()         );
	ASSERT(m_resourceName.empty());

	m_resourceName = name;
}

// --------------------------------------------------------------------------------
// -------------------- RDOPROCBlockForSeize
// --------------------------------------------------------------------------------
RDOPROCBlockForSeize::RDOPROCBlockForSeize(CREF(LPRDOPROCProcess) pProcess, CREF(tstring) name)
	: RDOPROCOperator(pProcess, name)
{}

// --------------------------------------------------------------------------------
// -------------------- RDOPROCSeize
// --------------------------------------------------------------------------------
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
			// �������� ������ ���� ��������
			rdoMBuilder::RDOResourceList rssList(RDOParser::s_parser());
			// �������� ��� �������
			rdoMBuilder::RDOResType rtp = rssList[res_name].getType();
			// "���������"
			tstring rtp_param_name = rdoRuntime::RDOPROCBlockForSeize::getStateParamName();
			// �������� ��� �� ������� ������������� ���������
			if (!rtp.m_params[rtp_param_name].exist())
			{
				RDOParser::s_parser()->error().error(rtp.src_info(), rdo::format(_T("� ���� ������� '%s' ��� ��������� ������������� ���� '%s'"), rtp.name().c_str(), rtp_param_name.c_str()));
			}
			rdoRuntime::parser_for_Seize bbb;
			bbb.Id_res   = pResource->getID();
			bbb.Id_param = rtp.m_params[rtp_param_name].id(); 
			m_parserForRuntime.push_back(bbb);
		}
		else
		{
			RDOParser::s_parser()->error().error(RDOParserSrcInfo(), rdo::format(_T("���������� ������ RDOPROCSeize: �� ����� parser-������ '%s'"), it->c_str()));
		}
	}

	if (!m_parserForRuntime.empty())
	{
		m_pRuntime = RF(rdoRuntime::RDOPROCSeize)::create(RDOParser::s_parser()->getLastPROCProcess()->getRunTime(), m_parserForRuntime);
		ASSERT(m_pRuntime);
	}
	else
	{
		RDOParser::s_parser()->error().error(RDOParserSrcInfo(), _T("���������� ������: ���� Seize �� �������� ��������"));
	}
}

void RDOPROCSeize::addResource(CREF(tstring) name)
{
	ASSERT(!name.empty());
	m_resourceList.push_back(name);
}

// --------------------------------------------------------------------------------
// -------------------- RDOPROCRelease
// --------------------------------------------------------------------------------
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
			//! �������� ������ ���� ��������
			rdoMBuilder::RDOResourceList rssList(RDOParser::s_parser());
			//! �������� ��� �������
			rdoMBuilder::RDOResType rtp = rssList[res_name].getType();
			//! "���������"
			tstring rtp_param_name = rdoRuntime::RDOPROCBlockForSeize::getStateParamName();
			//! �������� ��� �� ������� ������������� ���������
			if (!rtp.m_params[rtp_param_name].exist())
			{
				RDOParser::s_parser()->error().error(rtp.src_info(), rdo::format(_T("� ���� ������� '%s' ��� ��������� ������������� ���� '%s'"), rtp.name().c_str(), rtp_param_name.c_str()));
			}
			rdoRuntime::parser_for_Seize bbb;
			bbb.Id_res   = pResource->getID();
			bbb.Id_param = rtp.m_params[rtp_param_name].id(); 
			m_parserForRuntime.push_back(bbb);
		}
		else
		{
			RDOParser::s_parser()->error().error(RDOParserSrcInfo(), rdo::format(_T("���������� ������ RDOPROCRelease: �� ����� parser-������ '%s'"), it->c_str()));
		}
	}

	if (!m_parserForRuntime.empty())
	{
		m_pRuntime = RF(rdoRuntime::RDOPROCRelease)::create(RDOParser::s_parser()->getLastPROCProcess()->getRunTime(), m_parserForRuntime);
		ASSERT(m_pRuntime);
	}
	else
	{
		RDOParser::s_parser()->error().error(RDOParserSrcInfo(), _T("���������� ������: ���� Release �� �������� ��������"));
	}
}

void RDOPROCRelease::addResource(CREF(tstring) name)
{
	ASSERT(!name.empty());
	m_resourceList.push_back(name);
}

// --------------------------------------------------------------------------------
// -------------------- RDOPROCAdvance
// --------------------------------------------------------------------------------
RDOPROCAdvance::RDOPROCAdvance(CREF(LPRDOPROCProcess) pProcess, CREF(tstring) name, CREF(rdoRuntime::LPRDOCalc) pTimeCalc)
	: RDOPROCOperator(pProcess, name)
{
	m_pRuntime = RF(rdoRuntime::RDOPROCAdvance)::create(RDOParser::s_parser()->getLastPROCProcess()->getRunTime(), pTimeCalc);
	ASSERT(m_pRuntime);
}

// --------------------------------------------------------------------------------
// -------------------- RDOPROCTerminate
// --------------------------------------------------------------------------------
RDOPROCTerminate::RDOPROCTerminate(CREF(LPRDOPROCProcess) pProcess, CREF(tstring) name, CREF(ruint) term)
	: RDOPROCOperator(pProcess, name)
{
	m_pRuntime = RF(rdoRuntime::RDOPROCTerminate)::create(RDOParser::s_parser()->getLastPROCProcess()->getRunTime(), term);
	ASSERT(m_pRuntime);
}

// --------------------------------------------------------------------------------
// -------------------- RDOPROCAssign
// --------------------------------------------------------------------------------
RDOPROCAssign::RDOPROCAssign(CREF(LPRDOPROCProcess) pProcess, CREF(tstring) name, CREF(rdoRuntime::LPRDOCalc) pValue, int resID, int paramID)
	: RDOPROCOperator(pProcess, name)
{
	m_pRuntime = RF(rdoRuntime::RDOPROCAssign)::create(RDOParser::s_parser()->getLastPROCProcess()->getRunTime(), pValue, resID, paramID);
	ASSERT(m_pRuntime);
}

CLOSE_RDO_PARSER_NAMESPACE
