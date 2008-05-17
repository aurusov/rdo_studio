#include "pch.h"
#include "rdoprocess.h"
#include "rdocalc.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

namespace rdoRuntime {

// ----------------------------------------------------------------------------
// ---------- RDOPROCProcess
// ----------------------------------------------------------------------------
RDOPROCProcess::RDOPROCProcess( const std::string& _name, RDOSimulator* sim ):
	RDOLogic( sim ),
	name( _name ),
	parent( NULL )
{
}

void RDOPROCProcess::insertChild( RDOPROCProcess* value )
{
	if ( value ) {
		child.push_back( value );
		value->parent = this;
	}
}

void RDOPROCProcess::next( RDOPROCTransact* transact )
{
/*	Iterator it = std::find( begin(), end(), transact->block );
	if ( it != end() ) {
		RDOPROCBlock* block = static_cast<RDOPROCBlock*>(*it);
		std::list< RDOPROCTransact* >::iterator it_res = std::find( block->transacts.begin(), block->transacts.end(), transact );
		if ( it_res != block->transacts.end() ) {
			block->transacts.erase( it_res );
		}
		it++;
		if ( it != end() ) {
			transact->block = static_cast<RDOPROCBlock*>(*it);
			static_cast<RDOPROCBlock*>(*it)->transacts.push_back( transact );
		} else {
			// run-time error
		}
	} else {
		// run-time error
	}
*/
	if ( transact->block )
	{
	Iterator it = std::find( begin(), end(), transact->block );
	if ( it != end() ) {
		RDOPROCBlock* block = static_cast<RDOPROCBlock*>(*it);
		std::list< RDOPROCTransact* >::iterator it_res = std::find( block->transacts.begin(), block->transacts.end(), transact );
		if ( it_res != block->transacts.end() ) {
		block->TransactGoOut( *it_res );
		}
		it++;
			if ( it != end() ) 
			{
			transact->block = static_cast<RDOPROCBlock*>(*it);
			static_cast<RDOPROCBlock*>(*it)->TransactGoIn( transact );
			} 
			else 
			{
			//getRuntime()->error( "Внутренняя ошибка: block_it == blocks.end()" );
			//run-time error
			}
		} 
		else 
		{
		//getRuntime()->error( "Внутренняя ошибка: block_it == blocks.end()" );
		//run-time error
		}
	} 
	else 
	{
	//getRuntime()->error( "Внутренняя ошибка: transact->block == NULL" );
	//run-time error
	}

}

// ----------------------------------------------------------------------------
// ---------- RDOPROCTransact
// ----------------------------------------------------------------------------
int RDOPROCTransact::typeID = -1;

RDOPROCTransact::RDOPROCTransact( RDOSimulator* sim, RDOPROCBlock* _block ):
	RDOResource( static_cast<RDORuntime*>(sim), -1, typeID, true ),
	block( _block )
{
	static_cast<RDORuntime*>(sim)->insertNewResource( this );
	setTrace( true );
	m_temporary = true;
	m_state     = RDOResource::CS_Create;
	m_params.push_back( sim->getCurrentTime() );
}

void RDOPROCTransact::next()
{
	block->process->next( this );
}

void RDOPROCTransact::addRes (RDOPROCResource* res){
resources.push_back( res );
}

void RDOPROCTransact::removeRes (RDOPROCResource* res){
resources.remove( res );
}

bool RDOPROCTransact::findRes(RDOPROCResource* res)
{
//Ищем ресурс в списке освобождаемых ресурсов транзакта			
std::list< RDOPROCResource* >::iterator it_res = std::find( this->resources.begin(), this->resources.end(), res );
return it_res != this->resources.end() ? true : false; 
}
// ----------------------------------------------------------------------------
// ---------- RDOPROCResource
// ----------------------------------------------------------------------------
RDOPROCResource::RDOPROCResource( RDORuntime* _runtime, int _number, unsigned int type, bool _trace ):
	RDOResource( _runtime, _number, type, _trace )
{
}

// ----------------------------------------------------------------------------
// ---------- RDOPROCBlock
// ----------------------------------------------------------------------------
RDOPROCBlock::RDOPROCBlock( RDOPROCProcess* _process ):
	RDOBaseOperation( _process ),
	process( _process )
{
	process->append( this );
}


void RDOPROCBlock::TransactGoIn( RDOPROCTransact* _transact )
{
	transacts.push_back( _transact );
}

void RDOPROCBlock::TransactGoOut( RDOPROCTransact* _transact )
{
	transacts.remove( _transact );
}

// ----------------------------------------------------------------------------
// ---------- RDOPROCGenerate
// ----------------------------------------------------------------------------
void RDOPROCGenerate::onStart( RDOSimulator* sim )
{
	calcNextTimeInterval( sim );
}

bool RDOPROCGenerate::onCheckCondition( RDOSimulator* sim )
{
	return sim->getCurrentTime() >= timeNext ? true : false;
}

RDOBaseOperation::BOResult RDOPROCGenerate::onDoOperation( RDOSimulator* sim )
{
	TRACE( "%7.1f GENERATE\n", sim->getCurrentTime() );
	calcNextTimeInterval( sim );
	RDOPROCTransact* transact = new RDOPROCTransact( sim, this );
	RDOTrace* tracer = static_cast<RDORuntime*>(sim)->getTracer();
	if ( !tracer->isNull() ) {
		tracer->getOStream() << transact->traceResourceState('\0', static_cast<RDORuntime*>(sim)) << tracer->getEOL();
	}
	transact->next();
	return RDOBaseOperation::BOR_done;
}

void RDOPROCGenerate::calcNextTimeInterval( RDOSimulator* sim )
{
	sim->addTimePoint( timeNext = timeCalc->calcValue( static_cast<RDORuntime*>(sim) ).getDouble() + sim->getCurrentTime(), process, this );
}

// ----------------------------------------------------------------------------
// ---------- RDOPROCBlockForSeize
// ----------------------------------------------------------------------------
RDOPROCBlockForSeize::RDOPROCBlockForSeize( RDOPROCProcess* _process, std::vector < parser_for_Seize > From_Par ):
	RDOPROCBlock( _process ),
//	rss( NULL ),
	from_par( From_Par )
{	
}

void RDOPROCBlockForSeize::onStart( RDOSimulator* sim )
{
	// todo: если потребуется стоить деревья, вершинами которых будут полные снимки БД,
	// как при DPT search, то инициализацию атрибутов надо будет делать в checkOperation
	int size = from_par.size();
	std::vector < parser_for_Seize >::iterator it1 = from_par.begin();
	while ( it1 != from_par.end() ) {
		int Id_res = (*it1).Id_res;
		int Id_param = (*it1).Id_param;
		RDOResource* res = static_cast<RDORuntime*>(sim)->getResourceByID( Id_res );
		runtime_for_Seize bbb;
		bbb.rss = static_cast<RDOPROCResource*>(res);
		bbb.enum_free = RDOValue( bbb.rss->getParam(Id_param).getEnum(), RDOPROCBlockForSeize::getStateEnumFree() );
		bbb.enum_buzy = RDOValue( bbb.rss->getParam(Id_param).getEnum(), RDOPROCBlockForSeize::getStateEnumBuzy() );
		from_run.push_back(bbb);
		it1++;
	}
}


// ----------------------------------------------------------------------------
// ---------- RDOPROCSeize
// ----------------------------------------------------------------------------

bool RDOPROCSeize::onCheckCondition( RDOSimulator* sim )
{
	if ( !transacts.empty() ) {
	int Size_Seize = from_run.size();
	int aaa = transacts.front()->getTraceID();	
			for(int i=0; i<Size_Seize; i++)
			{
			int bbb = from_run[i].rss->transacts.front()->getTraceID();
				if(aaa == bbb){
				RDOTrace* tracer = static_cast<RDORuntime*>(sim)->getTracer();
					if ( !tracer->isNull() ) {
					tracer->getOStream() << from_run[i].rss->traceResourceState('\0', static_cast<RDORuntime*>(sim)) << tracer->getEOL();
					}
					// Занимаем все, что свободно
					if ( from_run[i].rss->getParam(from_par[i].Id_param) == from_run[i].enum_free ) {
					Busy_Res++;
					from_run[i].rss->setParam(from_par[i].Id_param, from_run[i].enum_buzy);			
					//Записываем в транзакт, какие ресурсы он занимает
					transacts.front()->addRes(from_run[i].rss);
					} 
					else {
					}
				}
				else
				{
				int t=0;
				}		
			}	
		if ( Busy_Res == Size_Seize){
		Busy_Res = 0;
		return true;
		}
		else {
		TRACE( "%7.1f SEIZE CANNOT\n", sim->getCurrentTime() );
		}
	}
return false;
}

RDOBaseOperation::BOResult RDOPROCSeize::onDoOperation( RDOSimulator* sim )
{
	TRACE( "%7.1f SEIZE\n", sim->getCurrentTime() );
	//from_run[0].rss->setParam(from_par[0].Id_param, from_run[0].enum_buzy);
	transacts.front()->next();
	return RDOBaseOperation::BOR_done;
}

void RDOPROCSeize::TransactGoIn( RDOPROCTransact* _transact )
{
int Size_Seize = from_run.size();
	for(int i=0;i<Size_Seize; i++){
	from_run[i].rss->transacts.push_back( _transact );
	}
	RDOPROCBlockForSeize::TransactGoIn( _transact );
}

void RDOPROCSeize::TransactGoOut( RDOPROCTransact* _transact )
{
int Size_Seize = from_run.size();
	for(int i=0;i<Size_Seize; i++){
	from_run[i].rss->transacts.remove( _transact );
	}
	RDOPROCBlockForSeize::TransactGoOut( _transact );
}
// ----------------------------------------------------------------------------
// ---------- RDOPROCRelease
// ----------------------------------------------------------------------------
bool RDOPROCRelease::onCheckCondition( RDOSimulator* sim )
{
	if ( !transacts.empty() ) {
	int Size_Seize = from_run.size();
		for(int i=0; i<Size_Seize; i++)
		{
			RDOTrace* tracer = static_cast<RDORuntime*>(sim)->getTracer();
			if ( !tracer->isNull() ) {
				tracer->getOStream() << from_run[i].rss->traceResourceState('\0', static_cast<RDORuntime*>(sim)) << tracer->getEOL();
			}
			// Занят
			if ( from_run[i].rss->getParam(from_par[i].Id_param) == from_run[i].enum_buzy ) {
				//Ищем ресурс в списке освобождаемых ресурсов транзакта			
				if ( transacts.front()->findRes(from_run[i].rss) ) {
				from_run[i].rss->setParam(from_par[i].Id_param, from_run[i].enum_free);
				transacts.front()->removeRes(from_run[i].rss);
				}
				else{
				//Модель составлена неправильно
				TRACE( "неправильно составлена модель\n", sim->getCurrentTime() );
				}
			}
			else{
			TRACE( "%7.1f RELEASE %s\n", sim->getCurrentTime(), from_par[i].Id_res);
			}
		}
	return true;
	}
return false;
}

RDOBaseOperation::BOResult RDOPROCRelease::onDoOperation( RDOSimulator* sim )
{
	TRACE( "%7.1f RELEASE\n", sim->getCurrentTime() );
	//from_run[0].rss->setParam(from_par[0].Id_param, from_run[0].enum_free);
	transacts.front()->next();
	return RDOBaseOperation::BOR_done;
}

// ----------------------------------------------------------------------------
// ---------- RDOPROCAdvance
// ----------------------------------------------------------------------------
bool RDOPROCAdvance::onCheckCondition( RDOSimulator* sim )
{
	if ( !transacts.empty() ) {
		return true;
	} else if ( !leave_list.empty() ) {
		double tnow = sim->getCurrentTime();
		std::list< LeaveTr >::iterator it = leave_list.begin();
		while ( it != leave_list.end() ) {
			if ( tnow >= it->timeLeave ) {
				return true;
			}
			it++;
		}
	}
	return false;
}

RDOBaseOperation::BOResult RDOPROCAdvance::onDoOperation( RDOSimulator* sim )
{
	if ( !transacts.empty() ) {
		TRACE( "%7.1f ADVANCE BEGIN\n", sim->getCurrentTime() );
		double timeLeave = delayCalc->calcValue( static_cast<RDORuntime*>(sim) ).getDouble() + sim->getCurrentTime();
		leave_list.push_back( LeaveTr(transacts.front(), timeLeave) );
		transacts.erase( transacts.begin() );
		sim->addTimePoint( timeLeave, process, this );
//		RDOTrace* tracer = sim->getTracer();
//		if ( !tracer->isNull() ) {
//			tracer->getOStream() << res->traceResourceState('\0', sim) << tracer->getEOL();
//		}
		return RDOBaseOperation::BOR_done;
	} else if ( !leave_list.empty() ) {
		double tnow = sim->getCurrentTime();
		std::list< LeaveTr >::iterator it = leave_list.begin();
		while ( it != leave_list.end() ) {
			if ( tnow >= it->timeLeave ) {
//				RDOTrace* tracer = sim->getTracer();
//				if ( !tracer->isNull() ) {
//					tracer->getOStream() << res->traceResourceState('\0', sim) << tracer->getEOL();
//				}
				TRACE( "%7.1f ADVANCE END\n", it->timeLeave );
				it->transact->next();
				leave_list.erase( it );
				return RDOBaseOperation::BOR_planned_and_run;
			}
			it++;
		}
	}
	return RDOBaseOperation::BOR_cant_run;
}

// ----------------------------------------------------------------------------
// ---------- RDOPROCTerminate
// ----------------------------------------------------------------------------
bool RDOPROCTerminate::onCheckCondition( RDOSimulator* sim )
{
	return !transacts.empty() ? true : false;
}

RDOBaseOperation::BOResult RDOPROCTerminate::onDoOperation( RDOSimulator* sim )
{
	TRACE( "%7.1f TERMINATE\n", sim->getCurrentTime() );
	RDOPROCTransact* transact = transacts.front();
	transact->setState( RDOResource::CS_Erase );
	RDOTrace* tracer = static_cast<RDORuntime*>(sim)->getTracer();
	if ( !tracer->isNull() ) {
		tracer->getOStream() << transact->traceResourceState('\0', static_cast<RDORuntime*>(sim)) << tracer->getEOL();
	}
	static_cast<RDORuntime*>(sim)->onEraseRes( transact->getTraceID(), NULL );
	transacts.erase( transacts.begin() );
	return RDOBaseOperation::BOR_done;
}

} // rdoRuntime
