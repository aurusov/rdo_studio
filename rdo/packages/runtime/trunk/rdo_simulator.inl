/*!
  \copyright (c) RDO-Team, 2011
  \file      rdo_simulator.inl
  \author    Лущан Дмитрий (dluschan@rk9.bmstu.ru)
  \date      06.08.2011
  \brief     Симулятор РДО
  \indent    4T
*/

// ----------------------------------------------------------------------- INCLUDES
// ----------------------------------------------------------------------- SYNOPSIS
#include "simulator/runtime/rdo.h"
#include "simulator/runtime/rdobase.h"
#include "simulator/runtime/rdo_logic_i.h"
// --------------------------------------------------------------------------------

OPEN_RDO_RUNTIME_NAMESPACE

// --------------------------------------------------------------------------------
// -------------------- RDOSimulator
// --------------------------------------------------------------------------------
inline LPIBaseOperation RDOSimulator::getMustContinueOpr() const
{
	return m_pOprMustContinue;
}

inline void RDOSimulator::setMustContinueOpr(CREF(LPIBaseOperation) pOperation)
{
	m_pOprMustContinue = pOperation;
}

inline ruint RDOSimulator::getSizeofSim() const
{
	return m_sizeofSim;
}

inline void RDOSimulator::appendBaseOperation(LPIBaseOperationContainer pLogic, CREF(LPIBaseOperation) pBaseOperation)
{
	ASSERT(pLogic        );
	ASSERT(pBaseOperation);
	pLogic->append(pBaseOperation);
}

CLOSE_RDO_RUNTIME_NAMESPACE
