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
#include "utils/src/common/warning_disable.h"
#include <QWidget>
#include "utils/src/common/warning_enable.h"
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
	View(QWidget* pParent);
	virtual ~View();

	void setModel(Model* pModel);

	TabCtrl& getTab();

private:
	typedef  QWidget  parent_type;

	FindDialog* m_pFindDialog;
	FindDialog::Settings m_findSettings;
	TabCtrl* m_pTabCtrl;
	Model* m_pModel;

	void closeEvent(QCloseEvent* event);

	void onFindDlgFind(const FindDialog::Settings& settings);
	void onFindDlgClose();

	void onSearchFindAll();

private slots:
	void onSearchFindInModel();
};

}}} // namespace rdo::gui::model

#endif // RDOSTUDIOMODELVIEW_H
