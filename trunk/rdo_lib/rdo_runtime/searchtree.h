#ifndef SEARCHTREE_H
#define SEARCHTREE_H

#include "rdo.h"

namespace rdoRuntime
{

class TreeNode;
class RDOSimulator;
class RDOActivity;
class RDODecisionPoint;

// ----------------------------------------------------------------------------
// ---------- TreeRoot - корень дерева DPT
// ----------------------------------------------------------------------------
// С помощью createRootTreeNode создает реальный узер-корень
// ----------------------------------------------------------------------------
class TreeRoot
{
protected:
	TreeRoot( RDOSimulator* sim, RDODecisionPoint* _dp);

public:
	virtual ~TreeRoot() {}

	virtual void createRootTreeNode( RDOSimulator* sim ) = 0;

	std::vector< TreeNode* > allLeafs;
	RDODecisionPoint* dp;
	TreeNode*         rootNode;
	TreeNode*         targetNode;
	RDOSimulator*     theRealSimulator;       // all others are copy
	int               nodeCount;
	int               nodesInGraphCount;
	int               expandedNodesCount;
	int               fullNodesCount;
	SYSTEMTIME        systime_begin;
	unsigned int      sizeof_dpt;
};

// ----------------------------------------------------------------------------
// ---------- TreeNode - узел графа DPT
// ----------------------------------------------------------------------------
class TreeNode
{
protected:
	TreeNode( RDOSimulator* i_sim, TreeNode* i_parent, TreeRoot* i_root, RDOActivity* i_activity, double cost, int cnt );

	RDOActivity*  currAct;
	RDOSimulator* childSim;

	double newCostPath; 
	double newCostRest;
	double newCostRule;

	virtual void onSearchOpenNode( RDOSimulator* sim )         {}
	virtual void onSearchNodeInfoDeleted( RDOSimulator* sim )  {}
	virtual void onSearchNodeInfoReplaced( RDOSimulator* sim ) {}
	virtual void onSearchNodeInfoNew( RDOSimulator* sim )      {}
	virtual TreeNode* createChildTreeNode();

public:
	virtual ~TreeNode();

	RDOSimulator* sim;
	std::vector< TreeNode* > children;
	TreeNode*    parent;
	TreeRoot*    root;
	RDOActivity* activity;
	double costRule;
	double costPath;
	double costRest;
	int count;

public:
	void ExpandChildren();
	int CheckIfExistBetter( RDOSimulator* childSim, double useCost, TreeNode** better ); // return 0 - no such simulator, 1 - exist better, 2 - exist not better
	void ReCostSubTree( double cost );
};

// ----------------------------------------------------------------------------
// ---------- compareNodes
// ----------------------------------------------------------------------------
// функция сравнения вершин графа для сортировки списка OPEN
// ----------------------------------------------------------------------------
inline bool compareNodes( const TreeNode* tn1, const TreeNode* tn2 ) 
{
	if ( fabs(tn1->costRest - tn2->costRest) > 0.0000001 ) {
		return ( tn1->costRest < tn2->costRest );
	} else {
		return ( tn1->count < tn2->count );
	}
}

} // namespace rdoRuntime

#endif // SEARCHTREE_H
