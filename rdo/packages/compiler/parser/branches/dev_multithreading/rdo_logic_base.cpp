/*!
  \copyright (c) RDO-Team, 2012
  \file      simulator/compiler/parser/rdo_logic_base.cpp
  \author    Клеванец Игорь (impus@hotbox.ru)
  \date      05.02.2012
  \brief     
  \indent    4T
*/

// ---------------------------------------------------------------------------- PCH
#include "simulator/compiler/parser/pch.h"
// ----------------------------------------------------------------------- INCLUDES
// ----------------------------------------------------------------------- SYNOPSIS
#include "simulator/compiler/parser/rdo_logic_base.h"
#include "simulator/compiler/parser/rdoparser.h"
// --------------------------------------------------------------------------------

OPEN_RDO_PARSER_NAMESPACE

// --------------------------------------------------------------------------------
// -------------------- RDOLogicBase
// --------------------------------------------------------------------------------

CREF(tstring) RDOLogicBase::name() const
{
	return src_info().src_text();
}

rbool RDOLogicBase::setPrior(REF(LPRDOFUNArithm) pPrior)
{
	LPIPriority pPriority = m_pRuntimeLogic;
	if (pPriority)
	{
		return pPriority->setPrior(pPrior->createCalc());
	}
	return false;
}

rbool RDOLogicBase::getMultithreading() const
{
	return m_multithreading;
}

void RDOLogicBase::setMultithreading (rbool multithreading)
{
	m_multithreading = multithreading;
}

CLOSE_RDO_PARSER_NAMESPACE
