/*
 * copyright: (c) RDO-Team, 2009
 * filename : rdo_rule_i.h
 * author   : Урусов Андрей
 * date     : 01.08.2009
 * bref     : 
 * indent   : 4T
 */

#ifndef _RDO_RULE_I_H_
#define _RDO_RULE_I_H_

// ====================================================================== INCLUDES
// ====================================================================== SYNOPSIS
#include "rdo_common/namespace.h"
#include "rdo_common/rdointerface.h"
// ===============================================================================

OPEN_RDO_RUNTIME_NAMESPACE

class RDOSimulator;
class RDORuntime;

CLOSE_RDO_RUNTIME_NAMESPACE

class IRule
{
public:
	virtual void  onBeforeChoiceFrom(PTR(rdoRuntime::RDOSimulator) sim    )             = 0;
	virtual rbool choiceFrom        (PTR(rdoRuntime::RDORuntime)   runtime)             = 0;
	virtual void  onBeforeRule      (PTR(rdoRuntime::RDOSimulator) sim    )             = 0;
	virtual void  convertRule       (PTR(rdoRuntime::RDORuntime)   runtime)             = 0;
	virtual void  onAfterRule       (PTR(rdoRuntime::RDOSimulator) sim, rbool inSearch) = 0;
};
#define DECLARE_IRule \
	virtual void  onBeforeChoiceFrom(PTR(rdoRuntime::RDOSimulator) sim    ); \
	virtual rbool choiceFrom        (PTR(rdoRuntime::RDORuntime)   runtime); \
	virtual void  onBeforeRule      (PTR(rdoRuntime::RDOSimulator) sim    ); \
	virtual void  convertRule       (PTR(rdoRuntime::RDORuntime)   runtime); \
	virtual void  onAfterRule       (PTR(rdoRuntime::RDOSimulator) sim, rbool inSearch);

#endif //! _RDO_RULE_I_H_
