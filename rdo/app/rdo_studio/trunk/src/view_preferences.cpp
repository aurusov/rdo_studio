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
#include <boost/foreach.hpp>
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

rbool ViewPreferences::null_wordwrap      = false;
rbool ViewPreferences::null_horzscrollbar = true;
rbool ViewPreferences::null_warning       = true;
rbool ViewPreferences::null_commentfold   = false;
RDOBookmarkStyle ViewPreferences::null_bookmarkstyle = RDOBOOKMARKS_NONE;
RDOFoldStyle     ViewPreferences::null_foldstyle     = RDOFOLDS_NONE;

ViewPreferences::ViewPreferences(PTR(QWidget) pParent)
	: QDialog(pParent)
	, all_font_size(-1)
	, all_font_name("")
	, null_font_style(rdoStyle::RDOStyleFont::NONE)
{
	setupUi(this);

	connect(buttonOk, SIGNAL(clicked()), this, SLOT(onOkButton()));
	connect(buttonCancel, SIGNAL(clicked()), this, SLOT(onCancelButton()));
	connect(buttonApply, SIGNAL(clicked()), this, SLOT(onApplyButton()));
	
	m_setup           = studioApp.getFileAssociationSetup();
	m_checkInFuture   = studioApp.getFileAssociationCheckInFuture();
	m_openLastProject = studioApp.getOpenLastProject();
	m_showFullName    = studioApp.getShowCaptionFullName();

	style_editor.init();
	style_build.init();
	style_debug.init();
	style_trace.init();
	style_results.init();
	style_find.init();

	style_editor  = studioApp.getStyle()->style_editor;
	style_build   = studioApp.getStyle()->style_build;
	style_debug   = studioApp.getStyle()->style_debug;
	style_trace   = studioApp.getStyle()->style_trace;
	style_results = studioApp.getStyle()->style_results;
	style_find    = studioApp.getStyle()->style_find;

	fontComboBox->setEditable(false);
	//Вкладка "Основные"
	connect(setupCheckBox, SIGNAL(stateChanged(int)), this, SLOT(onSetup(int)));
	connect(checkInFutureCheckBox, SIGNAL(stateChanged(int)), this, SLOT(onCheckInFuture(int)));
	connect(openLastProjectCheckBox, SIGNAL(stateChanged(int)), this, SLOT(onOpenLastProject(int)));
	connect(showFullNameCheckBox, SIGNAL(stateChanged(int)), this, SLOT(onShowFullName(int)));
	//Вкладка "Редактор"
	connect(checkBoxCodeCompUse, SIGNAL(stateChanged(int)), this, SLOT(onCodeCompUse(int)));
	connect(radioButtonFullList, SIGNAL(toggled(bool)), this, SLOT(onCodeCompShowFullList(bool)));
	connect(radioButtonNearestWords, SIGNAL(toggled(bool)), this, SLOT(onCodeCompShowFullList(bool)));
	connect(checkBoxMarginFold, SIGNAL(stateChanged(int)), this, SLOT(onMarginFold(int)));
	connect(checkBoxMarginBookmark, SIGNAL(stateChanged(int)), this, SLOT(onMarginBookmark(int)));
	connect(checkBoxMarginLineNum, SIGNAL(stateChanged(int)), this, SLOT(onMarginLineNumber(int)));
	//Вкладка "Табуляция"
	tabSizeLineEdit->setValidator(new QIntValidator(1, 100, this));
	tabSizeLineEdit->setText(QString::number(style_editor.tab->tabSize));
	indentSizeLineEdit->setValidator(new QIntValidator(1, 100, this));
	indentSizeLineEdit->setText(QString::number(style_editor.tab->indentSize));
	connect(useTabSymbolCheckBox, SIGNAL(stateChanged(int)), this, SLOT(onUseTabSymbol(int)));
	connect(indentAsTabcheckBox, SIGNAL(stateChanged(int)), this, SLOT(onIndentAsTab(int)));
	connect(autoIndentCheckBox, SIGNAL(stateChanged(int)), this, SLOT(onAutoIndent(int)));
	connect(eraseWithTabRadioButton, SIGNAL(toggled(bool)), this, SLOT(onEraseWithTab(bool)));
	connect(tabSizeLineEdit, SIGNAL(textEdited(const QString&)), this, SLOT(onTabSize(const QString&)));
	connect(indentSizeLineEdit, SIGNAL(textEdited(const QString&)), this, SLOT(onIndentSize(const QString&)));
	//Вкладка "Стиль и цвет"
	stackedWidget->setCurrentWidget(pageRoot);
	createTree();
	createPreview();
	createStyles();

	switchPreviewComboBox->addItem("Editor",  IT_EDITOR);
	switchPreviewComboBox->addItem("Build",   IT_BUILD);
	switchPreviewComboBox->addItem("Debug",   IT_DEBUG);
	switchPreviewComboBox->addItem("Tracer",  IT_LOG);
	switchPreviewComboBox->addItem("Results", IT_RESULT);
	switchPreviewComboBox->addItem("Find",    IT_FIND);
	switchPreviewComboBox->addItem("Chart",   IT_CHART);
	switchPreviewComboBox->addItem("Frame",   IT_FRAME);

	bookmarkComboBox->addItem(QString::fromStdWString(L"Нет"),           RDOBOOKMARKS_NONE);
	bookmarkComboBox->addItem(QString::fromStdWString(L"Круг"),          RDOBOOKMARKS_CIRCLE);
	bookmarkComboBox->addItem(QString::fromStdWString(L"Прямоугольник"), RDOBOOKMARKS_RECT);
	bookmarkComboBox->addItem(QString::fromStdWString(L"Овал"),          RDOBOOKMARKS_ROUNDRECT);
	bookmarkComboBox->addItem(QString::fromStdWString(L"Стрелка"),       RDOBOOKMARKS_ARROW);

	foldComboBox->addItem(QString::fromStdWString(L"Нет"),             RDOFOLDS_NONE);
	foldComboBox->addItem(QString::fromStdWString(L"Плюс"),            RDOFOLDS_PLUS);
	foldComboBox->addItem(QString::fromStdWString(L"Плюс + линия"),    RDOFOLDS_PLUSCONNECTED);
	foldComboBox->addItem(QString::fromStdWString(L"Стрелка"),         RDOFOLDS_ARROW);
	foldComboBox->addItem(QString::fromStdWString(L"Стрелка + линия"), RDOFOLDS_ARROWCONNECTED);
	foldComboBox->addItem(QString::fromStdWString(L"Квадрат + линия"), RDOFOLDS_BOXCONNECTED);
	foldComboBox->addItem(QString::fromStdWString(L"Круг + линия"),    RDOFOLDS_CIRCLECONNECTED);

	connect(treeWidget, SIGNAL(itemClicked(QTreeWidgetItem*, int)), this, SLOT(onTreeWidgetItemActivated(QTreeWidgetItem*, int)));
	connect(switchPreviewComboBox, SIGNAL(currentIndexChanged(int)), this, SLOT(onSwitchPreviewComboBox(int)));
	connect(fontSizeComboBox, SIGNAL(activated(int)), this, SLOT(onFontSize(int)));
	connect(fontComboBox, SIGNAL(activated(int)), this, SLOT(onFontType(int)));
	connect(boldCheckBox, SIGNAL(stateChanged(int)), this, SLOT(onFontBold(int)));
	connect(italicCheckBox, SIGNAL(stateChanged(int)), this, SLOT(onFontItalic(int)));
	connect(underlineCheckBox, SIGNAL(stateChanged(int)), this, SLOT(onFontUnderline(int)));
	connect(horzScrollEditorCheckBox, SIGNAL(stateChanged(int)), this, SLOT(onHorzScroll(int)));
	connect(horzScrollBuildCheckBox, SIGNAL(stateChanged(int)), this, SLOT(onHorzScroll(int)));
	connect(horzScrollDebugCheckBox, SIGNAL(stateChanged(int)), this, SLOT(onHorzScroll(int)));
	connect(horzScrollResultsCheckBox, SIGNAL(stateChanged(int)), this, SLOT(onHorzScroll(int)));
	connect(horzScrollFindCheckBox, SIGNAL(stateChanged(int)), this, SLOT(onHorzScroll(int)));
	connect(wordWrapEditorCheckBox, SIGNAL(stateChanged(int)), this, SLOT(onWordWrap(int)));
	connect(wordWrapBuildCheckBox, SIGNAL(stateChanged(int)), this, SLOT(onWordWrap(int)));
	connect(wordWrapDebugCheckBox, SIGNAL(stateChanged(int)), this, SLOT(onWordWrap(int)));
	connect(wordWrapResultsCheckBox, SIGNAL(stateChanged(int)), this, SLOT(onWordWrap(int)));
	connect(wordWrapFindCheckBox, SIGNAL(stateChanged(int)), this, SLOT(onWordWrap(int)));
	connect(bookmarkComboBox, SIGNAL(activated(int)), this, SLOT(onBookmark(int)));
	connect(foldComboBox, SIGNAL(activated(int)), this, SLOT(onFold(int)));


	updateDialog();
}

