/******************************************************************************//**
 * @copyright (c) RDO-Team, 2008
 * @file      rdo_logic_dptsearch.h
 * @author    ������ ������
 * @date      29.04.2008
 * @brief     DPTSearch
 * @indent    4T
 *********************************************************************************/

#ifndef _LIB_RUNTIME_LOGIC_DPTSEARCH_H_
#define _LIB_RUNTIME_LOGIC_DPTSEARCH_H_

// *********************************************************************** INCLUDES
// *********************************************************************** SYNOPSIS
#include "rdo_common/namespace.h"
#include "rdo_lib/rdo_runtime/rdo_logic.h"
#include "rdo_lib/rdo_runtime/rdo_priority.h"
#include "rdo_lib/rdo_runtime/rdocalc.h"
#include "rdo_lib/rdo_runtime/rdo_dptsearch_activity_i.h"
#include "rdo_lib/rdo_runtime/rdo_logic_dptsearch_i.h"
// ********************************************************************************

OPEN_RDO_RUNTIME_NAMESPACE

class TreeNode;
class TreeRoot;

/******************************************************************************//**
 * @class   RDODPTSearch
 * @brief   unknown
 *********************************************************************************/
class RDODPTSearch: public RDOLogicSimple, public IDPTSearchLogic, public RDOPatternPrior
{
QUERY_INTERFACE_BEGIN
QUERY_INTERFACE_PARENT(RDOLogic)
QUERY_INTERFACE_PARENT(RDOPatternPrior)
QUERY_INTERFACE(IDPTSearchLogic)
QUERY_INTERFACE_END

friend class RDOSimulator;
friend class TreeNode;

protected:
	RDODPTSearch(CREF(LPRDORuntime) pRuntime, LPIBaseOperationContainer pParent = NULL);
	virtual ~RDODPTSearch();

	typedef std::list<LPIDPTSearchActivity> ActivityList;
	ActivityList   m_activityList;

	virtual rbool         TermCondition         (CREF(LPRDORuntime) pRuntime)                          = 0;
	virtual double        EvaluateBy            (CREF(LPRDORuntime) pRuntime)                          = 0;
	virtual void          onSearchBegin         (CREF(LPRDORuntime) pRuntime)                          = 0;
	virtual void          onSearchDecisionHeader(CREF(LPRDORuntime) pRuntime)                          = 0;
	virtual void          onSearchDecision      (CREF(LPRDORuntime) pRuntime, PTR(TreeNode) node )     = 0;
	virtual void          onSearchResultSuccess (CREF(LPRDORuntime) pRuntime, PTR(TreeRoot) treeRoot ) = 0;
	virtual void          onSearchResultNotFound(CREF(LPRDORuntime) pRuntime, PTR(TreeRoot) treeRoot ) = 0;
	virtual rbool         NeedCompareTops       ()                                                     = 0;
	virtual PTR(TreeRoot) createTreeRoot        (CREF(LPRDORuntime) pRuntime)                          = 0;
	virtual BOResult      onContinue            (CREF(LPRDORuntime) pRuntime);

private:
	PTR(TreeRoot) treeRoot;
	virtual BOResult onDoOperation(CREF(LPRDORuntime) pRuntime);

	DECLARE_IDPTSearchLogic;
};

CLOSE_RDO_RUNTIME_NAMESPACE

#endif // _LIB_RUNTIME_LOGIC_DPTSEARCH_H_
