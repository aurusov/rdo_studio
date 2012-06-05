/*
 * copyright: (c) RDO-Team, 2009
 * filename : rdopokaz_interface.h
 * author   : Урусов Андрей
 * date     : 28.07.2009
 * bref     : 
 * indent   : 4T
 */

#ifndef _RDOPOKAZ_INTERFACE_H_
#define _RDOPOKAZ_INTERFACE_H_

// ====================================================================== INCLUDES
// ====================================================================== SYNOPSIS
#include <rdointerface.h>
#include "rdo_simulator.h"
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
	virtual void setLogicCalc(PTR(rdoRuntime::RDOCalc) logicCalc) = 0;
};
#define DECLARE_IPokazWatchQuant \
	virtual void setLogicCalc(PTR(rdoRuntime::RDOCalc) logicCalc);

class IPokazWatchValue
{
public:
	virtual void checkResourceErased(PTR(rdoRuntime::RDOResource) res   ) = 0;
	virtual void setLogicCalc       (PTR(rdoRuntime::RDOCalc) logicCalc ) = 0;
	virtual void setArithmCalc      (PTR(rdoRuntime::RDOCalc) arithmCalc) = 0;
};
#define DECLARE_IPokazWatchValue                                           \
	virtual void checkResourceErased(PTR(rdoRuntime::RDOResource) res   ); \
	virtual void setLogicCalc       (PTR(rdoRuntime::RDOCalc) logicCalc ); \
	virtual void setArithmCalc      (PTR(rdoRuntime::RDOCalc) arithmCalc);

#endif //! _RDOPOKAZ_INTERFACE_H_
