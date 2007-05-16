#include "pch.h"
#include "searchtree.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

namespace rdoRuntime
{

// ----------------------------------------------------------------------------
// ---------- TreeRoot - корень дерева DPT
// ----------------------------------------------------------------------------
TreeRoot::TreeRoot( RDOSimulator* sim, RDODecisionPoint* _dp ):
	dp( _dp ),
	rootNode( NULL ),
	targetNode( NULL ),
	theRealSimulator( sim ),
	nodeCount( 1 ),
	nodesInGraphCount( 1 ),
	expandedNodesCount( 0 ),
	fullNodesCount( 0 ),
	sizeof_dpt( 0 )
{
	::GetSystemTime( &systime_begin );
}

// ----------------------------------------------------------------------------
// ---------- TreeNode - узел графа DPT
// ----------------------------------------------------------------------------
TreeNode::TreeNode( RDOSimulator* i_sim, TreeNode* i_parent, TreeRoot* i_root, RDOActivity* i_activity, double cost, int cnt ):
	currAct( NULL ),
	childSim( NULL ),
	newCostPath( 0 ),
	newCostRest( 0 ),
	newCostRule( 0 ),
	sim( i_sim ),
	parent( i_parent ),
	root( i_root ),
	activity( i_activity ),
	costRule( 0 ),
	costPath( cost ),
	costRest( 0 ),
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
	DeleteAllObjects( children );

	// Вывели статистику
	onSearchOpenNode( root->theRealSimulator );

	// Проверили на конечную вершину
	// todo: возможно, надо проверить все вершины в списке OPEN
	if ( root->dp->TermCondition(sim) ) {
		root->targetNode = this;
		return;
	}

	// Только для статистики
	root->expandedNodesCount++;

