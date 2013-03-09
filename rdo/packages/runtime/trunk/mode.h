/*!
  \copyright (c) RDO-Team, 2012
  \file      mode.h
  \author    Урусов Андрей (rdo@rk9.bmstu.ru)
  \date      16.02.2012
  \brief     Режимы моделирования
  \indent    4T
*/

#ifndef _LIB_RUNTIME_NAMESPACE_MODE_H_
#define _LIB_RUNTIME_NAMESPACE_MODE_H_

// ----------------------------------------------------------------------- INCLUDES
// ----------------------------------------------------------------------- SYNOPSIS
#include "simulator/runtime/namespace.h"
// --------------------------------------------------------------------------------

OPEN_RDO_RUNTIME_NAMESPACE

//! Режимы моделирования
enum RunTimeMode
{
	RTM_MaxSpeed,   //!< Максимальная производительность
	RTM_Jump,       //!< Дискретная имитация
	RTM_Sync,       //!< Синхронная имитация
	RTM_Pause,      //!< Режим паузы
	RTM_BreakPoint  //!< Режим точки остановки
};

CLOSE_RDO_RUNTIME_NAMESPACE

#endif // _LIB_RUNTIME_NAMESPACE_MODE_H_
