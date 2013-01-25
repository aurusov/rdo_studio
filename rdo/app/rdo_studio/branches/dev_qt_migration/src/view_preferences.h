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

	typedef PTR(QTreeWidgetItem) treeItem;

	//Все окна
	treeItem m_pRoot;
	treeItem m_pText;
	treeItem m_pCompile;
	treeItem m_pDebug;
	treeItem m_pTrace;
	treeItem m_pResult;
	treeItem m_pSearch;
	treeItem m_pChart;
	treeItem m_pAnimation;

	//Исходный текст
	treeItem m_pPlainText;
	treeItem m_pVariable;
	treeItem m_pKeyword;
	treeItem m_pFunction;
	treeItem m_pTraceText;
	treeItem m_pColor;
	treeItem m_pComment;
	treeItem m_pNumber;
	treeItem m_pString;
	treeItem m_pOperator;
	treeItem m_pCaret;
	treeItem m_pSelection;
	treeItem m_pBookmark;
	treeItem m_pGroup;
	treeItem m_pError;

	enum ItemType
	{
		IT_ROOT = 0,
		IT_TEXT,
		IT_COMPILE,
		IT_DEBUG,
		IT_TRACE,
		IT_RESULT,
		IT_SEARCH,
		IT_CHART,
		IT_ANIMATION
	};

	void createTree();
	void createTreeItem(PTR(QTreeWidgetItem) parent, PTR(QTreeWidgetItem) item, CREF(QString) name, ItemType itemType);
};

#endif // _RDO_STUDIO_VIEW_PREFERENCES_H_