/*
 * @copyright (c) RDO-Team, 2009
 * @file      rdopokaz.h
 * @authors   Александ Барс, Урусов Андрей
 * @date      
 * @brief     
 * @indent    4T
 *********************************************************************************/

#ifndef _RDOPOKAZ_H_
#define _RDOPOKAZ_H_

// **************************************************************************** PCH
// *********************************************************************** INCLUDES
// *********************************************************************** SYNOPSIS
#include "rdo_common/namespace.h"
#include "rdo_lib/rdo_runtime/rdotrace.h"
#include "rdo_lib/rdo_runtime/rdotrace_i.h"
#include "rdo_lib/rdo_runtime/rdopokaz_i.h"
#include "rdo_lib/rdo_runtime/rdo_model_i.h"
#include "rdo_lib/rdo_runtime/rdo_value.h"
#include "rdo_lib/rdo_runtime/rdocalc.h"
// ********************************************************************************

OPEN_RDO_RUNTIME_NAMESPACE

// ********************************************************************************
// ******************** RDOPMDPokaz
// ********************************************************************************
class RDOPMDPokaz: public RDOPokazTrace, public IName
{
QUERY_INTERFACE_BEGIN
	QUERY_INTERFACE_PARENT(RDOPokazTrace)
	QUERY_INTERFACE       (IName        )
QUERY_INTERFACE_END

protected:
	RDOPMDPokaz(CREF(LPRDORuntime) pRuntime, CREF(tstring) name, rbool trace);
	virtual ~RDOPMDPokaz();

	DECLARE_IName;

private:
	tstring m_name;
};

// ********************************************************************************
// ******************** RDOPMDWatchPar
// ********************************************************************************
class RDOPMDWatchPar: public RDOPMDPokaz, public IPokaz, public IModelStructure, public INotify
{
DEFINE_IFACTORY(RDOPMDWatchPar);
QUERY_INTERFACE_BEGIN
	QUERY_INTERFACE_PARENT(RDOPMDPokaz     )
	QUERY_INTERFACE       (IPokaz          )
	QUERY_INTERFACE       (IModelStructure )
	QUERY_INTERFACE       (IPokazTraceValue)
	QUERY_INTERFACE       (INotify         )
QUERY_INTERFACE_END

private:
	RDOPMDWatchPar(CREF(LPRDORuntime) pRuntime, CREF(tstring) name, rbool trace, CREF(tstring) resName, CREF(tstring) parName, int resNumber, int parNumber);
	virtual ~RDOPMDWatchPar();

	int       m_resNumber;
	int       m_parNumber;

	int       m_watchNumber;
	RDOValue  m_currValue;
	double    m_sum;
	double    m_sumSqr;
	RDOValue  m_minValue;
	RDOValue  m_maxValue;

	double    m_timeBegin;
	double    m_timePrev;

	double    m_timeErase;

	DECLARE_INotify;
	DECLARE_IPokaz;
	DECLARE_IPokazTraceValue;
	DECLARE_IModelStructure;
};

// ********************************************************************************
// ******************** RDOPMDWatchState
// ********************************************************************************
class RDOPMDWatchState: public RDOPMDPokaz, public IPokaz, public IModelStructure
{
DEFINE_IFACTORY(RDOPMDWatchState);
QUERY_INTERFACE_BEGIN
	QUERY_INTERFACE_PARENT(RDOPMDPokaz     )
	QUERY_INTERFACE       (IPokaz          )
	QUERY_INTERFACE       (IModelStructure )
	QUERY_INTERFACE       (IPokazTraceValue)
QUERY_INTERFACE_END

private:
	RDOPMDWatchState(CREF(LPRDORuntime) pRuntime, CREF(tstring) name, rbool trace, CREF(LPRDOCalc) pLogic);
	virtual ~RDOPMDWatchState();

	LPRDOCalc m_pLogicCalc;

	int       m_watchNumber;
	rbool     m_currValue;
	double    m_sum;
	double    m_sumSqr;
	double    m_minValue;
	double    m_maxValue;

	double    m_timeBegin;
	double    m_timePrev;