	// Бегаем по всем активностям самой точки
	for ( std::list< RDOActivity* >::iterator i = root->dp->activities.begin(); i != root->dp->activities.end(); i++ ) {
		currAct  = (*i);
		childSim = sim->createCopy();
		root->sizeof_dpt += childSim->sizeof_sim;
		currAct->rule->onBeforeChoiceFrom( childSim );
		if ( !currAct->rule->choiceFrom( childSim ) ) {
			// Не прошел Choice from, удаляем симулятор и переходим к другой активности.
			// todo: а зачем удалять симулятор, ведь БД не поменялась ?
			// Такое будет возможно, если при подготовке параметров паттерна будет
			// вызываться calc, на котором весит уведомление по вызову для другого
			// объекта, который будут меняться параметры ресурсов
			// Пока таких сложных взаимодействий в системе нет.
			root->sizeof_dpt -= childSim->sizeof_sim;
			delete childSim;
			childSim = NULL;
		} else {
			// Только для статистики
			root->fullNodesCount++;
			// Расчитать стоимость применения правила (value before)
			if ( !currAct->valueAfter ) {
				newCostRule = currAct->costOfRule( childSim );
			}
			// Выполнить само правило (раскрыть вершину)
			currAct->rule->onBeforeRule( childSim );
			currAct->rule->convertRule( childSim );
			currAct->rule->onAfterRule( childSim, true );
			// Расчитать стоимость применения правила (value after)
			if ( currAct->valueAfter ) {
				newCostRule = currAct->costOfRule( childSim );
			}
			// Расчитали стоимость пути до текущей вершины
			newCostPath = costPath + newCostRule;
			// Расчитали стоимость пути до цели с учетом оценки
			// Именно по этому значению и должен упорядочиваться список OPEN
			newCostRest = newCostPath + root->dp->EvaluateBy( childSim );

			// Надо ли сравнивать вершины (запоминать уже пройденные)
			// Алгоритм смены родителя
			if ( root->dp->NeedCompareTops() ) {
				TreeNode* better = NULL;
				int res = root->rootNode->CheckIfExistBetter( childSim, newCostPath, &better );
				if ( res == 1 ) {
					// В графе есть более лучшая вершина, т.е. текущая активность
					// отработала хуже (дороже) до одного и того же состояния.
					// Граф перестраивать не надо.
					// Вывели трассировку раскрытой вершины.
					onSearchNodeInfoDeleted( root->theRealSimulator );
					root->sizeof_dpt -= childSim->sizeof_sim;
					delete childSim;
					// Переходим к следующей активности
					continue;
				} else if ( res == 2 ) {
					// Смена родителя
					// В графе нашлась вершина (better) с более плохой стоимостью пути,
					// т.е. текущая активность отработала лучше (дешевле).
					// Необходимо перестроить граф
					// Сначала отрываем всю ветку от старого родителя better->parent
					better->parent->children.erase( std::find(better->parent->children.begin(), better->parent->children.end(), better) );
					root->sizeof_dpt -= sim->sizeof_sim + sizeof(TreeNode);
					// Теперь пересчитываем стоимость этой вершины и всех её потомков
					// todo: а нет ли тут ошибки, т.к. costPath у старой вершины,
					// подсчитанная как costPath + newCostRule, может не совпасть
					// из-за newCostRule из-за разных стоимостей применения правил,
					// т.е. сначла надо обновить better->costRule (см. ниже), а потом
					// вызывать better->ReCostSubTree( costPath )
					better->ReCostSubTree( costPath );
					// Меняем родителя на текущую вершину
					better->parent   = this;
					// Присваеваем её новый номер
					better->count    = root->nodeCount++;
					// Обновляем стоимости
					better->costPath = newCostPath;
					better->costRest = newCostRest;
					better->costRule = newCostRule;
					// Меняем указатель на новую активность
					better->currAct  = currAct;
					// Выводим трассировку по смене родителя
					better->onSearchNodeInfoReplaced( root->theRealSimulator );
					// Добавляем в список потомков текущей
					children.push_back( better );
					root->sizeof_dpt -= childSim->sizeof_sim;
					delete childSim;
					// Переходим к следующей активности
					continue;
				}
			}

			// Не нашли в графе такого состояния
			// Его надо запомнить. Создадим для этого новую вершину
			TreeNode* tn = createChildTreeNode();
			root->nodesInGraphCount++;
			tn->costPath = newCostPath;
			tn->costRest = newCostRest;
			tn->costRule = newCostRule;
			tn->currAct  = currAct;
			// Выведем трассировку
			tn->onSearchNodeInfoNew( root->theRealSimulator );
			// Добавим к списку потомков текущей
			children.push_back(tn);
/*
			if(!root->dp->TermCondition(childSim))
				root->allLeafs.push_back(tn);
			else
			{
				root->targetNode = tn;
			}
*/
			// Добавим в список OPEN
			root->allLeafs.push_back( tn );
		}
	}
	// Удалим текущую вершину из списка OPEN
	root->allLeafs.erase( std::find(root->allLeafs.begin(), root->allLeafs.end(), this) );
	root->sizeof_dpt -= sim->sizeof_sim + sizeof(TreeNode);

	// Отсортируем все вершины в списке OPEN
	std::sort( root->allLeafs.begin(), root->allLeafs.end(), compareNodes );
}

int TreeNode::CheckIfExistBetter( RDOSimulator* childSim, double useCost, TreeNode** better )
{
	if ( *childSim == *sim ) {
		if ( costPath <= useCost ) {
			return 1;
		} else {
			// Первая 
			*better = this;
			return 2;
		}
	}

	for ( std::vector< TreeNode* >::iterator i = children.begin(); i != children.end(); i++ ) {
		int res = (*i)->CheckIfExistBetter( childSim, useCost, better );
		if ( res != 0 ) return res;
	}

	return 0;
}

void TreeNode::ReCostSubTree( double cost )
{
	costPath = cost + costRule;

	for ( std::vector< TreeNode* >::iterator i = children.begin(); i != children.end(); i++ ) {
		(*i)->ReCostSubTree( costPath );
	}
}

TreeNode* TreeNode::createChildTreeNode()
{
	root->sizeof_dpt += sizeof( TreeNode );
	return new TreeNode(childSim, this, root, currAct, costPath, root->nodeCount++);
}

} // namespace rdoRuntime;
