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
CLASS(RDOSimulator): INSTANCE_OF (RDOSimulatorBase)
{
public:
	RDOSimulator();
	virtual ~RDOSimulator();

	void             appendLogic       (CREF(LPIBaseOperation) pLogic, LPIBaseOperationContainer pParent);
	LPIBaseOperation getMustContinueOpr() const;
	void             setMustContinueOpr(CREF(LPIBaseOperation) pOperation);
	virtual void     onPutToTreeNode   () = 0;

	tstring          writeActivitiesStructure(REF(ruint) counter);

	/// @todo Найти к чему относится этот комментарий
	// Для DPT необходимо перекрыть две нижеследующие функции:
	// 1. Создает клон RDOSimulator с копиями всех ресурсов, но не более
	// virtual PTR(RDOSimulator) clone()                   = 0;
	// 2. Сравнение двух симуляторов по ресурсам
	// virtual rbool operator== (CREF(RDOSimulator) other) = 0;

	ruint getSizeofSim() const;

	LPIBaseOperationContainer m_pMetaLogic;

protected:
	void appendBaseOperation(LPIBaseOperationContainer pLogic, CREF(LPIBaseOperation) pBaseOperation);

	// Инициализирует нерегулярные события и блоки GENERATE: задает время первого срабатывания
	virtual void preProcess();

	virtual void onResetResult     () = 0;
	virtual void onCheckResult     () = 0;
	virtual void onAfterCheckResult() = 0;

	ruint m_sizeofSim;

private:
	LPIBaseOperation m_pOprMustContinue;

	virtual rbool doOperation();
};

CLOSE_RDO_RUNTIME_NAMESPACE

#include "simulator/runtime/rdo_simulator.inl"

#endif // _LIB_RUNTIME_SIMULATOR_H_
