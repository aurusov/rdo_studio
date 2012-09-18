/*!
  \copyright (c) RDO-Team, 2003-2012
  \file      app/rdo_studio_mfc/src/edit/view.h
  \author    Урусов Андрей (rdo@rk9.bmstu.ru)
  \date      20.02.2003
  \brief     
  \indent    4T
*/

#ifndef _RDO_STUDIO_MFC_EDIT_VIEW_H_
#define _RDO_STUDIO_MFC_EDIT_VIEW_H_

// ----------------------------------------------------------------------- INCLUDES
// ----------------------------------------------------------------------- SYNOPSIS
#include "app/rdo_studio_mfc/src/edit/view_base.h"
// --------------------------------------------------------------------------------

namespace rdoEditor {
	class RDOEditorEdit;
}

// --------------------------------------------------------------------------------
// -------------------- RDOStudioEditView
// --------------------------------------------------------------------------------
class RDOStudioEditDoc;

class RDOStudioEditView: public RDOStudioEditBaseView
{
friend class RDOStudioEditDoc;

private:
	rdoEditor::RDOEditorEdit* edit;

protected:
	RDOStudioEditView(QWidget* pParent);

public:
	virtual ~RDOStudioEditView();

	virtual rdoEditor::RDOEditorEdit* getEdit() const;
};

#endif // _RDO_STUDIO_MFC_EDIT_VIEW_H_
