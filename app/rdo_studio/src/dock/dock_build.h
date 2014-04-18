/*!
  \copyright (c) RDO-Team, 2012-2012
  \file      dock_build.h
  \author    Урусов Андрей (rdo@rk9.bmstu.ru)
  \date      30.09.2012
  \brief     
  \indent    4T
*/

#ifndef _RDO_STUDIO_DOCK_BUILD_H_
#define _RDO_STUDIO_DOCK_BUILD_H_

// ----------------------------------------------------------------------- INCLUDES
// ----------------------------------------------------------------------- SYNOPSIS
#include "app/rdo_studio/src/dock/dock_focusable.h"
#include "app/rdo_studio/src/editor/build_edit.h"
// --------------------------------------------------------------------------------

class DockBuild: public DockFocusable
{
public:
	typedef rdo::gui::editor::Build context_type;

	DockBuild(QWidget* pParent);
	virtual ~DockBuild();

	void appendString(const QString& str);
	void appendString(const rdo::simulation::report::FileMessage& message);
	void clear();

	context_type& getContext();
};

#endif // _RDO_STUDIO_DOCK_BUILD_H_
