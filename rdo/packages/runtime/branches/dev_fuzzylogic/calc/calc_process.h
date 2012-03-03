/*!
  \copyright (c) RDO-Team, 2011
  \file      calc_process.h
  \author    Лущан Дмитрий (dluschan@rk9.bmstu.ru)
  \date      10.03.2011
  \brief     RDOCalc для передачи управления в процесс из БЗ и событий
  \indent    4T
*/

#ifndef _LIB_RUNTIME_CALC_PROCESS_H_
#define _LIB_RUNTIME_CALC_PROCESS_H_

// ----------------------------------------------------------------------- INCLUDES
// ----------------------------------------------------------------------- SYNOPSIS
#include "simulator/runtime/calc/calc_base.h"
#include "simulator/runtime/rdo.h"
#include "simulator/runtime/rdo_res_type_i.h"
#include "simulator/runtime/rdo_runtime.h"
#include "simulator/runtime/process/rdoprocess.h"
#include "simulator/runtime/rdo_logic.h"
// --------------------------------------------------------------------------------

OPEN_RDO_RUNTIME_NAMESPACE

//! Передача транзакта в процесс и его запуск
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

//! Создание транзакта в блоке GENERATE и его запуск в процесс
CALC(RDOCalcCreateAndGoInTransact)
{
	DECLARE_FACTORY(RDOCalcCreateAndGoInTransact)
public:
	void setBlock(LPIPROCBlock pBlock);
protected:
	RDOCalcCreateAndGoInTransact(CREF(LPIResourceType) pType, CREF(std::vector<RDOValue>) rParamsCalcs, rbool traceFlag, rbool permanentFlag);
	DECLARE_ICalc;
private:
	LPIPROCBlock           m_pBlock;
	LPIResourceType        m_pResType;
	std::vector<RDOValue>  m_paramsCalcs;
	rbool                  m_traceFlag;
	rbool                  m_permanentFlag;
	ruint                  m_relResID;
};

//! Получение значения терминального счетчика
CALC(RDOCalcGetTermNow)
{
DECLARE_FACTORY(RDOCalcGetTermNow)
DECLARE_ICalc;
};

CLOSE_RDO_RUNTIME_NAMESPACE

#endif // _LIB_RUNTIME_CALC_PROCESS_H_
