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

INTERFACE_REGISTRATOR(IPokaz,           1);
INTERFACE_REGISTRATOR(IPokazTrace,      2);
INTERFACE_REGISTRATOR(IPokazTraceValue, 3);
INTERFACE_REGISTRATOR(IPokazWatchQuant, 4);
INTERFACE_REGISTRATOR(IPokazWatchValue, 5);
INTERFACE_REGISTRATOR(INotify,          6);
INTERFACE_REGISTRATOR(IModelStructure,  7);
INTERFACE_REGISTRATOR(ITrace,           8);
INTERFACE_REGISTRATOR(IName,            9);

#endif //! _RDO_RUNTIME_INTERFACE_REGISTRATOR_H_