void ViewPreferences::onOkButton()
{
	apply();
	done(Accepted);
}

void ViewPreferences::onCancelButton()
{
	done(Rejected);
}

void ViewPreferences::onApplyButton()
{
	apply();
}

void ViewPreferences::onCodeCompUse(int state)
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

void ViewPreferences::onCodeCompShowFullList(bool state)
{
	UNUSED(state);

	if(radioButtonFullList->isChecked())
		style_editor.autoComplete->showFullList = true;
	if(radioButtonNearestWords->isChecked())
		style_editor.autoComplete->showFullList = false;
}

void ViewPreferences::onMarginFold(int state)
{
	style_editor.margin->fold = state;
}

void ViewPreferences::onMarginBookmark(int state)
{
	style_editor.margin->bookmark = state;
}

void ViewPreferences::onMarginLineNumber(int state)
{
	style_editor.margin->lineNumber = state;
}

void ViewPreferences::onUseTabSymbol(int state)
{
	style_editor.tab->backspaceUntabs = state;
}

void ViewPreferences::onIndentAsTab(int state)
{
	style_editor.tab->tabIndents = state;
}

void ViewPreferences::onAutoIndent(int state)
{
	style_editor.tab->autoIndent = state;
}

void ViewPreferences::onEraseWithTab(bool state)
{
	UNUSED(state);

	if(eraseWithTabRadioButton->isChecked())
		style_editor.tab->useTabs = true;
	if(eraseWithIndentRadioButton->isChecked())
		style_editor.tab->useTabs = false;
}

void ViewPreferences::onTabSize(const QString& text)
{
	style_editor.tab->tabSize = text.toInt();
}

void ViewPreferences::onIndentSize(const QString& text)
{
	style_editor.tab->indentSize = text.toInt();
}

void ViewPreferences::onSetup(int state)
{
	m_setup = state;
}

void ViewPreferences::onCheckInFuture(int state)
{
	m_checkInFuture = state;
}

void ViewPreferences::onOpenLastProject(int state)
{
	m_openLastProject = state;
}

void ViewPreferences::onShowFullName(int state)
{
	m_showFullName = state;
}

void ViewPreferences::onTreeWidgetItemActivated(QTreeWidgetItem* item, int column)
{
	stackedWidget->setCurrentIndex(item->data(column, Qt::UserRole).toInt());
	if(item->data(column, Qt::UserRole).toInt() != 0)
	{
		switchPreviewComboBox->setCurrentIndex(item->data(column, Qt::UserRole).toInt() - 1);
	}
	previewStackedWidget->setCurrentIndex(getStyleItem()->type - 1);
}

