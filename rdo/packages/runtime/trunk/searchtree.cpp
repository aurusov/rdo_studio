/*!
  \copyright (c) RDO-Team, 2011
  \file      searchtree.cpp
  \author    Урусов Андрей (rdo@rk9.bmstu.ru)
  \date      11.06.2006
  \brief     Интерфейс IDPTSearchTraceStatistics
  \indent    4T
*/

// ---------------------------------------------------------------------------- PCH
#include "simulator/runtime/pch/stdpch.h"
// ----------------------------------------------------------------------- INCLUDES
// ----------------------------------------------------------------------- SYNOPSIS
#include "utils/src/debug/rdodebug.h"
#include "simulator/runtime/searchtree.h"
#include "simulator/runtime/searchtrace.h"
#include "simulator/runtime/rdo_runtime.h"
#include "simulator/runtime/rdo_rule.h"
// --------------------------------------------------------------------------------

OPEN_RDO_RUNTIME_NAMESPACE

// --------------------------------------------------------------------------------
// -------------------- TreeRoot
// --------------------------------------------------------------------------------
TreeRoot::TreeRoot(CREF(LPRDORuntime) pRuntime, PTR(RDODPTSearch) pDP)
	: m_dp                (pDP     )
	, m_rootNode          (NULL    )
	, m_targetNode        (NULL    )
	, m_theRealSimulator  (pRuntime)
	, m_nodesInGraphCount (1       ) //! Учёт начальной вершины
	, m_expandedNodesCount(0       )
	, m_fullNodesCount    (0       )
	, m_sizeof_dpt        (0       )
	, m_nodesCount        (0       )
{
	m_ptime = boost::posix_time::microsec_clock::local_time();
}

// --------------------------------------------------------------------------------
// -------------------- TreeNode
// --------------------------------------------------------------------------------
TreeNode::TreeNode(CREF(LPRDORuntime) pRuntime, PTR(TreeNode) pParent, PTR(TreeRoot) pRoot, LPIDPTSearchActivity pActivity, double cost, int cnt)
	: m_pRuntime     (pRuntime )
	, m_parent       (pParent  )
	, m_root         (pRoot    )
	, m_activity     (pActivity)
	, m_costRule     (0        )
	, m_costPath     (cost     )
	, m_costRest     (0        )
	, m_number       (cnt      )
	, m_newCostPath  (0        )
	, m_newCostRest  (0        )
	, m_newCostRule  (0        )
{
	m_pRuntime->onPutToTreeNode();
}

TreeNode::~TreeNode()
{
	for(std::vector<TreeNode*>::iterator i = m_children.begin(); i != m_children.end(); i++)
	{
		delete (*i);
		m_root->m_sizeof_dpt -= sizeof(TreeNode);
	}
	m_root->m_sizeof_dpt -= m_pRuntime->getSizeofSim();
	m_pRuntime = NULL;
}

