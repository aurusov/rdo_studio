#include "pch.h"
#include "rdotrace.h"
#include "searchtrace.h"
#include "ruletrace.h"
#include "simtrace.h"
#include "ietrace.h"
#include "operationtrace.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

namespace rdoRuntime {

#ifdef RDOSIM_COMPATIBLE
std::string doubleToString( double value )
{
	std::ostringstream _str;
	_str << value;
	std::string::size_type pos = _str.str().find( "e" );
	if ( pos != std::string::npos ) {
		std::string __str = _str.str();
		__str.erase( pos + 2, 1 );
		return __str.c_str();
	} else {
		return _str.str().c_str();
	}
}
#else
double doubleToString( double value )
{
	return value;
}
#endif

void RDOTrace::writeSearchBegin( double currentTime, std::string decisionPointId )
{
	if ( isNull() || !canWrite() ) return;

	getOStream() << "SB " << currentTime << " " << decisionPointId.c_str() << std::endl << getEOL();
}

void RDOTrace::writeSearchDecisionHeader()
{
	if ( isNull() || !canWrite() ) return;

	getOStream() << "SD" << std::endl << getEOL();
}

void RDOTrace::writeSearchDecision(RDOSimulator *sim, TreeNode *node) 
{
	if ( isNull() || !canWrite() ) return;

	RDOSimulatorTrace *simTr = (RDOSimulatorTrace *)sim;
	RDOActivityTrace *actTr = (RDOActivityTrace *)node->activity;
	RDORuleTrace *ruleTr = (RDORuleTrace *)actTr->rule();

	getOStream() << node->number
	             << " " << actTr->traceId()
	             << " " << ruleTr->tracePatternId()
	             << " " << ruleTr->traceResourcesListNumbers( simTr )
	             << std::endl << getEOL();
}
void RDOTrace::writeString(std::string str)
{
	if ( isNull() || !canWrite() ) return;
	
	getOStream() << str << std::endl << getEOL();
}
void RDOTrace::writeSearchOpenNode( int nodeCount, int parentCount, double pathCost, double restCost )
{
	if ( isNull() || !canWrite() ) return;

	getOStream() << "SO " << nodeCount
	             << " " << doubleToString(parentCount)
	             << " " << doubleToString(pathCost)
	             << " " << doubleToString(restCost) << std::endl << getEOL();
}

void RDOTrace::writeSearchNodeInfo(char sign, TreeNodeTrace *node)
{
	if ( isNull() || !canWrite() ) return;

	RDODPTSearchTrace* dpTrace = static_cast<RDODPTSearchTrace*>(node->root->dp);
	if ( dpTrace->traceFlag == RDODPTSearchTrace::DPT_trace_tops || dpTrace->traceFlag == RDODPTSearchTrace::DPT_trace_all ) {
		RDOActivityTrace*  actTr  = static_cast<RDOActivityTrace*>(node->currAct);
		RDORuleTrace*      ruleTr = static_cast<RDORuleTrace*>(actTr->rule());
		RDOSimulatorTrace* sim    = static_cast<RDOSimulatorTrace*>(node->sim);

		getOStream().precision(4);
		getOStream() << "ST" << sign
		             << " " << ((sign != 'D') ? node->number : node->root->getNodesCound())
		             << " " << ((sign != 'D') ? node->parent->number : node->number)
		             << " " << ((sign != 'D') ? doubleToString(node->costPath) : doubleToString(node->newCostPath) )
		             << " " << ((sign != 'D') ? doubleToString(node->costRest) : doubleToString(node->newCostRest) )
		             << " " << actTr->traceId()
		             << " " << ruleTr->tracePatternId()
		             << " " << ((sign != 'D') ? doubleToString(node->costRule) : doubleToString(node->newCostRule) )
		             << " " << ruleTr->traceResourcesListNumbers( sim )
		             << std::endl << getEOL();

		RDODPTSearchTrace* dpTrace = static_cast<RDODPTSearchTrace*>(node->root->dp);
		if ( dpTrace->traceFlag == RDODPTSearchTrace::DPT_trace_all ) {
			getOStream() << ruleTr->traceResourcesList( 'S', sim ) << getEOL();
		}
	}
}

void RDOTrace::writeSearchResult( char letter, RDOSimulatorTrace* simTr, TreeRoot* treeRoot )
{
	if ( isNull() || !canWrite() ) return;

	SYSTEMTIME systime_current;
	::GetSystemTime( &systime_current );
	unsigned int msec_current = RDOSimulatorBase::getMSec( systime_current );
	unsigned int msec_begin   = RDOSimulatorBase::getMSec( treeRoot->systime_begin );
	double sec_delay = 0;
	if ( systime_current.wYear == treeRoot->systime_begin.wYear && systime_current.wMonth == treeRoot->systime_begin.wMonth ) {
		sec_delay = static_cast<double>(msec_current - msec_begin) / 1000 + (systime_current.wDay - treeRoot->systime_begin.wDay) * 24 * 60 * 60;
	}
	static_cast<RDODPTSearchTrace*>(treeRoot->dp)->calc_times.push_back( sec_delay );

	getOStream() << "SE" << letter
	             << " " << simTr->getCurrentTime()
	             << " " << sec_delay            // realTime
	             << " " << treeRoot->sizeof_dpt // memUsed
	             << " " << (letter == 'S' ? treeRoot->targetNode->costPath : 0)
	             << " " << treeRoot->expandedNodesCount
	             << " " << treeRoot->nodesInGraphCount
	             << " " << treeRoot->getNodesCound()
	             << " " << treeRoot->fullNodesCount 
	             << std::endl << getEOL();
	if ( letter == 'S' ) {
		getOStream() << "ES"
		             << " " << simTr->getCurrentTime()
		             << " 4"
		             << std::endl << getEOL();
		static_cast<RDODPTSearchTrace*>(treeRoot->dp)->calc_cost.push_back( treeRoot->targetNode->costPath );
		static_cast<RDODPTSearchTrace*>(treeRoot->dp)->calc_nodes.push_back( treeRoot->getNodesCound() );
		static_cast<RDODPTSearchTrace*>(treeRoot->dp)->calc_nodes_expended.push_back( treeRoot->expandedNodesCount );
		static_cast<RDODPTSearchTrace*>(treeRoot->dp)->calc_nodes_full.push_back( treeRoot->fullNodesCount );
		static_cast<RDODPTSearchTrace*>(treeRoot->dp)->calc_nodes_in_graph.push_back( treeRoot->nodesInGraphCount );
	}
}

void RDOTrace::writePermanentResources( rdoRuntime::RDOSimulatorTrace* sim, const std::list< RDOResourceTrace* >& res_perm )
{
	if ( isNull() || !canWrite() ) return;

	getOStream() << traceResourcesList('\0', sim, res_perm) << getEOL();
}

std::string RDOTrace::traceResourcesList( char prefix, RDOSimulatorTrace* sim, const std::list< RDOResourceTrace* >& rel_res_list )
{
	std::string res;
	for ( std::list< RDOResourceTrace* >::const_iterator i = rel_res_list.begin(); i != rel_res_list.end(); i++ ) {
		if ( *i ) {
			res += (*i)->traceResourceState( prefix, sim );
		}
	}
	return res;
}

std::string RDOResourceTrace::traceResourceState( char prefix, RDOSimulatorTrace* sim )
{
	std::ostringstream res;
	if ( (trace) || (prefix != '\0') ) {
		if ( state == RDOResourceTrace::CS_NoChange || state == RDOResourceTrace::CS_NonExist ) return "";
		if ( prefix != '\0' ) res << prefix;
		switch ( state ) {
			case RDOResourceTrace::CS_Create: res << "RC "; break;
			case RDOResourceTrace::CS_Erase : res << "RE "
#ifdef RDOSIM_COMPATIBLE
				<< sim->getCurrentTime() << " " << traceTypeId() << " " << traceId() << std::endl; return res.str();
#else
				;
#endif
				break;
			default                         : res << "RK "; break;
		}
		res << sim->getCurrentTime() << " " << traceTypeId() << " " << traceId() << " " << traceParametersValue() << std::endl;
	}
	return res.str();
}

void RDOTrace::writeIrregularEvent(RDOIETrace *ie, RDOSimulatorTrace *sim)
{
	if ( isNull() || !canWrite() ) return;

#ifdef RDOSIM_COMPATIBLE
	getOStream() << ie->traceResourcesList( '\0', sim ) << getEOL();
#endif

	if ( ie->trace ) {
		getOStream() << "EI " << sim->getCurrentTime()
		             << " "   << ie->traceId() 
		             << " "   << ie->tracePatternId() 
		             << " "   << ie->traceResourcesListNumbers( sim )
		             << std::endl << getEOL();
	}

#ifndef RDOSIM_COMPATIBLE
	getOStream() << ie->traceResourcesList( '\0', sim ) << getEOL();
#endif
}

void RDOTrace::writeRule( RDORuleTrace* rule, RDOSimulatorTrace* sim )
{
	if ( isNullTracer || !canWrite() ) return;

	if ( rule->trace ) {
		int operId = sim->getFreeOperationId();
		getOStream() << "ER " << sim->getCurrentTime()
		             << " "   << operId
		             << " "   << rule->traceId() 
		             << " "   << rule->tracePatternId()
		             << " "   << rule->traceResourcesListNumbers( sim, false )
		             << std::endl << getEOL();
		sim->freeOperationId(operId);
	}
	getOStream() << rule->traceResourcesList( '\0', sim ) << getEOL();
}

void RDOTrace::writeAfterOperationBegin( RDOOperationTrace* op, RDOSimulatorTrace* sim )
{
	if ( isNull() || !canWrite() ) return;

	if ( op->trace ) {
		getOStream() << "EB " << sim->getCurrentTime() 
		             << " "   << op->traceOperId() 
		             << " "   << op->traceId() 
		             << " "   << op->tracePatternId() 
		             << " "   << op->traceResourcesListNumbers( sim, false )
		             << std::endl << getEOL(); 
	}
	getOStream() << op->traceResourcesList( '\0', sim ) << getEOL();
}

void RDOTrace::writeAfterOperationEnd( RDOOperationTrace* op, RDOSimulatorTrace* sim )
{
	if ( isNull() || !canWrite() ) return;

	if ( op->trace ) {
		getOStream() << "EF " << sim->getCurrentTime() 
		             << " "   << op->traceOperId() 
		             << " "   << op->traceId() 
		             << " "   << op->tracePatternId() 
		             << " "   << op->traceResourcesListNumbers( sim, false )
		             << std::endl << getEOL();
	}
	getOStream() << op->traceResourcesList( '\0', sim ) << getEOL();
}

RDOResourceTrace::RDOResourceTrace( RDOSimulatorTrace* i_sim, int _id, bool _tarce ):
	RDOTraceableObject(i_sim),
	state( RDOResourceTrace::CS_None ),
	temporary( false )
{
	if ( _id == -1 ) {
		// Для временного ресурса ищем дырку в нумерации
		id = sim->getFreeResourceId();
	} else {
		// Вызываем для увеличения счетчика maxResourcesId постоянных ресурсов
		sim->getFreeResourceId( _id );
		id = _id;
	}
	setTraceID( id, id + 1 );
	trace = _tarce;
}

RDOResourceTrace::RDOResourceTrace( const RDOResourceTrace& orig ):
	RDOTraceableObject( orig.sim )
{
	id        = orig.id;
	state     = orig.state;
	typeId    = orig.typeId;
	temporary = orig.temporary;
	trace     = orig.trace;
	sim->incrementResourceIdReference(id);
}

RDOResourceTrace::~RDOResourceTrace()
{
	sim->onResourceErase(this);
}

void RDOTrace::writeTraceBegin(RDOSimulatorTrace *sim)
{
	if ( isNull() ) return;

	getOStream() << "ES " << sim->getCurrentTime() 
      << " 1" << std::endl << getEOL();
}

void RDOTrace::writeModelBegin(RDOSimulatorTrace *sim)
{
	if ( isNull() ) return;

	getOStream() << "ES " << sim->getCurrentTime() 
      << " 3" << std::endl << getEOL();
}

void RDOTrace::writeTraceEnd(RDOSimulatorTrace *sim)
{
	if ( isNull() ) return;

   getOStream() << "ES " << sim->getCurrentTime() 
      << " 2" << std::endl << getEOL();
}

void RDOTrace::writeStatus( RDOSimulatorTrace* sim, char* status )
{
	if ( isNull() ) return;

	// Статус
	getOStream() << "$Status = " << status << " " << sim->getCurrentTime() << std::endl << getEOL();

	// Статистика по поиску на графе
	RDOLogicContainer::CIterator it = sim->m_logics.begin();
	while ( it != sim->m_logics.end() ) {
		RDODPTSearchTrace* dp = dynamic_cast<RDODPTSearchTrace*>(*it);
		if ( dp ) {
			// Информация о точке
			getOStream() << std::endl << getEOL();
			getOStream() << "DPS_C"
			             << "  " << dp->id
			             << "  " << dp->calc_cnt
			             << "  " << dp->calc_res_found_cnt
			             << std::endl << getEOL();
			if ( dp->calc_cnt ) {
				// Время поиска
				double d_min = 0;
				double d_max = 0;
				double d_med = 0;
				dp->getStats( dp->calc_times, d_min, d_max, d_med );
				getOStream() << rdo::format( "DPS_TM %0.3f  %0.3f  %0.3f", d_med, d_min, d_max ) << std::endl << getEOL();

				// Используемая память
				unsigned int ui_min = 0;
				unsigned int ui_max = 0;
				dp->getStats( dp->calc_mems, ui_min, ui_max, d_med );
				getOStream() << rdo::format( "DPS_ME %0.0f  %u  %u", d_med, ui_min, ui_max ) << std::endl << getEOL();

				// Стоимость решения
				dp->getStats( dp->calc_cost, d_min, d_max, d_med );
				getOStream() << "DPS_CO"
				             << " "  << d_med
				             << "  " << d_min
				             << "  " << d_max
				             << std::endl << getEOL();

				// Количество раскрытых вершин
				dp->getStats( dp->calc_nodes_expended, ui_min, ui_max, d_med );
				getOStream() << rdo::format( "DPS_TO %0.0f  %u  %u", d_med, ui_min, ui_max ) << std::endl << getEOL();

				// Количество вершин в графе
				dp->getStats( dp->calc_nodes_in_graph, ui_min, ui_max, d_med );
				getOStream() << rdo::format( "DPS_TT %0.0f  %u  %u", d_med, ui_min, ui_max ) << std::endl << getEOL();

				// Количество включавшихся в граф вершин (вершины, соответствующие одному и тому же состоянию системы, могут включаться в граф неоднократно, если порождается вершина с меньшей стоимостью пути)
				dp->getStats( dp->calc_nodes, ui_min, ui_max, d_med );
				getOStream() << rdo::format( "DPS_TI %0.0f  %u  %u", d_med, ui_min, ui_max ) << std::endl << getEOL();

				// Общее количество порожденных вершин-преемников
				dp->getStats( dp->calc_nodes_full, ui_min, ui_max, d_med );
				getOStream() << rdo::format( "DPS_TG %0.0f  %u  %u", d_med, ui_min, ui_max ) << std::endl << getEOL();
			}
		}
		it++;
	}

	// Используемая память
	getOStream() << std::endl << getEOL();
	getOStream() << "DPS_MM " << sim->memory_get() << std::endl << getEOL();
}

bool RDOPokazTrace::tracePokaz()
{
	if ( !trace || !wasChanged ) {
		return false;
	}
	sim->getTracer()->writePokaz( sim, this );
	wasChanged = false;
	return true;
}

void RDOTrace::writePokaz(RDOSimulatorTrace *sim, RDOPokazTrace *pok)
{
	if ( isNull() || !canWrite() ) return;

	getOStream() << "V  "  << sim->getCurrentTime() 
		<< " " << pok->traceId() 
		<< "  " << pok->traceValue() << std::endl << getEOL();
}

} // namespace rdoRuntime;
