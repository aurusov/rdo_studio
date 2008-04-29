#ifndef SEARCHTREE_H
#define SEARCHTREE_H

#include "rdo_logic_dptsearch.h"

namespace rdoRuntime
{

class TreeNode;

// ----------------------------------------------------------------------------
// ---------- TreeRoot - корень дерева DPT
// ----------------------------------------------------------------------------
// С помощью createRootTreeNode создает реальный узер-корень
// ----------------------------------------------------------------------------
class TreeRoot
{
public:
	virtual ~TreeRoot() {}

	virtual void createRootTreeNode( RDOSimulator* sim ) = 0;

	std::vector< TreeNode* > m_OPEN;
	RDODPTSearch*            m_dp;
	TreeNode*                m_rootNode;
	TreeNode*                m_targetNode;
	RDOSimulator*            m_theRealSimulator;
	int                      m_nodesInGraphCount;
	int                      m_expandedNodesCount;
	int                      m_fullNodesCount;
	SYSTEMTIME               m_systime_begin;
	unsigned int             m_sizeof_dpt;

	int getNodesCound() const { return m_nodesCount; }
	int getNewNodeNumber() {
		return ++m_nodesCount;
	}

protected:
	TreeRoot( RDOSimulator* sim, RDODPTSearch* _dp);

private:
	int m_nodesCount;
};

// ----------------------------------------------------------------------------
// ---------- TreeNode - узел графа DPT
// ----------------------------------------------------------------------------
class TreeNode
{
public:
	virtual ~TreeNode();

	RDOSimulator*            m_sim;
	std::vector< TreeNode* > m_children;
	TreeNode*                m_parent;
	TreeRoot*                m_root;
	RDODPTSearch::Activity*  m_activity; // активность (currAct), которую применил предок при создании this
	double                   m_costRule;
	double                   m_costPath;
	double                   m_costRest;
	int                      m_number; // Номер узла

	void ExpandChildren();

	enum NodeFoundInfo {
		nfi_notfound     = 0,
		nfi_found_better = 1,
		nfi_found_loser  = 2
	};

	NodeFoundInfo CheckIfExistBetter( RDOSimulator* childSim, double useCost, TreeNode** better ); // return 0 - no such simulator, 1 - exist better, 2 - exist not better
	void ReCostSubTree( double cost );

	int getActivityID() const;

protected:
	TreeNode( RDOSimulator* _sim, TreeNode* _parent, TreeRoot* _root, RDODPTSearch::Activity* _activity, double cost, int cnt );

	RDODPTSearch::Activity* m_currAct; // вершина пытается применять различные активности
	RDOSimulator*           m_childSim;

	double m_newCostPath;
	double m_newCostRest;
	double m_newCostRule;

	virtual void onSearchOpenNode( RDOSimulator* sim )         {}
	virtual void onSearchNodeInfoDeleted( RDOSimulator* sim )  {}
	virtual void onSearchNodeInfoReplaced( RDOSimulator* sim ) {}
	virtual void onSearchNodeInfoNew( RDOSimulator* sim )      {}
	virtual TreeNode* createChildTreeNode();
};

// ----------------------------------------------------------------------------
// ---------- compareNodes
// ----------------------------------------------------------------------------
// функция сравнения вершин графа для сортировки списка OPEN
// ----------------------------------------------------------------------------
inline bool compareNodes( const TreeNode* tn1, const TreeNode* tn2 )
{
	if ( fabs(tn1->m_costRest - tn2->m_costRest) > 0.0000001 ) {
		return ( tn1->m_costRest < tn2->m_costRest );
	} else {
		return ( tn1->m_number < tn2->m_number );
	}
}

} // namespace rdoRuntime

#endif // SEARCHTREE_H