	DECLARE_IPokaz;
	DECLARE_IPokazTraceValue;
	DECLARE_IModelStructure;
};

// ********************************************************************************
// ******************** RDOPMDWatchQuant
// ********************************************************************************
class RDOPMDWatchQuant: public RDOPMDPokaz, public IPokaz, public IPokazWatchQuant, public IModelStructure
{
DEFINE_IFACTORY(RDOPMDWatchQuant);
QUERY_INTERFACE_BEGIN
	QUERY_INTERFACE_PARENT(RDOPMDPokaz     )
	QUERY_INTERFACE       (IPokaz          )
	QUERY_INTERFACE       (IPokazTraceValue)
	QUERY_INTERFACE       (IPokazWatchQuant)
	QUERY_INTERFACE       (IModelStructure )
QUERY_INTERFACE_END

private:
	RDOPMDWatchQuant(CREF(LPRDORuntime) pRuntime, CREF(tstring) name, rbool trace, CREF(tstring) resTypeName, int rtpID);
	virtual ~RDOPMDWatchQuant();

	LPRDOCalc m_pLogicCalc;
	int       m_rtpID;

	int       m_watchNumber;
	int       m_currValue;
	double    m_sum;
	double    m_sumSqr;
	double    m_minValue;
	double    m_maxValue;

	double    m_timeBegin;
	double    m_timePrev;

	DECLARE_IPokaz;
	DECLARE_IPokazTraceValue;
	DECLARE_IPokazWatchQuant;
	DECLARE_IModelStructure;
};

// ********************************************************************************
// ******************** RDOPMDWatchValue
// ********************************************************************************
class RDOPMDWatchValue: public RDOPMDPokaz, public IPokaz, public IPokazWatchValue, public IModelStructure
{
DEFINE_IFACTORY(RDOPMDWatchValue);
QUERY_INTERFACE_BEGIN
	QUERY_INTERFACE_PARENT(RDOPMDPokaz     )
	QUERY_INTERFACE       (IPokaz          )
	QUERY_INTERFACE       (IPokazTraceValue)
	QUERY_INTERFACE       (IPokazWatchValue)
	QUERY_INTERFACE       (IModelStructure )
QUERY_INTERFACE_END

private:
	RDOPMDWatchValue(CREF(LPRDORuntime) pRuntime, CREF(tstring) name, rbool trace, CREF(tstring) resTypeName, int rtpID);
	virtual ~RDOPMDWatchValue();

	LPRDOCalc m_pLogicCalc;
	LPRDOCalc m_pArithmCalc;
	int       m_rtpID;

	int       m_watchNumber;
	RDOValue  m_currValue;
	double    m_sum;
	double    m_sumSqr;
	RDOValue  m_minValue;
	RDOValue  m_maxValue;

	DECLARE_IPokaz;
	DECLARE_IPokazTraceValue;
	DECLARE_IPokazWatchValue;
	DECLARE_IModelStructure;
};

// ********************************************************************************
// ******************** RDOPMDGetValue
// ********************************************************************************
class RDOPMDGetValue: public RDOPMDPokaz, public IPokaz, public IPokazGetValue, public IModelStructure
{
DEFINE_IFACTORY(RDOPMDGetValue);
QUERY_INTERFACE_BEGIN
	QUERY_INTERFACE_PARENT(RDOPMDPokaz     )
	QUERY_INTERFACE       (IPokaz          )
	QUERY_INTERFACE       (IModelStructure )
	QUERY_INTERFACE       (IPokazTraceValue)
	QUERY_INTERFACE       (IPokazGetValue  )
QUERY_INTERFACE_END

private:
	RDOPMDGetValue(CREF(LPRDORuntime) pRuntime, CREF(tstring) name, CREF(LPRDOCalc) pArithm);
	virtual ~RDOPMDGetValue();

	LPRDOCalc m_pArithmCalc;
	RDOValue  m_value;

	DECLARE_IPokaz;
	DECLARE_IPokazTraceValue;
	DECLARE_IPokazGetValue;
	DECLARE_IModelStructure;
};

CLOSE_RDO_RUNTIME_NAMESPACE

#endif // _RDOPOKAZ_H_