void ViewPreferences::onSwitchPreviewComboBox(int index)
{
	previewStackedWidget->setCurrentIndex(switchPreviewComboBox->itemData(index, Qt::UserRole).toInt() - 1);
}

void ViewPreferences::onFontSize(int index)
{
	UNUSED(index);
	int size = fontSizeComboBox->currentText().toInt();
	switch(getStyleItem()->type)
	{
	case IT_ROOT:
		all_font_size = size;
		style_editor.font->size  = size;
		style_build.font->size   = size;
		style_debug.font->size   = size;
		style_trace.font->size   = size;
		style_results.font->size = size;
		style_find.font->size    = size;
		//style_chart.font->size   = size;
		//style_frame.font->size   = size;
		break;
	case IT_EDITOR:
		style_editor.font->size = size;
		break;
	case IT_BUILD:
		style_build.font->size = size;
		break;
	case IT_DEBUG:
		style_debug.font->size   = size;
		break;
	case IT_LOG:
		style_trace.font->size = size;
		break;
	case IT_RESULT:
		style_results.font->size = size;
		break;
	case IT_FIND:
		style_find.font->size    = size;
		break;
	case IT_CHART:
		//style_chart.font->size   = size;
		break;
	case IT_FRAME:
		//style_frame.font->size   = size;
		break;
	}

	updatePreview();
}

void ViewPreferences::onFontType(int index)
{
	UNUSED(index);

	QString name = fontComboBox->currentFont().rawName();

	switch(getStyleItem()->type)
	{
	case IT_ROOT:
		all_font_name = name.toStdString();
		style_editor.font->name  = name.toStdString();
		style_build.font->name   = name.toStdString();
		style_debug.font->name   = name.toStdString();
		style_trace.font->name   = name.toStdString();
		style_results.font->name = name.toStdString();
		style_find.font->name    = name.toStdString();
		//style_chart.font->name   = name.toStdString();
		//style_frame.font->name   = name.toStdString();
		break;
	case IT_EDITOR:
		style_editor.font->name = name.toStdString();
		break;
	case IT_BUILD:
		style_build.font->name = name.toStdString();
		break;
	case IT_DEBUG:
		style_debug.font->name   = name.toStdString();
		break;
	case IT_LOG:
		style_trace.font->name = name.toStdString();
		break;
	case IT_RESULT:
		style_results.font->name = name.toStdString();
		break;
	case IT_FIND:
		style_find.font->name    = name.toStdString();
		break;
	case IT_CHART:
		//style_chart.font->name   = name.toStdString();
		break;
	case IT_FRAME:
		//style_frame.font->name   = name.toStdString();
		break;
	}

	updatePreview();
}

void ViewPreferences::onFontBold(int state)
{
		updatePreview();
}

void ViewPreferences::onFontItalic(int state)
{
	updatePreview();
}

void ViewPreferences::onFontUnderline(int state)
{
	updatePreview();
}

void ViewPreferences::onHorzScroll(int state)
{
	PTR(StyleItem) item = getStyleItem();
	item->horzscrollbar = state;
	updatePreview();
}

void ViewPreferences::onWordWrap(int state)
{
	PTR(StyleItem) item = getStyleItem();
	item->wordwrap = state;
		
	switch(item->type)
	{
	case IT_EDITOR:
		horzScrollEditorCheckBox->setEnabled(state ? false : true);
		break;
	case IT_BUILD:
		horzScrollBuildCheckBox->setEnabled(state ? false : true);
		break;
	case IT_DEBUG:
		horzScrollDebugCheckBox->setEnabled(state ? false : true);
		break;
	case IT_RESULT:
		horzScrollResultsCheckBox->setEnabled(state ? false : true);
		break;
	case IT_FIND:
		horzScrollFindCheckBox->setEnabled(state ? false : true);
		break;
	}
	if(state)
	{
		item->horzscrollbar = false;
	}
	else
	{
		switch(item->type)
		{
		case IT_EDITOR:
			item->horzscrollbar = horzScrollEditorCheckBox->checkState();
			break;
		case IT_BUILD:
			item->horzscrollbar = horzScrollBuildCheckBox->checkState();
			break;
		case IT_DEBUG:
			item->horzscrollbar = horzScrollDebugCheckBox->checkState();
			break;
		case IT_RESULT:
			item->horzscrollbar = horzScrollResultsCheckBox->checkState();
			break;
		case IT_FIND:
			item->horzscrollbar = horzScrollFindCheckBox->checkState();
			break;
		}
	}
	updatePreview();
}

void ViewPreferences::onBookmark(int index)
{
	PTR(StyleItem) item = getStyleItem();
	item->bookmarkstyle = static_cast<RDOBookmarkStyle>(index);
	updatePreview();
}

void ViewPreferences::onFold(int index)
{
	PTR(StyleItem) item = getStyleItem();
	item->foldstyle = static_cast<RDOFoldStyle>(index);
	updatePreview();
}

void ViewPreferences::updateDialog()
{
	setupCheckBox->setCheckState(m_setup
		? Qt::Checked
		: Qt::Unchecked
		);
	checkInFutureCheckBox->setCheckState(m_checkInFuture
		? Qt::Checked
		: Qt::Unchecked
		);
	openLastProjectCheckBox->setCheckState(m_openLastProject
		? Qt::Checked
		: Qt::Unchecked
		);
	showFullNameCheckBox->setCheckState(m_showFullName
		? Qt::Checked
		: Qt::Unchecked
		);
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
	useTabSymbolCheckBox->setCheckState(style_editor.tab->backspaceUntabs
		? Qt::Checked
		: Qt::Unchecked
		);
	indentAsTabcheckBox->setCheckState(style_editor.tab->tabIndents
		? Qt::Checked
		: Qt::Unchecked
		);
	autoIndentCheckBox->setCheckState(style_editor.tab->autoIndent
		? Qt::Checked
		: Qt::Unchecked
		);
	style_editor.tab->useTabs
		? eraseWithTabRadioButton->toggle()
		: eraseWithIndentRadioButton->toggle();	
}

