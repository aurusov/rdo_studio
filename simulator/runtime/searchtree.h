#pragma once

// ----------------------------------------------------------------------- INCLUDES
#include <boost/date_time/posix_time/posix_time.hpp>
// ----------------------------------------------------------------------- SYNOPSIS
#include "simulator/runtime/rdo_logic_dptsearch.h"
#include "simulator/runtime/rdo_dptsearch_activity_i.h"
// --------------------------------------------------------------------------------

OPEN_RDO_RUNTIME_NAMESPACE

class TreeNode;

class TreeRoot
{
public:
	virtual ~TreeRoot();

	virtual void createRootTreeNode(const LPRDORuntime& pRuntime) = 0;

	std::vector<TreeNode*> m_OPEN;
	RDODPTSearch* m_dp;
	TreeNode* m_rootNode;
	TreeNode* m_targetNode;
	LPRDORuntime m_theRealSimulator;
	int m_nodesInGraphCount;
	int m_expandedNodesCount;
	int m_fullNodesCount;
	boost::posix_time::ptime m_ptime;
	std::size_t m_sizeof_dpt;

	int getNodesCound() const;
	int getNewNodeNumber();

protected:
	TreeRoot(const LPRDORuntime& pRuntime, RDODPTSearch* pDP);

private:
	int m_nodesCount;
};

class TreeNode
{
public:
	virtual ~TreeNode();

	LPRDORuntime           m_pRuntime;
	std::vector<TreeNode*> m_children;
	TreeNode*              m_parent;
	TreeRoot*              m_root;
	LPIDPTSearchActivity   m_activity; // активность (currAct), которую применил предок при создании this
	double                 m_costRule;
	double                 m_costPath;
	double                 m_costRest;
	int                    m_number; // Номер узла

	void ExpandChildren();

		enum NodeFoundInfo
	{
		nfi_notfound     = 0,
		nfi_found_better = 1,
		nfi_found_loser  = 2
	};

	// TODO задокументировать функцию
	// return 0 - no such simulator, 1 - exist better, 2 - exist not better
	NodeFoundInfo CheckIfExistBetter(const LPRDORuntime& pChildRuntime, double useCost, TreeNode** better );
	void ReCostSubTree(double cost);

protected:
	TreeNode(const LPRDORuntime& pRuntime, TreeNode* pParent, TreeRoot* pRoot, LPIDPTSearchActivity pActivity, double cost, int cnt);

	LPIDPTSearchActivity  m_currAct; // вершина пытается применять различные активности
	LPRDORuntime          m_pChildRuntime;

	double m_newCostPath;
	double m_newCostRest;
	double m_newCostRule;

	virtual void      onSearchOpenNode        (const LPRDORuntime& pRuntime);
	virtual void      onSearchNodeInfoDeleted (const LPRDORuntime& pRuntime);
	virtual void      onSearchNodeInfoReplaced(const LPRDORuntime& pRuntime);
	virtual void      onSearchNodeInfoNew     (const LPRDORuntime& pRuntime);
	virtual TreeNode* createChildTreeNode     ();
};

CLOSE_RDO_RUNTIME_NAMESPACE
