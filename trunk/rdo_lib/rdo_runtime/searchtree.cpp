#include "pch.h"
#include "searchtree.h"
#include "searchtrace.h"
#include "rdo_runtime.h"

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
TreeRoot::TreeRoot( RDOSimulator* sim, RDODPTSearch* _dp ):
	dp( _dp ),
	rootNode( NULL ),
	targetNode( NULL ),
	theRealSimulator( sim ),
	nodesCount( 0 ),
	nodesInGraphCount( 1 ), // Учет начальной вершине
	expandedNodesCount( 0 ),
	fullNodesCount( 0 ),
	sizeof_dpt( 0 )
{
	::GetSystemTime( &systime_begin );
}

// ----------------------------------------------------------------------------
// ---------- TreeNode - узел графа DPT
// ----------------------------------------------------------------------------
TreeNode::TreeNode( RDOSimulator* _sim, TreeNode* _parent, TreeRoot* _root, RDODPTSearch::Activity* _activity, double cost, int cnt ):
	currAct( NULL ),
	childSim( NULL ),
	newCostPath( 0 ),
	newCostRest( 0 ),
	newCostRule( 0 ),
	sim( _sim ),
	parent( _parent ),
	root( _root ),
	activity( _activity ),
	costRule( 0 ),
	costPath( cost ),
	costRest( 0 ),
	number( cnt )
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

int TreeNode::getActivityID() const
{
	return ((RDOActivityTrace*)activity)->getTraceID();
}

