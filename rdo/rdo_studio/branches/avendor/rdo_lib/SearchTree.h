
#ifndef SEARCH_TREE
#define SEARCH_TREE

#include "Rdo.h"

class TreeNode;
class RDOSimulator;
class RDOActivity;
class RDODecisionPoint;

class TreeRoot
{
public:
   virtual void createRootTreeNode(RDOSimulator *sim);
   TreeRoot(RDOSimulator *sim, RDODecisionPoint *_dp);
   virtual ~TreeRoot() {}

   RDODecisionPoint *dp;
   TreeNode *rootNode;
   std::vector<TreeNode *> allLeafs;
   TreeNode *targetNode;
   RDOSimulator *theRealSimulator;       // all others are copy
   int nodeCount;
   int expandedNodesCount;
   int fullNodesCount;
   int nodesInGraphCount;
};

class TreeNode
{
protected:

   RDOActivity *currAct;
   RDOSimulator *childSim;
	
	double newCostPath; 
	double newCostRest;
	double newCostRule;

   virtual void onSearchOpenNode(RDOSimulator *sim) {}
   virtual void onSearchNodeInfoDeleted(RDOSimulator *sim) {}
   virtual void onSearchNodeInfoReplaced(RDOSimulator *sim) {}
   virtual void onSearchNodeInfoNew(RDOSimulator *sim) {}
   virtual TreeNode *createChildTreeNode();

public:
   RDOSimulator *sim;
   std::vector<TreeNode *> children;
   TreeNode *parent;
   TreeRoot *root;
   RDOActivity *activity;
   double costRule;
   double costPath;
   double costRest;
   int count;

public:
   TreeNode(RDOSimulator *i_sim, TreeNode *i_parent, TreeRoot *i_root, RDOActivity *i_activity, double cost, int cnt);
   virtual ~TreeNode();
   void ExpandChildren();
   int CheckIfExistBetter(RDOSimulator *childSim, double useCost, TreeNode **better); // return 0 - no such simulator, 1 - exist better, 2 - exist not better
   void ReCostSubTree(double cost);
};


inline bool compareNodes(const TreeNode *tn1, const TreeNode *tn2) 
{
   if(fabs(tn1->costRest - tn2->costRest) > 0.0000001)
      return (tn1->costRest < tn2->costRest); 
   else
      return (tn1->count < tn2->count); 
}

#endif //SEARCH_TREE
