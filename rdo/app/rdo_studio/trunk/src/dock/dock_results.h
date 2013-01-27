/*!
  \copyright (c) RDO-Team, 2012-2012
  \file      dock_results.h
  \author    Урусов Андрей (rdo@rk9.bmstu.ru)
  \date      03.10.2012
  \brief     
  \indent    4T
*/

#ifndef _RDO_STUDIO_DOCK_RESULTS_H_
#define _RDO_STUDIO_DOCK_RESULTS_H_

// ----------------------------------------------------------------------- INCLUDES
// ----------------------------------------------------------------------- SYNOPSIS
#include "app/rdo_studio_mfc/src/dock/dock_focusable.h"
#include "app/rdo_studio_mfc/rdo_edit/rdoeditorresults.h"
// --------------------------------------------------------------------------------

class DockResults: public DockFocusable
{
public:
	typedef rdoEditor::RDOEditorResults context_type;

	DockResults(PTR(QWidget) pParent);
	virtual ~DockResults();

	void appendString(CREF(tstring) str);
	void clear();

	REF(context_type) getContext();
};

#endif // _RDO_STUDIO_DOCK_RESULTS_H_
