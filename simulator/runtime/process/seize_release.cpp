// ---------------------------------------------------------------------------- PCH
#include "simulator/runtime/pch/stdpch.h"
// ----------------------------------------------------------------------- INCLUDES
// ----------------------------------------------------------------------- SYNOPSIS
#include "simulator/runtime/process/rdoprocess.h"
#include "simulator/runtime/process/seize_release.h"
#include "simulator/runtime/calc/calc_base.h"
#include "simulator/runtime/calc/resource/calc_relevant.h"
#include "simulator/runtime/rdo_enum.h"
// --------------------------------------------------------------------------------

OPEN_RDO_RUNTIME_NAMESPACE

// --------------------------------------------------------------------------------
// -------------------- RDOPROCBlockForSeize
// --------------------------------------------------------------------------------
RDOPROCBlockForSeize::RDOPROCBlockForSeize(LPIPROCProcess process, std::vector <parser_for_Seize> From_Par)
	: RDOPROCBlock(process )
	, fromParser  (From_Par)
{}

void RDOPROCBlockForSeize::_onStart(const LPRDORuntime& pRuntime)
{
	/// @todo: если потребуется стоить деревья, вершинами которых будут полные снимки БД,
	// как при DPT search, то инициализацию атрибутов надо будет делать в checkOperation
	std::vector<parser_for_Seize>::iterator it1 = fromParser.begin();
	while (it1 != fromParser.end())
	{
		int Id_res = (*it1).Id_res;
		int Id_param = (*it1).Id_param;
		LPRDOResource res = pRuntime->getResourceByID(Id_res);
		runtime_for_Seize bbb;
		bbb.Id_param = Id_param;
		bbb.rss = res.object_static_cast<RDOPROCResource>();
		bbb.enum_free = RDOValue(bbb.rss->getParam(Id_param).getEnum(), RDOPROCBlockForSeize::getStateEnumFree());
		bbb.enum_buzy = RDOValue(bbb.rss->getParam(Id_param).getEnum(), RDOPROCBlockForSeize::getStateEnumBuzy());
		forRes.push_back(bbb);
		++it1;
	}
}

std::string RDOPROCBlockForSeize::getStateParamName()
{
	return "Состояние";
}

std::string RDOPROCBlockForSeize::getStateEnumFree()
{
	return "Свободен";
}

std::string RDOPROCBlockForSeize::getStateEnumBuzy()
{
	return "Занят";
}

// --------------------------------------------------------------------------------
// -------------------- RDOPROCSeize
// --------------------------------------------------------------------------------
RDOPROCSeize::RDOPROCSeize(LPIPROCProcess process, std::vector<parser_for_Seize> From_Par)
	: RDOPROCBlockForSeize(process, From_Par)
{
	static std::size_t g_index = 1;
	index = g_index++;
}

void RDOPROCSeize::onStart(const LPRDORuntime& pRuntime)
{
	_onStart(pRuntime);
}

bool RDOPROCSeize::onCheckCondition(const LPRDORuntime& pRuntime)
{
	if (m_transacts.empty())
		return false;

	const std::size_t Size_Seizes = forRes.size();
	for (std::size_t i = 0; i < Size_Seizes; i++)
	{
		// если свободен
		if (forRes[i].rss->getParam(forRes[i].Id_param) == forRes[i].enum_free)
		{
			const std::size_t idBlocksTransact = m_transacts.front()->getTraceID();
			const std::size_t idResourcesTransact = forRes[i].rss->transacts.front()->getTraceID();
			if (idBlocksTransact != idResourcesTransact)
				return false;

			RDOTrace* tracer = pRuntime->getTracer();
			forRes[i].rss->setParam(forRes[i].Id_param, forRes[i].enum_buzy);
			TRACE3("%7.1f SEIZES-%d, resId = %d\n", pRuntime->getCurrentTime(), index, forRes[i].rss->getTraceID());
			if (!tracer->isNull())
				tracer->getOStream() << forRes[i].rss->traceResourceState('\0', pRuntime) << tracer->getEOL();

			m_transacts.front()->setRes(forRes[i].rss);
			return true;
		}
	}
	return false;
}

