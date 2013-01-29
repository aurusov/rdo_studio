/*!
  \copyright (c) RDO-Team, 2003-2012
  \file      app/rdo_studio/src/view_preferences.cpp
  \author    Романов Ярослав (robot.xet@gmail.com)
  \date      27.11.2012
  \brief     
  \indent    4T
*/

// ---------------------------------------------------------------------------- PCH
#include "app/rdo_studio/pch/stdpch.h"
// ----------------------------------------------------------------------- INCLUDES
// ----------------------------------------------------------------------- SYNOPSIS
#include "app/rdo_studio/src/view_preferences.h"
#include "app/rdo_studio/res/build_version.h"
#include "app/rdo_studio/src/application.h"
#include "simulator/report/build_edit_line_info.h"
// --------------------------------------------------------------------------------
using namespace rdo::simulation::report;
using namespace rdoEditCtrl;
using namespace rdoEditor;
using namespace rdo::gui::tracer;

ViewPreferences::ViewPreferences(PTR(QWidget) pParent)
	: QDialog(pParent),
	  style_editor()
{
	setupUi(this);

	connect(buttonOk, SIGNAL(clicked()), this, SLOT(okButtonClicked()));
	connect(buttonCancel, SIGNAL(clicked()), this, SLOT(cancelButtonClicked()));
	connect(buttonApply, SIGNAL(clicked()), this, SLOT(applyButtonClicked()));
	connect(buttonHelp, SIGNAL(clicked()), this, SLOT(helpButtonClicked()));

	//Вкладка "Редактор"
	connect(checkBoxCodeCompUse, SIGNAL(stateChanged(int)), this, SLOT(codeCompUseChanged(int)));
	connect(radioButtonFullList, SIGNAL(toggled(bool)), this, SLOT(codeCompShowFullListChanged(bool)));
	connect(radioButtonNearestWords, SIGNAL(toggled(bool)), this, SLOT(codeCompShowFullListChanged(bool)));
	connect(checkBoxMarginFold, SIGNAL(stateChanged(int)), this, SLOT(marginFoldChanged(int)));
	connect(checkBoxMarginBookmark, SIGNAL(stateChanged(int)), this, SLOT(marginBookmarkChanged(int)));
	connect(checkBoxMarginLineNum, SIGNAL(stateChanged(int)), this, SLOT(marginLineNumberChanged(int)));

	//Вкладка "Стиль и цвет"
	stackedWidget->setCurrentWidget(pageRoot);
	createTree();
	createPreview();

	switchPreviewComboBox->addItem("Editor",  IT_TEXT);
	switchPreviewComboBox->addItem("Build",   IT_COMPILE);
	switchPreviewComboBox->addItem("Debug",   IT_DEBUG);
	switchPreviewComboBox->addItem("Tracer",  IT_TRACE);
	switchPreviewComboBox->addItem("Results", IT_RESULT);
	switchPreviewComboBox->addItem("Find",    IT_SEARCH);
	switchPreviewComboBox->addItem("Chart",   IT_CHART);
	switchPreviewComboBox->addItem("Frame",   IT_ANIMATION);

	connect(treeWidget, SIGNAL(itemClicked(QTreeWidgetItem*, int)), this, SLOT(onTreeWidgetItemActivated(QTreeWidgetItem*, int)));
	connect(switchPreviewComboBox, SIGNAL(currentIndexChanged(int)), this, SLOT(onSwitchPreviewComboBox(int)));
}

void ViewPreferences::onUpdateStyleNotify(const rdoEditor::RDOEditorEditStyle& style)
{
	if(style_editor != style)
		setEditorPreferences(style);
	updateDialog();
}

void ViewPreferences::okButtonClicked()
{

}

void ViewPreferences::cancelButtonClicked()
{

}

void ViewPreferences::applyButtonClicked()
{
	//applyList
}

void ViewPreferences::helpButtonClicked()
{

}

