#include "pch.h"
#include "rdoprocess.h"
#include "rdocalc.h"
#include <rdotypes.h>
#include <rdomacros.h>

namespace rdoRuntime {

// ----------------------------------------------------------------------------
// ---------- RDOPROCProcess
// ----------------------------------------------------------------------------
RDOPROCProcess::RDOPROCProcess(CREF(tstring) name, PTR(RDOSimulator) sim)
	: RDOLogic(sim )
	, m_name  (name)
	, m_parent(NULL)
{}

void RDOPROCProcess::insertChild(PTR(RDOPROCProcess) value)
{
	if (value)
	{
		m_child.push_back(value);
		value->m_parent = this;
	}
}

void RDOPROCProcess::next( RDOPROCTransact* transact )
{
	if ( transact->block )
	{
	Iterator it = std::find( begin(), end(), transact->block );
	//Если у транзакта есть блок
		if ( it != end() ) 
		{
			//Берем этот блок
			RDOPROCBlock* block = static_cast<RDOPROCBlock*>(*it);
			//Находим перемещаемый транзакт в списке его транзактов
			RDOPROCBlock::TransactList::iterator it_res = std::find( block->transacts.begin(), block->transacts.end(), transact );
			//Если транзакт найден
			if ( it_res != block->transacts.end() ) 
			{
				//Удаляем его из списка транзактов этого блока
				block->TransactGoOut( *it_res );
			}
			else
			{
				//Скорее всего здесь не будет ошибки, поскольку advance удаляет транзакты из себя
				//	getRuntime()->error( "Внутренняя ошибка: неучтенный транзакт (транзакт потерял свой блок)" );
			}
			//Переходим к следующему блоку
			it++;
			//Если следующий блок существует
			if ( it != end() ) 
			{
				//Берем этот блок
				block = static_cast<RDOPROCBlock*>(*it);	
				transact->block = block;
				//Записываем в конец списка этого блока перемещаемый транзакт
				block->TransactGoIn( transact );
			}
			//Блок в из которого нужно было переместить транзакт был последним
			else 
			{
				//---------Вход в этот блок означает, что it-1 = последний блок для транзакта, 
				//---------в этом случае правильно будет удалить этот транзакт?????????
				//getRuntime()->error( "Некуда перемещать транзакт, блок в котором он был - последний" );
			}
		} 
		else
		{
			//getRuntime()->error( "Не существует такого блока, к которому привязан транзакт" );
		}
	} 
	else 
	{
		//getRuntime()->error( "К транзакту не привязан никакой блок" );
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

RDOPROCBlock* RDOPROCTransact::getBlock()
{
	return this->block;
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
//	TRACE1( "%7.1f GENERATE\n", sim->getCurrentTime() );
	calcNextTimeInterval( sim );
	RDOPROCTransact* transact = new RDOPROCTransact( sim, this );
	RDOTrace* tracer = static_cast<RDORuntime*>(sim)->getTracer();
	if ( !tracer->isNull() ) 
	{
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
// ---------- RDOPROCBlockForQueue
// ----------------------------------------------------------------------------
RDOPROCBlockForQueue::RDOPROCBlockForQueue( RDOPROCProcess* _process, parser_for_Queue From_Par ):
	RDOPROCBlock( _process ),
	fromParser ( From_Par)
{
}

void RDOPROCBlockForQueue::onStart( RDOSimulator* sim )
{
	int Id_res = fromParser.Id_res;
	int Id_param = fromParser.Id_param;
	RDOResource* res = static_cast<RDORuntime*>(sim)->getResourceByID( Id_res );
	forRes.Id_param = Id_param;
	forRes.rss = static_cast<RDOPROCResource*>(res);
	forRes.defaultValue = RDOValue( RDOPROCQueue::getDefaultValue() );
}

// ----------------------------------------------------------------------------
// ---------- RDOPROCQueue
// ----------------------------------------------------------------------------
bool RDOPROCQueue::onCheckCondition( RDOSimulator* sim )
{
	if ( !transacts.empty() ) 
	{
		RDOValue i = forRes.rss->getParam( forRes.Id_param );
		RDOValue j = RDOValue( int (1) );
		forRes.rss->setParam( forRes.Id_param, i + j );	
		return true;
	}
	else
	{
		return false;
	}
}

RDOBaseOperation::BOResult RDOPROCQueue::onDoOperation( RDOSimulator* sim )
{
	TRACE1(_T("%7.1f QUEUE\n"), sim->getCurrentTime());
	transacts.front()->next();
	return RDOBaseOperation::BOR_done;
}

// ----------------------------------------------------------------------------
// ---------- RDOPROCDepart
// ----------------------------------------------------------------------------
bool RDOPROCDepart::onCheckCondition( RDOSimulator* sim )
{
	if ( !transacts.empty() ) 
	{
		RDOValue i = forRes.rss->getParam( forRes.Id_param );
		RDOValue j = RDOValue( int (1) );
		forRes.rss->setParam( forRes.Id_param, i - j );	
		return true;
	}
	else
	{
		return false;
	}
}

RDOBaseOperation::BOResult RDOPROCDepart::onDoOperation( RDOSimulator* sim )
{
	TRACE1(_T("%7.1f DEPART\n"), sim->getCurrentTime());
	transacts.front()->next();
	return RDOBaseOperation::BOR_done;
}
/*
// ----------------------------------------------------------------------------
// ---------- RDOPROCBlockForSeize
// ----------------------------------------------------------------------------
RDOPROCBlockForSeize::RDOPROCBlockForSeize( RDOPROCProcess* _process, parser_for_Seize From_Par ):
	RDOPROCBlock( _process ),
	fromParser ( From_Par)
{
}

void RDOPROCBlockForSeize::onStart( RDOSimulator* sim )
{
	// todo: если потребуется стоить деревья, вершинами которых будут полные снимки БД,
	// как при DPT search, то инициализацию атрибутов надо будет делать в checkOperation
	int Id_res = fromParser.Id_res;
	int Id_param = fromParser.Id_param;
	RDOResource* res = static_cast<RDORuntime*>(sim)->getResourceByID( Id_res );
	forRes.Id_param = Id_param;
	forRes.rss = static_cast<RDOPROCResource*>(res);
	forRes.enum_free = RDOValue( forRes.rss->getParam(Id_param).getEnum(), RDOPROCBlockForSeize::getStateEnumFree() );
	forRes.enum_buzy = RDOValue( forRes.rss->getParam(Id_param).getEnum(), RDOPROCBlockForSeize::getStateEnumBuzy() );
	forRes.enum_break = RDOValue( forRes.rss->getParam(Id_param).getEnum(), RDOPROCBlockForSeize::getStateEnumBreak() );
}


// ----------------------------------------------------------------------------
// ---------- RDOPROCSeize
// ----------------------------------------------------------------------------
bool RDOPROCSeize::onCheckCondition( RDOSimulator* sim )
{
	if ( !transacts.empty() ) 
	{
		int idBlocksTransact = transacts.front()->getTraceID();	
		int idResourcesTransact = forRes.rss->transacts.front()->getTraceID();
		if( idBlocksTransact == idResourcesTransact )
		{
			// Занимаем ресурс, если свободен
			if ( forRes.rss->getParam(forRes.Id_param) == forRes.enum_free ) 
			{
				forRes.rss->setParam(forRes.Id_param, forRes.enum_buzy);	
				RDOTrace* tracer = static_cast<RDORuntime*>( sim)->getTracer();
				if ( !tracer->isNull() ) 
				{
					tracer->getOStream() << forRes.rss->traceResourceState('\0', static_cast<RDORuntime*>(sim)) << tracer->getEOL();
				}
				//TRACE2(_T("%7.1f SEIZE-%d\n"), sim->getCurrentTime(), index);
				return true;
			} 
			else 
			{
				return false;			
			}
		}
		else 
		{
			return false;			
		}
	}
	return false;
}
		
RDOBaseOperation::BOResult RDOPROCSeize::onDoOperation( RDOSimulator* sim )
{
	transacts.front()->next();
	return RDOBaseOperation::BOR_done;
}

void RDOPROCSeize::TransactGoIn( RDOPROCTransact* _transact )
{
	forRes.rss->transacts.push_back( _transact );
	RDOPROCBlockForSeize::TransactGoIn( _transact );
}

void RDOPROCSeize::TransactGoOut( RDOPROCTransact* _transact )
{
	forRes.rss->transacts.remove( _transact );
	RDOPROCBlockForSeize::TransactGoOut( _transact );
}
// ----------------------------------------------------------------------------
// ---------- RDOPROCRelease
// ----------------------------------------------------------------------------
bool RDOPROCRelease::onCheckCondition( RDOSimulator* sim )
{
	if ( !transacts.empty() ) 
	{
		RDOTrace* tracer = static_cast<RDORuntime*>(sim)->getTracer();
		if ( !tracer->isNull() ) 
		{
			tracer->getOStream() << forRes.rss->traceResourceState('\0', static_cast<RDORuntime*>(sim)) << tracer->getEOL();
		}
		// Занят
		if ( forRes.rss->getParam(forRes.Id_param) == forRes.enum_buzy ) 
		{
			forRes.rss->setParam(forRes.Id_param, forRes.enum_free);
			return true;
		}
	}
	return false;
}

RDOBaseOperation::BOResult RDOPROCRelease::onDoOperation( RDOSimulator* sim )
{
//	TRACE1(_T("%7.1f RELEASE\n"), sim->getCurrentTime());
	transacts.front()->next();
	return RDOBaseOperation::BOR_done;
}
*/

// ----------------------------------------------------------------------------
// ---------- RDOPROCBlockForSeizes
// ----------------------------------------------------------------------------
RDOPROCBlockForSeize::RDOPROCBlockForSeize( RDOPROCProcess* _process, std::vector < parser_for_Seize > From_Par ):
	RDOPROCBlock( _process ),
	fromParser( From_Par )
{	
}

void RDOPROCBlockForSeize::onStart( RDOSimulator* sim )
{
	// todo: если потребуется стоить деревья, вершинами которых будут полные снимки БД,
	// как при DPT search, то инициализацию атрибутов надо будет делать в checkOperation
	int size = fromParser.size();
	std::vector < parser_for_Seize >::iterator it1 = fromParser.begin();
	while ( it1 != fromParser.end() ) 
	{
		int Id_res = (*it1).Id_res;
		int Id_param = (*it1).Id_param;
		RDOResource* res = static_cast<RDORuntime*>(sim)->getResourceByID( Id_res );
		runtime_for_Seize bbb;
		bbb.Id_param = Id_param;
		bbb.rss = static_cast<RDOPROCResource*>(res);
		bbb.enum_free = RDOValue( bbb.rss->getParam(Id_param).getEnum(), RDOPROCBlockForSeize::getStateEnumFree() );
		bbb.enum_buzy = RDOValue( bbb.rss->getParam(Id_param).getEnum(), RDOPROCBlockForSeize::getStateEnumBuzy() );
		forRes.push_back(bbb);
		it1++;
	}
}


// ----------------------------------------------------------------------------
// ---------- RDOPROCSeizes
// ----------------------------------------------------------------------------
bool RDOPROCSeize::onCheckCondition(PTR(RDOSimulator) sim)
{
	if (transacts.empty())
		return false;

	ruint Size_Seizes = forRes.size();
	for (ruint i = 0; i < Size_Seizes; i++)
	{
		// если свободен
		if (forRes[i].rss->getParam(forRes[i].Id_param) == forRes[i].enum_free)
		{
			ruint idBlocksTransact    = transacts.front()->getTraceID();
			ruint idResourcesTransact = forRes[i].rss->transacts.front()->getTraceID();
			if (idBlocksTransact != idResourcesTransact)
				return false;

			PTR(RDOTrace) tracer = static_cast<PTR(RDORuntime)>(sim)->getTracer();
			forRes[i].rss->setParam(forRes[i].Id_param, forRes[i].enum_buzy);
			TRACE3(_T("%7.1f SEIZES-%d, resId = %d\n"), sim->getCurrentTime(), index, forRes[i].rss->getTraceID());
			if (!tracer->isNull())
				tracer->getOStream() << forRes[i].rss->traceResourceState(_T('\0'), static_cast<RDORuntime*>(sim)) << tracer->getEOL();

			transacts.front()->setRes(forRes[i].rss);
			return true;
		}
	}
	return false;
}

RDOBaseOperation::BOResult RDOPROCSeize::onDoOperation( RDOSimulator* sim )
{
	transacts.front()->next();
	return RDOBaseOperation::BOR_done;
}

void RDOPROCSeize::TransactGoIn( RDOPROCTransact* _transact )
{
	int Size_Seizes = forRes.size();
	for(int i=0;i<Size_Seizes; i++)
	{
		forRes[i].rss->transacts.push_back( _transact );
	}
	RDOPROCBlockForSeize::TransactGoIn( _transact );
}

void RDOPROCSeize::TransactGoOut( RDOPROCTransact* _transact )
{
	int Size_Seizes = forRes.size();
	for(int i=0;i<Size_Seizes; i++)
	{
		forRes[i].rss->transacts.remove( _transact );
	}
	RDOPROCBlockForSeize::TransactGoOut( _transact );
}

// ----------------------------------------------------------------------------
// ---------- RDOPROCReleases
// ----------------------------------------------------------------------------
bool RDOPROCRelease::onCheckCondition( RDOSimulator* sim )
{
	if ( !transacts.empty() ) 
	{
		int Size_Seizes = forRes.size();
		for(int i=0;i<Size_Seizes; i++)
		{
			if( forRes[i].rss == transacts.front()->getRes() )
			{
				// Занят
				if ( forRes[i].rss->getParam(forRes[i].Id_param) == forRes[i].enum_buzy ) 
				{
					RDOTrace* tracer = static_cast<RDORuntime*>(sim)->getTracer();
					TRACE3(_T("%7.1f RELEASES-%d, resId = %d\n"), sim->getCurrentTime(), index, forRes[i].rss->getTraceID());
					forRes[i].rss->setParam(forRes[i].Id_param, forRes[i].enum_free);
						if ( !tracer->isNull() ) 
						{
							tracer->getOStream() << forRes[i].rss->traceResourceState('\0', static_cast<RDORuntime*>(sim)) << tracer->getEOL();
						}				
					return true;
				}		
				// Не Занят и не свободен
				if ( forRes[i].rss->getParam(forRes[i].Id_param) != forRes[i].enum_free )
				{
					//Удаляем транзакт
					RDOTrace* tracer = static_cast<RDORuntime*>(sim)->getTracer();
					TRACE3(_T("%7.1f RELEASES_Bad-%d, resId = %d\n"), sim->getCurrentTime(), index, forRes[i].rss->getTraceID());
					RDOPROCTransact* transact = transacts.front();
					transact->setState( RDOResource::CS_Erase );
					if ( !tracer->isNull() )
					{
						tracer->getOStream() << transact->traceResourceState('\0', static_cast<RDORuntime*>(sim)) << tracer->getEOL();
					}
					transacts.remove( transact );
					forRes[i].rss->transacts.remove( transact );
					static_cast<RDORuntime*>(sim)->onEraseRes( transact->getTraceID(), NULL );
					return false;
				}
			}
		}
	}
	return false;
}

RDOBaseOperation::BOResult RDOPROCRelease::onDoOperation( RDOSimulator* sim )
{
	transacts.front()->next();
	return RDOBaseOperation::BOR_done;
}


// ----------------------------------------------------------------------------
// ---------- RDOPROCAdvance
// ----------------------------------------------------------------------------
bool RDOPROCAdvance::onCheckCondition( RDOSimulator* sim )
{
	if ( !transacts.empty() ) 
	{
		return true;
	} 
	else
	{
		if ( !leave_list.empty() ) 
		{
		double tnow = sim->getCurrentTime();
		std::list< LeaveTr >::iterator it = leave_list.begin();
			while ( it != leave_list.end() ) 
			{
				if ( tnow >= it->timeLeave ) 
				{
					return true;
				}
			it++;
			}
		}
	}
	return false;
}

RDOBaseOperation::BOResult RDOPROCAdvance::onDoOperation( RDOSimulator* sim )
{
	if ( !transacts.empty() ) {
//		TRACE1(_T("%7.1f ADVANCE BEGIN\n"), sim->getCurrentTime());
		double timeLeave = delayCalc->calcValue( static_cast<RDORuntime*>(sim) ).getDouble() + sim->getCurrentTime();
		leave_list.push_back( LeaveTr(transacts.front(), timeLeave) );
		transacts.erase( transacts.begin() );
		sim->addTimePoint( timeLeave, process, this );
		return RDOBaseOperation::BOR_done;
	} 
	else	if ( !leave_list.empty() ) {
			double tnow = sim->getCurrentTime();
			std::list< LeaveTr >::iterator it = leave_list.begin();
				while ( it != leave_list.end() ) {
					if ( tnow >= it->timeLeave ) {
//						TRACE1(_T("%7.1f ADVANCE END\n"), it->timeLeave);
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
//	TRACE1(_T("%7.1f TERMINATE\n"), sim->getCurrentTime());
	RDOPROCTransact* transact = transacts.front();
	transact->setState( RDOResource::CS_Erase );
	RDOTrace* tracer = static_cast<RDORuntime*>(sim)->getTracer();
	if ( !tracer->isNull() ) {
		tracer->getOStream() << transact->traceResourceState('\0', static_cast<RDORuntime*>(sim)) << tracer->getEOL();
	}
	static_cast<RDORuntime*>(sim)->onEraseRes( transact->getTraceID(), NULL );
	transacts.erase( transacts.begin() );
	int termNow = static_cast<RDORuntime*>(sim)->getCurrentTerm();
	termNow += getTerm();
	static_cast<RDORuntime*>(sim)->setCurrentTerm(termNow);
	return RDOBaseOperation::BOR_done;
}

// ----------------------------------------------------------------------------
// ---------- RDOPROCAssign
// ----------------------------------------------------------------------------
bool RDOPROCAssign::onCheckCondition( RDOSimulator* sim )
{
	if ( !transacts.empty() ) 
	{
		return true;
	} 
return false;
}

RDOBaseOperation::BOResult RDOPROCAssign::onDoOperation( RDOSimulator* sim )
{
	RDOResource* res = static_cast<RDORuntime*>(sim)->getResourceByID( t_resId );
	res->setParam( t_parId, paramValue->calcValue( static_cast<RDORuntime*>(sim) ) );	
	TRACE1(_T("%7.1f ASSIGN\n"), sim->getCurrentTime());
	transacts.front()->next();
	return RDOBaseOperation::BOR_done;
}

} // rdoRuntime
