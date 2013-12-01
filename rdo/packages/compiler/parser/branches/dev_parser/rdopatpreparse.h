/*!
  \copyright (c) RDO-Team, 2011
  \file      rdopatpreparse.h
  \author    Лущан Дмитрий (dluschan@rk9.bmstu.ru)
  \date      22.05.10
  \brief     
  \indent    4T
*/

#ifndef _RDOPATPREPARSE_H_
#define _RDOPATPREPARSE_H_

// ----------------------------------------------------------------------- INCLUDES
// ----------------------------------------------------------------------- SYNOPSIS
#include "utils/src/common/rdomacros.h"
#include "utils/src/common/rdotypes.h"
#include "utils/src/smart_ptr/factory/factory.h"
#include "simulator/runtime/calc/calc_event.h"
#include "simulator/runtime/rdo_event.h"
#include "simulator/compiler/parser/rdofun.h"
// --------------------------------------------------------------------------------

OPEN_RDO_PARSER_NAMESPACE

// --------------------------------------------------------------------------------
// -------------------- RDOEvent
// --------------------------------------------------------------------------------
OBJECT(RDOEvent)
{
DECLARE_FACTORY(RDOEvent);
public:
	typedef std::list<rdo::runtime::LPRDOCalcEvent> CalcList;

	CREF(tstring)            name           () const;
	void                     attachCalc     (CREF(rdo::runtime::LPRDOCalcEventStop) pCalc);
	void                     attachCalc     (CREF(rdo::runtime::LPRDOCalcEventPlan) pCalc);
	REF(CalcList)            getCalcList    ();
	LPIBaseOperation         getRuntimeEvent() const;
	void                     setRuntimeEvent(LPIBaseOperation pRuntimeEvent);
	rdo::runtime::LPRDOCalc  getInitCalc    () const;
	void                     setInitCalc    (CREF(rdo::runtime::LPRDOCalc) pCalc);
	void                     setParamList   (CREF(LPArithmContainer) pParamList);
	CREF(LPArithmContainer)  getParamList   () const;

private:
	RDOEvent(CREF(tstring) name);
	virtual ~RDOEvent();

	tstring                  m_name;
	CalcList                 m_calcList;
	LPIBaseOperation         m_runtimeEvent;
	rdo::runtime::LPRDOCalc  m_pInitCalc;
	LPArithmContainer        m_pParamList;
};

CLOSE_RDO_PARSER_NAMESPACE

#include "simulator/compiler/parser/rdopatpreparse.inl"

#endif // _RDOPATPREPARSE_H_