void TreeNode::ExpandChildren()
{
	root->sizeof_dpt -= (sim->sizeof_sim + sizeof(TreeNode)) * children.size();
	DeleteAllObjects( children );

	// Вывели статистику
	onSearchOpenNode( root->theRealSimulator );

	// Проверили на конечную вершину
	// TODO: возможно, надо проверить все вершины в списке OPEN
	if ( root->dp->TermCondition(sim) ) {
		root->targetNode = this;
#ifdef _DEBUG
		static_cast<RDORuntime*>(sim)->showResources(number);
#endif
		return;
	}

	// Только для статистики
	root->expandedNodesCount++;

	// Бегаем по всем активностям самой точки
	for ( std::list< RDODPTSearch::Activity* >::iterator i = root->dp->activities.begin(); i != root->dp->activities.end(); i++ ) {
		currAct  = (*i);
		childSim = sim->createCopy();
#ifdef _DEBUG
		if ( static_cast<RDORuntime*>(childSim)->checkState() ) {
			TRACE( "состояние, node = %d\n", number );
		}
#endif
		root->sizeof_dpt += childSim->sizeof_sim;
		currAct->rule()->onBeforeChoiceFrom( childSim );
		if ( !currAct->rule()->choiceFrom( childSim ) ) {
			// Не прошел Choice from, удаляем симулятор и переходим к другой активности.
			// TODO: а зачем удалять симулятор, ведь БД не поменялась ?
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
			if ( currAct->valueTime() == RDODPTSearch::Activity::vt_before ) {
				newCostRule = currAct->cost( childSim );
			}
			// Выполнить само правило (раскрыть вершину)
			currAct->rule()->onBeforeRule( childSim );
			currAct->rule()->convertRule( childSim );
			currAct->rule()->onAfterRule( childSim, true );

			// Расчитать стоимость применения правила (value after)
			if ( currAct->valueTime() == RDODPTSearch::Activity::vt_after ) {
				newCostRule = currAct->cost( childSim );
			}
			// Расчитали стоимость пути до текущей вершины
			newCostPath = costPath + newCostRule;
			// Расчитали стоимость пути до цели с учетом оценки
			// Именно по этому значению и должен упорядочиваться список OPEN
			newCostRest = newCostPath + root->dp->EvaluateBy( childSim );

			// Надо ли сравнивать вершины (запоминать уже пройденные)
			// Алгоритм смены родителя
			if ( root->dp->NeedCompareTops() ) {
				TreeNode* loser = NULL;
				NodeFoundInfo res = root->rootNode->CheckIfExistBetter( childSim, newCostPath, &loser );
				if ( res == nfi_found_better ) {
					// В графе есть более лучшая вершина, т.е. текущая активность
					// отработала хуже (дороже) до одного и того же состояния.
					// Граф перестраивать не надо.
					// Вывели трассировку раскрытой вершины.
					onSearchNodeInfoDeleted( root->theRealSimulator );
					root->sizeof_dpt -= childSim->sizeof_sim;
					delete childSim;
					// Переходим к следующей активности
					continue;
				} else if ( res == nfi_found_loser ) {
#ifdef _DEBUG
					if ( number == 294 ) {
						TRACE( "loser->number = %d\n", loser->number );
						TRACE( "loser->parent->number = %d\n", loser->parent->number );
						TRACE( "loser->parent->children.size() = %d\n", loser->parent->children.size() );
						static_cast<RDORuntime*>(loser->sim)->showResources(loser->number);
					}
#endif
					// Смена родителя
					// В графе нашлась вершина (loser) с более плохой стоимостью пути,
					// т.е. текущая активность отработала лучше (дешевле).
					// Необходимо перестроить граф
					// Сначала отрываем всю ветку от старого родителя loser->parent
					loser->parent->children.erase( std::find(loser->parent->children.begin(), loser->parent->children.end(), loser) );
#ifdef _DEBUG
					if ( number == 294 ) {
						TRACE( "loser->parent->children.size() after erase = %d\n", loser->parent->children.size() );
					}
#endif
					// Теперь пересчитываем стоимость этой вершины и всех её потомков
					// todo: а нет ли тут ошибки, т.к. costPath у старой вершины,
					// подсчитанная как costPath + newCostRule, может не совпасть
					// из-за newCostRule из-за разных стоимостей применения правил,
					// т.е. сначла надо обновить loser->costRule (см. ниже), а потом
					// вызывать loser->ReCostSubTree( costPath )
					loser->ReCostSubTree( costPath );
					// Меняем родителя на текущую вершину
					loser->parent   = this;
					// Присваеваем ей новый номер
					loser->number   = root->getNewNodeNumber();
					// Обновляем стоимости
					loser->costPath = newCostPath;
					loser->costRest = newCostRest;
					loser->costRule = newCostRule;
					// Меняем указатель на новую активность
					loser->activity = currAct;
					// Тоже, но только для правильного вывода статистики
					loser->currAct  = currAct;
					// Выводим трассировку по смене родителя
					loser->onSearchNodeInfoReplaced( root->theRealSimulator );
					// Добавляем в список потомков текущей
					children.push_back( loser );
					root->sizeof_dpt -= childSim->sizeof_sim;
					delete childSim;
#ifdef _DEBUG
					if ( number == 294 ) {
						static_cast<RDORuntime*>(loser->sim)->showResources( loser->number );
					}
#endif
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
				root->OPEN.push_back(tn);
			else
			{
				root->targetNode = tn;
			}
*/
			// Добавим в список OPEN
			root->OPEN.push_back( tn );
		}
	}
	// Удалим текущую вершину из списка OPEN
	root->OPEN.erase( std::find(root->OPEN.begin(), root->OPEN.end(), this) );

	// Отсортируем все вершины в списке OPEN
	std::sort( root->OPEN.begin(), root->OPEN.end(), compareNodes );
}

TreeNode::NodeFoundInfo TreeNode::CheckIfExistBetter( RDOSimulator* childSim, double useCost, TreeNode** loser )
{
	if ( *childSim == *sim ) {
		if ( costPath <= useCost ) {
			return nfi_found_better;
		} else {
			*loser = this;
			return nfi_found_loser;
		}
	}

	for ( std::vector< TreeNode* >::iterator i = children.begin(); i != children.end(); i++ ) {
		NodeFoundInfo res = (*i)->CheckIfExistBetter( childSim, useCost, loser );
		if ( res != nfi_notfound ) return res;
	}

	return nfi_notfound;
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
	return new TreeNode(childSim, this, root, currAct, costPath, root->getNewNodeNumber());
}

} // namespace rdoRuntime;
