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
// ---------- IPokaz
// ----------------------------------------------------------------------------
class IPokaz
{
public:
	virtual void resetPokaz(CREF(rdoRuntime::LPRDORuntime) Runtime) = 0;
	virtual void checkPokaz(CREF(rdoRuntime::LPRDORuntime) Runtime) = 0;
	virtual void calcStat  (CREF(rdoRuntime::LPRDORuntime) Runtime, REF(std::ostream) stream) = 0;
};
#define DECLARE_IPokaz                                               \
	virtual void resetPokaz(CREF(rdoRuntime::LPRDORuntime) Runtime); \
	virtual void checkPokaz(CREF(rdoRuntime::LPRDORuntime) Runtime); \
	virtual void calcStat  (CREF(rdoRuntime::LPRDORuntime) Runtime, REF(std::ostream) stream);

// ----------------------------------------------------------------------------
// ---------- IPokazWatchQuant
// ----------------------------------------------------------------------------
class IPokazWatchQuant
{
public:
	virtual void setLogicCalc(CREF(rdoRuntime::LPRDOCalc) pLogicCalc) = 0;
};
#define DECLARE_IPokazWatchQuant \
	virtual void setLogicCalc(CREF(rdoRuntime::LPRDOCalc) pLogicCalc);

// ----------------------------------------------------------------------------
// ---------- IPokazWatchValue
// ----------------------------------------------------------------------------
class IPokazWatchValue
{
public:
	virtual void checkResourceErased(CREF(rdoRuntime::LPRDOResource) pResource  ) = 0;
	virtual void setLogicCalc       (CREF(rdoRuntime::LPRDOCalc)     pLogicCalc ) = 0;
	virtual void setArithmCalc      (CREF(rdoRuntime::LPRDOCalc)     pArithmCalc) = 0;
};
#define DECLARE_IPokazWatchValue                                                   \
	virtual void checkResourceErased(CREF(rdoRuntime::LPRDOResource) pResource  ); \
	virtual void setLogicCalc       (CREF(rdoRuntime::LPRDOCalc)     pLogicCalc ); \
	virtual void setArithmCalc      (CREF(rdoRuntime::LPRDOCalc)     pArithmCalc);

// ----------------------------------------------------------------------------
// ---------- IPokazGetValue
// ----------------------------------------------------------------------------
class IPokazGetValue
{
public:
	virtual CREF(rdoRuntime::RDOValue) getValue() const = 0;
};
#define DECLARE_IPokazGetValue \
	CREF(rdoRuntime::RDOValue) getValue() const;

#endif //! _RDOPOKAZ_I_H_
