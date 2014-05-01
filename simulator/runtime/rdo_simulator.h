/*!
  \copyright (c) RDO-Team, 2011
  \file      rdo_simulator.h
  \authors   Барс Александр
  \authors   Урусов Андрей (rdo@rk9.bmstu.ru)
  \date      19.04.2008
  \brief     Симулятор РДО
  \indent    4T
*/

#ifndef _LIB_RUNTIME_SIMULATOR_H_
#define _LIB_RUNTIME_SIMULATOR_H_

// ----------------------------------------------------------------------- INCLUDES
// ----------------------------------------------------------------------- SYNOPSIS
#include "simulator/runtime/rdo.h"
#include "simulator/runtime/rdobase.h"
#include "simulator/runtime/rdo_logic_i.h"
// --------------------------------------------------------------------------------

OPEN_RDO_RUNTIME_NAMESPACE

/*!
  \class     RDOSimulator
  \brief     Один из базовых классов для RDORuntime
*/
class RDOSimulator: public RDOSimulatorBase
{
public:
	RDOSimulator();
	virtual ~RDOSimulator();

	void appendLogic(const LPIBaseOperation& pLogic, LPIBaseOperationContainer pParent);
	LPIBaseOperation getMustContinueOpr() const;
	void setMustContinueOpr(const LPIBaseOperation& pOperation);
	virtual void onPutToTreeNode() = 0;

	std::string writeActivitiesStructure(std::size_t& counter);

	std::size_t getSizeofSim() const;

	LPIBaseOperationContainer m_pMetaLogic;

protected:
	void appendBaseOperation(LPIBaseOperationContainer pLogic, const LPIBaseOperation& pBaseOperation);

	// Инициализирует нерегулярные события и блоки GENERATE: задает время первого срабатывания
	virtual void preProcess();

	virtual void onResetResult     () = 0;
	virtual void onCheckResult     () = 0;
	virtual void onAfterCheckResult() = 0;

	std::size_t m_sizeofSim;

private:
	LPIBaseOperation m_pOprMustContinue;

	virtual bool doOperation();
};

CLOSE_RDO_RUNTIME_NAMESPACE

#include "simulator/runtime/rdo_simulator-inl.h"

#endif // _LIB_RUNTIME_SIMULATOR_H_