void TreeNode::ExpandChildren()
{
	m_root->m_sizeof_dpt -= (m_pRuntime->getSizeofSim() + sizeof(TreeNode)) * m_children.size();
	rdo::deleteAllObjects(m_children);

	// Вывели статистику
	onSearchOpenNode(m_root->m_theRealSimulator);

	// Проверили на конечную вершину
	/// @todo возможно, надо проверить все вершины в списке OPEN
	if (m_root->m_dp->TermCondition(m_pRuntime))
	{
		m_root->m_targetNode = this;
#ifdef _DEBUG
		m_pRuntime->showResources(m_number);
#endif
		return;
	}

	// Только для статистики
	m_root->m_expandedNodesCount++;

	// Бегаем по всем активностям самой точки
	for (RDODPTSearch::ActivityList::iterator i = m_root->m_dp->m_activityList.begin(); i != m_root->m_dp->m_activityList.end(); ++i)
	{
		m_currAct       = *i;
		m_pChildRuntime = m_pRuntime->clone();
#ifdef _DEBUG
		if (m_pChildRuntime->checkState())
		{
			TRACE1("состояние, node = %d\n", m_number);
		}
#endif
		m_root->m_sizeof_dpt += m_pChildRuntime->getSizeofSim();
		m_currAct->rule()->onBeforeChoiceFrom(m_pChildRuntime);
		if (!m_currAct->rule()->choiceFrom(m_pChildRuntime))
		{
			// Не прошел Choice from, удаляем симулятор и переходим к другой активности.
			/// @todo а зачем удалять симулятор, ведь БД не поменялась ?
			// Такое будет возможно, если при подготовке параметров паттерна будет
			// вызываться calc, на котором весит уведомление по вызову для другого
			// объекта, который будут меняться параметры ресурсов
			// Пока таких сложных взаимодействий в системе нет.
			m_root->m_sizeof_dpt -= m_pChildRuntime->getSizeofSim();
			m_pChildRuntime = NULL;
		}
		else
		{
			// Только для статистики
			m_root->m_fullNodesCount++;
			// Расчитать стоимость применения правила (value before)
			if (m_currAct->valueTime() == IDPTSearchActivity::vt_before)
			{
				m_newCostRule = m_currAct->cost(m_pChildRuntime);
			}
			// Выполнить само правило (раскрыть вершину)
			m_currAct->rule()->onBeforeRule(m_pChildRuntime);
			m_currAct->rule()->convertRule(m_pChildRuntime);
			m_currAct->rule()->onAfterRule(m_pChildRuntime, true);

			// Расчитать стоимость применения правила (value after)
			if (m_currAct->valueTime() == IDPTSearchActivity::vt_after)
			{
				m_newCostRule = m_currAct->cost(m_pChildRuntime);
			}
			// Расчитали стоимость пути до текущей вершины
			m_newCostPath = m_costPath + m_newCostRule;
			// Расчитали стоимость пути до цели с учетом оценки
			// Именно по этому значению и должен упорядочиваться список OPEN
			m_newCostRest = m_newCostPath + m_root->m_dp->EvaluateBy(m_pChildRuntime);

			// Надо ли сравнивать вершины (запоминать уже пройденные)
			// Алгоритм смены родителя
			if (m_root->m_dp->NeedCompareTops())
			{
				TreeNode* loser = NULL;
				NodeFoundInfo res = m_root->m_rootNode->CheckIfExistBetter(m_pChildRuntime, m_newCostPath, &loser);
				if (res == nfi_found_better)
				{
					// В графе есть более лучшая вершина, т.е. текущая активность
					// отработала хуже (дороже) до одного и того же состояния.
					// Граф перестраивать не надо.
					// Вывели трассировку раскрытой вершины.
					onSearchNodeInfoDeleted(m_root->m_theRealSimulator);
					m_root->m_sizeof_dpt -= m_pChildRuntime->getSizeofSim();
					m_pChildRuntime = NULL;
					// Переходим к следующей активности
					continue;
				}
				else if (res == nfi_found_loser)
				{
#ifdef _DEBUG
					if (m_number == 294)
					{
						TRACE1("loser->m_number = %d\n", loser->m_number);
						TRACE1("loser->m_parent->m_number = %d\n", loser->m_parent->m_number);
						TRACE1("loser->m_parent->m_children.size() = %d\n", loser->m_parent->m_children.size());
						loser->m_pRuntime->showResources(loser->m_number);
					}
#endif
					// Смена родителя
					// В графе нашлась вершина (loser) с более плохой стоимостью пути,
					// т.е. текущая активность отработала лучше (дешевле).
					// Необходимо перестроить граф
					// Сначала отрываем всю ветку от старого родителя loser->parent
					loser->m_parent->m_children.erase(std::find(loser->m_parent->m_children.begin(), loser->m_parent->m_children.end(), loser));
#ifdef _DEBUG
					if (m_number == 294)
					{
						TRACE1("loser->m_parent->m_children.size() after erase = %d\n", loser->m_parent->m_children.size());
					}
#endif
					// Теперь пересчитываем стоимость этой вершины и всех её потомков
					/// @todo: а нет ли тут ошибки, т.к. costPath у старой вершины,
					// подсчитанная как costPath + newCostRule, может не совпасть
					// из-за newCostRule из-за разных стоимостей применения правил,
					// т.е. сначла надо обновить loser->costRule (см. ниже), а потом
					// вызывать loser->ReCostSubTree( costPath )
					loser->ReCostSubTree(m_costPath);
					// Меняем родителя на текущую вершину
					loser->m_parent   = this;
					// Присваеваем ей новый номер
					loser->m_number   = m_root->getNewNodeNumber();
					// Обновляем стоимости
					loser->m_costPath = m_newCostPath;
					loser->m_costRest = m_newCostRest;
					loser->m_costRule = m_newCostRule;
					// Меняем указатель на новую активность
					loser->m_activity = m_currAct;
					// Тоже, но только для правильного вывода статистики
					loser->m_currAct  = m_currAct;
					// Выводим трассировку по смене родителя
					loser->onSearchNodeInfoReplaced(m_root->m_theRealSimulator);
					// Добавляем в список потомков текущей
					m_children.push_back(loser);
					m_root->m_sizeof_dpt -= m_pChildRuntime->getSizeofSim();
					m_pChildRuntime = NULL;
#ifdef _DEBUG
					if (m_number == 294)
					{
						loser->m_pRuntime->showResources(loser->m_number);
					}
#endif
					// Переходим к следующей активности
					continue;
				}
			}

			// Не нашли в графе такого состояния
			// Его надо запомнить. Создадим для этого новую вершину
			TreeNode* tn = createChildTreeNode();
			m_root->m_nodesInGraphCount++;
			tn->m_costPath = m_newCostPath;
			tn->m_costRest = m_newCostRest;
			tn->m_costRule = m_newCostRule;
			tn->m_currAct  = m_currAct;
			// Выведем трассировку
			tn->onSearchNodeInfoNew(m_root->m_theRealSimulator);
			// Добавим к списку потомков текущей
			m_children.push_back(tn);
/*
			if(!root->m_dp->TermCondition(pChildRuntime))
				root->OPEN.push_back(tn);
			else
			{
				root->targetNode = tn;
			}
*/
			// Добавим в список OPEN
			m_root->m_OPEN.push_back(tn);
		}
	}
	// Удалим текущую вершину из списка OPEN
	m_root->m_OPEN.erase(std::find(m_root->m_OPEN.begin(), m_root->m_OPEN.end(), this));

	// Отсортируем все вершины в списке OPEN
	std::sort(m_root->m_OPEN.begin(), m_root->m_OPEN.end(), compareNodes);
}

