/******************************************************************************//**
 * @copyright (c) RDO-Team, 2008
 * @file      rdo_logic_dptsearch.cpp
 * @author    ”русов јндрей
 * @date      29.04.2008
 * @brief     unknown
 * @indent    4T
 *********************************************************************************/

// **************************************************************************** PCH
#include "rdo_lib/rdo_runtime/pch.h"
// *********************************************************************** INCLUDES
// *********************************************************************** SYNOPSIS
#include "rdo_lib/rdo_runtime/rdo_logic_dptsearch.h"
#include "rdo_lib/rdo_runtime/searchtree.h"
#include "rdo_lib/rdo_runtime/rdo_rule.h"
#include "rdo_lib/rdo_runtime/rdo_runtime.h"
// ********************************************************************************

#pragma warning(disable : 4786)  

OPEN_RDO_RUNTIME_NAMESPACE

// ********************************************************************************
// ******************** RDODPTSearch
// ********************************************************************************
RDODPTSearch::RDODPTSearch(CREF(LPRDORuntime) pRuntime, LPIBaseOperationContainer pParent)
	: RDOLogicSimple(pRuntime, pParent)
	, treeRoot      (NULL)
{}

RDODPTSearch::~RDODPTSearch()
{}

IBaseOperation::BOResult RDODPTSearch::onDoOperation(CREF(LPRDORuntime) pRuntime)
{
	// Ќачало поиска: вывели трасировку, обновили статистику
	onSearchBegin(pRuntime);
	treeRoot = createTreeRoot(pRuntime);
	treeRoot->createRootTreeNode(pRuntime->clone());

	return onContinue(pRuntime);
}

IBaseOperation::BOResult RDODPTSearch::onContinue(CREF(LPRDORuntime) pRuntime)
{
	DWORD time_begin = ::GetTickCount();
	while (true)
	{
		// ¬озмем дл€ раскрыти€ первую вершину из списка OPEN
		TreeNode* curr = *(treeRoot->m_OPEN.begin());
		curr->ExpandChildren();
		if (treeRoot->m_OPEN.empty() || treeRoot->m_targetNode) break;

		DWORD time_current = ::GetTickCount();
		if (time_current - time_begin > 1000 / 40)
		{
			return BOR_must_continue;
		}
	}

	rbool success = treeRoot->m_targetNode ? true : false;
	if (success)
	{
		// Ќашли решение, собрали путь
		std::list<TreeNode*> bestPath;
//		TRACE( "решение... \n" );
		for (TreeNode* i = treeRoot->m_targetNode; i->m_parent; i = i->m_parent)
		{
#ifdef _DEBUG
			i->m_pRuntime->showResources(i->m_number);
#endif
			bestPath.push_front(i);
		}
//		TRACE( "решение... done\n" );
		// ќтработали предварительные действи€: вывели трассировку
		onSearchDecisionHeader(treeRoot->m_theRealSimulator);
		// ќтработали рулы
		for (std::list<TreeNode*>::iterator ii = bestPath.begin(); ii != bestPath.end(); ++ii)
		{
			TreeNode* node = (*ii);
			node->m_activity->rule()->onBeforeChoiceFrom(treeRoot->m_theRealSimulator);
			node->m_activity->rule()->choiceFrom        (treeRoot->m_theRealSimulator);
			node->m_activity->rule()->onBeforeRule      (treeRoot->m_theRealSimulator);
			node->m_activity->rule()->convertRule       (treeRoot->m_theRealSimulator);
			node->m_activity->rule()->onAfterRule       (treeRoot->m_theRealSimulator, true);
			// ќтработали каждую вершину: вывели трассировку
			onSearchDecision(treeRoot->m_theRealSimulator, node);
		}
		// ќтработали завершающие действи€: вывели трассировку, обновили статистику по поиску
		onSearchResultSuccess(treeRoot->m_theRealSimulator, treeRoot);
	}
	else
	{
		// Ќеудачное завершение поиска: вывели статистику
		onSearchResultNotFound(treeRoot->m_theRealSimulator, treeRoot);
	}
	delete treeRoot->m_rootNode;
	delete treeRoot;
	treeRoot = NULL;
	return success ? IBaseOperation::BOR_done : IBaseOperation::BOR_cant_run;
}

void RDODPTSearch::addActivity(LPIDPTSearchActivity activity)
{
	m_activityList.push_back(activity);
}

CLOSE_RDO_RUNTIME_NAMESPACE