IBaseOperation::BOResult RDOPROCSeize::onDoOperation(const LPRDORuntime& /*pRuntime*/)
{
	m_transacts.front()->next();
	return IBaseOperation::BOR_done;
}

void RDOPROCSeize::transactGoIn(const LPTransact& pTransact)
{
	int sizeSeizes = forRes.size();
	for(int i = 0; i < sizeSeizes; ++i)
	{
		forRes[i].rss->transacts.push_back(pTransact);
	}
	RDOPROCBlockForSeize::transactGoIn(pTransact);
}

void RDOPROCSeize::transactGoOut(const LPTransact& pTransact)
{
	int Size_Seizes = forRes.size();
	for(int i = 0; i < Size_Seizes; ++i)
	{
		forRes[i].rss->transacts.remove(pTransact);
	}
	RDOPROCBlockForSeize::transactGoOut(pTransact);
}

void RDOPROCSeize::onStop(const LPRDORuntime& /*pRuntime*/)
{}

IBaseOperation::BOResult RDOPROCSeize::onContinue(const LPRDORuntime& /*pRuntime*/)
{
	return IBaseOperation::BOR_cant_run;
}

// --------------------------------------------------------------------------------
// -------------------- RDOPROCRelease
// --------------------------------------------------------------------------------
RDOPROCRelease::RDOPROCRelease(LPIPROCProcess process, std::vector<parser_for_Seize> From_Par)
	: RDOPROCBlockForSeize(process, From_Par)
{
	static std::size_t g_index = 1;
	index = g_index++;
}

void RDOPROCRelease::onStart(const LPRDORuntime& pRuntime)
{
	_onStart(pRuntime);
}

bool RDOPROCRelease::onCheckCondition(const LPRDORuntime& pRuntime)
{
	if (!m_transacts.empty())
	{
		int Size_Seizes = forRes.size();
		for(int i=0;i<Size_Seizes; i++)
		{
			if(forRes[i].rss == m_transacts.front()->getRes())
			{
				// Занят
				if (forRes[i].rss->getParam(forRes[i].Id_param) == forRes[i].enum_buzy)
				{
					RDOTrace* tracer = pRuntime->getTracer();
					TRACE3("%7.1f RELEASES-%d, resId = %d\n", pRuntime->getCurrentTime(), index, forRes[i].rss->getTraceID());
					forRes[i].rss->setParam(forRes[i].Id_param, forRes[i].enum_free);
					if (!tracer->isNull())
					{
						tracer->getOStream() << forRes[i].rss->traceResourceState('\0', pRuntime) << tracer->getEOL();
					}				
					return true;
				}		
				// Не Занят и не свободен
				if (forRes[i].rss->getParam(forRes[i].Id_param) != forRes[i].enum_free)
				{
					//Удаляем транзакт
					RDOTrace* tracer = pRuntime->getTracer();
					TRACE3("%7.1f RELEASES_Bad-%d, resId = %d\n", pRuntime->getCurrentTime(), index, forRes[i].rss->getTraceID());
					LPRDOPROCTransact transact = m_transacts.front();
					ASSERT(transact);
					transact->setState(RDOResource::CS_Erase);
					if (!tracer->isNull())
					{
						tracer->getOStream() << transact->traceResourceState('\0', pRuntime) << tracer->getEOL();
					}
					m_transacts.remove(transact);
					forRes[i].rss->transacts.remove(transact);
					pRuntime->onEraseRes(transact->getTraceID(), NULL);
					return false;
				}
			}
		}
	}
	return false;
}

IBaseOperation::BOResult RDOPROCRelease::onDoOperation(const LPRDORuntime& /*pRuntime*/)
{
	m_transacts.front()->next();
	return IBaseOperation::BOR_done;
}

void RDOPROCRelease::onStop(const LPRDORuntime& /*pRuntime*/)
{}

IBaseOperation::BOResult RDOPROCRelease::onContinue(const LPRDORuntime& /*pRuntime*/)
{
	return IBaseOperation::BOR_cant_run;
}

CLOSE_RDO_RUNTIME_NAMESPACE
