#include "pch.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

#include "searchtree.h"

bool RDODecisionPoint::RunSearchInTree(RDOSimulator *sim)
{
	onSearchBegin( sim );
	TreeRoot *treeRoot = createTreeRoot(sim);
	treeRoot->createRootTreeNode( sim->createCopy() );

	for (;;) {
		TreeNode* curr = *(treeRoot->allLeafs.begin());     // the first is the best
		curr->ExpandChildren();
		if ( treeRoot->allLeafs.empty() || treeRoot->targetNode ) break;
	}

	bool success = !!treeRoot->targetNode;
	if ( success ) {
		std::list< TreeNode* > bestPath;
		for ( TreeNode* i = treeRoot->targetNode; i->parent; i = i->parent ) {
			bestPath.push_front(i);
		}
		onSearchDecisionHeader( treeRoot->theRealSimulator );
		for ( std::list< TreeNode* >::iterator ii = bestPath.begin(); ii != bestPath.end(); ii++ ) {
			TreeNode* node = (*ii);
			node->activity->rule->onBeforeChoiceFrom( treeRoot->theRealSimulator );
			node->activity->rule->choiceFrom( treeRoot->theRealSimulator );
			node->activity->rule->onBeforeRule( treeRoot->theRealSimulator );
			node->activity->rule->convertRule( treeRoot->theRealSimulator );
			node->activity->rule->onAfterRule( treeRoot->theRealSimulator, true );
			onSearchDecision( treeRoot->theRealSimulator, node );
		}
		onSearchResultSuccess( treeRoot->theRealSimulator, treeRoot );
	} else {
		onSearchResultNotFound( treeRoot->theRealSimulator, treeRoot );
	}
	delete treeRoot->rootNode;
	delete treeRoot;
	return success;
}

TreeNode::TreeNode( RDOSimulator* i_sim, TreeNode* i_parent, TreeRoot* i_root, RDOActivity* i_activity, double cost, int cnt ):
	sim( i_sim ),
	parent( i_parent ),
	root( i_root ),
	activity( i_activity ),
	costPath( cost ),
	count( cnt )
{
	sim->onPutToTreeNode();
}

TreeNode::~TreeNode()
{
	for( std::vector<TreeNode*>::iterator i = children.begin(); i != children.end(); i++ ) {
		delete (*i);
		root->sizeof_dpt -= sizeof(TreeNode);
	}
	root->sizeof_dpt -= sim->sizeof_sim;
	delete sim;
}

void TreeNode::ExpandChildren()
{
	root->sizeof_dpt -= (sim->sizeof_sim + sizeof(TreeNode)) * children.size();
	DeleteAllObjects(children);

	onSearchOpenNode(root->theRealSimulator);

	if( root->dp->TermCondition(sim) ) {
		root->targetNode = this;
		return;
	}

	root->expandedNodesCount ++;
	for( std::list<RDOActivity *>::iterator i = root->dp->activities.begin(); i != root->dp->activities.end(); i++ ) {
		currAct = (*i);
		childSim = sim->createCopy();
		root->sizeof_dpt += childSim->sizeof_sim;
		currAct->rule->onBeforeChoiceFrom(childSim);
		if ( !currAct->rule->choiceFrom(childSim) ) {
			root->sizeof_dpt -= childSim->sizeof_sim;
			delete childSim;
		} else {
			root->fullNodesCount++;
			if ( !currAct->valueAfter ) {
				newCostRule = currAct->costOfRule(childSim);
			}
			currAct->rule->onBeforeRule(childSim);
			currAct->rule->convertRule(childSim);
			currAct->rule->onAfterRule(childSim, true);
			if ( currAct->valueAfter ) {
				newCostRule = currAct->costOfRule(childSim);
			}
			newCostPath = costPath + newCostRule;
			newCostRest = newCostPath + root->dp->EvaluateBy(childSim);

			if ( root->dp->NeedCompareTops() ) {
				TreeNode* better;
				int res = root->rootNode->CheckIfExistBetter( childSim, newCostPath, &better );
				if ( res == 1 ) {
					onSearchNodeInfoDeleted(root->theRealSimulator);
					root->sizeof_dpt -= childSim->sizeof_sim;
					delete childSim;
					continue;
				} else if ( res == 2 ) {
					unsigned int size1 = better->parent->children.size();
					better->parent->children.erase( std::find(better->parent->children.begin(), better->parent->children.end(), better) );
					unsigned int size2 = better->parent->children.size();
					root->sizeof_dpt -= (sim->sizeof_sim + sizeof(TreeNode)) * (size2 - size1);
					better->ReCostSubTree(costPath);
					better->parent = this;
					better->count = root->nodeCount++;
					better->costPath = newCostPath;
					better->costRest = newCostRest;
					better->costRule = newCostRule;
					better->currAct = currAct;
					better->onSearchNodeInfoReplaced(root->theRealSimulator);
					children.push_back(better);
					root->sizeof_dpt -= childSim->sizeof_sim;
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
			tn->onSearchNodeInfoNew( root->theRealSimulator );
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
	unsigned int size1 = root->allLeafs.size();
	root->allLeafs.erase( std::find(root->allLeafs.begin(), root->allLeafs.end(), this) );
	unsigned int size2 = root->allLeafs.size();
	root->sizeof_dpt -= (sim->sizeof_sim + sizeof(TreeNode)) * (size2 - size1);

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
	root->sizeof_dpt += sizeof( TreeNode );
	return new TreeNode(childSim, this, root, currAct, costPath, root->nodeCount++);
}

void TreeRoot::createRootTreeNode(RDOSimulator *sim)
{
	rootNode = new TreeNode(sim, NULL, this, NULL, 0, nodeCount++);
	allLeafs.push_back(rootNode);
	sizeof_dpt += sizeof( TreeNode ) + sizeof( void* );
}

TreeRoot::TreeRoot( RDOSimulator* sim, RDODecisionPoint* _dp ):
	dp( _dp ),
	nodeCount( 1 ),
	nodesInGraphCount( 1 ),
	expandedNodesCount( 0 ),
	fullNodesCount( 0 ),
	targetNode( NULL ),
	theRealSimulator( sim ),
	sizeof_dpt( 0 )
{
	::GetSystemTime( &systime_begin );
}
