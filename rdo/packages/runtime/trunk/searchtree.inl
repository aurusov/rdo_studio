/*!
  \copyright (c) RDO-Team, 2011
  \file      searchtree.inl
  \author    Лущан Дмитрий (dluschan@rk9.bmstu.ru)
  \date      22.07.2011
  \brief     Интерфейс IDPTSearchTraceStatistics
  \indent    4T
*/

// ----------------------------------------------------------------------- INCLUDES
// ----------------------------------------------------------------------- SYNOPSIS
#include "simulator/runtime/rdo_logic_dptsearch.h"
#include "simulator/runtime/rdo_dptsearch_activity_i.h"
// --------------------------------------------------------------------------------

OPEN_RDO_RUNTIME_NAMESPACE

// --------------------------------------------------------------------------------
// -------------------- TreeRoot
// --------------------------------------------------------------------------------
inline TreeRoot::~TreeRoot()
{}

inline int TreeRoot::getNodesCound() const
{
	return m_nodesCount;
}

inline int TreeRoot::getNewNodeNumber()
{
	return ++m_nodesCount;
}

// --------------------------------------------------------------------------------
// -------------------- TreeRoot
// --------------------------------------------------------------------------------
inline void TreeNode::onSearchOpenNode(CREF(LPRDORuntime) pRuntime)
{
	UNUSED(pRuntime);
}

inline void TreeNode::onSearchNodeInfoDeleted(CREF(LPRDORuntime) pRuntime)
{
	UNUSED(pRuntime);
}

inline void TreeNode::onSearchNodeInfoReplaced(CREF(LPRDORuntime) pRuntime)
{
	UNUSED(pRuntime);
}

inline void TreeNode::onSearchNodeInfoNew(CREF(LPRDORuntime) pRuntime)
{
	UNUSED(pRuntime);
}

/*!
  \fn      rbool compareNodes(const TreeNode* tn1, const TreeNode* tn2)
  \brief   функция сравнения вершин графа для сортировки списка OPEN
*/
inline rbool compareNodes(const TreeNode* tn1, const TreeNode* tn2)
{
	if (fabs(tn1->m_costRest - tn2->m_costRest) > 0.0000001)
	{
		return (tn1->m_costRest < tn2->m_costRest);
	}
	else
	{
		return (tn1->m_number < tn2->m_number);
	}
}

CLOSE_RDO_RUNTIME_NAMESPACE
