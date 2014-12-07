// ---------------------------------------------------------------------------- PCH
#include "simulator/compiler/parser/pch.h"
// ----------------------------------------------------------------------- INCLUDES
// ----------------------------------------------------------------------- SYNOPSIS
#include "simulator/compiler/parser/rdoparser.h"
#include "simulator/compiler/parser/rdo_logic_base.h"
#include "simulator/runtime/rdo_priority.h"
// --------------------------------------------------------------------------------

OPEN_RDO_PARSER_NAMESPACE

// --------------------------------------------------------------------------------
// -------------------- RDOLogicBase
// --------------------------------------------------------------------------------
RDOLogicBase::RDOLogicBase(const RDOParserSrcInfo& src_info)
	: RDOParserSrcInfo(src_info)
	, m_multithreading(false   )
{}

RDOLogicBase::~RDOLogicBase()
{}

const std::string& RDOLogicBase::name() const
{
	return src_info().src_text();
}

bool RDOLogicBase::setPrior(LPRDOFUNArithm& pPrior)
{
	LPIPriority pPriority = m_pRuntimeLogic.object_dynamic_cast<IPriority>();
	if (pPriority)
	{
		return pPriority->setPrior(pPrior->createCalc());
	}
	return false;
}

bool RDOLogicBase::getMultithreading() const
{
	return m_multithreading;
}

void RDOLogicBase::setMultithreading(bool multithreading)
{
	m_multithreading = multithreading;
}

void RDOLogicBase::setCondition(const LPRDOFUNLogic& pConditon)
{
	m_pConditon = pConditon;
}

LPRDOFUNLogic RDOLogicBase::getConditon() const
{
	return m_pConditon;
}

LPILogic RDOLogicBase::getLogic() const
{
	return m_pRuntimeLogic;
}

void RDOLogicBase::end()
{
	if (getConditon())
	{
		m_pRuntimeLogic->setCondition(getConditon()->getCalc());
	}
	m_pRuntimeLogic->setMultithreading(m_multithreading);
	RDOParser::s_parser()->contextStack()->pop<RDOLogicBase>();
}

CLOSE_RDO_PARSER_NAMESPACE
