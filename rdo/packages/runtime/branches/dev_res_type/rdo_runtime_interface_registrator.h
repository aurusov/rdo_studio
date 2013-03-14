/*!
  \copyright (c) RDO-Team, 2011
  \file      rdo_runtime_interface_registrator.h
  \author    Урусов Андрей (rdo@rk9.bmstu.ru)
  \date      29.07.2009
  \brief     Глобальная регистрация интерфейсов
  \indent    4T
*/

#ifndef _LIB_RUNTIME_RUNTIME_INTERFACE_REGISTRATOR_H_
#define _LIB_RUNTIME_RUNTIME_INTERFACE_REGISTRATOR_H_

// ----------------------------------------------------------------------- INCLUDES
// ----------------------------------------------------------------------- SYNOPSIS
#include "utils/rdointerface.h"
// --------------------------------------------------------------------------------

INTERFACE_REGISTRATOR(IResult,                     1);
INTERFACE_REGISTRATOR(IResultTrace,                2);
INTERFACE_REGISTRATOR(IResultTraceValue,           3);
INTERFACE_REGISTRATOR(IResultWatchQuant,           4);
INTERFACE_REGISTRATOR(IResultWatchValue,           5);
INTERFACE_REGISTRATOR(INotify,                     6);
INTERFACE_REGISTRATOR(IModelStructure,             7);
INTERFACE_REGISTRATOR(ITrace,                      8);
INTERFACE_REGISTRATOR(IName,                       9);
INTERFACE_REGISTRATOR(IBaseOperation,             10);
INTERFACE_REGISTRATOR(IEvent,                     11);
INTERFACE_REGISTRATOR(IRule,                      12);
INTERFACE_REGISTRATOR(IOperation,                 13);
INTERFACE_REGISTRATOR(IKeyboard,                  14);
INTERFACE_REGISTRATOR(ILogic,                     15);
INTERFACE_REGISTRATOR(IDPTSearchTraceStatistics,  16);
INTERFACE_REGISTRATOR(IBaseOperationContainer,    17);
INTERFACE_REGISTRATOR(IPROCBlock,                 18);
INTERFACE_REGISTRATOR(IPriority,                  19);
INTERFACE_REGISTRATOR(IActivity,                  20);
INTERFACE_REGISTRATOR(IActivityTrace,             21);
INTERFACE_REGISTRATOR(IActivityPatternTrace,      22);
INTERFACE_REGISTRATOR(IOperationTrace,            23);
INTERFACE_REGISTRATOR(IDPTSearchActivity,         24);
INTERFACE_REGISTRATOR(IDPTSearchLogic,            25);
INTERFACE_REGISTRATOR(IPROCProcess,               26);
INTERFACE_REGISTRATOR(IResultGroup,               27);
INTERFACE_REGISTRATOR(IResultGetValue,            28);
INTERFACE_REGISTRATOR(IInternalStatisticsManager, 29);

#endif // _LIB_RUNTIME_RUNTIME_INTERFACE_REGISTRATOR_H_
