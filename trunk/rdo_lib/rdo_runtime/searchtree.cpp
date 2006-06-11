#include "pch.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

#include "searchtree.h"

bool RDODecisionPoint::RunSearchInTree(RDOSimulator *sim)
{
   onSearchBegin(sim);
   TreeRoot *treeRoot = createTreeRoot(sim);
   treeRoot->createRootTreeNode(sim->createCopy());

   for(;;)
   {
      TreeNode *curr = *(treeRoot->allLeafs.begin());     // the first is the best
      curr->ExpandChildren();
      if(treeRoot->allLeafs.empty() || treeRoot->targetNode)
         break;
   }

   bool success = !!treeRoot->targetNode;
   if(success)
   {
      std::list<TreeNode *> bestPath;
      for(TreeNode *i = treeRoot->targetNode; i->parent; i = i->parent)
         bestPath.push_front(i);
							  
      onSearchDecisionHeader(treeRoot->theRealSimulator);
      for(std::list<TreeNode *>::iterator ii = bestPath.begin(); ii != bestPath.end(); ii++)
      {
         TreeNode *node = (*ii);
		   node->activity->rule->onBeforeChoiceFrom(treeRoot->theRealSimulator);
         node->activity->rule->choiceFrom(treeRoot->theRealSimulator);
         node->activity->rule->onBeforeRule(treeRoot->theRealSimulator);
         node->activity->rule->convertRule(treeRoot->theRealSimulator);
         node->activity->rule->onAfterRule(treeRoot->theRealSimulator, true);
         onSearchDecision(treeRoot->theRealSimulator, node);
      }

      onSearchResultSuccess(treeRoot->theRealSimulator, treeRoot);
   }
	else
      onSearchResultNotFound(treeRoot->theRealSimulator, treeRoot);


   delete treeRoot->rootNode;
   delete treeRoot;

   return success;
}

TreeNode::TreeNode(RDOSimulator *i_sim, TreeNode *i_parent, TreeRoot *i_root, RDOActivity *i_activity, 
   double cost, int cnt): 
   sim(i_sim),
   parent(i_parent),
   root(i_root),
   activity(i_activity),
   costPath(cost),
   count(cnt) 
{
	sim->onPutToTreeNode();
}

TreeNode::~TreeNode()
{
   for(std::vector<TreeNode *>::iterator i = children.begin(); i != children.end(); i++)
   {
      delete (*i);
   }

   delete sim;
}

void TreeNode::ExpandChildren()
{
   DeleteAllObjects(children);
   onSearchOpenNode(root->theRealSimulator);

   if(root->dp->TermCondition(sim))
   {
      root->targetNode = this;
		return;
   }

   root->expandedNodesCount ++;
   for(std::list<RDOActivity *>::iterator i = root->dp->activities.begin(); 
   i != root->dp->activities.end(); i++)
   {
      currAct = (*i);
      childSim = sim->createCopy();
	   currAct->rule->onBeforeChoiceFrom(childSim);
      if(!currAct->rule->choiceFrom(childSim))
         delete childSim;
      else
      {
         root->fullNodesCount++;

			if(!currAct->valueAfter)
				newCostRule = currAct->costOfRule(childSim);

         currAct->rule->onBeforeRule(childSim);
         currAct->rule->convertRule(childSim);
         currAct->rule->onAfterRule(childSim, true);

			if(currAct->valueAfter)
				newCostRule = currAct->costOfRule(childSim);

         newCostPath = costPath + newCostRule;
         newCostRest = newCostPath + root->dp->EvaluateBy(childSim);

         if(root->dp->NeedCompareTops())
         {
            TreeNode *better;
            int res = root->rootNode->CheckIfExistBetter(childSim, newCostPath, &better);
            if(res == 1)
            {
               onSearchNodeInfoDeleted(root->theRealSimulator);
               delete childSim;
               continue;
            }
            else if(res == 2)
            {
               better->parent->children.erase(std::find(better->parent->children.begin(), better->parent->children.end(), better));
               better->ReCostSubTree(costPath);
               better->parent = this;
					better->count = root->nodeCount++;
					better->costPath = newCostPath;
					better->costRest = newCostRest;
					better->costRule = newCostRule;
					better->currAct = currAct;
               better->onSearchNodeInfoReplaced(root->theRealSimulator);
               children.push_back(better);
               delete childSim;
               continue;
            }
         }

         TreeNode *tn = createChildTreeNode();
			root->nodesInGraphCount++;
			tn->costPath = newCostPath;
			tn->costRest = newCostRest;
			tn->costRule = newCostRule;
	      tn->currAct = currAct;

         tn->onSearchNodeInfoNew(root->theRealSimulator);
         children.push_back(tn);
/*
         if(!root->dp->TermCondition(childSim))
            root->allLeafs.push_back(tn);
         else
         {
            root->targetNode = tn;
         }
*/
         root->allLeafs.push_back(tn);

      }
   }

   root->allLeafs.erase(std::find(root->allLeafs.begin(), root->allLeafs.end(), this));
   std::sort(root->allLeafs.begin(), root->allLeafs.end(), compareNodes);
}

int TreeNode::CheckIfExistBetter(RDOSimulator *childSim, double useCost, TreeNode **better)
{
   if(*childSim == *sim)
   {
      if(costPath <= useCost)
      {
         return 1;
      }
      else
      {
         *better = this;
         return 2;
      }
   }

   for(std::vector<TreeNode *>::iterator i = children.begin(); i != children.end(); i++)
   {
      int res = (*i)->CheckIfExistBetter(childSim, useCost, better);
      if(res != 0)
         return res;
   }

   return 0;
}

void TreeNode::ReCostSubTree(double cost)
{
   costPath = cost + costRule;

   for(std::vector<TreeNode *>::iterator i = children.begin(); i != children.end(); i++)
   {
      (*i)->ReCostSubTree(costPath);
   }
}

TreeNode *TreeNode::createChildTreeNode()
{
   return new TreeNode(childSim, this, root, currAct, costPath, root->nodeCount++);
}

void TreeRoot::createRootTreeNode(RDOSimulator *sim)
{
   rootNode = new TreeNode(sim, NULL, this, NULL, 0, nodeCount++);
   allLeafs.push_back(rootNode);
}

TreeRoot::TreeRoot(RDOSimulator *sim, RDODecisionPoint *_dp):
   dp(_dp)
{
   nodeCount = 1;
	nodesInGraphCount = 1;
   expandedNodesCount = 0;
   fullNodesCount = 0;
   targetNode = NULL;
   theRealSimulator = sim;
}
