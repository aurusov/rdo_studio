#pragma once

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
