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
#include "utils/src/interface/rdointerface.h"
#include "utils/src/smart_ptr/intrusive_ptr/intrusive_ptr.h"
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
	virtual void  onBeforeChoiceFrom(const rdo::runtime::LPRDORuntime& pRuntime)                = 0;
	virtual bool  choiceFrom        (const rdo::runtime::LPRDORuntime& pRuntime)                = 0;
	virtual void  onBeforeRule      (const rdo::runtime::LPRDORuntime& pRuntime)                = 0;
	virtual void  convertRule       (const rdo::runtime::LPRDORuntime& pRuntime)                = 0;
	virtual void  onAfterRule       (const rdo::runtime::LPRDORuntime& pRuntime, bool inSearch) = 0;
};

#define DECLARE_IRule \
	virtual void  onBeforeChoiceFrom(const rdo::runtime::LPRDORuntime& pRuntime); \
	virtual bool  choiceFrom        (const rdo::runtime::LPRDORuntime& pRuntime); \
	virtual void  onBeforeRule      (const rdo::runtime::LPRDORuntime& pRuntime); \
	virtual void  convertRule       (const rdo::runtime::LPRDORuntime& pRuntime); \
	virtual void  onAfterRule       (const rdo::runtime::LPRDORuntime& pRuntime, bool inSearch);

#endif // _LIB_RUNTIME_RULE_I_H_
