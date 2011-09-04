/*!
  \copyright (c) RDO-Team, 2011
  \file      process_control.h
  \author    Лущан Дмитрий (dluschan@rk9.bmstu.ru)
  \date      10.03.2011
  \brief     RDOCalc для передачи управления в процесс из БЗ и событий
  \indent    4T
*/

#ifndef _LIB_RUNTIME_CALC_PROCESS_CONTROL_H_
#define _LIB_RUNTIME_CALC_PROCESS_CONTROL_H_

// ----------------------------------------------------------------------- INCLUDES
// ----------------------------------------------------------------------- SYNOPSIS
#include "rdo_common/namespace.h"
#include "rdo_lib/rdo_runtime/rdocalc.h"
#include "rdo_lib/rdo_runtime/rdo.h"
#include "rdo_lib/rdo_runtime/rdo_runtime.h"
#include "rdo_lib/rdo_runtime/rdoprocess.h"
#include "rdo_lib/rdo_runtime/rdo_logic.h"
// --------------------------------------------------------------------------------

OPEN_RDO_RUNTIME_NAMESPACE

/*!
  \class   RDOCalcProcessControl
  \brief   Передача транзакта в процесс и его запуск
*/
CALC(RDOCalcProcessControl)
{
DECLARE_FACTORY(RDOCalcProcessControl)
public:
protected:
	RDOCalcProcessControl(LPIPROCBlock pBlock, rsint relResNum);
	DECLARE_ICalc;
private:
	LPIPROCBlock  m_Block;
	rsint         m_relResNum;
};

CLOSE_RDO_RUNTIME_NAMESPACE

#endif // _LIB_RUNTIME_CALC_PROCESS_CONTROL_H_
