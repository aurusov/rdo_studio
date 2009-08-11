/*
 * copyright: (c) RDO-Team, 2009
 * filename : rdo_runtime_interface_registrator.h
 * author   : Урусов Андрей
 * date     : 29.07.2009
 * bref     : Присваивает интерфейсам уникальные номера
 * indent   : 4T
 */

#ifndef _RDO_RUNTIME_INTERFACE_REGISTRATOR_H_
#define _RDO_RUNTIME_INTERFACE_REGISTRATOR_H_

// ====================================================================== INCLUDES
// ====================================================================== SYNOPSIS
#include <rdointerface.h>
// ===============================================================================

INTERFACE_REGISTRATOR(IPokaz,                     1);
INTERFACE_REGISTRATOR(IPokazTrace,                2);
INTERFACE_REGISTRATOR(IPokazTraceValue,           3);
INTERFACE_REGISTRATOR(IPokazWatchQuant,           4);
INTERFACE_REGISTRATOR(IPokazWatchValue,           5);
INTERFACE_REGISTRATOR(INotify,                    6);
INTERFACE_REGISTRATOR(IModelStructure,            7);
INTERFACE_REGISTRATOR(ITrace,                     8);
INTERFACE_REGISTRATOR(IName,                      9);
INTERFACE_REGISTRATOR(IBaseOperation,            10);
INTERFACE_REGISTRATOR(IIrregEvent,               11);
INTERFACE_REGISTRATOR(IRule,                     12);
INTERFACE_REGISTRATOR(IOperation,                13);
INTERFACE_REGISTRATOR(IKeyboard,                 14);
INTERFACE_REGISTRATOR(ILogic,                    15);
INTERFACE_REGISTRATOR(IDPTSearchTraceStatistics, 16);
INTERFACE_REGISTRATOR(IBaseOperationContainer,   17);
INTERFACE_REGISTRATOR(IPROCBlock,                18);
INTERFACE_REGISTRATOR(IPriority,                 19);
INTERFACE_REGISTRATOR(IActivity,                 20);
INTERFACE_REGISTRATOR(IActivityTrace,            21);
INTERFACE_REGISTRATOR(IActivityPatternTrace,     22);
INTERFACE_REGISTRATOR(IOperationTrace,           23);
INTERFACE_REGISTRATOR(IDPTSearchActivity,        24);
INTERFACE_REGISTRATOR(IDPTSearchLogic,           25);
INTERFACE_REGISTRATOR(IPROCProcess,              26);

#endif //! _RDO_RUNTIME_INTERFACE_REGISTRATOR_H_
