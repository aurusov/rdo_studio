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
{}

void RDOPROCProcess::insertChild(LPIPROCProcess process)
{
	if (process)
	{
		m_child.push_back(process);
		process->setParent(this);
	}
}

void RDOPROCProcess::setParent(LPIPROCProcess process)
{
	m_parent = process;
}

void RDOPROCProcess::next(PTR(RDOPROCTransact) transact)
{
	if ( transact->getBlock() )
	{
		Iterator it = std::find(begin(), end(), transact->getBlock());
		// Если у транзакта есть блок
		if (it != end())
		{
			// Берем этот блок
			LPIPROCBlock block = *it;
			ASSERT(block);
			// Находим перемещаемый транзакт в списке его транзактов
			RDOPROCBlock::TransactIt it_res = block->transactFind(transact);
			// Если транзакт найден
			if (it_res != block->transactEnd()) 
			{
				// Удаляем его из списка транзактов этого блока
				block->transactGoOut(*it_res);
			}
			else
			{
				// Скорее всего здесь не будет ошибки, поскольку advance удаляет транзакты из себя
				//	getRuntime()->error( "Внутренняя ошибка: неучтенный транзакт (транзакт потерял свой блок)" );
			}
			// Переходим к следующему блоку
			it++;
			// Если следующий блок существует
			if (it != end())
			{
				// Берем этот блок
				block = *it;
				ASSERT(block);
				transact->setBlock(block);
				// Записываем в конец списка этого блока перемещаемый транзакт
				block->transactGoIn(transact);
			}
			// Блок в из которого нужно было переместить транзакт был последним
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

RDOPROCTransact::RDOPROCTransact(PTR(RDOSimulator) sim, CREF(LPIPROCBlock) block)
	: RDOResource(static_cast<PTR(RDORuntime)>(sim), -1, typeID, true)
	, m_block    (block                                              )
{
	static_cast<RDORuntime*>(sim)->insertNewResource(this);
	setTrace( true );
	m_temporary = true;
	m_state     = RDOResource::CS_Create;
	m_params.push_back( sim->getCurrentTime() );
}

void RDOPROCTransact::next()
{
	m_block->getProcess()->next(this);
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
RDOPROCBlock::RDOPROCBlock(LPIPROCProcess process)
	: m_process(process)
{}

rbool RDOPROCBlock::init()
{
	if (!m_process)
		return false;

	m_process.query_cast<IBaseOperationContainer>()->append(this);
	return true;
}

RDOPROCBlock::TransactIt RDOPROCBlock::transactFind(PTR(Transact) transact)
{
	return std::find(m_transacts.begin(), m_transacts.end(), transact);
}

RDOPROCBlock::TransactIt RDOPROCBlock::transactEnd()
{
	return m_transacts.end();
}

void RDOPROCBlock::transactGoIn(PTR(Transact) transact)
{
	m_transacts.push_back(transact);
}

void RDOPROCBlock::transactGoOut(PTR(Transact) transact)
{
	m_transacts.remove(transact);
}

LPIPROCProcess RDOPROCBlock::getProcess() const
{
	return m_process;
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

IBaseOperation::BOResult RDOPROCGenerate::onDoOperation( RDOSimulator* sim )
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
	return IBaseOperation::BOR_done;
}

void RDOPROCGenerate::calcNextTimeInterval( RDOSimulator* sim )
{
	sim->addTimePoint( timeNext = timeCalc->calcValue( static_cast<RDORuntime*>(sim) ).getDouble() + sim->getCurrentTime(), m_process, this );
}

void                     RDOPROCGenerate::onStop      (PTR(rdoRuntime::RDOSimulator) sim)                  {}
void                     RDOPROCGenerate::onMakePlaned(PTR(rdoRuntime::RDOSimulator) sim, PTR(void) param) {}
IBaseOperation::BOResult RDOPROCGenerate::onContinue  (PTR(rdoRuntime::RDOSimulator) sim)                  { return IBaseOperation::BOR_cant_run; }

// ----------------------------------------------------------------------------
// ---------- RDOPROCBlockForQueue
// ----------------------------------------------------------------------------
RDOPROCBlockForQueue::RDOPROCBlockForQueue(LPIPROCProcess process, parser_for_Queue From_Par)
	: RDOPROCBlock(process )
	, fromParser  (From_Par)
{}

void RDOPROCBlockForQueue::_onStart( RDOSimulator* sim )
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
void RDOPROCQueue::onStart(PTR(rdoRuntime::RDOSimulator) sim)
{
	_onStart(sim);
}

bool RDOPROCQueue::onCheckCondition( RDOSimulator* sim )
{
	if ( !m_transacts.empty() ) 
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

IBaseOperation::BOResult RDOPROCQueue::onDoOperation( RDOSimulator* sim )
{
	TRACE1(_T("%7.1f QUEUE\n"), sim->getCurrentTime());
	m_transacts.front()->next();
	return IBaseOperation::BOR_done;
}

void                     RDOPROCQueue::onStop      (PTR(rdoRuntime::RDOSimulator) sim)                  {}
void                     RDOPROCQueue::onMakePlaned(PTR(rdoRuntime::RDOSimulator) sim, PTR(void) param) {}
IBaseOperation::BOResult RDOPROCQueue::onContinue  (PTR(rdoRuntime::RDOSimulator) sim)                  { return IBaseOperation::BOR_cant_run; }

// ----------------------------------------------------------------------------
// ---------- RDOPROCDepart
// ----------------------------------------------------------------------------
void RDOPROCDepart::onStart(PTR(rdoRuntime::RDOSimulator) sim)
{
	_onStart(sim);
}

bool RDOPROCDepart::onCheckCondition( RDOSimulator* sim )
{
	if ( !m_transacts.empty() ) 
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

IBaseOperation::BOResult RDOPROCDepart::onDoOperation( RDOSimulator* sim )
{
	TRACE1(_T("%7.1f DEPART\n"), sim->getCurrentTime());
	m_transacts.front()->next();
	return IBaseOperation::BOR_done;
}

void                     RDOPROCDepart::onStop      (PTR(rdoRuntime::RDOSimulator) sim)                  {}
void                     RDOPROCDepart::onMakePlaned(PTR(rdoRuntime::RDOSimulator) sim, PTR(void) param) {}
IBaseOperation::BOResult RDOPROCDepart::onContinue  (PTR(rdoRuntime::RDOSimulator) sim)                  { return IBaseOperation::BOR_cant_run; }

// ----------------------------------------------------------------------------
// ---------- RDOPROCBlockForSeizes
// ----------------------------------------------------------------------------
RDOPROCBlockForSeize::RDOPROCBlockForSeize(LPIPROCProcess process, std::vector < parser_for_Seize > From_Par)
	: RDOPROCBlock(process )
	, fromParser  (From_Par)
{}

void RDOPROCBlockForSeize::_onStart( RDOSimulator* sim )
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
void RDOPROCSeize::onStart(PTR(rdoRuntime::RDOSimulator) sim)
{
	_onStart(sim);
}

bool RDOPROCSeize::onCheckCondition(PTR(RDOSimulator) sim)
{
	if (m_transacts.empty())
		return false;

	ruint Size_Seizes = forRes.size();
	for (ruint i = 0; i < Size_Seizes; i++)
	{
		// если свободен
		if (forRes[i].rss->getParam(forRes[i].Id_param) == forRes[i].enum_free)
		{
			ruint idBlocksTransact    = m_transacts.front()->getTraceID();
			ruint idResourcesTransact = forRes[i].rss->transacts.front()->getTraceID();
			if (idBlocksTransact != idResourcesTransact)
				return false;

			PTR(RDOTrace) tracer = static_cast<PTR(RDORuntime)>(sim)->getTracer();
			forRes[i].rss->setParam(forRes[i].Id_param, forRes[i].enum_buzy);
			TRACE3(_T("%7.1f SEIZES-%d, resId = %d\n"), sim->getCurrentTime(), index, forRes[i].rss->getTraceID());
			if (!tracer->isNull())
				tracer->getOStream() << forRes[i].rss->traceResourceState(_T('\0'), static_cast<RDORuntime*>(sim)) << tracer->getEOL();

			m_transacts.front()->setRes(forRes[i].rss);
			return true;
		}
	}
	return false;
}

IBaseOperation::BOResult RDOPROCSeize::onDoOperation( RDOSimulator* sim )
{
	m_transacts.front()->next();
	return IBaseOperation::BOR_done;
}

void RDOPROCSeize::transactGoIn( RDOPROCTransact* _transact )
{
	int Size_Seizes = forRes.size();
	for(int i=0;i<Size_Seizes; i++)
	{
		forRes[i].rss->transacts.push_back( _transact );
	}
	RDOPROCBlockForSeize::transactGoIn( _transact );
}

void RDOPROCSeize::transactGoOut( RDOPROCTransact* _transact )
{
	int Size_Seizes = forRes.size();
	for(int i=0;i<Size_Seizes; i++)
	{
		forRes[i].rss->transacts.remove( _transact );
	}
	RDOPROCBlockForSeize::transactGoOut( _transact );
}

void                     RDOPROCSeize::onStop      (PTR(rdoRuntime::RDOSimulator) sim)                  {}
void                     RDOPROCSeize::onMakePlaned(PTR(rdoRuntime::RDOSimulator) sim, PTR(void) param) {}
IBaseOperation::BOResult RDOPROCSeize::onContinue  (PTR(rdoRuntime::RDOSimulator) sim)                  { return IBaseOperation::BOR_cant_run; }

// ----------------------------------------------------------------------------
// ---------- RDOPROCReleases
// ----------------------------------------------------------------------------
void RDOPROCRelease::onStart(PTR(rdoRuntime::RDOSimulator) sim)
{
	_onStart(sim);
}

bool RDOPROCRelease::onCheckCondition( RDOSimulator* sim )
{
	if ( !m_transacts.empty() ) 
	{
		int Size_Seizes = forRes.size();
		for(int i=0;i<Size_Seizes; i++)
		{
			if( forRes[i].rss == m_transacts.front()->getRes() )
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
					RDOPROCTransact* transact = m_transacts.front();
					transact->setState( RDOResource::CS_Erase );
					if ( !tracer->isNull() )
					{
						tracer->getOStream() << transact->traceResourceState('\0', static_cast<RDORuntime*>(sim)) << tracer->getEOL();
					}
					m_transacts.remove( transact );
					forRes[i].rss->transacts.remove( transact );
					static_cast<RDORuntime*>(sim)->onEraseRes( transact->getTraceID(), NULL );
					return false;
				}
			}
		}
	}
	return false;
}

IBaseOperation::BOResult RDOPROCRelease::onDoOperation( RDOSimulator* sim )
{
	m_transacts.front()->next();
	return IBaseOperation::BOR_done;
}

void                     RDOPROCRelease::onStop      (PTR(rdoRuntime::RDOSimulator) sim)                  {}
void                     RDOPROCRelease::onMakePlaned(PTR(rdoRuntime::RDOSimulator) sim, PTR(void) param) {}
IBaseOperation::BOResult RDOPROCRelease::onContinue  (PTR(rdoRuntime::RDOSimulator) sim)                  { return IBaseOperation::BOR_cant_run; }

// ----------------------------------------------------------------------------
// ---------- RDOPROCAdvance
// ----------------------------------------------------------------------------
bool RDOPROCAdvance::onCheckCondition( RDOSimulator* sim )
{
	if ( !m_transacts.empty() ) 
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

IBaseOperation::BOResult RDOPROCAdvance::onDoOperation( RDOSimulator* sim )
{
	if ( !m_transacts.empty() )
	{
//		TRACE1(_T("%7.1f ADVANCE BEGIN\n"), sim->getCurrentTime());
		double timeLeave = delayCalc->calcValue( static_cast<RDORuntime*>(sim) ).getDouble() + sim->getCurrentTime();
		leave_list.push_back( LeaveTr(m_transacts.front(), timeLeave) );
		m_transacts.erase( m_transacts.begin() );
		sim->addTimePoint( timeLeave, m_process, this );
		return IBaseOperation::BOR_done;
	} 
	else if ( !leave_list.empty() )
	{
		double tnow = sim->getCurrentTime();
		std::list< LeaveTr >::iterator it = leave_list.begin();
		while ( it != leave_list.end() )
		{
			if ( tnow >= it->timeLeave )
			{
//				TRACE1(_T("%7.1f ADVANCE END\n"), it->timeLeave);
				it->transact->next();
				leave_list.erase( it );
				return IBaseOperation::BOR_planned_and_run;
			}
			it++;
		}
	}
	return IBaseOperation::BOR_cant_run;
}

void                     RDOPROCAdvance::onStart     (PTR(rdoRuntime::RDOSimulator) sim)                  {}
void                     RDOPROCAdvance::onStop      (PTR(rdoRuntime::RDOSimulator) sim)                  {}
void                     RDOPROCAdvance::onMakePlaned(PTR(rdoRuntime::RDOSimulator) sim, PTR(void) param) {}
IBaseOperation::BOResult RDOPROCAdvance::onContinue  (PTR(rdoRuntime::RDOSimulator) sim)                  { return IBaseOperation::BOR_cant_run; }

// ----------------------------------------------------------------------------
// ---------- RDOPROCTerminate
// ----------------------------------------------------------------------------
bool RDOPROCTerminate::onCheckCondition( RDOSimulator* sim )
{
	return !m_transacts.empty() ? true : false;
}

IBaseOperation::BOResult RDOPROCTerminate::onDoOperation( RDOSimulator* sim )
{
//	TRACE1(_T("%7.1f TERMINATE\n"), sim->getCurrentTime());
	RDOPROCTransact* transact = m_transacts.front();
	transact->setState( RDOResource::CS_Erase );
	RDOTrace* tracer = static_cast<RDORuntime*>(sim)->getTracer();
	if ( !tracer->isNull() ) {
		tracer->getOStream() << transact->traceResourceState('\0', static_cast<RDORuntime*>(sim)) << tracer->getEOL();
	}
	static_cast<RDORuntime*>(sim)->onEraseRes( transact->getTraceID(), NULL );
	m_transacts.erase( m_transacts.begin() );
	int termNow = static_cast<RDORuntime*>(sim)->getCurrentTerm();
	termNow += getTerm();
	static_cast<RDORuntime*>(sim)->setCurrentTerm(termNow);
	return IBaseOperation::BOR_done;
}

void                     RDOPROCTerminate::onStart     (PTR(rdoRuntime::RDOSimulator) sim)                  {}
void                     RDOPROCTerminate::onStop      (PTR(rdoRuntime::RDOSimulator) sim)                  {}
void                     RDOPROCTerminate::onMakePlaned(PTR(rdoRuntime::RDOSimulator) sim, PTR(void) param) {}
IBaseOperation::BOResult RDOPROCTerminate::onContinue  (PTR(rdoRuntime::RDOSimulator) sim)                  { return IBaseOperation::BOR_cant_run; }

// ----------------------------------------------------------------------------
// ---------- RDOPROCAssign
// ----------------------------------------------------------------------------
bool RDOPROCAssign::onCheckCondition( RDOSimulator* sim )
{
	return !m_transacts.empty();
}

IBaseOperation::BOResult RDOPROCAssign::onDoOperation( RDOSimulator* sim )
{
	RDOResource* res = static_cast<RDORuntime*>(sim)->getResourceByID( t_resId );
	res->setParam( t_parId, paramValue->calcValue( static_cast<RDORuntime*>(sim) ) );	
	TRACE1(_T("%7.1f ASSIGN\n"), sim->getCurrentTime());
	m_transacts.front()->next();
	return IBaseOperation::BOR_done;
}

void                     RDOPROCAssign::onStart     (PTR(rdoRuntime::RDOSimulator) sim)                  {}
void                     RDOPROCAssign::onStop      (PTR(rdoRuntime::RDOSimulator) sim)                  {}
void                     RDOPROCAssign::onMakePlaned(PTR(rdoRuntime::RDOSimulator) sim, PTR(void) param) {}
IBaseOperation::BOResult RDOPROCAssign::onContinue  (PTR(rdoRuntime::RDOSimulator) sim)                  { return IBaseOperation::BOR_cant_run; }

} // rdoRuntime
