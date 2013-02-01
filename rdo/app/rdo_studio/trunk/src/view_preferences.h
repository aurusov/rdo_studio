/*!
  \copyright (c) RDO-Team, 2003-2012
  \file      app/rdo_studio/src/view_preferences.h
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
#include "app/rdo_studio/projects/common/bin/rdo_studio/generated/ui_view_preferenses.h"
#include "app/rdo_studio/src/options.h"
#include "app/rdo_studio/rdo_edit/rdoeditoredit.h"
#include "app/rdo_studio/edit_ctrls/rdobuildedit.h"
#include "app/rdo_studio/edit_ctrls/rdodebugedit.h"
#include "app/rdo_studio/edit_ctrls/rdofindedit.h"
#include "app/rdo_studio/rdo_edit/rdoeditoreditstyle.h"
#include "app/rdo_studio/rdo_edit/rdoeditorresultsstyle.h"
#include "app/rdo_studio/edit_ctrls/rdobuildeditstyle.h"
#include "app/rdo_studio/edit_ctrls/rdofindeditstyle.h"
#include "app/rdo_studio/rdo_edit/rdoeditorresults.h"
#include "app/rdo_studio/src/tracer/ctrls/log_main_wnd.h"
#include "app/rdo_studio/src/frame/view.h"
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

	void onSetup(int state);
	void onCheckInFuture(int state);
	void onOpenLastProject(int state);
	void onShowFullName(int state);

	void codeCompUseChanged(int state);
	void codeCompShowFullListChanged(bool state);
	void marginFoldChanged(int state);
	void marginBookmarkChanged(int state);
	void marginLineNumberChanged(int state);

	void onUseTabSymbol(int state);
	void onIndentAsTab(int state);
	void onAutoIndent(int state);
	void onEraseWithTab(bool state);
	void onTabSize(const QString& text);
	void onIndentSize(const QString& text);

	void onTreeWidgetItemActivated(QTreeWidgetItem* item, int column);
	void onSwitchPreviewComboBox(int index);

private:
	rbool m_setup;
	rbool m_checkInFuture;
	rbool m_openLastProject;
	rbool m_showFullName;

	rdoEditor::RDOEditorEditStyle    style_editor;

	PTR(rdoEditor::RDOEditorEdit)     preview_editor;
	PTR(rdoEditCtrl::RDOBuildEdit)    preview_build;
	PTR(rdoEditCtrl::RDODebugEdit)    preview_debug;
	PTR(rdo::gui::tracer::LogMainWnd) preview_trace;
	PTR(rdoEditor::RDOEditorResults)  preview_results;
	PTR(rdoEditCtrl::RDOFindEdit)     preview_find;

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

	//Окно компиляции
	treeItem m_pTextCompile;
	treeItem m_pSelectedString;
	treeItem m_pCaretCompile;
	treeItem m_pSelectionCompile;
	treeItem m_pBookmarkCompile;

	//Окно отладки
	treeItem m_pTextDebug;
	treeItem m_pCaretDebug;
	treeItem m_pSelectionDebug;
	treeItem m_pBookmarkDebug;

	//Окно трассировки
	treeItem m_pES;
	treeItem m_pEB;
	treeItem m_pEF;
	treeItem m_pEI;
	treeItem m_pER;
	treeItem m_pRC;
	treeItem m_pRE;
	treeItem m_pRK;
	treeItem m_pV;
	treeItem m_pStatus;
	treeItem m_pDPS;
	treeItem m_pSB;
	treeItem m_pSO;
	treeItem m_pSTN;
	treeItem m_pSTD;
	treeItem m_pSTR;
	treeItem m_pSRC;
	treeItem m_pSRE;
	treeItem m_pSRK;
	treeItem m_pSD;
	treeItem m_pSES;
	treeItem m_pSEN;
	treeItem m_pSEM;
	treeItem m_pSEF;
	treeItem m_pSEU;

	//Окно результатов
	treeItem m_pPlainTextResult;
	treeItem m_pVariableResult;
	treeItem m_pKeywordResult;
	treeItem m_pNumberResult;
	treeItem m_pStringResult;
	treeItem m_pOperatorResult;
	treeItem m_pCaretResult;
	treeItem m_pSelectionResult;
	treeItem m_pBookmarkResult;

	//Окно поиска
	treeItem m_pTextSearch;
	treeItem m_pStringSearch;
	treeItem m_pSelectedStringSearch;
	treeItem m_pCaretSearch;
	treeItem m_pSelectionSearch;
	treeItem m_pBookmarkSearch;

	//Окно графиков
	treeItem m_pAxis;
	treeItem m_pTitle;
	treeItem m_pLegend;
	treeItem m_pGraph;
	treeItem m_pTime;

	//Окно анимации
	treeItem m_pEdgingColor;
	treeItem m_pBackgroundColor;	

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
	
	void createPreview();
	void createTree();
	PTR(QTreeWidgetItem) createTreeItem (PTR(QTreeWidgetItem) parent, CREF(QString) name, ItemType itemType);

	void apply();
};

#endif // _RDO_STUDIO_VIEW_PREFERENCES_H_