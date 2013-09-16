/*!
  \copyright (c) RDO-Team, 2011
  \file      rdo_logic_dptsearch.cpp
  \author    Урусов Андрей (rdo@rk9.bmstu.ru)
  \date      29.04.2008
  \brief     DPTSearch
  \indent    4T
*/

// ----------------------------------------------------------------------- PLATFORM
#include "utils/src/common/platform.h"
// ---------------------------------------------------------------------------- PCH
#include "simulator/runtime/pch/stdpch.h"
// ----------------------------------------------------------------------- INCLUDES
// ----------------------------------------------------------------------- SYNOPSIS
#include "simulator/runtime/rdo_logic_dptsearch.h"
#include "simulator/runtime/searchtree.h"
#include "simulator/runtime/rdo_rule.h"
#include "simulator/runtime/rdo_runtime.h"
// --------------------------------------------------------------------------------

#ifndef OST_WINDOWS
	ruint32 GetTickCount()
	{ 
		struct timeval tv;
		gettimeofday(&tv,NULL);
		return (tv.tv_sec * 1000 + tv.tv_usec / 1000);
	}
#endif

OPEN_RDO_RUNTIME_NAMESPACE

// --------------------------------------------------------------------------------
// -------------------- RDODPTSearch
// --------------------------------------------------------------------------------
RDODPTSearch::RDODPTSearch(CREF(LPRDORuntime) pRuntime, LPIBaseOperationContainer pParent)
	: RDOLogicSimple(pRuntime, pParent)
	, treeRoot      (NULL)
{}

RDODPTSearch::~RDODPTSearch()
{}

IBaseOperation::BOResult RDODPTSearch::onDoOperation(CREF(LPRDORuntime) pRuntime)
{
	// Начало поиска: вывели трасировку, обновили статистику
	onSearchBegin(pRuntime);
	treeRoot = createTreeRoot(pRuntime);
	treeRoot->createRootTreeNode(pRuntime->clone());

	return onContinue(pRuntime);
}

IBaseOperation::BOResult RDODPTSearch::onContinue(CREF(LPRDORuntime) pRuntime)
{
	UNUSED(pRuntime);
	ruint32 time_begin = ::GetTickCount();
	for (;;)
	{
		// Возмем для раскрытия первую вершину из списка OPEN
		TreeNode* curr = *(treeRoot->m_OPEN.begin());
		curr->ExpandChildren();
		if (treeRoot->m_OPEN.empty() || treeRoot->m_targetNode) break;

		ruint32 time_current = ::GetTickCount();
		if (time_current - time_begin > 1000 / 40)
		{
			return BOR_must_continue;
		}
	}

	rbool success = treeRoot->m_targetNode ? true : false;
	if (success)
	{
		// Нашли решение, собрали путь
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
		// Отработали предварительные действия: вывели трассировку
		onSearchDecisionHeader(treeRoot->m_theRealSimulator);
		// Отработали рулы
		for (std::list<TreeNode*>::iterator ii = bestPath.begin(); ii != bestPath.end(); ++ii)
		{
			TreeNode* node = (*ii);
			node->m_activity->rule()->onBeforeChoiceFrom(treeRoot->m_theRealSimulator);
			node->m_activity->rule()->choiceFrom        (treeRoot->m_theRealSimulator);
			node->m_activity->rule()->onBeforeRule      (treeRoot->m_theRealSimulator);
			node->m_activity->rule()->convertRule       (treeRoot->m_theRealSimulator);
			node->m_activity->rule()->onAfterRule       (treeRoot->m_theRealSimulator, true);
			// Отработали каждую вершину: вывели трассировку
			onSearchDecision(treeRoot->m_theRealSimulator, node);
		}
		// Отработали завершающие действия: вывели трассировку, обновили статистику по поиску
		onSearchResultSuccess(treeRoot->m_theRealSimulator, treeRoot);
	}
	else
	{
		// Неудачное завершение поиска: вывели статистику
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
