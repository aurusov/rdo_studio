/*
 * copyright: (c) RDO-Team, 2009
 * filename : rdo_action_interface.h
 * author   : Ћущан ƒмитрий
 * date     : 27.03.2010
 * bref     : 
 * indent   : 4T
 */

#ifndef _RDO_ACTION_INTERFACE_H_
#define _RDO_ACTION_INTERFACE_H_

// ====================================================================== INCLUDES
// ====================================================================== SYNOPSIS
#include "rdo_common/rdointerface.h"
// ===============================================================================

class IAction
{
public:
	virtual void   onBeforeChoiceFrom (PTR(rdoRuntime::RDOSimulator) sim) = 0;
	virtual bool   choiceFrom         (PTR(rdoRuntime::RDOSimulator) sim) = 0;
	virtual void   onBeforeActionBegin(PTR(rdoRuntime::RDOSimulator) sim) = 0;
	virtual void   convertBegin       (PTR(rdoRuntime::RDOSimulator) sim) = 0;
	virtual void   onAfterActionBegin (PTR(rdoRuntime::RDOSimulator) sim) = 0;
	virtual void   onBeforeActionEnd  (PTR(rdoRuntime::RDOSimulator) sim) = 0;
	virtual void   convertEnd         (PTR(rdoRuntime::RDOSimulator) sim) = 0;
	virtual void   onAfterActionEnd   (PTR(rdoRuntime::RDOSimulator) sim) = 0;
	virtual double getNextTimeInterval(PTR(rdoRuntime::RDOSimulator) sim) = 0;
};
#define DECLARE_IAction \
	virtual void   onBeforeChoiceFrom (PTR(rdoRuntime::RDOSimulator) sim); \
	virtual bool   choiceFrom         (PTR(rdoRuntime::RDOSimulator) sim); \
	virtual void   onBeforeActionBegin(PTR(rdoRuntime::RDOSimulator) sim); \
	virtual void   convertBegin       (PTR(rdoRuntime::RDOSimulator) sim); \
	virtual void   onAfterActionBegin (PTR(rdoRuntime::RDOSimulator) sim); \
	virtual void   onBeforeActionEnd  (PTR(rdoRuntime::RDOSimulator) sim); \
	virtual void   convertEnd         (PTR(rdoRuntime::RDOSimulator) sim); \
	virtual void   onAfterActionEnd   (PTR(rdoRuntime::RDOSimulator) sim); \
	virtual double getNextTimeInterval(PTR(rdoRuntime::RDOSimulator) sim)

// пока не знаю зачем этот класс. наверное, пригодитс€ дл€ трассировки.
class IActionTrace
{
public:
	virtual tstring traceActionId() const = 0;
};
#define DECLARE_IActionTrace \
	virtual tstring traceActionId() const;

#endif //! _RDO_ACTION_INTERFACE_H_
