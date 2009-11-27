#include "pch.h"
#include "rdo_simulator.h"
#include "rdo_logic_dptprior.h"
#include <rdostream.h>
#include "rdo_model_interface.h"
//#include "rdo_ie.h"
//#include "rdo_rule.h"
//#include "rdo_operation.h"

#pragma warning(disable : 4786)  

namespace rdoRuntime {

// ----------------------------------------------------------------------------
// ---------- RDOSimulator - один из базовых классов для RDORuntime
// ----------------------------------------------------------------------------
RDOSimulator::RDOSimulator()
	: RDOSimulatorBase( )
	, m_sizeof_sim    (0)
{
	m_metaLogic = F(RDOLogicMeta)::create();
}

RDOSimulator::~RDOSimulator()
{}

void RDOSimulator::appendLogic(CREF(LPIBaseOperation) logic)
{
	ASSERT(m_metaLogic);
	m_metaLogic->append(logic);
}

bool RDOSimulator::doOperation()
{
	bool res;
	if ( getMustContinueOpr() ) {
		// Есть действие, которое необходимо продолжить. Используется в DPT
		IBaseOperation::BOResult result = getMustContinueOpr()->onContinue( this );
		if ( result != IBaseOperation::BOR_must_continue ) {
			setMustContinueOpr( NULL );
		}
		return result != IBaseOperation::BOR_cant_run;
	} else {
		bool found_planed = false;
		// Отработаем все запланированные на данный момент события
		if ( !m_check_operation && !m_timePoints.empty() ) {
			m_check_operation = true;
			double newTime = m_timePoints.begin()->first;
			if ( getCurrentTime() >= newTime ) {
				BOPlannedItem* list = m_timePoints.begin()->second;
				if ( list && !list->empty() ) {
#ifdef RDOSIM_COMPATIBLE
					// Дисциплина списка текущих событий LIFO
					LPIBaseOperation opr   = list->back().m_opr;
					void*            param = list->back().m_param;
					list->pop_back();
#else
					// Дисциплина списка текущих событий FIFO
					LPIBaseOperation opr   = list->front().m_opr;
					void*            param = list->front().m_param;
					list->pop_front();
#endif
					if ( list->empty() ) {
						delete list;
//						delete m_timePoints.begin()->second;
						m_timePoints.erase( m_timePoints.begin() );
					}
					opr->onMakePlaned( this, param );
					found_planed = true;
				}
			}
		}
		res = found_planed;
		if ( !found_planed ) {
			// Не нашли запланированное событие
			// Проверить все возможные события и действия, вызвать первое, которое может быть вызвано
			res = m_metaLogic.query_cast<IBaseOperation>()->onCheckCondition(this);
			if ( res )
			{
				res = m_metaLogic.query_cast<IBaseOperation>()->onDoOperation(this) != IBaseOperation::BOR_cant_run;
			}
			if ( !res ) m_check_operation = false;
		}
	}
	onCheckPokaz();
	onAfterCheckPokaz();
	return res;
}

void RDOSimulator::preProcess()
{
	m_metaLogic.query_cast<IBaseOperation>()->onStart( this );
	onResetPokaz();
}

RDOSimulator* RDOSimulator::createCopy()
{
	RDOSimulator* sim_clone = clone();
	sim_clone->setCurrentTime( getCurrentTime() );
	return sim_clone;
}


std::string writeActivitiesStructureRecurse(CREF(LPIBaseOperationContainer) logic, REF(int) counter)
{
	rdo::textstream stream;
	IBaseOperationContainer::CIterator it = logic->begin();
	while (it != logic->end())
	{
		LPIModelStructure pModelStructure = *it;
		if (pModelStructure && (pModelStructure.query_cast<IRule>() || pModelStructure.query_cast<IOperation>()))
		{
			stream << counter++ << " ";
			pModelStructure->writeModelStructure(stream);
		}
		it++;
	}
#ifdef RDOSIM_COMPATIBLE
#else
	stream << std::endl;
#endif

	int _counter = 1;
	it = logic->begin();
	while ( it != logic->end() )
	{
		LPIModelStructure pModelStructure = *it;
		if (pModelStructure && pModelStructure.query_cast<IIrregEvent>())
		{
			stream << _counter++ << " ";
			counter++;
			pModelStructure->writeModelStructure(stream);
		}
		it++;
	}

	it = logic->begin();
	while (it != logic->end())
	{
		LPILogic logic = *it;
		if (logic)
		{
			stream << writeActivitiesStructureRecurse(logic, counter);
		}
		it++;
	}

	return stream.str();
}

std::string RDOSimulator::writeActivitiesStructure(REF(int) counter)
{
	return writeActivitiesStructureRecurse(m_metaLogic, counter);
}

} // namespace rdoRuntime