void ViewPreferences::updatePreview()
{
	preview_editor->setEditorStyle(&style_editor);
	preview_editor->repaint();

	preview_build->setEditorStyle(&style_build);
	preview_build->repaint();

	preview_debug->setEditorStyle(&style_debug);
	preview_debug->repaint();

	//preview_trace->view().setStyle(&style_trace);
	//preview_trace->repaint();

	preview_results->setEditorStyle(&style_results);
	preview_results->repaint();

	preview_find->setEditorStyle(&style_find);
	preview_find->repaint();
}

void ViewPreferences::createPreview()
{
	preview_editor = new RDOEditorEdit(previewStackedWidget->currentWidget(), previewStackedWidget->currentWidget());
	ASSERT(preview_editor);
	preview_editor->setEditorStyle(&style_editor);
	preview_editor->setCanClearErrorLine(false);
	preview_editor->appendText(QString::fromLocal8Bit("{ comments }\n$Pattern pattern_name : operation trace\n$Relevant_resources\n  rel_res2  : res_type2     Keep    Keep\n  rel_res1  : res_type1     Create  NoChange\n$Time = Abs(rel_res2.par1 - rel_res2.par3)\n{...}\n$End\n\ntext [ 10, 20, ... = 'text' ]\n\n$Re levant_resources"));
	preview_editor->scrollToLine(0);
	preview_editor->setReadOnly(true);
	preview_editor->bookmarkToggle();
	preview_editor->setErrorLine(preview_editor->getLineCount() - 1);
	previewStackedWidget->addWidget(preview_editor);

	preview_build = new RDOBuildEdit(previewStackedWidget->currentWidget());
	preview_build->setEditorStyle(&style_build);
	preview_build->appendLine(new BuildEditLineInfo(rdo::format(IDS_COLORSTYLE_BUILD_SAMPLE1)));
	preview_build->appendLine(new BuildEditLineInfo(rdo::simulation::report::FileMessage(rdo::format(IDS_COLORSTYLE_BUILD_SAMPLE2), rdoModelObjects::PAT, 40, 0)));
	preview_build->appendLine(new BuildEditLineInfo(rdo::format(IDS_COLORSTYLE_BUILD_SAMPLE3)));
	preview_build->gotoNext();
	previewStackedWidget->addWidget(preview_build);

	preview_debug = new RDODebugEdit(previewStackedWidget->currentWidget());
	preview_debug->setEditorStyle(&style_debug);
	preview_debug->appendLine(QString::fromLocal8Bit("Получение структуры модели...ok\nМодель запущена"));
	previewStackedWidget->addWidget(preview_debug);

	preview_trace = new LogMainWnd(previewStackedWidget->currentWidget());
	preview_trace->view().setStyle(&studioApp.getStyle()->style_trace);
	preview_trace->view().setFocusOnly(true);
	preview_trace->view().setText(rdo::format(IDS_COLORSTYLE_LOG_SAMPLE));
	preview_trace->view().selectLine(0);
	previewStackedWidget->addWidget(preview_trace);

	preview_results = new RDOEditorResults(previewStackedWidget->currentWidget());
	preview_results->setEditorStyle(&style_results);
	preview_results->setReadOnly(false);
	preview_results->replaceCurrent(rdo::format(IDS_COLORSTYLE_RESULTS_SAMPLE), 0);
	preview_results->setReadOnly(true);
	previewStackedWidget->addWidget(preview_results);

	preview_find = new RDOFindEdit(previewStackedWidget->currentWidget());
	preview_find->setEditorStyle(&style_find);
	preview_find->setKeyword("$Time");
	preview_find->appendLine(new LogEditLineInfo(rdo::format(IDS_COLORSTYLE_FIND_SAMPLE1)));
	preview_find->appendLine(new LogEditLineInfo(rdo::simulation::report::FileMessage(rdo::format(IDS_COLORSTYLE_FIND_SAMPLE2), rdoModelObjects::PAT, 3, 0)));
	preview_find->appendLine(new LogEditLineInfo(rdo::simulation::report::FileMessage(rdo::format(IDS_COLORSTYLE_FIND_SAMPLE3), rdoModelObjects::PAT, 13, 0)));
	preview_find->appendLine(new LogEditLineInfo(rdo::format(IDS_COLORSTYLE_FIND_SAMPLE4)));
	preview_find->gotoNext();
	previewStackedWidget->addWidget(preview_find);
}

