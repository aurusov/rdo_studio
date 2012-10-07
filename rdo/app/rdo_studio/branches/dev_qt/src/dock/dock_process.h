/*!
  \copyright (c) RDO-Team, 2012-2012
  \file      dock_process.h
  \author    Урусов Андрей (rdo@rk9.bmstu.ru)
  \date      07.10.2012
  \brief     
  \indent    4T
*/

#ifndef _RDO_STUDIO_DOCK_PROCESS_H_
#define _RDO_STUDIO_DOCK_PROCESS_H_

// ----------------------------------------------------------------------- INCLUDES
// ----------------------------------------------------------------------- SYNOPSIS
#include "app/rdo_studio_mfc/src/dock/dock_base.h"
#include "app/rdo_studio_mfc/rdo_process/rdoprocess_pagectrl.h"
// --------------------------------------------------------------------------------

class DockProcess: public DockBase<RPPageCtrl>
{
public:
	DockProcess(PTR(QWidget) pParent);
	virtual ~DockProcess();

private:
	typedef  DockBase<RPPageCtrl>  parent_class;
};

#endif // _RDO_STUDIO_DOCK_PROCESS_H_
