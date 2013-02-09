/*!
  \copyright (c) RDO-Team, 2003-2012
  \file      app/rdo_studio/src/model/model_view.h
  \author    Урусов Андрей (rdo@rk9.bmstu.ru)
  \date      20.02.2003
  \brief     
  \indent    4T
*/

#ifndef _RDO_STUDIO_MODEL_VIEW_H_
#define _RDO_STUDIO_MODEL_VIEW_H_

// ----------------------------------------------------------------------- INCLUDES
#include <QtWidgets/qwidget.h>
// ----------------------------------------------------------------------- SYNOPSIS
#include "app/rdo_studio/src/model/model.h"
#include "app/rdo_studio/src/dialog/find_dialog.h"
// --------------------------------------------------------------------------------

// --------------------------------------------------------------------------------
// -------------------- RDOStudioModelView
// --------------------------------------------------------------------------------
namespace rdoEditor {
	class RDOEditorTabCtrl;
}

class RDOStudioModelView
	: public QWidget
{
Q_OBJECT
public:
	RDOStudioModelView(PTR(QWidget) pParent);
	virtual ~RDOStudioModelView();

	void setModel(PTR(RDOStudioModel) pModel);

	REF(rdoEditor::RDOEditorTabCtrl) getTab();

private:
	typedef  QWidget  parent_type;

	FindDialog*          m_pFindDialog;
	FindDialog::Settings m_findSettings;

	PTR(rdoEditor::RDOEditorTabCtrl) m_pTabCtrl;
	PTR(RDOStudioModel)              m_pModel;

	void closeEvent(PTR(QCloseEvent) event);

	void onFindDlgFind(CREF(FindDialog::Settings) settings);
	void onFindDlgClose();

	void onSearchFindAll();

private slots:
	void onSearchFindInModel();

};

#endif // RDOSTUDIOMODELVIEW_H
