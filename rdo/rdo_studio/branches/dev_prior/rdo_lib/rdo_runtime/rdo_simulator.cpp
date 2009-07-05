#include "pch.h"
#include "rdo_simulator.h"
#include "rdo_ie.h"
#include "rdo_rule.h"
#include "rdo_operation.h"

#pragma warning(disable : 4786)  

namespace rdoRuntime {

// ----------------------------------------------------------------------------
// ---------- RDOSimulator - один из базовых классов для RDORuntime
// ----------------------------------------------------------------------------
bool RDOSimulator::doOperation()
{
	bool res;
	if ( getMustContinueOpr() ) {
		// Есть действие, которое необходимо продолжить. Используется в DPT
		RDOBaseOperation::BOResult result = getMustContinueOpr()->onContinue( this );
		if ( result != RDOBaseOperation::BOR_must_continue ) {
			setMustContinueOpr( NULL );
		}
		return result != RDOBaseOperation::BOR_cant_run;
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
					RDOBaseOperation* opr   = list->back().m_opr;
					void*             param = list->back().m_param;
					list->pop_back();
#else
					// Дисциплина списка текущих событий FIFO
					RDOBaseOperation* opr   = list->front().m_opr;
					void*             param = list->front().m_param;
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
			// Проверить все возможные события и действия, вызвать первое, которое может буть вызвано
			res = m_logics.onCheckCondition(this);
			if ( res )
			{
				res = m_logics.onDoOperation(this) != RDOBaseOperation::BOR_cant_run;
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
	m_logics.onStart( this );
	onResetPokaz();
}

RDOSimulator* RDOSimulator::createCopy()
{
	RDOSimulator* sim_clone = clone();
	sim_clone->setCurrentTime( getCurrentTime() );
	return sim_clone;
}


std::string writeActivitiesStructureRecurse( RDOLogic* logic, int& counter )
{
	rdo::textstream stream;
	RDOLogic::CIterator it = logic->begin();
	while ( it != logic->end() ) {
		RDORule* rule = dynamic_cast<RDORule*>(*it);
		if ( rule ) {
			stream << counter++ << " ";
			rule->writeModelStructure( stream );
		} else {
			RDOOperation* opr = dynamic_cast<RDOOperation*>(*it);
			if ( opr ) {
				stream << counter++ << " ";
				opr->writeModelStructure( stream );
			}
		}
		it++;
	}
#ifdef RDOSIM_COMPATIBLE
#else
	stream << std::endl;
#endif

	int _counter = 1;
	it = logic->begin();
	while ( it != logic->end() ) {
		RDOIrregEvent* ie = dynamic_cast<RDOIrregEvent*>(*it);
		if ( ie ) {
			stream << _counter++ << " ";
			counter++;
			ie->writeModelStructure( stream );
		}
		it++;
	}

	it = logic->begin();
	while ( it != logic->end() ) {
		RDOLogic* logic = dynamic_cast<RDOLogic*>(*it);
		if ( logic )
		{
			stream << writeActivitiesStructureRecurse( logic, counter );
		}
		it++;
	}

	return stream.str();
}

std::string RDOSimulator::writeActivitiesStructure( int& counter )
{
	return writeActivitiesStructureRecurse( &m_logics, counter );
}

} // namespace rdoRuntime
