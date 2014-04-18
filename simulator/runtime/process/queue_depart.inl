/*!
  \copyright (c) RDO-Team, 2012
  \file      queue_depart.inl
  \authors   Урусов Андрей (rdo@rk9.bmstu.ru)
  \authors   Лущан Дмитрий (dluschan@rk9.bmstu.ru)
  \date      12.02.2012
  \brief     Процессные операторы QUEUE и DEPART
  \indent    4T
*/

// ----------------------------------------------------------------------- INCLUDES
// ----------------------------------------------------------------------- SYNOPSIS
#include "simulator/runtime/rdo.h"
#include "simulator/runtime/rdo_runtime.h"
#include "simulator/runtime/process/rdoprocess_i.h"
#include "simulator/runtime/rdo_logic.h"
// --------------------------------------------------------------------------------

OPEN_RDO_RUNTIME_NAMESPACE

// --------------------------------------------------------------------------------
// -------------------- RDOPROCQueue
// --------------------------------------------------------------------------------
inline RDOPROCQueue::RDOPROCQueue(LPIPROCProcess process, parser_for_Queue From_Par)
	: RDOPROCBlockForQueue(process, From_Par)
{}

inline std::size_t RDOPROCQueue::getDefaultValue()
{
	return 0;
}

inline std::string RDOPROCQueue::getQueueParamName()
{
	return "длина_очереди";
}

// --------------------------------------------------------------------------------
// -------------------- RDOPROCDepart
// --------------------------------------------------------------------------------
inline RDOPROCDepart::RDOPROCDepart(LPIPROCProcess process, parser_for_Queue From_Par)
	: RDOPROCBlockForQueue(process, From_Par)
{}

inline std::size_t RDOPROCDepart::getDefaultValue()
{
	return 0;
}

inline std::string RDOPROCDepart::getDepartParamName()
{
	return "длина_очереди";
}

CLOSE_RDO_RUNTIME_NAMESPACE
