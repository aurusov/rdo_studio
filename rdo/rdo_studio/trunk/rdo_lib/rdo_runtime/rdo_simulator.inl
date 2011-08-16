/******************************************************************************//**
 * @copyright (c) RDO-Team, 2011
 * @file      rdo_simulator.inl
 * @author    Лущан Дмитрий
 * @date      06.08.2011
 * @brief     Симулятор РДО
 * @indent    4T
 *********************************************************************************/

// ----------------------------------------------------------------------- INCLUDES
// ----------------------------------------------------------------------- SYNOPSIS
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

inline void RDOSimulator::appendBaseOperation(LPIBaseOperationContainer logic, CREF(LPIBaseOperation) op)
{
	ASSERT(op);
	ASSERT(logic);
	logic->append(op);
}

CLOSE_RDO_RUNTIME_NAMESPACE