void ViewPreferences::createStyles()
{
	StyleItem* item;
	item = new StyleItem(IT_ROOT, all_font_size, all_font_name);
	item->properties.push_back(new StyleProperty(item, IT_ROOT, null_font_style));
	style_list.push_back(item);

	RDOEditorEditTheme* editor_theme = static_cast<RDOEditorEditTheme*>(style_editor.theme);
	item = new StyleItem(IT_EDITOR, style_editor.font->size, style_editor.font->name, style_editor.window->wordWrap, style_editor.window->showHorzScrollBar, editor_theme->bookmarkStyle, editor_theme->foldStyle, editor_theme->commentFold);
	item->properties.push_back(new StyleProperty(item, IT_EDITOR, editor_theme->identifierStyle));
	item->properties.push_back(new StyleProperty(item, IT_EDITOR_PLAINTEXT, editor_theme->defaultStyle));
	item->properties.push_back(new StyleProperty(item, IT_EDITOR_IDENTIFICATOR, editor_theme->identifierStyle));
	item->properties.push_back(new StyleProperty(item, IT_EDITOR_KEYWORD, editor_theme->keywordStyle));
	item->properties.push_back(new StyleProperty(item, IT_EDITOR_FUNCTION, editor_theme->functionsStyle));
	item->properties.push_back(new StyleProperty(item, IT_EDITOR_TRACE, editor_theme->traceStyle));
	item->properties.push_back(new StyleProperty(item, IT_EDITOR_COLOR, editor_theme->colorStyle));
	item->properties.push_back(new StyleProperty(item, IT_EDITOR_COMMENT, editor_theme->commentStyle));
	item->properties.push_back(new StyleProperty(item, IT_EDITOR_NUMBER, editor_theme->numberStyle));
	item->properties.push_back(new StyleProperty(item, IT_EDITOR_STRING, editor_theme->stringStyle));
	item->properties.push_back(new StyleProperty(item, IT_EDITOR_OPERATOR, editor_theme->operatorStyle));
	//item->properties.push_back(new StyleProperty(item, IT_EDITOR_CARET, editor_theme->caretColor));
	//item->properties.push_back(new StyleProperty(item, IT_EDITOR_TEXTSELECTION, null_font_style));
	//item->properties.push_back(new StyleProperty(item, IT_EDITOR_BOOKMARK, null_font_style));
	//item->properties.push_back(new StyleProperty(item, IT_EDITOR_FOLD, null_font_style));
	//item->properties.push_back(new StyleProperty(item, IT_EDITOR_ERROR, null_font_style));
	style_list.push_back(item);

	RDOBuildEditTheme* build_theme = static_cast<RDOBuildEditTheme*>(style_build.theme);
	item = new StyleItem(IT_BUILD, style_build.font->size, style_build.font->name, style_build.window->wordWrap, style_build.window->showHorzScrollBar, null_bookmarkstyle, null_foldstyle, null_commentfold, build_theme->warning);
	item->properties.push_back(new StyleProperty(item, IT_BUILD, build_theme->defaultStyle));
	item->properties.push_back(new StyleProperty(item, IT_BUILD_TEXT, build_theme->defaultStyle));
	item->properties.push_back(new StyleProperty(item, IT_BUILD_SELECTEDLINE, null_font_style));
	item->properties.push_back(new StyleProperty(item, IT_EDITOR_CARET, null_font_style));
	item->properties.push_back(new StyleProperty(item, IT_EDITOR_TEXTSELECTION, null_font_style));
	item->properties.push_back(new StyleProperty(item, IT_EDITOR_BOOKMARK, null_font_style));
	style_list.push_back(item);

	RDOBaseEditTheme* debug_theme = static_cast<RDOBaseEditTheme*>(style_debug.theme);
	item = new StyleItem(IT_DEBUG, style_debug.font->size, style_debug.font->name, style_debug.window->wordWrap, style_debug.window->showHorzScrollBar);
	item->properties.push_back(new StyleProperty(item, IT_DEBUG, debug_theme->defaultStyle));
	item->properties.push_back(new StyleProperty(item, IT_BUILD_TEXT, debug_theme->defaultStyle));
	item->properties.push_back(new StyleProperty(item, IT_EDITOR_CARET, null_font_style));
	item->properties.push_back(new StyleProperty(item, IT_EDITOR_TEXTSELECTION, null_font_style));
	item->properties.push_back(new StyleProperty(item, IT_EDITOR_BOOKMARK, null_font_style));
	style_list.push_back(item);

	LogTheme* trace_theme = style_trace.theme;
	item = new StyleItem(IT_LOG, style_trace.font->size, style_trace.font->name);
	item->properties.push_back(new StyleProperty(item, IT_LOG, trace_theme->style));
	item->properties.push_back(new StyleProperty(item, IT_LOG_ES, trace_theme->style));
	item->properties.push_back(new StyleProperty(item, IT_LOG_EB, trace_theme->style));
	item->properties.push_back(new StyleProperty(item, IT_LOG_EF, trace_theme->style));
	item->properties.push_back(new StyleProperty(item, IT_LOG_EI, trace_theme->style));
	item->properties.push_back(new StyleProperty(item, IT_LOG_ER, trace_theme->style));
	item->properties.push_back(new StyleProperty(item, IT_LOG_RC, trace_theme->style));
	item->properties.push_back(new StyleProperty(item, IT_LOG_RE, trace_theme->style));
	item->properties.push_back(new StyleProperty(item, IT_LOG_RK, trace_theme->style));
	item->properties.push_back(new StyleProperty(item, IT_LOG_V, trace_theme->style));
	item->properties.push_back(new StyleProperty(item, IT_LOG_STATUS, trace_theme->style));
	item->properties.push_back(new StyleProperty(item, IT_LOG_DPS, trace_theme->style));
	item->properties.push_back(new StyleProperty(item, IT_LOG_SB, trace_theme->style));
	item->properties.push_back(new StyleProperty(item, IT_LOG_SO, trace_theme->style));
	item->properties.push_back(new StyleProperty(item, IT_LOG_STN, trace_theme->style));
	item->properties.push_back(new StyleProperty(item, IT_LOG_STD, trace_theme->style));
	item->properties.push_back(new StyleProperty(item, IT_LOG_STR, trace_theme->style));
	item->properties.push_back(new StyleProperty(item, IT_LOG_SRC, trace_theme->style));
	item->properties.push_back(new StyleProperty(item, IT_LOG_SRE, trace_theme->style));
	item->properties.push_back(new StyleProperty(item, IT_LOG_SRK, trace_theme->style));
	item->properties.push_back(new StyleProperty(item, IT_LOG_SD, trace_theme->style));
	item->properties.push_back(new StyleProperty(item, IT_LOG_SES, trace_theme->style));
	item->properties.push_back(new StyleProperty(item, IT_LOG_SEN, trace_theme->style));
	item->properties.push_back(new StyleProperty(item, IT_LOG_SEM, trace_theme->style));
	item->properties.push_back(new StyleProperty(item, IT_LOG_SEF, trace_theme->style));
	item->properties.push_back(new StyleProperty(item, IT_LOG_SEU, trace_theme->style));
	style_list.push_back(item);

	RDOEditorBaseEditTheme* results_theme = static_cast<RDOEditorBaseEditTheme*>(style_results.theme);
	item = new StyleItem(IT_RESULT, style_results.font->size, style_results.font->name, style_results.window->wordWrap, style_results.window->showHorzScrollBar);
	item->properties.push_back(new StyleProperty(item, IT_RESULT, results_theme->identifierStyle));
	item->properties.push_back(new StyleProperty(item, IT_EDITOR_PLAINTEXT, results_theme->defaultStyle));
	item->properties.push_back(new StyleProperty(item, IT_EDITOR_IDENTIFICATOR, results_theme->identifierStyle));
	item->properties.push_back(new StyleProperty(item, IT_EDITOR_KEYWORD, results_theme->keywordStyle));
	item->properties.push_back(new StyleProperty(item, IT_EDITOR_NUMBER, results_theme->numberStyle));
	item->properties.push_back(new StyleProperty(item, IT_EDITOR_STRING, results_theme->stringStyle));
	item->properties.push_back(new StyleProperty(item, IT_EDITOR_OPERATOR, results_theme->operatorStyle));
	item->properties.push_back(new StyleProperty(item, IT_EDITOR_CARET, null_font_style));
	item->properties.push_back(new StyleProperty(item, IT_EDITOR_TEXTSELECTION, null_font_style));
	item->properties.push_back(new StyleProperty(item, IT_EDITOR_BOOKMARK, null_font_style));
	style_list.push_back(item);

	RDOFindEditTheme* find_theme = static_cast<RDOFindEditTheme*>(style_find.theme);
	item = new StyleItem(IT_FIND, style_find.font->size, style_find.font->name, style_find.window->wordWrap, style_find.window->showHorzScrollBar);
	item->properties.push_back(new StyleProperty(item, IT_FIND, find_theme->defaultStyle));
	item->properties.push_back(new StyleProperty(item, IT_BUILD_TEXT, find_theme->defaultStyle));
	item->properties.push_back(new StyleProperty(item, IT_FIND_SEARCHTEXT, results_theme->identifierStyle));
	item->properties.push_back(new StyleProperty(item, IT_BUILD_SELECTEDLINE, null_font_style));
	item->properties.push_back(new StyleProperty(item, IT_EDITOR_CARET, null_font_style));
	item->properties.push_back(new StyleProperty(item, IT_EDITOR_TEXTSELECTION, null_font_style));
	item->properties.push_back(new StyleProperty(item, IT_EDITOR_BOOKMARK, null_font_style));
	style_list.push_back(item);

}