void ViewPreferences::codeCompUseChanged(int state)
{
	style_editor.autoComplete->useAutoComplete = state;
	
	switch(state)
	{
		case 0:
			radioButtonFullList->setEnabled(false);
			radioButtonNearestWords->setEnabled(false);
			break;
		case 2:
			radioButtonFullList->setEnabled(true);
			radioButtonNearestWords->setEnabled(true);
			break;
	}

}

void ViewPreferences::codeCompShowFullListChanged(bool state)
{
	UNUSED(state);

	if(radioButtonFullList->isChecked())
		style_editor.autoComplete->showFullList = true;
	if(radioButtonNearestWords->isChecked())
		style_editor.autoComplete->showFullList = false;
}

void ViewPreferences::marginFoldChanged(int state)
{
	style_editor.margin->fold = state;
}

void ViewPreferences::marginBookmarkChanged(int state)
{
	style_editor.margin->bookmark = state;
}

void ViewPreferences::marginLineNumberChanged(int state)
{
	style_editor.margin->lineNumber = state;
}

void ViewPreferences::setEditorPreferences(const rdoEditor::RDOEditorEditStyle& style)
{
	style_editor = style;
}

void ViewPreferences::updateDialog()
{
	checkBoxCodeCompUse->setCheckState(style_editor.autoComplete->useAutoComplete
		? Qt::Checked
		: Qt::Unchecked
		);
	checkBoxMarginFold->setCheckState(style_editor.margin->fold
		? Qt::Checked
		: Qt::Unchecked
		);
	checkBoxMarginBookmark->setCheckState(style_editor.margin->bookmark
		? Qt::Checked
		: Qt::Unchecked
		);
	checkBoxMarginLineNum->setCheckState(style_editor.margin->lineNumber
		? Qt::Checked
		: Qt::Unchecked
		);

	style_editor.autoComplete->showFullList
		? radioButtonFullList->toggle()
		: radioButtonNearestWords->toggle();
}
void ViewPreferences::createPreview()
{
	preview_editor = new RDOEditorEdit(previewStackedWidget->currentWidget(), previewStackedWidget->currentWidget());
	ASSERT(preview_editor);
	preview_editor->setEditorStyle(&studioApp.getStyle()->style_editor);
	preview_editor->setCanClearErrorLine(false);
	preview_editor->appendText(rdo::format(IDS_COLORSTYLE_EDITOR_SAMPLE));
	preview_editor->scrollToLine(0);
	preview_editor->setReadOnly(true);
	preview_editor->bookmarkToggle();
	preview_editor->setErrorLine(preview_editor->getLineCount() - 1);
	previewStackedWidget->addWidget(preview_editor);

	preview_build = new RDOBuildEdit(previewStackedWidget->currentWidget());
	preview_build->setEditorStyle(&studioApp.getStyle()->style_build);
	preview_build->appendLine(new BuildEditLineInfo(rdo::format(IDS_COLORSTYLE_BUILD_SAMPLE1)));
	preview_build->appendLine(new BuildEditLineInfo(rdo::simulation::report::FileMessage(rdo::format(IDS_COLORSTYLE_BUILD_SAMPLE2), rdoModelObjects::PAT, 40, 0)));
	preview_build->appendLine(new BuildEditLineInfo(rdo::format(IDS_COLORSTYLE_BUILD_SAMPLE3)));
	preview_build->gotoNext();
	previewStackedWidget->addWidget(preview_build);

	preview_debug = new RDODebugEdit(previewStackedWidget->currentWidget());
	preview_debug->setEditorStyle(&studioApp.getStyle()->style_debug);
	preview_debug->appendLine(rdo::format(IDS_COLORSTYLE_DEBUG_SAMPLE));
	previewStackedWidget->addWidget(preview_debug);

	preview_trace = new LogMainWnd(previewStackedWidget->currentWidget());
	preview_trace->view().setStyle(&studioApp.getStyle()->style_trace);
	preview_trace->view().setFocusOnly(true);
	preview_trace->view().setText(rdo::format(IDS_COLORSTYLE_LOG_SAMPLE));
	preview_trace->view().selectLine(0);
	previewStackedWidget->addWidget(preview_trace);

	preview_results = new RDOEditorResults(previewStackedWidget->currentWidget());
	preview_results->setEditorStyle(&studioApp.getStyle()->style_results);
	preview_results->setReadOnly(false);
	preview_results->replaceCurrent(rdo::format(IDS_COLORSTYLE_RESULTS_SAMPLE), 0);
	preview_results->setReadOnly(true);
	previewStackedWidget->addWidget(preview_results);

	preview_find = new RDOFindEdit(previewStackedWidget->currentWidget());
	preview_find->setEditorStyle(&studioApp.getStyle()->style_find);
	preview_find->setKeyword("$Time");
	preview_find->appendLine(new LogEditLineInfo(rdo::format(IDS_COLORSTYLE_FIND_SAMPLE1)));
	preview_find->appendLine(new LogEditLineInfo(rdo::simulation::report::FileMessage(rdo::format(IDS_COLORSTYLE_FIND_SAMPLE2), rdoModelObjects::PAT, 3, 0)));
	preview_find->appendLine(new LogEditLineInfo(rdo::simulation::report::FileMessage(rdo::format(IDS_COLORSTYLE_FIND_SAMPLE3), rdoModelObjects::PAT, 13, 0)));
	preview_find->appendLine(new LogEditLineInfo(rdo::format(IDS_COLORSTYLE_FIND_SAMPLE4)));
	preview_find->gotoNext();
	previewStackedWidget->addWidget(preview_find);
}
void ViewPreferences::createTree()
{
	treeWidget->setColumnCount(1);
	treeWidget->setHeaderHidden(true);
	treeWidget->setRootIsDecorated(false);

	m_pRoot = new QTreeWidgetItem(treeWidget);
	m_pRoot->setText(0, "Все окна");
	m_pRoot->setData(0, Qt::UserRole, IT_ROOT);

	m_pText      = createTreeItem(m_pRoot, "Исходный текст",   IT_TEXT);
	m_pCompile   = createTreeItem(m_pRoot, "Окно компиляции",  IT_COMPILE);
	m_pDebug     = createTreeItem(m_pRoot, "Окно отладки",     IT_DEBUG);
	m_pTrace     = createTreeItem(m_pRoot, "Окно трассировки", IT_TRACE);
	m_pResult    = createTreeItem(m_pRoot, "Окно результатов", IT_RESULT);
	m_pSearch    = createTreeItem(m_pRoot, "Окно поиска",      IT_SEARCH);
	m_pChart     = createTreeItem(m_pRoot, "Окно графиков",    IT_CHART);
	m_pAnimation = createTreeItem(m_pRoot, "Окно анимации",    IT_ANIMATION);

	m_pRoot->setExpanded(true);

	m_pPlainText = createTreeItem(m_pText, "Обыкновенный текст", IT_TEXT);
	m_pVariable  = createTreeItem(m_pText, "Переменная",         IT_TEXT);
	m_pKeyword   = createTreeItem(m_pText, "Ключевое слово",     IT_TEXT);
	m_pTraceText = createTreeItem(m_pText, "Трассировка",        IT_TEXT);
	m_pColor     = createTreeItem(m_pText, "Цвет",               IT_TEXT);
	m_pComment   = createTreeItem(m_pText, "Комментарии",        IT_TEXT);
	m_pNumber    = createTreeItem(m_pText, "Число",              IT_TEXT);
	m_pString    = createTreeItem(m_pText, "Строка",             IT_TEXT);
	m_pOperator  = createTreeItem(m_pText, "Оператор",           IT_TEXT);
	m_pCaret     = createTreeItem(m_pText, "Каретка",            IT_TEXT);
	m_pSelection = createTreeItem(m_pText, "Выделение",          IT_TEXT);
	m_pBookmark  = createTreeItem(m_pText, "Закладка",           IT_TEXT);
	m_pGroup     = createTreeItem(m_pText, "Группа",             IT_TEXT);
	m_pError     = createTreeItem(m_pText, "Ошибка",             IT_TEXT);

	m_pTextCompile      = createTreeItem(m_pCompile, "текст",             IT_COMPILE);
	m_pSelectedString   = createTreeItem(m_pCompile, "выделенная строка", IT_COMPILE);
	m_pCaretCompile     = createTreeItem(m_pCompile, "каретка",           IT_COMPILE);
	m_pSelectionCompile = createTreeItem(m_pCompile, "выделение",         IT_COMPILE);
	m_pBookmarkCompile  = createTreeItem(m_pCompile, "закладка",          IT_COMPILE);

	m_pTextDebug      = createTreeItem(m_pDebug, "текст",     IT_DEBUG);
	m_pCaretDebug     = createTreeItem(m_pDebug, "каретка",   IT_DEBUG);
	m_pSelectionDebug = createTreeItem(m_pDebug, "выделение", IT_DEBUG);
	m_pBookmarkDebug  = createTreeItem(m_pDebug, "закладка",  IT_DEBUG);

	m_pES     = createTreeItem(m_pTrace, "Служебное событие (ES)",                         IT_TRACE);
	m_pEB     = createTreeItem(m_pTrace, "Начало действия (EB)",                           IT_TRACE);
	m_pEF     = createTreeItem(m_pTrace, "Окончание действия (EF)",                        IT_TRACE);
	m_pEI     = createTreeItem(m_pTrace, "Нерегулярное событие (EI)",                      IT_TRACE);
	m_pER     = createTreeItem(m_pTrace, "Продукционное правило (ER)",                     IT_TRACE);
	m_pRC     = createTreeItem(m_pTrace, "Создание ресурса (RC)",                          IT_TRACE);
	m_pRE     = createTreeItem(m_pTrace, "Удаление ресурса (RE)",                          IT_TRACE);
	m_pRK     = createTreeItem(m_pTrace, "Изменение состояния ресурса (RK)",               IT_TRACE);
	m_pV      = createTreeItem(m_pTrace, "Трассировка индекса (V)",                        IT_TRACE);
	m_pStatus = createTreeItem(m_pTrace, "Статус окончания моделирования ($Status)",       IT_TRACE);
	m_pDPS    = createTreeItem(m_pTrace, "Статистика по поиску на графе (DPS)",            IT_TRACE);
	m_pSB     = createTreeItem(m_pTrace, "Начало поиска (SB)",                             IT_TRACE);
	m_pSO     = createTreeItem(m_pTrace, "Трассировка раскрываемой вершины (SO)",          IT_TRACE);
	m_pSTN    = createTreeItem(m_pTrace, "Признак вершины (STN)",                          IT_TRACE);
	m_pSTD    = createTreeItem(m_pTrace, "Признак вершины (STD)",                          IT_TRACE);
	m_pSTR    = createTreeItem(m_pTrace, "Признак вершины (STR)",                          IT_TRACE);
	m_pSRC    = createTreeItem(m_pTrace, "Создание ресурса (при поиске) (SRC)",            IT_TRACE);
	m_pSRE    = createTreeItem(m_pTrace, "Удаление ресурса (при поиске) (SRE)",            IT_TRACE);
	m_pSRK    = createTreeItem(m_pTrace, "Изменение состояния ресурса (при поиске) (SRK)", IT_TRACE);
	m_pSD     = createTreeItem(m_pTrace, "Трассировка решения (SD)",                       IT_TRACE);
	m_pSES    = createTreeItem(m_pTrace, "Завершение поиска (SES)",                        IT_TRACE);
	m_pSEN    = createTreeItem(m_pTrace, "Завершение поиска (SEN)",                        IT_TRACE);
	m_pSEM    = createTreeItem(m_pTrace, "Завершение поиска (SEM)",                        IT_TRACE);
	m_pSEF    = createTreeItem(m_pTrace, "Завершение поиска (SEF)",                        IT_TRACE);
	m_pSEU    = createTreeItem(m_pTrace, "Завершение поиска (SEU)",                        IT_TRACE);

	m_pPlainTextResult = createTreeItem(m_pResult, "исходный текст", IT_RESULT);
	m_pVariableResult  = createTreeItem(m_pResult, "переменная",     IT_RESULT);
	m_pKeywordResult   = createTreeItem(m_pResult, "ключевое слово", IT_RESULT);
	m_pNumberResult    = createTreeItem(m_pResult, "число",          IT_RESULT);
	m_pStringResult    = createTreeItem(m_pResult, "строка",         IT_RESULT);
	m_pOperatorResult  = createTreeItem(m_pResult, "оператор",       IT_RESULT);
	m_pCaretResult     = createTreeItem(m_pResult, "каретка",        IT_RESULT);
	m_pSelectionResult = createTreeItem(m_pResult, "выделение",      IT_RESULT);
	m_pBookmarkResult  = createTreeItem(m_pResult, "закладка",       IT_RESULT);

	m_pTextSearch           = createTreeItem(m_pSearch, "текст",             IT_SEARCH);
	m_pStringSearch         = createTreeItem(m_pSearch, "строка для поиска", IT_SEARCH);
	m_pSelectedStringSearch = createTreeItem(m_pSearch, "выделенная строка", IT_SEARCH);
	m_pCaretSearch          = createTreeItem(m_pSearch, "каретка",           IT_SEARCH);
	m_pSelectionSearch      = createTreeItem(m_pSearch, "выделение",         IT_SEARCH);
	m_pBookmarkSearch       = createTreeItem(m_pSearch, "закладка",          IT_SEARCH);

	m_pAxis   = createTreeItem(m_pChart, "ось",       IT_CHART);
	m_pTitle  = createTreeItem(m_pChart, "заголовок", IT_CHART);
	m_pLegend = createTreeItem(m_pChart, "легенда",   IT_CHART);
	m_pGraph  = createTreeItem(m_pChart, "график",    IT_CHART);
	m_pTime   = createTreeItem(m_pChart, "время",     IT_CHART);

	m_pEdgingColor     = createTreeItem(m_pAnimation, "цвет окантовки",               IT_ANIMATION);
	m_pBackgroundColor = createTreeItem(m_pAnimation, "цвет фона за пределами кадра", IT_ANIMATION);
}

PTR(QTreeWidgetItem) ViewPreferences::createTreeItem(PTR(QTreeWidgetItem) parent, CREF(QString) name, ItemType itemType)
{
	PTR(QTreeWidgetItem) item = new QTreeWidgetItem(parent);
	item->setText(0, name);
	item->setData(0, Qt::UserRole, QVariant(itemType));
	return item;
}

void ViewPreferences::onTreeWidgetItemActivated(QTreeWidgetItem* item, int column)
{
	stackedWidget->setCurrentIndex(item->data(column, Qt::UserRole).toInt());
	if(item->data(column, Qt::UserRole).toInt() != 0)
	{
		switchPreviewComboBox->setCurrentIndex(item->data(column, Qt::UserRole).toInt() - 1);
	}
	previewStackedWidget->setCurrentIndex(item->data(column, Qt::UserRole).toInt() - 1);
}

void ViewPreferences::onSwitchPreviewComboBox(int index)
{
	previewStackedWidget->setCurrentIndex(switchPreviewComboBox->itemData(index, Qt::UserRole).toInt() - 1);
}