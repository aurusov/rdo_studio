/*!
  \copyright (c) RDO-Team, 2012
  \file      generate.inl
  \authors   Урусов Андрей (rdo@rk9.bmstu.ru)
  \authors   Лущан Дмитрий (dluschan@rk9.bmstu.ru)
  \date      12.02.2012
  \brief     Процессный оператор GENERATE
  \indent    4T
*/

// ----------------------------------------------------------------------- INCLUDES
// ----------------------------------------------------------------------- SYNOPSIS
#include "simulator/runtime/rdo.h"
#include "simulator/runtime/rdo_runtime.h"
#include "simulator/runtime/rdo_logic.h"
// --------------------------------------------------------------------------------

OPEN_RDO_RUNTIME_NAMESPACE

// --------------------------------------------------------------------------------
// -------------------- RDOPROCGenerate
// --------------------------------------------------------------------------------
inline RDOPROCGenerate::RDOPROCGenerate(LPIPROCProcess process, const LPRDOCalc& pTime, const LPRDOCalc& pCreateAndGoOnTransactCalc, boost::optional<std::size_t> maxCreateTransactCount)
	: RDOPROCBlock                (process                   )
	, timeNext                    (0.0                       )
	, m_pTimeCalc                 (pTime                     )
	, m_pCreateAndGoOnTransactCalc(pCreateAndGoOnTransactCalc)
	, m_maxCreateTransactCount    (maxCreateTransactCount    )
	, m_createdTransactCount      (0                         )
{}

CLOSE_RDO_RUNTIME_NAMESPACE
