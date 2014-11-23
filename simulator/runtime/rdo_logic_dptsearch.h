/*!
  \copyright (c) RDO-Team, 2011
  \file      rdo_logic_dptsearch.h
  \author    Урусов Андрей (rdo@rk9.bmstu.ru)
  \date      29.04.2008
  \brief     DPTSearch
  \indent    4T
*/

#ifndef _LIB_RUNTIME_LOGIC_DPTSEARCH_H_
#define _LIB_RUNTIME_LOGIC_DPTSEARCH_H_

// ----------------------------------------------------------------------- INCLUDES
// ----------------------------------------------------------------------- SYNOPSIS
#include "simulator/runtime/rdo_logic.h"
#include "simulator/runtime/rdo_priority.h"
#include "simulator/runtime/calc/calc_base.h"
#include "simulator/runtime/rdo_dptsearch_activity_i.h"
#include "simulator/runtime/rdo_logic_dptsearch_i.h"
// --------------------------------------------------------------------------------

OPEN_RDO_RUNTIME_NAMESPACE

class TreeNode;
class TreeRoot;

/*!
  \class   RDODPTSearch
  \brief   Точка принятия решений DPTSearch
*/
class RDODPTSearch: public RDOLogicSimple, public IDPTSearchLogic, public RDOPatternPrior
{
friend class RDOSimulator;
friend class TreeNode;

protected:
	RDODPTSearch(const LPRDORuntime& pRuntime, LPIBaseOperationContainer pParent = NULL);
	virtual ~RDODPTSearch();

	typedef std::list<LPIDPTSearchActivity> ActivityList;
	ActivityList   m_activityList;

	virtual bool      TermCondition         (const LPRDORuntime& pRuntime)                      = 0;
	virtual double    EvaluateBy            (const LPRDORuntime& pRuntime)                      = 0;
	virtual void      onSearchBegin         (const LPRDORuntime& pRuntime)                      = 0;
	virtual void      onSearchDecisionHeader(const LPRDORuntime& pRuntime)                      = 0;
	virtual void      onSearchDecision      (const LPRDORuntime& pRuntime, TreeNode* node )     = 0;
	virtual void      onSearchResultSuccess (const LPRDORuntime& pRuntime, TreeRoot* treeRoot ) = 0;
	virtual void      onSearchResultNotFound(const LPRDORuntime& pRuntime, TreeRoot* treeRoot ) = 0;
	virtual bool      NeedCompareTops       ()                                                  = 0;
	virtual TreeRoot* createTreeRoot        (const LPRDORuntime& pRuntime)                      = 0;
	virtual BOResult  onContinue            (const LPRDORuntime& pRuntime);

private:
	TreeRoot* treeRoot;
	virtual BOResult onDoOperation(const LPRDORuntime& pRuntime);

	DECLARE_IDPTSearchLogic;
};

CLOSE_RDO_RUNTIME_NAMESPACE

#endif // _LIB_RUNTIME_LOGIC_DPTSEARCH_H_
