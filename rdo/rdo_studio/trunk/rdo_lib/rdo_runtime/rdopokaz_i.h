/*
 * copyright: (c) RDO-Team, 2009
 * filename : rdopokaz_i.h
 * author   : Урусов Андрей
 * date     : 28.07.2009
 * bref     : 
 * indent   : 4T
 */

#ifndef _RDOPOKAZ_I_H_
#define _RDOPOKAZ_I_H_

// ====================================================================== INCLUDES
// ====================================================================== SYNOPSIS
#include "rdo_common/rdointerface.h"
#include "rdo_lib/rdo_runtime/rdo_simulator.h"
#include "rdo_lib/rdo_runtime/rdocalc.h"
// ===============================================================================

// ----------------------------------------------------------------------------
// ---------- RDOPokaz - базовый класс для собираемых показателей
// ----------------------------------------------------------------------------
class IPokaz
{
public:
	virtual rbool resetPokaz(PTR(rdoRuntime::RDOSimulator) sim) = 0;
	virtual rbool checkPokaz(PTR(rdoRuntime::RDOSimulator) sim) = 0;
	virtual rbool calcStat  (PTR(rdoRuntime::RDOSimulator) sim) = 0;
};
#define DECLARE_IPokaz                                           \
	virtual rbool resetPokaz(PTR(rdoRuntime::RDOSimulator) sim); \
	virtual rbool checkPokaz(PTR(rdoRuntime::RDOSimulator) sim); \
	virtual rbool calcStat  (PTR(rdoRuntime::RDOSimulator) sim);

class IPokazWatchQuant
{
public:
	virtual void setLogicCalc(CREF(rdoRuntime::LPRDOCalc) pLogicCalc) = 0;
};
#define DECLARE_IPokazWatchQuant \
	virtual void setLogicCalc(CREF(rdoRuntime::LPRDOCalc) pLogicCalc);

class IPokazWatchValue
{
public:
	virtual void checkResourceErased(PTR(rdoRuntime::RDOResource) res       ) = 0;
	virtual void setLogicCalc       (CREF(rdoRuntime::LPRDOCalc) pLogicCalc ) = 0;
	virtual void setArithmCalc      (CREF(rdoRuntime::LPRDOCalc) pArithmCalc) = 0;
};
#define DECLARE_IPokazWatchValue                                               \
	virtual void checkResourceErased(PTR(rdoRuntime::RDOResource) res       ); \
	virtual void setLogicCalc       (CREF(rdoRuntime::LPRDOCalc) pLogicCalc ); \
	virtual void setArithmCalc      (CREF(rdoRuntime::LPRDOCalc) pArithmCalc);

#endif //! _RDOPOKAZ_I_H_
