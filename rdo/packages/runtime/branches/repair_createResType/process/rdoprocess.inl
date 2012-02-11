/*!
  \copyright (c) RDO-Team, 2011
  \file      rdoprocess.inl
  \author    Лущан Дмитрий (dluschan@rk9.bmstu.ru)
  \date      24.07.2011
  \brief     Процессы
  \indent    4T
*/

// ----------------------------------------------------------------------- INCLUDES
// ----------------------------------------------------------------------- SYNOPSIS
#include "simulator/runtime/rdo.h"
#include "simulator/runtime/rdo_runtime.h"
#include "simulator/runtime/rdoprocess_i.h"
#include "simulator/runtime/rdo_logic.h"
// --------------------------------------------------------------------------------

OPEN_RDO_RUNTIME_NAMESPACE

// --------------------------------------------------------------------------------
// -------------------- RDOPROCBlock
// --------------------------------------------------------------------------------
inline RDOPROCBlock::~RDOPROCBlock()
{}

// --------------------------------------------------------------------------------
// -------------------- RDOPROCTransact
// --------------------------------------------------------------------------------
inline LPRDOPROCResource RDOPROCTransact::getRes()
{
	return m_res;
}

inline void RDOPROCTransact::setRes(CREF(LPRDOPROCResource) pResource)
{
	m_res = pResource;
}

inline REF(LPIPROCBlock) RDOPROCTransact::getBlock()
{
	return m_block;
}

inline void RDOPROCTransact::setBlock(CREF(LPIPROCBlock) block)
{
	m_block = block;
}

// --------------------------------------------------------------------------------
// -------------------- RDOPROCResource
// --------------------------------------------------------------------------------
inline tstring RDOPROCResource::whoAreYou()
{
	return "procRes";
}

// --------------------------------------------------------------------------------
// -------------------- RDOPROCGenerate
// --------------------------------------------------------------------------------
inline RDOPROCGenerate::RDOPROCGenerate(LPIPROCProcess process, CREF(LPRDOCalc) pTime, CREF(LPRDOCalc) pCreateAndGoOnTransactCalc, int maxTransCount)
	: RDOPROCBlock   (process      )
	, timeNext       (0.0          )
	, m_pTimeCalc    (pTime        )
	, m_pCreateAndGoOnTransactCalc(pCreateAndGoOnTransactCalc)
	, m_maxTransCount(maxTransCount)
{
	m_TransCount = 0;
}

// --------------------------------------------------------------------------------
// -------------------- RDOPROCQueue
// --------------------------------------------------------------------------------
inline RDOPROCQueue::RDOPROCQueue(LPIPROCProcess process, parser_for_Queue From_Par)
	: RDOPROCBlockForQueue(process, From_Par)
{}

inline ruint RDOPROCQueue::getDefaultValue()
{
	return 0;
}

inline tstring RDOPROCQueue::getQueueParamName()
{
	return "длина_очереди";
}

// --------------------------------------------------------------------------------
// -------------------- RDOPROCDepart
// --------------------------------------------------------------------------------
inline RDOPROCDepart::RDOPROCDepart(LPIPROCProcess process, parser_for_Queue From_Par)
	: RDOPROCBlockForQueue(process, From_Par)
{}

inline ruint RDOPROCDepart::getDefaultValue()
{
	return 0;
}

inline tstring RDOPROCDepart::getDepartParamName()
{
	return "длина_очереди";
}

// --------------------------------------------------------------------------------
// -------------------- RDOPROCBlockForSeize
// --------------------------------------------------------------------------------
inline tstring RDOPROCBlockForSeize::getStateParamName()
{
	return "Состояние";
}

inline tstring RDOPROCBlockForSeize::getStateEnumFree()
{
	return "Свободен";
}

inline tstring RDOPROCBlockForSeize::getStateEnumBuzy()
{
	return "Занят";
}

// --------------------------------------------------------------------------------
// -------------------- RDOPROCSeize
// --------------------------------------------------------------------------------
inline RDOPROCSeize::RDOPROCSeize(LPIPROCProcess process, std::vector<parser_for_Seize> From_Par)
	: RDOPROCBlockForSeize(process, From_Par)
{
	static ruint g_index = 1;
	index = g_index++;
}

// --------------------------------------------------------------------------------
// -------------------- RDOPROCRelease
// --------------------------------------------------------------------------------
inline RDOPROCRelease::RDOPROCRelease(LPIPROCProcess process, std::vector<parser_for_Seize> From_Par)
	: RDOPROCBlockForSeize(process, From_Par)
{
	static ruint g_index = 1;
	index = g_index++;
}

// --------------------------------------------------------------------------------
// -------------------- RDOPROCAdvance
// --------------------------------------------------------------------------------
inline RDOPROCAdvance::RDOPROCAdvance(LPIPROCProcess process, CREF(LPRDOCalc) _pDelayCalc)
	: RDOPROCBlock(process    )
	, pDelayCalc  (_pDelayCalc)
{}

inline RDOPROCAdvance::LeaveTr::LeaveTr(CREF(LPRDOPROCTransact) _transact, double _timeLeave)
	: transact (_transact )
	, timeLeave(_timeLeave)
{}


// --------------------------------------------------------------------------------
// -------------------- RDOPROCTerminate
// --------------------------------------------------------------------------------
inline RDOPROCTerminate::RDOPROCTerminate(LPIPROCProcess process, ruint _term)
	: RDOPROCBlock(process)
	, term        (_term  )
{}

inline int RDOPROCTerminate::getTerm()
{
	return term;
}

// --------------------------------------------------------------------------------
// -------------------- RDOPROCAssign
// --------------------------------------------------------------------------------
inline RDOPROCAssign::RDOPROCAssign(LPIPROCProcess process, CREF(LPRDOCalc) pValue, int Id_res, int Id_param)
	: RDOPROCBlock(process )
	, pParamValue (pValue  )
	, t_resId     (Id_res  )
	, t_parId     (Id_param)
{}

CLOSE_RDO_RUNTIME_NAMESPACE
