/*!
  \copyright (c) RDO-Team, 2003-2012
  \file      app/rdo_studio_mfc/src/view_preferences.h
  \author    Романов Ярослав (robot.xet@gmail.com)
  \date      27.11.2012
  \brief     
  \indent    4T
*/

#ifndef _RDO_STUDIO_VIEW_PREFERENCES_H_
#define _RDO_STUDIO_VIEW_PREFERENCES_H_

// ----------------------------------------------------------------------- INCLUDES
#include <QtGui/qdialog.h>
// ----------------------------------------------------------------------- SYNOPSIS
#include "app/rdo_studio_mfc/projects/common/bin/rdo_studio/generated/ui_view_preferenses.h"
#include "app/rdo_studio_mfc/src/options.h"
// --------------------------------------------------------------------------------

class ViewPreferences
	: public QDialog
	, private Ui::ViewPreferencesDialog
{
Q_OBJECT
public:
	explicit ViewPreferences(PTR(QWidget) pParent = NULL);

	void onUpdateStyleNotify(const rdoEditor::RDOEditorEditStyle& style);
	void setEditorPreferences(const rdoEditor::RDOEditorEditStyle& style);
	void updateDialog();
	rbool checkNewOptions(const rdoEditor::RDOEditorEditStyle& style);

private slots:
	void okButtonClicked();
	void cancelButtonClicked();
	void applyButtonClicked();
	void helpButtonClicked();

	void codeCompUseChanged(int state);
	void codeCompShowFullListChanged(bool state);
	void marginFoldChanged(int state);
	void marginBookmarkChanged(int state);
	void marginLineNumberChanged(int state);

private:
	rdoEditor::RDOEditorEditStyle style_editor;
};

#endif // _RDO_STUDIO_VIEW_PREFERENCES_H_