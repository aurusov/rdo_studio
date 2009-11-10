/*
 * copyright: (c) RDO-Team, 2009
 * filename : rdo_logic_dptsearch.h
 * author   : Урусов Андрей
 * date     : 
 * bref     : 
 * indent   : 4T
 */

#ifndef _RDO_LOGIC_DPTSEARCH_H_
#define _RDO_LOGIC_DPTSEARCH_H_

// ====================================================================== INCLUDES
// ====================================================================== SYNOPSIS
#include <namespace.h>
#include "rdo_logic.h"
#include "rdo_priority.h"
#include "rdocalc.h"
#include "rdo_dptsearch_activity_interface.h"
#include "rdo_logic_dptsearch_interface.h"
// ===============================================================================

OPEN_RDO_RUNTIME_NAMESPACE

class RDOSimulator;
class TreeNode;
class TreeRoot;

// ----------------------------------------------------------------------------
// ---------- RDODPTSearch
// ----------------------------------------------------------------------------
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
	RDODPTSearch( RDOSimulator* sim );
	virtual ~RDODPTSearch();

	typedef std::list<LPIDPTSearchActivity> ActivityList;
	ActivityList   m_activityList;

	virtual bool   TermCondition( RDOSimulator* sim )                              = 0;
	virtual double EvaluateBy( RDOSimulator* sim )                                 = 0;
	virtual void   onSearchBegin( RDOSimulator* sim )                              = 0;
	virtual void   onSearchDecisionHeader( RDOSimulator* sim )                     = 0;
	virtual void   onSearchDecision( RDOSimulator* sim, TreeNode* node )           = 0;
	virtual void   onSearchResultSuccess( RDOSimulator* sim, TreeRoot* treeRoot )  = 0;
	virtual void   onSearchResultNotFound( RDOSimulator* sim, TreeRoot* treeRoot ) = 0;
	virtual bool   NeedCompareTops()                                               = 0;
	virtual TreeRoot* createTreeRoot( RDOSimulator* sim )                          = 0;
	virtual BOResult  onContinue( RDOSimulator* sim );

private:
	TreeRoot* treeRoot;
	virtual BOResult onDoOperation( RDOSimulator* sim );

	DECLARE_IDPTSearchLogic;
};

CLOSE_RDO_RUNTIME_NAMESPACE

#endif // _RDO_LOGIC_DPTSEARCH_H_
