/******************************************************************************//**
 * @copyright (c) RDO-Team, 2011
 * @file      searchtree.inl
 * @author    Лущан Дмитрий
 * @date      22.07.2011
 * @brief     unknown
 * @indent    4T
 *********************************************************************************/

// *********************************************************************** INCLUDES
// *********************************************************************** SYNOPSIS
// ********************************************************************************

OPEN_RDO_RUNTIME_NAMESPACE

// ********************************************************************************
// ******************** TreeRoot
// ********************************************************************************
TreeRoot::~TreeRoot()
{}

int TreeRoot::getNodesCound() const
{
	return m_nodesCount;
}

int TreeRoot::getNewNodeNumber()
{
	return ++m_nodesCount;
}

// ********************************************************************************
// ******************** TreeRoot
// ********************************************************************************
void TreeNode::onSearchOpenNode(CREF(LPRDORuntime) pRuntime)
{}

void TreeNode::onSearchNodeInfoDeleted(CREF(LPRDORuntime) pRuntime)
{}

void TreeNode::onSearchNodeInfoReplaced(CREF(LPRDORuntime) pRuntime)
{}

void TreeNode::onSearchNodeInfoNew(CREF(LPRDORuntime) pRuntime)
{}

/******************************************************************************//**
 * @fn      compareNodes
 * @brief   функция сравнения вершин графа для сортировки списка OPEN
 *********************************************************************************/
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