void ViewPreferences::createTree()
{
	treeWidget->setColumnCount(1);
	treeWidget->setHeaderHidden(true);
	treeWidget->setRootIsDecorated(false);

	m_pRoot = new QTreeWidgetItem(treeWidget);
	m_pRoot->setText(0, QString::fromStdWString(L"Все окна"));
	m_pRoot->setData(0, Qt::UserRole, IT_ROOT);

	m_pText      = createTreeItem(m_pRoot, QString::fromStdWString(L"Исходный текст"),   IT_EDITOR);
	m_pCompile   = createTreeItem(m_pRoot, QString::fromStdWString(L"Окно компиляции"),  IT_BUILD);
	m_pDebug     = createTreeItem(m_pRoot, QString::fromStdWString(L"Окно отладки"),     IT_DEBUG);
	m_pTrace     = createTreeItem(m_pRoot, QString::fromStdWString(L"Окно трассировки"), IT_LOG);
	m_pResult    = createTreeItem(m_pRoot, QString::fromStdWString(L"Окно результатов"), IT_RESULT);
	m_pSearch    = createTreeItem(m_pRoot, QString::fromStdWString(L"Окно поиска"),      IT_FIND);
	m_pChart     = createTreeItem(m_pRoot, QString::fromStdWString(L"Окно графиков"),    IT_CHART);
	m_pAnimation = createTreeItem(m_pRoot, QString::fromStdWString(L"Окно анимации"),    IT_FRAME);

	m_pRoot->setExpanded(true);

	m_pPlainText = createTreeItem(m_pText, QString::fromStdWString(L"Обыкновенный текст"), IT_EDITOR_PLAINTEXT);
	m_pVariable  = createTreeItem(m_pText, QString::fromStdWString(L"Переменная"),         IT_EDITOR_IDENTIFICATOR);
	m_pKeyword   = createTreeItem(m_pText, QString::fromStdWString(L"Ключевое слово"),     IT_EDITOR_KEYWORD);
	m_pFunction  = createTreeItem(m_pText, QString::fromStdWString(L"Функция"),            IT_EDITOR_FUNCTION);
	m_pTraceText = createTreeItem(m_pText, QString::fromStdWString(L"Трассировка"),        IT_EDITOR_TRACE);
	m_pColor     = createTreeItem(m_pText, QString::fromStdWString(L"Цвет"),               IT_EDITOR_COLOR);
	m_pComment   = createTreeItem(m_pText, QString::fromStdWString(L"Комментарии"),        IT_EDITOR_COMMENT);
	m_pNumber    = createTreeItem(m_pText, QString::fromStdWString(L"Число"),              IT_EDITOR_NUMBER);
	m_pString    = createTreeItem(m_pText, QString::fromStdWString(L"Строка"),             IT_EDITOR_STRING);
	m_pOperator  = createTreeItem(m_pText, QString::fromStdWString(L"Оператор"),           IT_EDITOR_OPERATOR);
	m_pCaret     = createTreeItem(m_pText, QString::fromStdWString(L"Каретка"),            IT_EDITOR_CARET);
	m_pSelection = createTreeItem(m_pText, QString::fromStdWString(L"Выделение"),          IT_EDITOR_TEXTSELECTION);
	m_pBookmark  = createTreeItem(m_pText, QString::fromStdWString(L"Закладка"),           IT_EDITOR_BOOKMARK);
	m_pGroup     = createTreeItem(m_pText, QString::fromStdWString(L"Группа"),             IT_EDITOR_FOLD);
	m_pError     = createTreeItem(m_pText, QString::fromStdWString(L"Ошибка"),             IT_EDITOR_ERROR);

	m_pTextCompile      = createTreeItem(m_pCompile, QString::fromStdWString(L"Текст"),             IT_BUILD_TEXT);
	m_pSelectedString   = createTreeItem(m_pCompile, QString::fromStdWString(L"Выделенная строка"), IT_BUILD_SELECTEDLINE);
	m_pCaretCompile     = createTreeItem(m_pCompile, QString::fromStdWString(L"Каретка"),           IT_EDITOR_CARET);
	m_pSelectionCompile = createTreeItem(m_pCompile, QString::fromStdWString(L"Выделение"),         IT_EDITOR_TEXTSELECTION);
	m_pBookmarkCompile  = createTreeItem(m_pCompile, QString::fromStdWString(L"Закладка"),          IT_EDITOR_BOOKMARK);

	m_pTextDebug      = createTreeItem(m_pDebug, QString::fromStdWString(L"Текст"),     IT_BUILD_TEXT);
	m_pCaretDebug     = createTreeItem(m_pDebug, QString::fromStdWString(L"Каретка"),   IT_EDITOR_CARET);
	m_pSelectionDebug = createTreeItem(m_pDebug, QString::fromStdWString(L"Выделение"), IT_EDITOR_TEXTSELECTION);
	m_pBookmarkDebug  = createTreeItem(m_pDebug, QString::fromStdWString(L"Закладка"),  IT_EDITOR_BOOKMARK);

	m_pES     = createTreeItem(m_pTrace, QString::fromStdWString(L"Служебное событие (ES)"),                         IT_LOG_ES);
	m_pEB     = createTreeItem(m_pTrace, QString::fromStdWString(L"Начало действия (EB)"),                           IT_LOG_EB);
	m_pEF     = createTreeItem(m_pTrace, QString::fromStdWString(L"Окончание действия (EF)"),                        IT_LOG_EF);
	m_pEI     = createTreeItem(m_pTrace, QString::fromStdWString(L"Нерегулярное событие (EI)"),                      IT_LOG_EI);
	m_pER     = createTreeItem(m_pTrace, QString::fromStdWString(L"Продукционное правило (ER)"),                     IT_LOG_ER);
	m_pRC     = createTreeItem(m_pTrace, QString::fromStdWString(L"Создание ресурса (RC)"),                          IT_LOG_RC);
	m_pRE     = createTreeItem(m_pTrace, QString::fromStdWString(L"Удаление ресурса (RE)"),                          IT_LOG_RE);
	m_pRK     = createTreeItem(m_pTrace, QString::fromStdWString(L"Изменение состояния ресурса (RK)"),               IT_LOG_RK);
	m_pV      = createTreeItem(m_pTrace, QString::fromStdWString(L"Трассировка индекса (V)"),                        IT_LOG_V);
	m_pStatus = createTreeItem(m_pTrace, QString::fromStdWString(L"Статус окончания моделирования ($Status)"),       IT_LOG_STATUS);
	m_pDPS    = createTreeItem(m_pTrace, QString::fromStdWString(L"Статистика по поиску на графе (DPS)"),            IT_LOG_DPS);
	m_pSB     = createTreeItem(m_pTrace, QString::fromStdWString(L"Начало поиска (SB)"),                             IT_LOG_SB);
	m_pSO     = createTreeItem(m_pTrace, QString::fromStdWString(L"Трассировка раскрываемой вершины (SO)"),          IT_LOG_SO);
	m_pSTN    = createTreeItem(m_pTrace, QString::fromStdWString(L"Признак вершины (STN)"),                          IT_LOG_STN);
	m_pSTD    = createTreeItem(m_pTrace, QString::fromStdWString(L"Признак вершины (STD)"),                          IT_LOG_STD);
	m_pSTR    = createTreeItem(m_pTrace, QString::fromStdWString(L"Признак вершины (STR)"),                          IT_LOG_STR);
	m_pSRC    = createTreeItem(m_pTrace, QString::fromStdWString(L"Создание ресурса (при поиске) (SRC)"),            IT_LOG_SRC);
	m_pSRE    = createTreeItem(m_pTrace, QString::fromStdWString(L"Удаление ресурса (при поиске) (SRE)"),            IT_LOG_SRE);
	m_pSRK    = createTreeItem(m_pTrace, QString::fromStdWString(L"Изменение состояния ресурса (при поиске) (SRK)"), IT_LOG_SRK);
	m_pSD     = createTreeItem(m_pTrace, QString::fromStdWString(L"Трассировка решения (SD)"),                       IT_LOG_SD);
	m_pSES    = createTreeItem(m_pTrace, QString::fromStdWString(L"Завершение поиска (SES)"),                        IT_LOG_SES);
	m_pSEN    = createTreeItem(m_pTrace, QString::fromStdWString(L"Завершение поиска (SEN)"),                        IT_LOG_SEN);
	m_pSEM    = createTreeItem(m_pTrace, QString::fromStdWString(L"Завершение поиска (SEM)"),                        IT_LOG_SEM);
	m_pSEF    = createTreeItem(m_pTrace, QString::fromStdWString(L"Завершение поиска (SEF)"),                        IT_LOG_SEF);
	m_pSEU    = createTreeItem(m_pTrace, QString::fromStdWString(L"Завершение поиска (SEU)"),                        IT_LOG_SEU);

	m_pPlainTextResult = createTreeItem(m_pResult, QString::fromStdWString(L"Исходный текст"), IT_EDITOR_PLAINTEXT);
	m_pVariableResult  = createTreeItem(m_pResult, QString::fromStdWString(L"Переменная"),     IT_EDITOR_IDENTIFICATOR);
	m_pKeywordResult   = createTreeItem(m_pResult, QString::fromStdWString(L"Ключевое слово"), IT_EDITOR_KEYWORD);
	m_pNumberResult    = createTreeItem(m_pResult, QString::fromStdWString(L"Число"),          IT_EDITOR_NUMBER);
	m_pStringResult    = createTreeItem(m_pResult, QString::fromStdWString(L"Строка"),         IT_EDITOR_STRING);
	m_pOperatorResult  = createTreeItem(m_pResult, QString::fromStdWString(L"Оператор"),       IT_EDITOR_OPERATOR);
	m_pCaretResult     = createTreeItem(m_pResult, QString::fromStdWString(L"Каретка"),        IT_EDITOR_CARET);
	m_pSelectionResult = createTreeItem(m_pResult, QString::fromStdWString(L"Выделение"),      IT_EDITOR_TEXTSELECTION);
	m_pBookmarkResult  = createTreeItem(m_pResult, QString::fromStdWString(L"Закладка"),       IT_EDITOR_BOOKMARK);

	m_pTextSearch           = createTreeItem(m_pSearch, QString::fromStdWString(L"Текст"),             IT_BUILD_TEXT);
	m_pStringSearch         = createTreeItem(m_pSearch, QString::fromStdWString(L"Строка для поиска"), IT_FIND_SEARCHTEXT);
	m_pSelectedStringSearch = createTreeItem(m_pSearch, QString::fromStdWString(L"Выделенная строка"), IT_BUILD_SELECTEDLINE);
	m_pCaretSearch          = createTreeItem(m_pSearch, QString::fromStdWString(L"Каретка"),           IT_EDITOR_CARET);
	m_pSelectionSearch      = createTreeItem(m_pSearch, QString::fromStdWString(L"Выделение"),         IT_EDITOR_TEXTSELECTION);
	m_pBookmarkSearch       = createTreeItem(m_pSearch, QString::fromStdWString(L"Закладка"),          IT_EDITOR_BOOKMARK);

	m_pAxis   = createTreeItem(m_pChart, QString::fromStdWString(L"Ось"),       IT_CHART_AXIS);
	m_pTitle  = createTreeItem(m_pChart, QString::fromStdWString(L"Заголовок"), IT_CHART_TITLE);
	m_pLegend = createTreeItem(m_pChart, QString::fromStdWString(L"Легенда"),   IT_CHART_LEGEND);
	m_pGraph  = createTreeItem(m_pChart, QString::fromStdWString(L"График"),    IT_CHART_CHART);
	m_pTime   = createTreeItem(m_pChart, QString::fromStdWString(L"Время"),     IT_CHART_TIME);

	m_pEdgingColor     = createTreeItem(m_pAnimation, QString::fromStdWString(L"Цвет окантовки"),               IT_FRAME_BORDER);
	m_pBackgroundColor = createTreeItem(m_pAnimation, QString::fromStdWString(L"Цвет фона за пределами кадра"), IT_FRAME_BACKGROUND);

	treeWidget->setCurrentItem(m_pRoot);
}

