/*
 * copyright: (c) RDO-Team, 2009
 * filename : rdo_logic.h
 * author   : Урусов Андрей
 * date     : 
 * bref     : 
 * indent   : 4T
 */

#ifndef _RDO_LOGIC_H_
#define _RDO_LOGIC_H_

#pragma warning(disable : 4786)

// ====================================================================== INCLUDES
// ====================================================================== SYNOPSIS
#include <namespace.h>
#include "rdo.h"
#include "rdocalc.h"
#include "rdo_runtime_interface_registrator.h"
#include "rdo_logic_interface.h"
// ===============================================================================

OPEN_RDO_RUNTIME_NAMESPACE

// ----------------------------------------------------------------------------
// ---------- RDOLogic
// ----------------------------------------------------------------------------
class RDOLogic: public IBaseOperation, public IBaseOperationContainer, public ILogic, CAST_TO_UNKNOWN
{
DEFINE_FACTORY(RDOLogic)

QUERY_INTERFACE_BEGIN
	QUERY_INTERFACE(IBaseOperation)
	QUERY_INTERFACE(IBaseOperationContainer)
	QUERY_INTERFACE(ILogic)
QUERY_INTERFACE_END

protected:
	RDOLogic ();
	RDOLogic (PTR(RDOSimulator) sim);
	virtual ~RDOLogic()
	{}

protected:
	DECLARE_IBaseOperationContainer;

	PTR(RDOCalc)               m_condition;
	rbool                      m_lastCondition;
	LPIBaseOperationContainer  m_childList;

private:
	virtual void actionWithRDOOprContainer(PTR(RDOSimulator) sim);

	rbool checkSelfCondition(PTR(RDOSimulator) sim);
	void  start             (PTR(RDOSimulator) sim);
	void  stop              (PTR(RDOSimulator) sim);

	DECLARE_IBaseOperation;
	DECLARE_ILogic;
};

CLOSE_RDO_RUNTIME_NAMESPACE

#endif //! _RDO_LOGIC_H_
