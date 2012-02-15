/*!
  \copyright (c) RDO-Team, 2011
  \file      rdo_rule_i.h
  \author    Урусов Андрей (rdo@rk9.bmstu.ru)
  \date      01.08.2009
  \brief     Интерфейс продукционных правил
  \indent    4T
*/

#ifndef _LIB_RUNTIME_RULE_I_H_
#define _LIB_RUNTIME_RULE_I_H_

// ----------------------------------------------------------------------- INCLUDES
// ----------------------------------------------------------------------- SYNOPSIS
#include "utils/rdointerface.h"
#include "utils/smart_ptr/intrusive_ptr.h"
#include "simulator/runtime/namespace.h"
// --------------------------------------------------------------------------------

OPEN_RDO_RUNTIME_NAMESPACE

PREDECLARE_POINTER(RDORuntime);

CLOSE_RDO_RUNTIME_NAMESPACE

/*!
  \interface IRule
  \brief     Интерфейс продукционного правила
*/
class IRule
{
public:
	virtual void  onBeforeChoiceFrom(CREF(rdoRuntime::LPRDORuntime) pRuntime)                 = 0;
	virtual rbool choiceFrom        (CREF(rdoRuntime::LPRDORuntime) pRuntime)                 = 0;
	virtual void  onBeforeRule      (CREF(rdoRuntime::LPRDORuntime) pRuntime)                 = 0;
	virtual void  convertRule       (CREF(rdoRuntime::LPRDORuntime) pRuntime)                 = 0;
	virtual void  onAfterRule       (CREF(rdoRuntime::LPRDORuntime) pRuntime, rbool inSearch) = 0;
};

#define DECLARE_IRule \
	virtual void  onBeforeChoiceFrom(CREF(rdoRuntime::LPRDORuntime) pRuntime); \
	virtual rbool choiceFrom        (CREF(rdoRuntime::LPRDORuntime) pRuntime); \
	virtual void  onBeforeRule      (CREF(rdoRuntime::LPRDORuntime) pRuntime); \
	virtual void  convertRule       (CREF(rdoRuntime::LPRDORuntime) pRuntime); \
	virtual void  onAfterRule       (CREF(rdoRuntime::LPRDORuntime) pRuntime, rbool inSearch);

#endif // _LIB_RUNTIME_RULE_I_H_
