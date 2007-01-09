#include "pch.h"
#include "rdotrace.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

void RDOTrace::writeSearchBegin(double currentTime, std::string decisionPointId)
{
	if(isNullTracer)
		return;

   getOStream() << "SB " << currentTime << " " << decisionPointId.c_str() << std::endl << getEOL();
}
void RDOTrace::writeSearchDecisionHeader()
{
	if(isNullTracer)
		return;

   getOStream() << "SD" << std::endl << getEOL();
}
void RDOTrace::writeSearchDecision(RDOSimulator *sim, TreeNode *node) 
{
	if(isNullTracer)
		return;

   RDOSimulatorTrace *simTr = (RDOSimulatorTrace *)sim;
   RDOActivityTrace *actTr = (RDOActivityTrace *)node->activity;
   RDORuleTrace *ruleTr = (RDORuleTrace *)actTr->rule;

   getOStream() << node->count
       << " " << actTr->traceId()
       << " " << ruleTr->tracePatternId()
       << " " << ruleTr->traceResourcesListNumbers( simTr )
       << std::endl << getEOL();
}
void RDOTrace::writeString(std::string str)
{
	if(isNullTracer)
		return;

   getOStream() << str << std::endl << getEOL();
}
void RDOTrace::writeSearchOpenNode(int nodeCount, 
   int parentCount,
   double pathCost, 
   double restCost)
{
	if(isNullTracer)
		return;

   getOStream() << "SO " << nodeCount
       << " " << parentCount
       << " " << pathCost
       << " " << restCost << std::endl << getEOL();
}
void RDOTrace::writeSearchNodeInfo(char sign, TreeNodeTrace *node)
{
	if(isNullTracer)
		return;

   RDODecisionPointTrace *dpTrace = (RDODecisionPointTrace *)node->root->dp;
   if(dpTrace->traceFlag == DPT_trace_tops || dpTrace->traceFlag == DPT_trace_all)
   {
      RDOActivityTrace *actTr = (RDOActivityTrace *)node->currAct;
      RDORuleTrace *ruleTr = (RDORuleTrace *)actTr->rule;
      RDOSimulatorTrace *sim = (RDOSimulatorTrace *)node->sim;

		getOStream().precision(4);
      getOStream() << "ST" << sign
         << " " << ((sign != 'D')?node->count:node->root->nodeCount-1)
			<< " " << ((sign != 'D')?node->parent->count:node->count)
         << " " << ((sign != 'D')?node->costPath:node->newCostPath)
         << " " << ((sign != 'D')?node->costRest:node->newCostRest)
         << " " << actTr->traceId()
         << " " << ruleTr->tracePatternId()
         << " " << ((sign != 'D')?node->costRule:node->newCostRule)
         << " " << ruleTr->traceResourcesListNumbers( sim ) << std::endl << getEOL();
   
      RDODecisionPointTrace *dpTrace = (RDODecisionPointTrace *)node->root->dp;
      if(dpTrace->traceFlag == DPT_trace_all)
         getOStream() << ruleTr->traceResourcesList( 'S', sim ) << getEOL();
   }
}

void RDOTrace::writeSearchResult( char letter, RDOSimulatorTrace* simTr, TreeRoot* treeRoot )
{
	if ( isNullTracer ) return;

	SYSTEMTIME systime_current;
	::GetSystemTime( &systime_current );
	unsigned int msec_current = RDOSimulatorBase::getMSec( systime_current );
	unsigned int msec_begin   = RDOSimulatorBase::getMSec( treeRoot->systime_begin );
	double sec_delay = 0;
	if ( systime_current.wYear == treeRoot->systime_begin.wYear && systime_current.wMonth == treeRoot->systime_begin.wMonth ) {
		sec_delay = static_cast<double>(msec_current - msec_begin) / 1000 + (systime_current.wDay - treeRoot->systime_begin.wDay) * 24 * 60 * 60;
	}
	static_cast<RDODecisionPointTrace*>(treeRoot->dp)->calc_times.push_back( sec_delay );

	getOStream() << "SE" << letter
	             << " " << simTr->getCurrentTime()
	             << " " << sec_delay            // realTime
	             << " " << treeRoot->sizeof_dpt // memUsed
	             << " " << (letter == 'S' ? treeRoot->targetNode->costPath : 0)
	             << " " << treeRoot->expandedNodesCount
	             << " " << treeRoot->nodesInGraphCount
	             << " " << treeRoot->nodeCount - 1
	             << " " << treeRoot->fullNodesCount 
	             << std::endl << getEOL();
	if ( letter == 'S' ) {
		getOStream() << "ES"
		             << " " << simTr->getCurrentTime()
		             << " 4"
		             << std::endl << getEOL();
		static_cast<RDODecisionPointTrace*>(treeRoot->dp)->calc_cost.push_back( treeRoot->targetNode->costPath );
		static_cast<RDODecisionPointTrace*>(treeRoot->dp)->calc_nodes.push_back( treeRoot->nodeCount - 1 );
		static_cast<RDODecisionPointTrace*>(treeRoot->dp)->calc_nodes_expended.push_back( treeRoot->expandedNodesCount );
		static_cast<RDODecisionPointTrace*>(treeRoot->dp)->calc_nodes_full.push_back( treeRoot->fullNodesCount );
		static_cast<RDODecisionPointTrace*>(treeRoot->dp)->calc_nodes_in_graph.push_back( treeRoot->nodesInGraphCount );
	}
}

