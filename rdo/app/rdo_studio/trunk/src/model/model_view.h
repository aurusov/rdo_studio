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
#include "utils/warning_disable.h"
#include <QWidget>
#include "utils/warning_enable.h"
// ----------------------------------------------------------------------- SYNOPSIS
#include "app/rdo_studio/src/model/model.h"
#include "app/rdo_studio/src/dialog/find_dialog.h"
// --------------------------------------------------------------------------------

namespace rdo { namespace gui { namespace model {

class TabCtrl;

class View: public QWidget
{
Q_OBJECT

public:
	View(PTR(QWidget) pParent);
	virtual ~View();

	void setModel(PTR(Model) pModel);

	REF(TabCtrl) getTab();

private:
	typedef  QWidget  parent_type;

	FindDialog*          m_pFindDialog;
	FindDialog::Settings m_findSettings;

	PTR(TabCtrl) m_pTabCtrl;
	PTR(Model)   m_pModel;

	void closeEvent(PTR(QCloseEvent) event);

	void onFindDlgFind(CREF(FindDialog::Settings) settings);
	void onFindDlgClose();

	void onSearchFindAll();

private slots:
	void onSearchFindInModel();
};

}}} // namespace rdo::gui::model

#endif // RDOSTUDIOMODELVIEW_H
