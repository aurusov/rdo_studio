#ifndef RDO_LOGIC_DPTSEARCH_H
#define RDO_LOGIC_DPTSEARCH_H

#include "rdo_logic.h"
#include "rdocalc.h"
#include "rdo_dptsearch_activity_interface.h"
#include "rdo_logic_dptsearch_interface.h"

namespace rdoRuntime {

class RDOSimulator;
class TreeNode;
class TreeRoot;

// ----------------------------------------------------------------------------
// ---------- RDODPTSearch
// ----------------------------------------------------------------------------
class RDODPTSearch: public RDOLogic, public IDPTSearchLogic
{
QUERY_INTERFACE_BEGIN
QUERY_INTERFACE_PARENT(RDOLogic)
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

} // namespace rdoRuntime

#endif // RDO_LOGIC_DPTSEARCH_H