PTR(QTreeWidgetItem) ViewPreferences::createTreeItem(PTR(QTreeWidgetItem) parent, CREF(QString) name, ItemType itemType)
{
	PTR(QTreeWidgetItem) item = new QTreeWidgetItem(parent);
	item->setText(0, name);
	item->setData(0, Qt::UserRole, QVariant(itemType));
	return item;
}

int ViewPreferences::getStylePropertyType()
{
	PTR(StyleItem) item = getStyleItem();
	BOOST_FOREACH(StyleProperty* prop, item->properties)
	{
		if(prop->identificator == treeWidget->currentItem()->data(0, Qt::UserRole).toInt())
			return prop->identificator;
	}
	return -1;
}

PTR(ViewPreferences::StyleProperty) ViewPreferences::getStyleProperty()
{
	PTR(StyleItem) item =getStyleItem();
	BOOST_FOREACH(StyleProperty* prop, item->properties)
	{
		if(prop->identificator == treeWidget->currentItem()->data(0, Qt::UserRole).toInt())
			return prop;
	}
	return NULL;
}

PTR(ViewPreferences::StyleItem) ViewPreferences::getStyleItem()
{
	if(treeWidget->currentItem()->data(0, Qt::UserRole).toInt() == IT_ROOT)
	{
		BOOST_FOREACH(StyleItem* item, style_list)
		{
			if(item->type == treeWidget->currentItem()->data(0, Qt::UserRole).toInt())
				return item;
		}
	}

	if(treeWidget->currentItem()->parent()->data(0, Qt::UserRole).toInt() == IT_ROOT)
	{
		BOOST_FOREACH(StyleItem* item, style_list)
		{
			if(item->type == treeWidget->currentItem()->data(0, Qt::UserRole).toInt())
			return item;
		}
	}

	BOOST_FOREACH(StyleItem* item, style_list)
	{
		if(item->type == treeWidget->currentItem()->parent()->data(0, Qt::UserRole).toInt())
			return item;
	}
	return NULL;
}

void ViewPreferences::apply()
{
	studioApp.getStyle()->style_editor  = style_editor;
	studioApp.getStyle()->style_build   = style_build;
	studioApp.getStyle()->style_debug   = style_debug;
	//studioApp.getStyle()->style_trace   = style_trace;
	studioApp.getStyle()->style_results = style_results;
	studioApp.getStyle()->style_find    = style_find;
	studioApp.setFileAssociationSetup(m_setup);
	studioApp.setFileAssociationCheckInFuture(m_checkInFuture);
	studioApp.setOpenLastProject(m_openLastProject);
	studioApp.setShowCaptionFullName(m_showFullName);
	studioApp.getStyle()->updateAllStyles();
}
