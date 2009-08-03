/*
 * copyright: (c) RDO-Team, 2009
 * filename : rdo_rule_interface.h
 * author   : Урусов Андрей
 * date     : 01.08.2009
 * bref     : 
 * indent   : 4T
 */

#ifndef _RDO_RULE_INTERFACE_H_
#define _RDO_RULE_INTERFACE_H_

// ====================================================================== INCLUDES
// ====================================================================== SYNOPSIS
#include <rdointerface.h>
// ===============================================================================

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

#endif //! _RDO_RULE_INTERFACE_H_