void RDOTrace::writePermanentResources( RDOSimulatorTrace* sim, const std::list< RDOResourceTrace* >& res_perm )
{
	if(isNullTracer)
		return;

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
		if ( state == RDOResourceTrace::CS_NoChange || state == RDOResourceTrace::CS_NonExist || state == RDOResourceTrace::CS_None ) return "";
		if ( prefix != '\0' ) res << prefix;
		switch ( state ) {
			case RDOResourceTrace::CS_Create: res << "RC "; break;
			case RDOResourceTrace::CS_Erase : res << "RE " << sim->getCurrentTime() << " " << traceTypeId() << " " << traceId() << std::endl; return res.str(); break;
			default                         : res << "RK "; break;
		}
		res << sim->getCurrentTime() << " " << traceTypeId() << " " << traceId() << " " << traceParametersValue() << std::endl;
	}
	return res.str();
}

void RDOTrace::writeIrregularEvent(RDOIETrace *ie, RDOSimulatorTrace *sim)
{
	if ( isNullTracer ) return;

	if ( ie->trace && canWrite() ) {
		getOStream() << "EI " << sim->getCurrentTime()
		             << " "   << ie->traceId() 
		             << " "   << ie->tracePatternId() 
		             << " "   << ie->traceResourcesListNumbers( sim )
		             << std::endl << getEOL();
	}
	getOStream() << ie->traceResourcesList( '\0', sim ) << getEOL();
}

void RDOTrace::writeRule(RDORuleTrace *rule, RDOSimulatorTrace *sim)
{
	if ( isNullTracer ) return;

	if ( rule->trace ) {
		int operId = sim->getFreeOperationId();
		getOStream() << "ER " << sim->getCurrentTime()
		             << " "   << operId
		             << " "   << rule->traceId() 
		             << " "   << rule->tracePatternId()
		             << " "   << rule->traceResourcesListNumbers( sim )
		             << std::endl << getEOL();
		sim->freeOperationId(operId);
	}
	getOStream() << rule->traceResourcesList( '\0', sim ) << getEOL();
}

void RDOTrace::writeAfterOperationBegin( RDOOperationTrace* op, RDOSimulatorTrace* sim )
{
	if ( isNullTracer ) return;

	if ( op->trace ) {
		getOStream() << "EB " << sim->getCurrentTime() 
		             << " "   << op->traceOperId() 
		             << " "   << op->traceId() 
		             << " "   << op->tracePatternId() 
		             << " "   << op->traceResourcesListNumbers( sim ) << std::endl << getEOL(); 
	}
	getOStream() << op->traceResourcesList( '\0', sim ) << getEOL();
}

void RDOTrace::writeAfterOperationEnd( RDOOperationTrace* op, RDOSimulatorTrace* sim )
{
	if ( isNullTracer ) return;

	if ( op->trace ) {
		getOStream() << "EF " << sim->getCurrentTime() 
		             << " "   << op->traceOperId() 
		             << " "   << op->traceId() 
		             << " "   << op->tracePatternId() 
		             << " "   << op->traceResourcesListNumbers( sim )
		             << std::endl << getEOL();
	}
	getOStream() << op->traceResourcesList( '\0', sim ) << getEOL();
}

RDOResourceTrace::RDOResourceTrace(RDOSimulatorTrace *i_sim):
	RDOTraceableObject(i_sim),
	state( RDOResourceTrace::CS_None ),
	temporary( false )
{
	id = sim->getFreeResourceId();
	trace = false;
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
	if(isNullTracer)
		return;

   getOStream() << "ES " << sim->getCurrentTime() 
      << " 1" << std::endl << getEOL();
}

void RDOTrace::writeModelBegin(RDOSimulatorTrace *sim)
{
	if(isNullTracer)
		return;

   getOStream() << "ES " << sim->getCurrentTime() 
      << " 3" << std::endl << getEOL();
}

void RDOTrace::writeTraceEnd(RDOSimulatorTrace *sim)
{
	if(isNullTracer)
		return;

   getOStream() << "ES " << sim->getCurrentTime() 
      << " 2" << std::endl << getEOL();
}

void RDOTrace::writeStatus( RDOSimulatorTrace* sim, char* status )
{
	if ( isNullTracer ) return;

	// Статус
	getOStream() << "$Status = " << status << " " << sim->getCurrentTime() << std::endl << getEOL();

	// Статистика по поиску на графе
	std::vector< RDOBaseOperation* >::const_iterator it = sim->haveBaseOperations.begin();
	while ( it != sim->haveBaseOperations.end() ) {
		RDODecisionPointTrace* dp = dynamic_cast<RDODecisionPointTrace*>(*it);
		if ( dp ) {
			// Информация и точке
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
	if(!trace || !wasChanged)
		return false;

	sim->getTracer()->writePokaz(sim, this);
	wasChanged = false;
	return true;
}

void RDOTrace::writePokaz(RDOSimulatorTrace *sim, RDOPokazTrace *pok)
{
	if(isNullTracer)
		return;

	getOStream() << "V  "  << sim->getCurrentTime() 
		<< " " << pok->traceId() 
		<< "  " << pok->traceValue() << std::endl << getEOL();
}