TreeNode::NodeFoundInfo TreeNode::CheckIfExistBetter(CREF(LPRDORuntime) pChildRuntime, double useCost, TreeNode** loser)
{
	ASSERT(pChildRuntime);

	if (pChildRuntime->equal(m_pRuntime))
	{
		if (m_costPath <= useCost)
		{
			return nfi_found_better;
		}
		else
		{
			*loser = this;
			return nfi_found_loser;
		}
	}

	for (std::vector<TreeNode*>::iterator i = m_children.begin(); i != m_children.end(); ++i)
	{
		NodeFoundInfo res = (*i)->CheckIfExistBetter(pChildRuntime, useCost, loser);
		if (res != nfi_notfound)
			return res;
	}

	return nfi_notfound;
}

void TreeNode::ReCostSubTree(double cost)
{
	m_costPath = cost + m_costRule;

	for (std::vector<TreeNode*>::iterator i = m_children.begin(); i != m_children.end(); ++i)
	{
		(*i)->ReCostSubTree(m_costPath);
	}
}

TreeNode* TreeNode::createChildTreeNode()
{
	m_root->m_sizeof_dpt += sizeof(TreeNode);
	return new TreeNode(m_pChildRuntime, this, m_root, m_currAct, m_costPath, m_root->getNewNodeNumber());
}

CLOSE_RDO_RUNTIME_NAMESPACE
