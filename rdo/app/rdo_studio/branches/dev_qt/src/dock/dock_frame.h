/*!
  \copyright (c) RDO-Team, 2012-2012
  \file      dock_frame.h
  \author    Урусов Андрей (rdo@rk9.bmstu.ru)
  \date      07.10.2012
  \brief     
  \indent    4T
*/

#ifndef _RDO_STUDIO_DOCK_DOCK_FRAME_H_
#define _RDO_STUDIO_DOCK_DOCK_FRAME_H_

// ----------------------------------------------------------------------- INCLUDES
// ----------------------------------------------------------------------- SYNOPSIS
#include "app/rdo_studio_mfc/src/dock/dock_base.h"
#include "app/rdo_studio_mfc/src/frame/tree_ctrl.h"
// --------------------------------------------------------------------------------

class DockFrame: public DockBase<RDOStudioFrameTreeCtrl>
{
public:
	DockFrame(PTR(QWidget) pParent);
	virtual ~DockFrame();

private:
	typedef  DockBase<RDOStudioFrameTreeCtrl>  parent_class;
};

#endif // _RDO_STUDIO_DOCK_DOCK_FRAME_H_
