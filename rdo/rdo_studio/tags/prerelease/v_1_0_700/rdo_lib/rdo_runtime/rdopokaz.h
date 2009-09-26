/*
 * copyright: (c) RDO-Team, 2009
 * filename : rdopokaz.cpp
 * author   : Александ Барс, Урусов Андрей
 * date     : 
 * bref     : 
 * indent   : 4T
 */

#ifndef _RDOPOKAZ_H_
#define _RDOPOKAZ_H_

// =========================================================================== PCH
// ====================================================================== INCLUDES
// ====================================================================== SYNOPSIS
#include <namespace.h>
#include "rdotrace.h"
#include "rdotrace_interface.h"
#include "rdopokaz_interface.h"
#include "rdo_model_interface.h"
// ===============================================================================

OPEN_RDO_RUNTIME_NAMESPACE

// ----------------------------------------------------------------------------
// ---------- RDOPMDPokaz
// ----------------------------------------------------------------------------
class RDOPMDPokaz: public RDOPokazTrace, public IName
{
RDO_IOBJECT(RDOPMDPokaz, RDOPokazTrace);
QUERY_INTERFACE_BEGIN
	QUERY_INTERFACE(IName)
QUERY_INTERFACE_END

public:
	RDOPMDPokaz( RDORuntime* sim, const std::string& name, bool trace );
	virtual ~RDOPMDPokaz() {}

protected:
	DECLARE_IName;

private:
	std::string m_name;
};

// ----------------------------------------------------------------------------
// ---------- RDOPMDWatchPar
// ----------------------------------------------------------------------------
class RDOPMDWatchPar: public RDOPMDPokaz, public IPokaz, public IModelStructure, public INotify
{
RDO_IOBJECT(RDOPMDWatchPar, RDOPMDPokaz);
QUERY_INTERFACE_BEGIN
	QUERY_INTERFACE(IPokaz)
	QUERY_INTERFACE(IModelStructure)
	QUERY_INTERFACE(IPokazTraceValue)
	QUERY_INTERFACE(INotify)
QUERY_INTERFACE_END

public:
	RDOPMDWatchPar( RDORuntime* sim, const std::string& name, bool trace, const std::string& resName, const std::string& parName, int resNumber, int parNumber );

private:
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

// ----------------------------------------------------------------------------
// ---------- RDOPMDWatchState
// ----------------------------------------------------------------------------
class RDOPMDWatchState: public RDOPMDPokaz, public IPokaz, public IModelStructure
{
RDO_IOBJECT(RDOPMDWatchState, RDOPMDPokaz);
QUERY_INTERFACE_BEGIN
	QUERY_INTERFACE(IPokaz)
	QUERY_INTERFACE(IModelStructure)
	QUERY_INTERFACE(IPokazTraceValue)
QUERY_INTERFACE_END

public:
	RDOPMDWatchState( RDORuntime* sim, const std::string& name, bool trace, RDOCalc* logic );

private:
	RDOCalc* m_logicCalc;

	int      m_watchNumber;
	bool     m_currValue;
	double   m_sum;
	double   m_sumSqr;
	double   m_minValue;
	double   m_maxValue;

	double   m_timeBegin;
	double   m_timePrev;

	DECLARE_IPokaz;
	DECLARE_IPokazTraceValue;
	DECLARE_IModelStructure;
};

// ----------------------------------------------------------------------------
// ---------- RDOPMDWatchQuant
// ----------------------------------------------------------------------------
class RDOPMDWatchQuant: public RDOPMDPokaz, public IPokaz, public IPokazWatchQuant, public IModelStructure
{
RDO_IOBJECT(RDOPMDWatchQuant, RDOPMDPokaz);
QUERY_INTERFACE_BEGIN
	QUERY_INTERFACE(IPokaz)
	QUERY_INTERFACE(IPokazTraceValue)
	QUERY_INTERFACE(IPokazWatchQuant)
	QUERY_INTERFACE(IModelStructure)
QUERY_INTERFACE_END

public:
	RDOPMDWatchQuant( RDORuntime* sim, const std::string& name, bool trace, const std::string& resTypeName, int rtp_id );

private:
	RDOCalc* m_logicCalc;
	int      m_rtp_id;

	int      m_watchNumber;
	int      m_currValue;
	double   m_sum;
	double   m_sumSqr;
	double   m_minValue;
	double   m_maxValue;

	double   m_timeBegin;
	double   m_timePrev;

	DECLARE_IPokaz;
	DECLARE_IPokazTraceValue;
	DECLARE_IPokazWatchQuant;
	DECLARE_IModelStructure;
};

// ----------------------------------------------------------------------------
// ---------- RDOPMDWatchValue
// ----------------------------------------------------------------------------
class RDOPMDWatchValue: public RDOPMDPokaz, public IPokaz, public IPokazWatchValue, public IModelStructure
{
RDO_IOBJECT(RDOPMDWatchValue, RDOPMDPokaz);
QUERY_INTERFACE_BEGIN
	QUERY_INTERFACE(IPokaz)
	QUERY_INTERFACE(IPokazTraceValue)
	QUERY_INTERFACE(IPokazWatchValue)
	QUERY_INTERFACE(IModelStructure)
QUERY_INTERFACE_END

public:
	RDOPMDWatchValue( RDORuntime* sim, const std::string& name, bool trace, const std::string& resTypeName, int rtp_id );

private:
	PTR(RDOCalc)  m_logicCalc;
	PTR(RDOCalc)  m_arithmCalc;
	int           m_rtp_id;

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

// ----------------------------------------------------------------------------
// ---------- RDOPMDGetValue
// ----------------------------------------------------------------------------
class RDOPMDGetValue: public RDOPMDPokaz, public IPokaz, public IModelStructure
{
RDO_IOBJECT(RDOPMDGetValue, RDOPMDPokaz);
QUERY_INTERFACE_BEGIN
	QUERY_INTERFACE(IPokaz)
	QUERY_INTERFACE(IModelStructure)
	QUERY_INTERFACE(IPokazTraceValue)
QUERY_INTERFACE_END

public:
	RDOPMDGetValue( RDORuntime* sim, const std::string& name, RDOCalc* arithm );

private:
	PTR(RDOCalc) m_arithmCalc;
	DECLARE_IPokaz;
	DECLARE_IPokazTraceValue;
	DECLARE_IModelStructure;
};

CLOSE_RDO_RUNTIME_NAMESPACE

#endif //! _RDOPOKAZ_H_
