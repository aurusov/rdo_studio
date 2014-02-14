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
#include "app/rdo_studio/src/application.h"
#include "simulator/report/build_edit_line_info.h"
// --------------------------------------------------------------------------------

using namespace rdo::simulation::report;
using namespace rdo::gui::editor;
using namespace rdo::gui::style;
using namespace rdo::gui::tracer;
using namespace rdo::gui::frame;

bool ViewPreferences::null_wordwrap      = false;
bool ViewPreferences::null_horzscrollbar = true;
bool ViewPreferences::null_warning       = true;
bool ViewPreferences::null_commentfold   = false;
EditStyle::Bookmark ViewPreferences::null_bookmarkstyle = EditStyle::B_NONE;
ModelStyle::Fold    ViewPreferences::null_foldstyle     = ModelStyle::F_NONE;
QColor ViewPreferences::null_fg_color = QColor(0x00, 0x00, 0x00);
QColor ViewPreferences::null_bg_color = QColor(0xFF, 0xFF, 0xFF);

ViewPreferences::ViewPreferences(QWidget* pParent)
	: QDialog(pParent, Qt::Dialog | Qt::CustomizeWindowHint | Qt::WindowTitleHint | Qt::WindowCloseButtonHint)
	, all_font_size(-1)
	, all_font_name("")
	, all_fg_color(0x00, 0x00, 0x00)
	, all_bg_color(0xFF, 0xFF, 0xFF)
	, null_font_style(StyleFont::NONE)
	, m_pPluginInfoList(g_pApp->getPluginLoader().getPluginInfoList())
{
	setupUi(this);

	connect(buttonOk, SIGNAL(clicked()), this, SLOT(onOkButton()));
	connect(buttonCancel, SIGNAL(clicked()), this, SLOT(onCancelButton()));
	connect(buttonApply, SIGNAL(clicked()), this, SLOT(onApplyButton()));
	
	m_setup           = g_pApp->getFileAssociationSetup();
	m_checkInFuture   = g_pApp->getFileAssociationCheckInFuture();
	m_openLastProject = g_pApp->getOpenLastProject();
	m_showFullName    = g_pApp->getShowCaptionFullName();

	style_editor  = g_pApp->getStyle()->style_editor;
	style_build   = g_pApp->getStyle()->style_build;
	style_debug   = g_pApp->getStyle()->style_debug;
	style_trace   = g_pApp->getStyle()->style_trace;
	style_results = g_pApp->getStyle()->style_results;
	style_find    = g_pApp->getStyle()->style_find;
	style_chart   = g_pApp->getStyle()->style_chart;
	style_frame   = g_pApp->getStyle()->style_frame;

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
	tabSizeLineEdit->setText(QString::number(style_editor.tab.tabSize));
	indentSizeLineEdit->setValidator(new QIntValidator(1, 100, this));
	indentSizeLineEdit->setText(QString::number(style_editor.tab.indentSize));
	connect(useTabSymbolCheckBox, SIGNAL(stateChanged(int)), this, SLOT(onUseTabSymbol(int)));
	connect(indentAsTabcheckBox, SIGNAL(stateChanged(int)), this, SLOT(onIndentAsTab(int)));
	connect(autoIndentCheckBox, SIGNAL(stateChanged(int)), this, SLOT(onAutoIndent(int)));
	connect(eraseWithTabRadioButton, SIGNAL(toggled(bool)), this, SLOT(onEraseWithTab(bool)));
	connect(tabSizeLineEdit, SIGNAL(textEdited(const QString&)), this, SLOT(onTabSize(const QString&)));
	connect(indentSizeLineEdit, SIGNAL(textEdited(const QString&)), this, SLOT(onIndentSize(const QString&)));
	//Вкладка "Стиль и цвет"
	stackedWidget->setCurrentWidget(pageRoot);

	PTR(QPalette) palette = new QPalette();
	palette->setColor(QPalette::Foreground, Qt::gray);
	previewStackedWidget->setPalette(*palette);

	createTree();
	createPreview();
	createStyles();

	insertColors(fgColorComboBox);
	insertColors(bgColorComboBox);

	switchPreviewComboBox->addItem("Editor",  IT_EDITOR);
	switchPreviewComboBox->addItem("Build",   IT_BUILD);
	switchPreviewComboBox->addItem("Debug",   IT_DEBUG);
	switchPreviewComboBox->addItem("Tracer",  IT_LOG);
	switchPreviewComboBox->addItem("Results", IT_RESULT);
	switchPreviewComboBox->addItem("Find",    IT_FIND);
	switchPreviewComboBox->addItem("Chart",   IT_CHART);
	switchPreviewComboBox->addItem("Frame",   IT_FRAME);

	bookmarkComboBox->addItem("Нет",           EditStyle::B_NONE);
	bookmarkComboBox->addItem("Круг",          EditStyle::B_CIRCLE);
	bookmarkComboBox->addItem("Прямоугольник", EditStyle::B_RECT);
	bookmarkComboBox->addItem("Овал",          EditStyle::B_ROUNDRECT);
	bookmarkComboBox->addItem("Стрелка",       EditStyle::B_ARROW);

	foldComboBox->addItem("Нет",             ModelStyle::F_NONE);
	foldComboBox->addItem("Плюс",            ModelStyle::F_PLUS);
	foldComboBox->addItem("Плюс + линия",    ModelStyle::F_PLUSCONNECTED);
	foldComboBox->addItem("Стрелка",         ModelStyle::F_ARROW);
	foldComboBox->addItem("Стрелка + линия", ModelStyle::F_ARROWCONNECTED);
	foldComboBox->addItem("Квадрат + линия", ModelStyle::F_BOXCONNECTED);
	foldComboBox->addItem("Круг + линия",    ModelStyle::F_CIRCLECONNECTED);

	boldCheckBox->setEnabled(false);
	italicCheckBox->setEnabled(false);
	underlineCheckBox->setEnabled(false);

	horzIndentLineEdit->setValidator(new QIntValidator(1, 100, this));
	horzIndentLineEdit->setText(QString::number(style_trace.borders.horzBorder));
	vertIndentLineEdit->setValidator(new QIntValidator(1, 100, this));
	vertIndentLineEdit->setText(QString::number(style_trace.borders.vertBorder));

	tickWidthLineEdit->setValidator(new QIntValidator(1, 100, this));
	tickWidthLineEdit->setText(QString::number(style_chart.pFontsTicks.tickWidth));

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
	connect(commentCheckBox, SIGNAL(stateChanged(int)), this, SLOT(onComment(int)));
	connect(warningCheckBox, SIGNAL(stateChanged(int)), this, SLOT(onWarning(int)));
	connect(horzIndentLineEdit, SIGNAL(textEdited(const QString&)), this, SLOT(onHorzIndent(const QString&)));
	connect(vertIndentLineEdit, SIGNAL(textEdited(const QString&)), this, SLOT(onVertIndent(const QString&)));
	connect(fgColorComboBox, SIGNAL(activated(int)), this, SLOT(onFgColor(int)));
	connect(bgColorComboBox, SIGNAL(activated(int)), this, SLOT(onBgColor(int)));
	connect(fgColorToolButton, SIGNAL(clicked()), this, SLOT(onFgColorDialog()));
	connect(bgColorToolButton, SIGNAL(clicked()), this, SLOT(onBgColorDialog()));
	connect(titleComboBox, SIGNAL(activated(int)), this, SLOT(onTitleSize(int)));
	connect(legendComboBox, SIGNAL(activated(int)), this, SLOT(onLegendSize(int)));
	connect(tickWidthLineEdit, SIGNAL(textEdited(const QString&)), this, SLOT(onTickWidth(const QString&)));
	connect(themeComboBox, SIGNAL(activated(int)), this, SLOT(onThemeComboBox(int)));

	connect(tabSizeLineEdit, SIGNAL(textEdited(const QString&)), this, SLOT(onCheckInput(const QString&)));
	connect(indentSizeLineEdit, SIGNAL(textEdited(const QString&)), this, SLOT(onCheckInput(const QString&)));
	connect(horzIndentLineEdit, SIGNAL(textEdited(const QString&)), this, SLOT(onCheckInput(const QString&)));
	connect(vertIndentLineEdit, SIGNAL(textEdited(const QString&)), this, SLOT(onCheckInput(const QString&)));
	connect(tickWidthLineEdit, SIGNAL(textEdited(const QString&)), this, SLOT(onCheckInput(const QString&)));

	// Вкладка "Пплагины"
	QStringList tableColumnHeaders;
	tableColumnHeaders << "Name" << "Version" << "Author" << "Autoload";
	pluginInfoTable->setColumnCount(4);
	pluginInfoTable->setHorizontalHeaderLabels(tableColumnHeaders);
	pluginInfoTable->verticalHeader()->hide();
	pluginInfoTable->setSelectionBehavior(QAbstractItemView::SelectRows);
	pluginInfoTable->setEditTriggers     (QAbstractItemView::NoEditTriggers);
	pluginInfoTable->horizontalHeader()->setSectionResizeMode(QHeaderView::ResizeToContents);
	pluginInfoTable->horizontalHeader()->setSectionResizeMode(0,QHeaderView::Stretch);
	QObject::connect(buttonDeletePlugin, &QAbstractButton::clicked, this, &ViewPreferences::deletePlugin);
	QObject::connect(buttonStartPlugin , &QAbstractButton::clicked, this, &ViewPreferences::onStartPlugin);
	QObject::connect(buttonStopPlugin  , &QAbstractButton::clicked, this, &ViewPreferences::onStopPlugin);
	QObject::connect(pluginInfoTable->selectionModel(),
	        &QItemSelectionModel::selectionChanged,
	        this,
	        &ViewPreferences::updateButtonsState);
	fillPluginInfoTable();
	updateButtonsState();

	updateDialog();
	checkAllData();
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
	buttonApply->setEnabled(false);
}

void ViewPreferences::onCheckInput(const QString& text)
{
	UNUSED(text);

	bool check = tabSizeLineEdit->text().toInt() >= 1 &&
		indentSizeLineEdit->text().toInt() >= 1 &&
		horzIndentLineEdit->text().toInt() >= 1 &&
		vertIndentLineEdit->text().toInt() >= 1 &&
		tickWidthLineEdit->text().toInt() >= 1;
		
	buttonOk->setEnabled(check);
	buttonApply->setEnabled(check);
}

void ViewPreferences::onCodeCompUse(int state)
{
	style_editor.autoComplete.useAutoComplete = state == Qt::Checked ? true : false;
	
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
	checkAllData();
}

void ViewPreferences::onCodeCompShowFullList(bool state)
{
	UNUSED(state);

	if(radioButtonFullList->isChecked())
		style_editor.autoComplete.showFullList = true;
	if(radioButtonNearestWords->isChecked())
		style_editor.autoComplete.showFullList = false;
	checkAllData();
}

void ViewPreferences::onMarginFold(int state)
{
	style_editor.margin.fold = state == Qt::Checked ? true : false;
	checkAllData();
}

void ViewPreferences::onMarginBookmark(int state)
{
	style_editor.margin.bookmark = state == Qt::Checked ? true : false;
	checkAllData();
}

void ViewPreferences::onMarginLineNumber(int state)
{
	style_editor.margin.lineNumber = state == Qt::Checked ? true : false;
	checkAllData();
}

void ViewPreferences::onUseTabSymbol(int state)
{
	style_editor.tab.useTabs = state == Qt::Checked ? true : false;
	checkAllData();
}

void ViewPreferences::onIndentAsTab(int state)
{
	style_editor.tab.tabIndents = state == Qt::Checked ? true : false;
	checkAllData();
}

void ViewPreferences::onAutoIndent(int state)
{
	style_editor.tab.autoIndent = state == Qt::Checked ? true : false;
	checkAllData();
}

void ViewPreferences::onEraseWithTab(bool state)
{
	UNUSED(state);

	if(eraseWithTabRadioButton->isChecked())
		style_editor.tab.backspaceUntabs = true;
	if(eraseWithIndentRadioButton->isChecked())
		style_editor.tab.backspaceUntabs = false;
	checkAllData();
}

void ViewPreferences::onTabSize(const QString& text)
{
	style_editor.tab.tabSize = text.toInt();
	checkAllData();
}

void ViewPreferences::onIndentSize(const QString& text)
{
	style_editor.tab.indentSize = text.toInt();
	checkAllData();
}

void ViewPreferences::onSetup(int state)
{
	m_setup = state == Qt::Checked ? true : false;
	checkAllData();
}

void ViewPreferences::onCheckInFuture(int state)
{
	m_checkInFuture = state == Qt::Checked ? true : false;
	checkAllData();
}

void ViewPreferences::onOpenLastProject(int state)
{
	m_openLastProject = state == Qt::Checked ? true : false;
	checkAllData();
}

void ViewPreferences::onShowFullName(int state)
{
	m_showFullName = state == Qt::Checked ? true : false;
	checkAllData();
}

void ViewPreferences::onTreeWidgetItemActivated(QTreeWidgetItem* item, int column)
{
	stackedWidget->setCurrentIndex(item->data(column, Qt::UserRole).toInt());
	if(item->data(column, Qt::UserRole).toInt() != 0)
	{
		switchPreviewComboBox->setCurrentIndex(item->data(column, Qt::UserRole).toInt() - 1);
	}
	previewStackedWidget->setCurrentIndex(getStyleItem()->type - 1);

	switch(item->data(column, Qt::UserRole).toInt())
	{
	case IT_ROOT:
	case IT_EDITOR_CARET:
	case IT_EDITOR_TEXTSELECTION:
	case IT_EDITOR_BOOKMARK:
	case IT_EDITOR_FOLD:
	case IT_EDITOR_ERROR:
	case IT_BUILD_SELECTEDLINE:
	case IT_CHART_CHART:
	case IT_CHART_TIME:
		boldCheckBox->setEnabled(false);
		italicCheckBox->setEnabled(false);
		underlineCheckBox->setEnabled(false);
		break;
	default:
		boldCheckBox->setEnabled(true);
		italicCheckBox->setEnabled(true);
		underlineCheckBox->setEnabled(true);
	}

	updateStyleTab();
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
		style_editor.font.size  = size;
		style_build.font.size   = size;
		style_debug.font.size   = size;
		style_trace.font.size   = size;
		style_results.font.size = size;
		style_find.font.size    = size;
		style_chart.font.size   = size;
		style_frame.font.size   = size;
		break;
	case IT_EDITOR:
		style_editor.font.size = size;
		break;
	case IT_BUILD:
		style_build.font.size = size;
		break;
	case IT_DEBUG:
		style_debug.font.size   = size;
		break;
	case IT_LOG:
		style_trace.font.size = size;
		break;
	case IT_RESULT:
		style_results.font.size = size;
		break;
	case IT_FIND:
		style_find.font.size    = size;
		break;
	case IT_CHART:
		style_chart.font.size   = size;
		break;
	case IT_FRAME:
		style_frame.font.size   = size;
		break;
	default:
		break;
	}

	updatePreview();
}

void ViewPreferences::onFontType(int index)
{
	UNUSED(index);

	tstring name = fontComboBox->currentFont().family().toStdString();

	switch(getStyleItem()->type)
	{
	case IT_ROOT:
		all_font_name = name;
		style_editor.font.name  = name;
		style_build.font.name   = name;
		style_debug.font.name   = name;
		style_trace.font.name   = name;
		style_results.font.name = name;
		style_find.font.name    = name;
		style_chart.font.name   = name;
		style_frame.font.name   = name;
		break;
	case IT_EDITOR:
		style_editor.font.name = name;
		break;
	case IT_BUILD:
		style_build.font.name = name;
		break;
	case IT_DEBUG:
		style_debug.font.name   = name;
		break;
	case IT_LOG:
		style_trace.font.name = name;
		break;
	case IT_RESULT:
		style_results.font.name = name;
		break;
	case IT_FIND:
		style_find.font.name    = name;
		break;
	case IT_CHART:
		style_chart.font.name   = name;
		break;
	case IT_FRAME:
		style_frame.font.name   = name;
		break;
	default:
		break;
	}

	updatePreview();
}

void ViewPreferences::onFontBold(int state)
{
	PTR(StyleProperty) prop = getStyleProperty();
	if (prop && &prop->font_style != &null_font_style)
	{
		prop->font_style = static_cast<StyleFont::style>(prop->font_style & ~StyleFont::BOLD);
		if (state)
		{
			prop->font_style = static_cast<StyleFont::style>(prop->font_style | StyleFont::BOLD);
		}
		updatePreview();
	}
}

void ViewPreferences::onFontItalic(int state)
{
	PTR(StyleProperty) prop = getStyleProperty();
	if (prop && &prop->font_style != &null_font_style)
	{
		prop->font_style = static_cast<StyleFont::style>(prop->font_style & ~StyleFont::ITALIC);
		if (state)
		{
			prop->font_style = static_cast<StyleFont::style>(prop->font_style | StyleFont::ITALIC);
		}
		updatePreview();
	}
}

void ViewPreferences::onFontUnderline(int state)
{
	PTR(StyleProperty) prop = getStyleProperty();
	if (prop && &prop->font_style != &null_font_style)
	{
		prop->font_style = static_cast<StyleFont::style>(prop->font_style & ~StyleFont::UNDERLINE);
		if (state)
		{
			prop->font_style = static_cast<StyleFont::style>(prop->font_style | StyleFont::UNDERLINE);
		}
		updatePreview();
	}
}

void ViewPreferences::onHorzScroll(int state)
{
	PTR(StyleItem) item = getStyleItem();
	item->horzscrollbar = state == Qt::Checked ? true : false;
	updatePreview();
}

void ViewPreferences::onWordWrap(int state)
{
	PTR(StyleItem) item = getStyleItem();
	item->wordwrap = state == Qt::Checked ? true : false;
		
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
	default:
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
			item->horzscrollbar = horzScrollEditorCheckBox->checkState() == Qt::Checked ? true : false;
			break;
		case IT_BUILD:
			item->horzscrollbar = horzScrollBuildCheckBox->checkState() == Qt::Checked ? true : false;
			break;
		case IT_DEBUG:
			item->horzscrollbar = horzScrollDebugCheckBox->checkState() == Qt::Checked ? true : false;
			break;
		case IT_RESULT:
			item->horzscrollbar = horzScrollResultsCheckBox->checkState() == Qt::Checked ? true : false;
			break;
		case IT_FIND:
			item->horzscrollbar = horzScrollFindCheckBox->checkState() == Qt::Checked ? true : false;
			break;
		default:
			break;
		}
	}
	updatePreview();
}

void ViewPreferences::onBookmark(int index)
{
	PTR(StyleItem) item = getStyleItem();
	item->bookmarkstyle = static_cast<EditStyle::Bookmark>(index);
	updatePreview();
}

void ViewPreferences::onFold(int index)
{
	PTR(StyleItem) item = getStyleItem();
	item->foldstyle = static_cast<ModelStyle::Fold>(index);
	updatePreview();
}

void ViewPreferences::onComment(int state)
{
	PTR(StyleItem) item = getStyleItem();
	item->commentfold = state ? true : false;
	updatePreview();
}

void ViewPreferences::onWarning(int state)
{
	PTR(StyleItem) item = getStyleItem();
	item->warning = state ? true : false;
	updatePreview();
}

void ViewPreferences::onHorzIndent(const QString& text)
{
	style_trace.borders.horzBorder = text.toInt();
	updatePreview();
}

void ViewPreferences::onVertIndent(const QString& text)
{
	style_trace.borders.vertBorder = text.toInt();
	updatePreview();
}

void ViewPreferences::onFgColor(int index)
{
	QColor color = qvariant_cast<QColor>(fgColorComboBox->itemData(index, Qt::UserRole));
	PTR(StyleProperty) prop = getStyleProperty();
	if (&prop->fg_color != &null_fg_color)
	{
		prop->fg_color = color;
		if (&prop->fg_color == &all_fg_color)
		{
			StyleItemList::const_iterator it = style_list.begin();
			if (it != style_list.end())
			{
				++it;
				while (it != style_list.end())
				{
					PropertyList::const_iterator it_prop = (*it)->properties.begin();
					if (it_prop != (*it)->properties.end())
					{
						(*it_prop)->fg_color = all_fg_color;
					}
					++it;
				}
			}
		}
		updatePreview();
	}
}

void ViewPreferences::onBgColor(int index)
{
	QColor color = qvariant_cast<QColor>(bgColorComboBox->itemData(index, Qt::UserRole));
	PTR(StyleProperty) prop = getStyleProperty();
	if (&prop->bg_color != &null_bg_color)
	{
		prop->bg_color = color;
		if (&prop->bg_color == &all_bg_color)
		{
			StyleItemList::const_iterator it = style_list.begin();
			if (it != style_list.end())
			{
				++it;
				while (it != style_list.end())
				{
					PropertyList::const_iterator it_prop = (*it)->properties.begin();
					if (it_prop != (*it)->properties.end())
					{
						(*it_prop)->bg_color = all_bg_color;
					}
					++it;
				}
			}
		}
		updatePreview();
	}
}

void ViewPreferences::onFgColorDialog()
{
	QColor color = fgColorComboBox->itemData(fgColorComboBox->currentIndex(), Qt::UserRole).value<QColor>();
	fgColorDlg = new QColorDialog(color, this);
	connect(fgColorDlg, SIGNAL(colorSelected(const QColor&)), this, SLOT(onFgColorSelected(const QColor&)));
	fgColorDlg->exec();
}

void ViewPreferences::onBgColorDialog()
{
	QColor color = bgColorComboBox->itemData(bgColorComboBox->currentIndex(), Qt::UserRole).value<QColor>();
	bgColorDlg = new QColorDialog(color, this);
	connect(bgColorDlg, SIGNAL(colorSelected(const QColor&)), this, SLOT(onBgColorSelected(const QColor&)));
	bgColorDlg->exec();
}

void ViewPreferences::onFgColorSelected(const QColor& color)
{
	if(fgColorComboBox->findData(color, Qt::UserRole) == -1)
	{
		insertColor(color, QString("[%1, %2, %3]").arg(color.red()).arg(color.green()).arg(color.blue()), fgColorComboBox);
	}
	fgColorComboBox->setCurrentIndex(fgColorComboBox->findData(color, Qt::UserRole));
	onFgColor(fgColorComboBox->findData(color, Qt::UserRole));
}

void ViewPreferences::onBgColorSelected(const QColor& color)
{
	if(bgColorComboBox->findData(color, Qt::UserRole) == -1)
	{
		insertColor(color, QString("[%1, %2, %3]").arg(color.red()).arg(color.green()).arg(color.blue()), bgColorComboBox);
	}
	bgColorComboBox->setCurrentIndex(bgColorComboBox->findData(color, Qt::UserRole));
	onBgColor(bgColorComboBox->findData(color, Qt::UserRole));
}

void ViewPreferences::onTitleSize(int index)
{
	UNUSED(index);
	style_chart.pFontsTicks.titleFontSize = titleComboBox->currentText().toInt();
	updatePreview();
}

void ViewPreferences::onLegendSize(int index)
{
	UNUSED(index);
	style_chart.pFontsTicks.legendFontSize = legendComboBox->currentText().toInt();
	updatePreview();
}

void ViewPreferences::onTickWidth(const QString& text)
{
	style_chart.pFontsTicks.tickWidth = text.toInt();
	updatePreview();
}

void ViewPreferences::onThemeComboBox(int index)
{
	switch (getStyleProperty()->item->type) 
	{
	case IT_ROOT:
		if (index == 1) {
			style_editor = ModelStyle::getDefaultStyle();
			style_build = BuildStyle::getDefaultStyle();
			style_debug = EditStyle::getDefaultStyle();
			style_trace = rdo::gui::tracer::LogStyle::getDefaultStyle();
			style_results = ResultsStyle::getDefaultStyle();
			style_find = FindStyle::getDefaultStyle();
			style_chart = ChartViewStyle::getDefaultStyle();
			style_frame = FrameStyle::getDefaultStyle();
			style_editor.font  = StyleFont::getDefaultFont();
			style_build.font   = StyleFont::getDefaultFont();
			style_debug.font   = StyleFont::getDefaultFont();
			style_trace.font   = StyleFont::getTracerLogFont();
			style_results.font = StyleFont::getDefaultFont();
			style_find.font    = StyleFont::getDefaultFont();
			style_chart.font   = StyleFont::getChartViewFont();
			style_frame.font   = StyleFont::getFrameFont();
		}
		break;
	case IT_EDITOR:
		switch (index) 
		{
		case 1: style_editor = ModelStyle::getDefaultStyle();  style_editor.font = StyleFont::getDefaultFont(); break;
		case 2: style_editor = ModelStyle::getCppStyle();      style_editor.font = StyleFont::getDefaultFont(); break;
		case 3: style_editor = ModelStyle::getPascalStyle();   style_editor.font = StyleFont::getDefaultFont(); break;
		case 4: style_editor = ModelStyle::getHtmlStyle();     style_editor.font = StyleFont::getDefaultFont(); break;
		case 5: style_editor = ModelStyle::getClassicStyle();  style_editor.font = StyleFont::getClassicFont(); break;
		case 6: style_editor = ModelStyle::getTwilightStyle(); style_editor.font = StyleFont::getDefaultFont(); break;
		case 7: style_editor = ModelStyle::getOceanStyle();    style_editor.font = StyleFont::getDefaultFont(); break;
		}
		break;
	case IT_BUILD:
		switch (index)
		{
		case 1: style_build = BuildStyle::getDefaultStyle();  style_build.font = StyleFont::getDefaultFont(); break;
		case 2: style_build = BuildStyle::getClassicStyle();  style_build.font = StyleFont::getClassicFont(); break;
		case 3: style_build = BuildStyle::getTwilightStyle(); style_build.font = StyleFont::getDefaultFont(); break;
		case 4: style_build = BuildStyle::getOceanStyle();    style_build.font = StyleFont::getDefaultFont(); break;
		}
		break;
	case IT_DEBUG:
		switch (index)
		{
		case 1: style_debug = EditStyle::getDefaultStyle();  style_debug.font = StyleFont::getDefaultFont(); break;
		case 2: style_debug = EditStyle::getClassicStyle();  style_debug.font = StyleFont::getClassicFont(); break;
		case 3: style_debug = EditStyle::getTwilightStyle(); style_debug.font = StyleFont::getDefaultFont(); break;
		case 4: style_debug = EditStyle::getOceanStyle();    style_debug.font = StyleFont::getDefaultFont(); break;
		}
		break;
	case IT_LOG:
		switch (index)
		{
		case 1: style_trace = rdo::gui::tracer::LogStyle::getDefaultStyle(); style_trace.font = StyleFont::getTracerLogFont(); break;
		}
		break;
	case IT_RESULT:
		switch (index)
		{
		case 1: style_results = ResultsStyle::getDefaultStyle();  style_results.font = StyleFont::getDefaultFont(); break;
		case 2: style_results = ResultsStyle::getCppStyle();      style_results.font = StyleFont::getDefaultFont(); break;
		case 3: style_results = ResultsStyle::getPascalStyle();   style_results.font = StyleFont::getDefaultFont(); break;
		case 4: style_results = ResultsStyle::getHtmlStyle();     style_results.font = StyleFont::getDefaultFont(); break;
		case 5: style_results = ResultsStyle::getClassicStyle();  style_results.font = StyleFont::getClassicFont(); break;
		case 6: style_results = ResultsStyle::getTwilightStyle(); style_results.font = StyleFont::getDefaultFont(); break;
		case 7: style_results = ResultsStyle::getOceanStyle();    style_results.font = StyleFont::getDefaultFont(); break;
		}
		break;
	case IT_FIND:
		switch (index)
		{
		case 1: style_find = FindStyle::getDefaultStyle();  style_find.font = StyleFont::getDefaultFont(); break;
		case 2: style_find = FindStyle::getClassicStyle();  style_find.font = StyleFont::getClassicFont(); break;
		case 3: style_find = FindStyle::getTwilightStyle(); style_find.font = StyleFont::getDefaultFont(); break;
		case 4: style_find = FindStyle::getOceanStyle();    style_find.font = StyleFont::getDefaultFont(); break;
		}
		break;
	case IT_CHART:
		switch (index)
		{
		case 1: style_chart = ChartViewStyle::getDefaultStyle(); style_chart.font = StyleFont::getChartViewFont(); break;
		}
		break;
	case IT_FRAME:
		switch (index)
		{
		case 1: style_frame = FrameStyle::getDefaultStyle(); style_frame.font = StyleFont::getFrameFont(); break;
		}
		break;
	default:
		break;
	}
	updatePreview();
}

void ViewPreferences::updateTheme()
{
	switch (getStyleProperty()->item->type)
	{
	case IT_ROOT:
		if ( style_editor == ModelStyle::getDefaultStyle() &&
			style_build == BuildStyle::getDefaultStyle() &&
			style_debug == EditStyle::getDefaultStyle() &&
			style_trace == rdo::gui::tracer::LogStyle::getDefaultStyle() &&
			style_results == ResultsStyle::getDefaultStyle() &&
			style_find == FindStyle::getDefaultStyle() &&
			style_chart == ChartViewStyle::getDefaultStyle() &&
			style_frame == FrameStyle::getDefaultStyle() &&
			style_editor.font == StyleFont::getDefaultFont() &&
			style_build.font == StyleFont::getDefaultFont() &&
			style_debug.font == StyleFont::getDefaultFont() &&
			style_trace.font == StyleFont::getTracerLogFont() &&
			style_results.font == StyleFont::getDefaultFont() &&
			style_find.font == StyleFont::getDefaultFont() &&
			style_chart.font == StyleFont::getChartViewFont() &&
			style_frame.font == StyleFont::getFrameFont()) {
				themeComboBox->setCurrentIndex(1);
		} else {
			themeComboBox->setCurrentIndex(0);
		}
		break;
	case IT_EDITOR:
		if ( style_editor == ModelStyle::getDefaultStyle() && style_editor.font == StyleFont::getDefaultFont()) {
			themeComboBox->setCurrentIndex(1);
		} else if ( style_editor == ModelStyle::getCppStyle() && style_editor.font == StyleFont::getDefaultFont() ) {
			themeComboBox->setCurrentIndex(2);
		} else if ( style_editor == ModelStyle::getPascalStyle() && style_editor.font == StyleFont::getDefaultFont() ) {
			themeComboBox->setCurrentIndex(3);
		} else if ( style_editor == ModelStyle::getHtmlStyle() && style_editor.font == StyleFont::getDefaultFont() ) {
			themeComboBox->setCurrentIndex(4);
		} else if ( style_editor == ModelStyle::getClassicStyle() && style_editor.font == StyleFont::getClassicFont() ) {
			themeComboBox->setCurrentIndex(5);
		} else if ( style_editor == ModelStyle::getTwilightStyle() && style_editor.font == StyleFont::getDefaultFont() ) {
			themeComboBox->setCurrentIndex(6);
		} else if ( style_editor == ModelStyle::getOceanStyle() && style_editor.font == StyleFont::getDefaultFont() ) {
			themeComboBox->setCurrentIndex(7);
		} else {
			themeComboBox->setCurrentIndex(0);
		}
		break;
	case IT_BUILD:
		if ( style_build == BuildStyle::getDefaultStyle() && style_build.font == StyleFont::getDefaultFont()) {
			themeComboBox->setCurrentIndex(1);
		} else if ( style_build == BuildStyle::getClassicStyle() && style_build.font == StyleFont::getClassicFont()) {
			themeComboBox->setCurrentIndex(2);
		} else if ( style_build == BuildStyle::getTwilightStyle() && style_build.font == StyleFont::getDefaultFont()) {
			themeComboBox->setCurrentIndex(3);
		} else if ( style_build == BuildStyle::getOceanStyle() && style_build.font == StyleFont::getDefaultFont()) {
			themeComboBox->setCurrentIndex(4);
		} else {
			themeComboBox->setCurrentIndex(0);
		}
		break;
	case IT_DEBUG: 
		if ( style_debug == EditStyle::getDefaultStyle() && style_debug.font == StyleFont::getDefaultFont()) {
			themeComboBox->setCurrentIndex(1);
		} else if ( style_debug == EditStyle::getClassicStyle() && style_debug.font == StyleFont::getClassicFont()) {
			themeComboBox->setCurrentIndex(2);
		} else if ( style_debug == EditStyle::getTwilightStyle() && style_debug.font == StyleFont::getDefaultFont()) {
			themeComboBox->setCurrentIndex(3);
		} else if ( style_debug == EditStyle::getOceanStyle() && style_debug.font == StyleFont::getDefaultFont()) {
			themeComboBox->setCurrentIndex(4);
		} else {
			themeComboBox->setCurrentIndex(0);
		}
		break;
	case IT_LOG:
		if ( style_trace == rdo::gui::tracer::LogStyle::getDefaultStyle() && style_trace.font == StyleFont::getTracerLogFont()) {
			themeComboBox->setCurrentIndex(1);
		} else {
			themeComboBox->setCurrentIndex(0);
		}
		break;
	case IT_RESULT:
		if ( style_results == ResultsStyle::getDefaultStyle() && style_results.font == StyleFont::getDefaultFont()) {
			themeComboBox->setCurrentIndex(1);
		} else if ( style_results == ResultsStyle::getCppStyle() && style_results.font == StyleFont::getDefaultFont()) {
			themeComboBox->setCurrentIndex(2);
		} else if ( style_results == ResultsStyle::getPascalStyle() && style_results.font == StyleFont::getDefaultFont()) {
			themeComboBox->setCurrentIndex(3);
		} else if ( style_results == ResultsStyle::getHtmlStyle() && style_results.font == StyleFont::getDefaultFont()) {
			themeComboBox->setCurrentIndex(4);
		} else if ( style_results == ResultsStyle::getClassicStyle() && style_results.font == StyleFont::getClassicFont()) {
			themeComboBox->setCurrentIndex(5);
		} else if ( style_results == ResultsStyle::getTwilightStyle() && style_results.font == StyleFont::getDefaultFont()) {
			themeComboBox->setCurrentIndex(6);
		} else if ( style_results == ResultsStyle::getOceanStyle() && style_results.font == StyleFont::getDefaultFont()) {
			themeComboBox->setCurrentIndex(7);
		} else {
			themeComboBox->setCurrentIndex(0);
		}
		break;
	case IT_FIND:
		if ( style_find == FindStyle::getDefaultStyle() && style_find.font == StyleFont::getDefaultFont()) {
			themeComboBox->setCurrentIndex(1);
		} else if ( style_find == FindStyle::getClassicStyle() && style_find.font == StyleFont::getClassicFont()) {
			themeComboBox->setCurrentIndex(2);
		} else if ( style_find == FindStyle::getTwilightStyle() && style_find.font == StyleFont::getDefaultFont()) {
			themeComboBox->setCurrentIndex(3);
		} else if ( style_find == FindStyle::getOceanStyle() && style_find.font == StyleFont::getDefaultFont()) {
			themeComboBox->setCurrentIndex(4);
		} else {
			themeComboBox->setCurrentIndex(0);
		}
		break;
	case IT_CHART:
		if ( style_chart == ChartViewStyle::getDefaultStyle() && style_chart.font == StyleFont::getChartViewFont()) {
			themeComboBox->setCurrentIndex(1);
		} else {
			themeComboBox->setCurrentIndex(0);
		}
		break;
	case IT_FRAME:
		if ( style_frame == FrameStyle::getDefaultStyle() && style_frame.font == StyleFont::getFrameFont()) {
			themeComboBox->setCurrentIndex(1);
		} else {
			themeComboBox->setCurrentIndex(0);
		}
		break;
	default:
		break;
	}
}

void ViewPreferences::onHelpContext()
{
	QByteArray ba;
	switch (tabWidget->currentIndex())
	{
	case 0:
		ba.append("setSource qthelp://studio/doc/rdo_studio_rus/html/work_options/work_options_general.htm\n");
		break;
	case 1:
		ba.append("setSource qthelp://studio/doc/rdo_studio_rus/html/work_options/work_options_editor.htm\n");
		break;
	case 2:
		ba.append("setSource qthelp://studio/doc/rdo_studio_rus/html/work_options/work_options_tabs.htm\n");
		break;
	case 3:
		ba.append("setSource qthelp://studio/doc/rdo_studio_rus/html/work_options/work_options_styles_and_color.htm\n");
		break;
	}
	g_pApp->callQtAssistant(ba);
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
	checkBoxCodeCompUse->setCheckState(style_editor.autoComplete.useAutoComplete
		? Qt::Checked
		: Qt::Unchecked
		);
	checkBoxMarginFold->setCheckState(style_editor.margin.fold
		? Qt::Checked
		: Qt::Unchecked
		);
	checkBoxMarginBookmark->setCheckState(style_editor.margin.bookmark
		? Qt::Checked
		: Qt::Unchecked
		);
	checkBoxMarginLineNum->setCheckState(style_editor.margin.lineNumber
		? Qt::Checked
		: Qt::Unchecked
		);
	style_editor.autoComplete.showFullList
		? radioButtonFullList->toggle()
		: radioButtonNearestWords->toggle();	
	useTabSymbolCheckBox->setCheckState(style_editor.tab.useTabs
		? Qt::Checked
		: Qt::Unchecked
		);
	indentAsTabcheckBox->setCheckState(style_editor.tab.tabIndents
		? Qt::Checked
		: Qt::Unchecked
		);
	autoIndentCheckBox->setCheckState(style_editor.tab.autoIndent
		? Qt::Checked
		: Qt::Unchecked
		);
	style_editor.tab.backspaceUntabs
		? eraseWithTabRadioButton->toggle()
		: eraseWithIndentRadioButton->toggle();

	updateStyleTab();
}

void ViewPreferences::updateStyleTab()
{
	PTR(StyleProperty) prop = getStyleProperty();
	QString fontName = QString::fromStdString(prop->item->font_name);
	if (!fontName.isEmpty())
	{
		fontComboBox->setCurrentFont(QFont(fontName));
	}
	else
	{
		fontComboBox->setCurrentIndex(-1);
	}
	fontSizeComboBox->setCurrentIndex(fontSizeComboBox->findText(QString::number(prop->item->font_size)));
	
	if(fgColorComboBox->findData(prop->fg_color) == -1)
	{
		insertColor(prop->fg_color, QString::fromStdString(rdo::format("[%d, %d, %d]", prop->fg_color.red(), prop->fg_color.green(), prop->fg_color.blue())), fgColorComboBox);
	}
	fgColorComboBox->setCurrentIndex(fgColorComboBox->findData(prop->fg_color));
	
	QColor bgColor(prop->bg_color.blue(), prop->bg_color.green(), prop->bg_color.red());
	if(bgColorComboBox->findData(prop->bg_color) == -1)
	{
		insertColor(prop->bg_color, QString::fromStdString(rdo::format("[%d, %d, %d]", prop->bg_color.red(), prop->bg_color.green(), prop->bg_color.blue())), bgColorComboBox);
	}
	bgColorComboBox->setCurrentIndex(bgColorComboBox->findData(prop->bg_color));

	if(boldCheckBox->isEnabled())
	{
		boldCheckBox->setCheckState((prop->font_style & StyleFont::BOLD) != 0 ? Qt::Checked : Qt::Unchecked);
	}
	if(italicCheckBox->isEnabled())
	{
		italicCheckBox->setCheckState((prop->font_style & StyleFont::ITALIC) != 0 ? Qt::Checked : Qt::Unchecked);
	}
	if(underlineCheckBox->isEnabled())
	{
		underlineCheckBox->setCheckState((prop->font_style & StyleFont::UNDERLINE) != 0 ? Qt::Checked : Qt::Unchecked);
	}
	switch(prop->item->type)
	{
	case IT_ROOT:
		if((all_font_size == style_editor.font.size)  &&
		   (all_font_size == style_debug.font.size)   &&
		   (all_font_size == style_build.font.size)   &&
		   (all_font_size == style_find.font.size)    &&
		   (all_font_size == style_frame.font.size)   &&
		   (all_font_size == style_results.font.size) &&
		   (all_font_size == style_trace.font.size)   &&
		   (all_font_size == style_chart.font.size)
		)
		{
			fontSizeComboBox->setCurrentIndex(fontSizeComboBox->findText(QString::number(prop->item->font_size)));
		}
		else
			fontSizeComboBox->setCurrentIndex(-1);
		
		if((all_font_name == style_editor.font.name)  &&
		   (all_font_name == style_debug.font.name)   &&
		   (all_font_name == style_build.font.name)   &&
		   (all_font_name == style_find.font.name)    &&
		   (all_font_name == style_frame.font.name)   &&
		   (all_font_name == style_results.font.name) &&
		   (all_font_name == style_trace.font.name)   &&
		   (all_font_name == style_chart.font.name)
		)
		{
			fontComboBox->setCurrentFont(QFont(QString::fromStdString(all_font_name)));
		}
		else
			fontComboBox->setCurrentIndex(-1);
	case IT_EDITOR:
		wordWrapEditorCheckBox->setCheckState(prop->item->wordwrap ? Qt::Checked : Qt::Unchecked);
		horzScrollEditorCheckBox->setCheckState(prop->item->horzscrollbar ? Qt::Checked : Qt::Unchecked);
		commentCheckBox->setCheckState(prop->item->commentfold ? Qt::Checked : Qt::Unchecked);
		foldComboBox->setCurrentIndex(static_cast<int>(prop->item->foldstyle));
		bookmarkComboBox->setCurrentIndex(static_cast<int>(prop->item->bookmarkstyle));
		break;
	case IT_BUILD:
		wordWrapBuildCheckBox->setCheckState(prop->item->wordwrap ? Qt::Checked : Qt::Unchecked);
		horzScrollBuildCheckBox->setCheckState(prop->item->horzscrollbar ? Qt::Checked : Qt::Unchecked);
		warningCheckBox->setCheckState(prop->item->warning ? Qt::Checked : Qt::Unchecked);
		break;
	case IT_DEBUG:
		wordWrapDebugCheckBox->setCheckState(prop->item->wordwrap ? Qt::Checked : Qt::Unchecked);
		horzScrollDebugCheckBox->setCheckState(prop->item->horzscrollbar ? Qt::Checked : Qt::Unchecked);
		break;
	case IT_LOG:
		vertIndentLineEdit->setText(QString::number(style_trace.borders.vertBorder));
		horzIndentLineEdit->setText(QString::number(style_trace.borders.horzBorder));
		break;
	case IT_RESULT:
		wordWrapResultsCheckBox->setCheckState(prop->item->wordwrap ? Qt::Checked : Qt::Unchecked);
		horzScrollResultsCheckBox->setCheckState(prop->item->horzscrollbar ? Qt::Checked : Qt::Unchecked);
		break;
	case IT_FIND:
		wordWrapFindCheckBox->setCheckState(prop->item->wordwrap ? Qt::Checked : Qt::Unchecked);
		horzScrollFindCheckBox->setCheckState(prop->item->horzscrollbar ? Qt::Checked : Qt::Unchecked);
		break;
	case IT_CHART:
		titleComboBox->setCurrentIndex(titleComboBox->findText(QString::number(style_chart.pFontsTicks.titleFontSize)));
		legendComboBox->setCurrentIndex(legendComboBox->findText(QString::number(style_chart.pFontsTicks.legendFontSize)));
		tickWidthLineEdit->setText(QString::number(style_chart.pFontsTicks.tickWidth));
		switch(prop->identificator)
		{
		case IT_CHART:
			fgColorComboBox->setEnabled(true);
			bgColorComboBox->setEnabled(true);
			fgColorToolButton->setEnabled(true);
			bgColorToolButton->setEnabled(true);
			break;
		case IT_CHART_AXIS:
			fgColorComboBox->setEnabled(true);
			bgColorComboBox->setEnabled(false);
			fgColorToolButton->setEnabled(true);
			bgColorToolButton->setEnabled(false);
			break;
		case IT_CHART_TITLE:
			fgColorComboBox->setEnabled(true);
			bgColorComboBox->setEnabled(false);
			fgColorToolButton->setEnabled(true);
			bgColorToolButton->setEnabled(false);
			break;
		case IT_CHART_CHART:
			fgColorComboBox->setEnabled(false);
			bgColorComboBox->setEnabled(true);
			fgColorToolButton->setEnabled(false);
			bgColorToolButton->setEnabled(true);
			break;
		case IT_CHART_LEGEND:
			fgColorComboBox->setEnabled(true);
			bgColorComboBox->setEnabled(false);
			fgColorToolButton->setEnabled(true);
			bgColorToolButton->setEnabled(false);
			break;
		case IT_CHART_TIME:
			fgColorComboBox->setEnabled(false);
			bgColorComboBox->setEnabled(true);
			fgColorToolButton->setEnabled(false);
			bgColorToolButton->setEnabled(true);
			break;
		default:
			break;
		}
		break;
	case IT_FRAME:
		switch(prop->identificator)
		{
		case IT_FRAME:
			fgColorComboBox->setEnabled(true);
			bgColorComboBox->setEnabled(true);
			fgColorToolButton->setEnabled(true);
			bgColorToolButton->setEnabled(true);
			break;
		case IT_FRAME_BORDER:
			fgColorComboBox->setEnabled(true);
			bgColorComboBox->setEnabled(false);
			fgColorToolButton->setEnabled(true);
			bgColorToolButton->setEnabled(false);
			break;
		case IT_FRAME_BACKGROUND:
			fgColorComboBox->setEnabled(false);
			bgColorComboBox->setEnabled(true);
			fgColorToolButton->setEnabled(false);
			bgColorToolButton->setEnabled(true);
			break;
		}
		break;
	default:
		break;
	}
	updateThemeComboBox(prop);
	updateTheme();
}

void ViewPreferences::updateThemeComboBox(PTR(StyleProperty) prop)
{
	themeComboBox->clear();
	switch(prop->item->type)
	{
	case IT_ROOT:
	case IT_LOG:
	case IT_CHART:
	case IT_FRAME:
		themeComboBox->addItem("Текущий", ST_CURRENT);
		themeComboBox->addItem("По умолчанию", ST_DEFAULT);
		break;
	case IT_EDITOR:
	case IT_RESULT:
		themeComboBox->addItem("Текущий", ST_CURRENT);
		themeComboBox->addItem("По умолчанию", ST_DEFAULT);
		themeComboBox->addItem("C++", ST_CPP);
		themeComboBox->addItem("Pascal", ST_PASCAL);
		themeComboBox->addItem("HTML", ST_HTML);
		themeComboBox->addItem("Классический", ST_CLASSIC);
		themeComboBox->addItem("Яркий", ST_TWILIGHT);
		themeComboBox->addItem("Океан", ST_OCEAN);
		break;
	case IT_BUILD:
	case IT_DEBUG:
	case IT_FIND:
		themeComboBox->addItem("Текущий", ST_CURRENT);
		themeComboBox->addItem("По умолчанию", ST_DEFAULT);
		themeComboBox->addItem("Классический", ST_CLASSIC);
		themeComboBox->addItem("Яркий", ST_TWILIGHT);
		themeComboBox->addItem("Океан", ST_OCEAN);
		break;
	default:
		break;
	}
}

void ViewPreferences::updatePreview()
{
	preview_editor->setEditorStyle(&style_editor);
	preview_editor->repaint();

	preview_build->setEditorStyle(&style_build);
	preview_build->repaint();

	preview_debug->setEditorStyle(&style_debug);
	preview_debug->repaint();
	
	preview_trace->view().setStyle(&style_trace);
	preview_trace->repaint();

	preview_results->setEditorStyle(&style_results);
	preview_results->repaint();

	preview_find->setEditorStyle(&style_find);
	preview_find->repaint();

	preview_chart->setStyle(&style_chart, true);

	preview_frame->setStyle(&style_frame);
	preview_frame->repaint();

	checkAllData();
}

void ViewPreferences::createPreview()
{
	preview_editor = new Model(previewStackedWidget->currentWidget(), previewStackedWidget->currentWidget());
	ASSERT(preview_editor);
	preview_editor->setEditorStyle(&style_editor);
	preview_editor->setCanClearErrorLine(false);
	preview_editor->appendText("{ comments }\n$Pattern pattern_name : operation trace\n$Relevant_resources\n  rel_res2  : res_type2     Keep    Keep\n  rel_res1  : res_type1     Create  NoChange\n$Time = Abs(rel_res2.par1 - rel_res2.par3)\n{...}\n$End\n\ntext [ 10, 20, ... = 'text' ]\n\n$Re levant_resources");
	preview_editor->scrollToLine(0);
	preview_editor->setReadOnly(true);
	preview_editor->bookmarkToggle();
	preview_editor->setErrorLine(preview_editor->getLineCount() - 1);
	previewStackedWidget->addWidget(preview_editor);

	preview_build = new Build(previewStackedWidget->currentWidget());
	preview_build->setEditorStyle(&style_build);
	preview_build->appendLine(new BuildEditLineInfo("Компиляция..."));
	preview_build->appendLine(new BuildEditLineInfo(rdo::simulation::report::FileMessage("Неправильное значение параметра: 4", rdoModelObjects::PAT, 40, 0)));
	preview_build->appendLine(new BuildEditLineInfo("найдено ошибок: 1, предупреждений: 0"));
	preview_build->gotoNext();
	previewStackedWidget->addWidget(preview_build);

	preview_debug = new Debug(previewStackedWidget->currentWidget());
	preview_debug->setEditorStyle(&style_debug);
	preview_debug->appendLine("Получение структуры модели...ok\nМодель запущена");
	previewStackedWidget->addWidget(preview_debug);

	preview_trace = new rdo::gui::tracer::LogMainWnd(previewStackedWidget->currentWidget());
	preview_trace->view().setStyle(&g_pApp->getStyle()->style_trace);
	preview_trace->view().setFocusOnly(true);
	preview_trace->view().setText("Простая строка\nES 0 3\nEB 0 1 1 2 2 1 2\nEF 0.335153 1 1 2 2 1 2\nEI 0.427752 1 1 2 1 2\nER 1.07933 2 2 3 1 3\nRC 0.427752 2 2 0 0.427752 0\nRE 0.335153 2 2\nRK 0.427752 1 1 1 1 1\nV  0.427752 1  1\n$Status = USER_BREAK    607.228\nDPS_C  1  1  1\nSB 0 1\nSO 1 0 0 0\nSTN 3 1 3 6 2 1 3 2  5 6\nSTD 4 3 7 11 1 1 4 2  5 6\nSTR 5 2 8 12 2 1 4 2  1 6\nSRC 0 1 1 1 4\nSRE 0 1 2 2 4\nSRK 0 1 2 2 2\nSD\nSES 0 0.065 397312 10 8 13 13 19\nSEN 0 0.065 397312 10 8 13 13 19\nSEM 0 0.065 397312 10 8 13 13 19\nSEF 0 0.065 397312 10 8 13 13 19\nSEU 0 0.065 397312 10 8 13 13 19");
	preview_trace->view().selectLine(0);
	previewStackedWidget->addWidget(preview_trace);

	preview_results = new Results(previewStackedWidget->currentWidget());
	preview_results->setEditorStyle(&style_results);
	preview_results->setReadOnly(false);
	preview_results->replaceCurrent("Длина_очереди                2  194  0.675957  21.6506  0  4\r\nЗанятость_парикмахера         TRUE  96  0.877351  21.7041  0.0397544  0.918872\r\nВсего_обслужено              96\r\nПропускная_способность       1.99198\r\n", 0);
	preview_results->setReadOnly(true);
	previewStackedWidget->addWidget(preview_results);

	preview_find = new Find(previewStackedWidget->currentWidget());
	preview_find->setEditorStyle(&style_find);
	preview_find->setKeyword("$Time");
	preview_find->appendLine(new LogEditLineInfo("Поиск '$Time'..."));
	preview_find->appendLine(new LogEditLineInfo(rdo::simulation::report::FileMessage("$Time = Равномерный(0.25, 0.75)", rdoModelObjects::PAT, 3, 0)));
	preview_find->appendLine(new LogEditLineInfo(rdo::simulation::report::FileMessage("$Time = Нормальный(0.45, 0.2)", rdoModelObjects::PAT, 13, 0)));
	preview_find->appendLine(new LogEditLineInfo("'2' раз было найдено."));
	preview_find->gotoNext();
	previewStackedWidget->addWidget(preview_find);

	preview_chart_doc = new ChartDoc(true);
	PTR(ChartViewMainWnd) pViewQt = new ChartViewMainWnd(NULL, preview_chart_doc, true);
	preview_chart = &pViewQt->view();
	preview_chart_doc->attachView(preview_chart);
	preview_chart_doc->setTitle(QString("график 1"));
	preview_chart->setPreviwMode(true);
	preview_chart->setStyle(&style_chart, false);
	////initializing times vector
	preview_times.push_back(Time(0, 3));
	preview_times.push_back(Time(2, 3));
	preview_times.push_back(Time(4, 3));
	preview_times.push_back(Time(6, 3));
	preview_times.push_back(Time(8, 3));
	preview_times.push_back(Time(10, 3));
	preview_serie = rdo::Factory<rdo::gui::tracer::Serie>::create();
	preview_serie->setTitle(QString("значение 1"));
	preview_serie->addValue(new Value(&preview_times.at(0), 2, 0));
	preview_serie->addValue(new Value(&preview_times.at(1), 1, 1));
	preview_serie->addValue(new Value(&preview_times.at(2), 0, 4));
	preview_serie->addValue(new Value(&preview_times.at(3), 3, 3));
	preview_serie->addValue(new Value(&preview_times.at(4), 1, 2));
	preview_serie->addValue(new Value(&preview_times.at(5), 0, 3));
	preview_chart_doc->addSerie(preview_serie);

	previewStackedWidget->addWidget(pViewQt);

	preview_frame = new rdo::gui::frame::OptionsView(previewStackedWidget->currentWidget());
	preview_frame->setStyle(&style_frame);
	previewStackedWidget->addWidget(preview_frame);
}

void ViewPreferences::createStyles()
{
	StyleItem* item;
	item = new StyleItem(IT_ROOT, all_font_size, all_font_name);
	item->properties.push_back(new StyleProperty(item, IT_ROOT, null_font_style, all_fg_color, all_bg_color));
	style_list.push_back(item);

	item = new StyleItem(IT_EDITOR, style_editor.font.size, style_editor.font.name, style_editor.window.wordWrap, style_editor.window.showHorzScrollBar, style_editor.bookmarkStyle, style_editor.foldStyle, style_editor.commentFold);
	item->properties.push_back(new StyleProperty(item, IT_EDITOR, style_editor.identifierStyle, style_editor.identifierColor, style_editor.backgroundColor));
	item->properties.push_back(new StyleProperty(item, IT_EDITOR_PLAINTEXT, style_editor.defaultStyle, style_editor.defaultColor, null_bg_color, null_fg_color, style_editor.backgroundColor));
	item->properties.push_back(new StyleProperty(item, IT_EDITOR_IDENTIFICATOR, style_editor.identifierStyle, style_editor.identifierColor, null_bg_color, null_fg_color, style_editor.backgroundColor));
	item->properties.push_back(new StyleProperty(item, IT_EDITOR_KEYWORD, style_editor.keywordStyle, style_editor.keywordColor, null_bg_color, null_fg_color, style_editor.backgroundColor));
	item->properties.push_back(new StyleProperty(item, IT_EDITOR_FUNCTION, style_editor.functionsStyle, style_editor.functionsColor, null_bg_color, null_fg_color, style_editor.backgroundColor));
	item->properties.push_back(new StyleProperty(item, IT_EDITOR_TRACE, style_editor.traceStyle, style_editor.traceColor, null_bg_color, null_fg_color, style_editor.backgroundColor));
	item->properties.push_back(new StyleProperty(item, IT_EDITOR_COLOR, style_editor.colorStyle, style_editor.colorColor, null_bg_color, null_fg_color, style_editor.backgroundColor));
	item->properties.push_back(new StyleProperty(item, IT_EDITOR_COMMENT, style_editor.commentStyle, style_editor.commentColor, null_bg_color, null_fg_color, style_editor.backgroundColor));
	item->properties.push_back(new StyleProperty(item, IT_EDITOR_NUMBER, style_editor.numberStyle, style_editor.numberColor, null_bg_color, null_fg_color, style_editor.backgroundColor));
	item->properties.push_back(new StyleProperty(item, IT_EDITOR_STRING, style_editor.stringStyle, style_editor.stringColor, null_bg_color, null_fg_color, style_editor.backgroundColor));
	item->properties.push_back(new StyleProperty(item, IT_EDITOR_OPERATOR, style_editor.operatorStyle, style_editor.operatorColor, null_bg_color, null_fg_color, style_editor.backgroundColor));
	item->properties.push_back(new StyleProperty(item, IT_EDITOR_CARET, null_font_style, style_editor.caretColor, null_bg_color));
	item->properties.push_back(new StyleProperty(item, IT_EDITOR_TEXTSELECTION, null_font_style, null_fg_color, style_editor.selectionBgColor));
	item->properties.push_back(new StyleProperty(item, IT_EDITOR_BOOKMARK, null_font_style, style_editor.bookmarkFgColor, style_editor.bookmarkBgColor));
	item->properties.push_back(new StyleProperty(item, IT_EDITOR_FOLD, null_font_style, style_editor.foldFgColor, style_editor.foldBgColor));
	item->properties.push_back(new StyleProperty(item, IT_EDITOR_ERROR, null_font_style, null_fg_color, style_editor.errorBgColor));
	style_list.push_back(item);

	item = new StyleItem(IT_BUILD, style_build.font.size, style_build.font.name, style_build.window.wordWrap, style_build.window.showHorzScrollBar, null_bookmarkstyle, null_foldstyle, null_commentfold, style_build.warning);
	item->properties.push_back(new StyleProperty(item, IT_BUILD, style_build.defaultStyle, style_build.defaultColor, style_build.backgroundColor));
	item->properties.push_back(new StyleProperty(item, IT_BUILD_TEXT, style_build.defaultStyle, style_build.defaultColor, style_build.backgroundColor));
	item->properties.push_back(new StyleProperty(item, IT_BUILD_SELECTEDLINE, null_font_style, null_fg_color, style_build.selectLineBgColor));
	item->properties.push_back(new StyleProperty(item, IT_EDITOR_CARET, null_font_style, style_build.caretColor, null_bg_color));
	item->properties.push_back(new StyleProperty(item, IT_EDITOR_TEXTSELECTION, null_font_style, null_fg_color, style_build.selectionBgColor));
	item->properties.push_back(new StyleProperty(item, IT_EDITOR_BOOKMARK, null_font_style, null_fg_color, style_build.bookmarkBgColor));
	style_list.push_back(item);

	item = new StyleItem(IT_DEBUG, style_debug.font.size, style_debug.font.name, style_debug.window.wordWrap, style_debug.window.showHorzScrollBar);
	item->properties.push_back(new StyleProperty(item, IT_DEBUG, style_debug.defaultStyle, style_debug.defaultColor, style_debug.backgroundColor));
	item->properties.push_back(new StyleProperty(item, IT_BUILD_TEXT, style_debug.defaultStyle, style_debug.defaultColor, style_debug.backgroundColor));
	item->properties.push_back(new StyleProperty(item, IT_EDITOR_CARET, null_font_style, style_debug.caretColor, null_bg_color));
	item->properties.push_back(new StyleProperty(item, IT_EDITOR_TEXTSELECTION, null_font_style, null_fg_color, style_debug.selectionBgColor));
	item->properties.push_back(new StyleProperty(item, IT_EDITOR_BOOKMARK, null_font_style, null_fg_color, style_debug.bookmarkBgColor));
	style_list.push_back(item);

	item = new StyleItem(IT_LOG, style_trace.font.size, style_trace.font.name);
	item->properties.push_back(new StyleProperty(item, IT_LOG, style_trace.fontStyle, style_trace.defaultColor.foregroundColor, style_trace.defaultColor.backgroundColor));
	item->properties.push_back(new StyleProperty(item, IT_LOG_ES, style_trace.fontStyle, style_trace.es.foregroundColor, style_trace.es.backgroundColor));
	item->properties.push_back(new StyleProperty(item, IT_LOG_EB, style_trace.fontStyle, style_trace.eb.foregroundColor, style_trace.eb.backgroundColor));
	item->properties.push_back(new StyleProperty(item, IT_LOG_EF, style_trace.fontStyle, style_trace.ef.foregroundColor, style_trace.ef.backgroundColor));
	item->properties.push_back(new StyleProperty(item, IT_LOG_EI, style_trace.fontStyle, style_trace.ei.foregroundColor, style_trace.ei.backgroundColor));
	item->properties.push_back(new StyleProperty(item, IT_LOG_ER, style_trace.fontStyle, style_trace.er.foregroundColor, style_trace.er.backgroundColor));
	item->properties.push_back(new StyleProperty(item, IT_LOG_RC, style_trace.fontStyle, style_trace.rc.foregroundColor, style_trace.rc.backgroundColor));
	item->properties.push_back(new StyleProperty(item, IT_LOG_RE, style_trace.fontStyle, style_trace.re.foregroundColor, style_trace.re.backgroundColor));
	item->properties.push_back(new StyleProperty(item, IT_LOG_RK, style_trace.fontStyle, style_trace.rk.foregroundColor, style_trace.rk.backgroundColor));
	item->properties.push_back(new StyleProperty(item, IT_LOG_V, style_trace.fontStyle, style_trace.v.foregroundColor, style_trace.v.backgroundColor));
	item->properties.push_back(new StyleProperty(item, IT_LOG_STATUS, style_trace.fontStyle, style_trace.s.foregroundColor, style_trace.s.backgroundColor));
	item->properties.push_back(new StyleProperty(item, IT_LOG_DPS, style_trace.fontStyle, style_trace.dps.foregroundColor, style_trace.dps.backgroundColor));
	item->properties.push_back(new StyleProperty(item, IT_LOG_SB, style_trace.fontStyle, style_trace.sb.foregroundColor, style_trace.sb.backgroundColor));
	item->properties.push_back(new StyleProperty(item, IT_LOG_SO, style_trace.fontStyle, style_trace.so.foregroundColor, style_trace.so.backgroundColor));
	item->properties.push_back(new StyleProperty(item, IT_LOG_STN, style_trace.fontStyle, style_trace.stn.foregroundColor, style_trace.stn.backgroundColor));
	item->properties.push_back(new StyleProperty(item, IT_LOG_STD, style_trace.fontStyle, style_trace.std.foregroundColor, style_trace.std.backgroundColor));
	item->properties.push_back(new StyleProperty(item, IT_LOG_STR, style_trace.fontStyle, style_trace.str.foregroundColor, style_trace.str.backgroundColor));
	item->properties.push_back(new StyleProperty(item, IT_LOG_SRC, style_trace.fontStyle, style_trace.src.foregroundColor, style_trace.src.backgroundColor));
	item->properties.push_back(new StyleProperty(item, IT_LOG_SRE, style_trace.fontStyle, style_trace.sre.foregroundColor, style_trace.sre.backgroundColor));
	item->properties.push_back(new StyleProperty(item, IT_LOG_SRK, style_trace.fontStyle, style_trace.srk.foregroundColor, style_trace.srk.backgroundColor));
	item->properties.push_back(new StyleProperty(item, IT_LOG_SD, style_trace.fontStyle, style_trace.sd.foregroundColor, style_trace.sd.backgroundColor));
	item->properties.push_back(new StyleProperty(item, IT_LOG_SES, style_trace.fontStyle, style_trace.ses.foregroundColor, style_trace.ses.backgroundColor));
	item->properties.push_back(new StyleProperty(item, IT_LOG_SEN, style_trace.fontStyle, style_trace.sen.foregroundColor, style_trace.sen.backgroundColor));
	item->properties.push_back(new StyleProperty(item, IT_LOG_SEM, style_trace.fontStyle, style_trace.sem.foregroundColor, style_trace.sem.backgroundColor));
	item->properties.push_back(new StyleProperty(item, IT_LOG_SEF, style_trace.fontStyle, style_trace.sef.foregroundColor, style_trace.sef.backgroundColor));
	item->properties.push_back(new StyleProperty(item, IT_LOG_SEU, style_trace.fontStyle, style_trace.seu.foregroundColor, style_trace.seu.backgroundColor));
	style_list.push_back(item);

	item = new StyleItem(IT_RESULT, style_results.font.size, style_results.font.name, style_results.window.wordWrap, style_results.window.showHorzScrollBar);
	item->properties.push_back(new StyleProperty(item, IT_RESULT, style_results.identifierStyle, style_results.identifierColor, style_results.backgroundColor));
	item->properties.push_back(new StyleProperty(item, IT_EDITOR_PLAINTEXT, style_results.defaultStyle, style_results.defaultColor, null_bg_color, null_fg_color, style_results.backgroundColor));
	item->properties.push_back(new StyleProperty(item, IT_EDITOR_IDENTIFICATOR, style_results.identifierStyle, style_results.identifierColor, null_bg_color, null_fg_color, style_results.backgroundColor));
	item->properties.push_back(new StyleProperty(item, IT_EDITOR_KEYWORD, style_results.keywordStyle, style_results.keywordColor, null_bg_color, null_fg_color, style_results.backgroundColor));
	item->properties.push_back(new StyleProperty(item, IT_EDITOR_NUMBER, style_results.numberStyle, style_results.numberColor, null_bg_color, null_fg_color, style_results.backgroundColor));
	item->properties.push_back(new StyleProperty(item, IT_EDITOR_STRING, style_results.stringStyle, style_results.stringColor, null_bg_color, null_fg_color, style_results.backgroundColor));
	item->properties.push_back(new StyleProperty(item, IT_EDITOR_OPERATOR, style_results.operatorStyle, style_results.operatorColor, null_bg_color, null_fg_color, style_results.backgroundColor));
	item->properties.push_back(new StyleProperty(item, IT_EDITOR_CARET, null_font_style, style_results.caretColor, null_bg_color));
	item->properties.push_back(new StyleProperty(item, IT_EDITOR_TEXTSELECTION, null_font_style, null_fg_color, style_results.selectionBgColor));
	item->properties.push_back(new StyleProperty(item, IT_EDITOR_BOOKMARK, null_font_style, style_results.bookmarkFgColor, style_results.bookmarkBgColor));
	style_list.push_back(item);

	item = new StyleItem(IT_FIND, style_find.font.size, style_find.font.name, style_find.window.wordWrap, style_find.window.showHorzScrollBar);
	item->properties.push_back(new StyleProperty(item, IT_FIND, style_find.defaultStyle, style_find.defaultColor, style_find.backgroundColor));
	item->properties.push_back(new StyleProperty(item, IT_BUILD_TEXT, style_find.defaultStyle, style_find.defaultColor, style_find.backgroundColor));
	item->properties.push_back(new StyleProperty(item, IT_FIND_SEARCHTEXT, style_find.keywordStyle, style_find.keywordColor, null_bg_color, null_fg_color, style_find.backgroundColor));
	item->properties.push_back(new StyleProperty(item, IT_BUILD_SELECTEDLINE, null_font_style, null_fg_color, style_find.selectLineBgColor));
	item->properties.push_back(new StyleProperty(item, IT_EDITOR_CARET, null_font_style, style_find.caretColor, null_bg_color));
	item->properties.push_back(new StyleProperty(item, IT_EDITOR_TEXTSELECTION, null_font_style, null_fg_color, style_find.selectionBgColor));
	item->properties.push_back(new StyleProperty(item, IT_EDITOR_BOOKMARK, null_font_style, null_fg_color, style_find.bookmarkBgColor));
	style_list.push_back(item);

	item = new StyleItem(IT_CHART, style_chart.font.size, style_chart.font.name);
	item->properties.push_back(new StyleProperty(item, IT_CHART, style_chart.defaultStyle, style_chart.defaultColor, style_chart.backgroundColor));
	item->properties.push_back(new StyleProperty(item, IT_CHART_AXIS, style_chart.defaultStyle, style_chart.axisFgColor, null_bg_color, null_fg_color, style_chart.backgroundColor));
	item->properties.push_back(new StyleProperty(item, IT_CHART_TITLE, style_chart.titleStyle, style_chart.titleFGColor, null_bg_color, null_fg_color, style_chart.backgroundColor));
	item->properties.push_back(new StyleProperty(item, IT_CHART_LEGEND, style_chart.legendStyle, style_chart.legendFgColor, null_bg_color, null_fg_color, style_chart.backgroundColor));
	item->properties.push_back(new StyleProperty(item, IT_CHART_CHART, null_font_style, null_fg_color, style_chart.chartBgColor));
	item->properties.push_back(new StyleProperty(item, IT_CHART_TIME, null_font_style, null_fg_color, style_chart.timeBgColor));
	style_list.push_back(item);

	item = new StyleItem(IT_FRAME, style_frame.font.size, style_frame.font.name);
	item->properties.push_back(new StyleProperty(item, IT_FRAME, style_frame.defaultStyle, style_frame.defaultColor, style_frame.backgroundColor));
	item->properties.push_back(new StyleProperty(item, IT_FRAME_BORDER, style_frame.defaultStyle, style_frame.defaultColor, null_bg_color));
	item->properties.push_back(new StyleProperty(item, IT_FRAME_BACKGROUND, style_frame.defaultStyle, null_fg_color, style_frame.backgroundColor));
	style_list.push_back(item);
}

void ViewPreferences::createTree()
{
	treeWidget->setColumnCount(1);
	treeWidget->setHeaderHidden(true);
	treeWidget->setRootIsDecorated(false);

	m_pRoot = new QTreeWidgetItem(treeWidget);
	m_pRoot->setText(0, "Все окна");
	m_pRoot->setData(0, Qt::UserRole, IT_ROOT);

	m_pText      = createTreeItem(m_pRoot, "Исходный текст",   IT_EDITOR);
	m_pCompile   = createTreeItem(m_pRoot, "Окно компиляции",  IT_BUILD);
	m_pDebug     = createTreeItem(m_pRoot, "Окно отладки",     IT_DEBUG);
	m_pTrace     = createTreeItem(m_pRoot, "Окно трассировки", IT_LOG);
	m_pResult    = createTreeItem(m_pRoot, "Окно результатов", IT_RESULT);
	m_pSearch    = createTreeItem(m_pRoot, "Окно поиска",      IT_FIND);
	m_pChart     = createTreeItem(m_pRoot, "Окно графиков",    IT_CHART);
	m_pAnimation = createTreeItem(m_pRoot, "Окно анимации",    IT_FRAME);

	m_pRoot->setExpanded(true);

	m_pPlainText = createTreeItem(m_pText, "Обыкновенный текст", IT_EDITOR_PLAINTEXT);
	m_pVariable  = createTreeItem(m_pText, "Переменная",         IT_EDITOR_IDENTIFICATOR);
	m_pKeyword   = createTreeItem(m_pText, "Ключевое слово",     IT_EDITOR_KEYWORD);
	m_pFunction  = createTreeItem(m_pText, "Функция",            IT_EDITOR_FUNCTION);
	m_pTraceText = createTreeItem(m_pText, "Трассировка",        IT_EDITOR_TRACE);
	m_pColor     = createTreeItem(m_pText, "Цвет",               IT_EDITOR_COLOR);
	m_pComment   = createTreeItem(m_pText, "Комментарии",        IT_EDITOR_COMMENT);
	m_pNumber    = createTreeItem(m_pText, "Число",              IT_EDITOR_NUMBER);
	m_pString    = createTreeItem(m_pText, "Строка",             IT_EDITOR_STRING);
	m_pOperator  = createTreeItem(m_pText, "Оператор",           IT_EDITOR_OPERATOR);
	m_pCaret     = createTreeItem(m_pText, "Каретка",            IT_EDITOR_CARET);
	m_pSelection = createTreeItem(m_pText, "Выделение",          IT_EDITOR_TEXTSELECTION);
	m_pBookmark  = createTreeItem(m_pText, "Закладка",           IT_EDITOR_BOOKMARK);
	m_pGroup     = createTreeItem(m_pText, "Группа",             IT_EDITOR_FOLD);
	m_pError     = createTreeItem(m_pText, "Ошибка",             IT_EDITOR_ERROR);

	m_pTextCompile      = createTreeItem(m_pCompile, "Текст",             IT_BUILD_TEXT);
	m_pSelectedString   = createTreeItem(m_pCompile, "Выделенная строка", IT_BUILD_SELECTEDLINE);
	m_pCaretCompile     = createTreeItem(m_pCompile, "Каретка",           IT_EDITOR_CARET);
	m_pSelectionCompile = createTreeItem(m_pCompile, "Выделение",         IT_EDITOR_TEXTSELECTION);
	m_pBookmarkCompile  = createTreeItem(m_pCompile, "Закладка",          IT_EDITOR_BOOKMARK);

	m_pTextDebug      = createTreeItem(m_pDebug, "Текст",     IT_BUILD_TEXT);
	m_pCaretDebug     = createTreeItem(m_pDebug, "Каретка",   IT_EDITOR_CARET);
	m_pSelectionDebug = createTreeItem(m_pDebug, "Выделение", IT_EDITOR_TEXTSELECTION);
	m_pBookmarkDebug  = createTreeItem(m_pDebug, "Закладка",  IT_EDITOR_BOOKMARK);

	m_pES     = createTreeItem(m_pTrace, "Служебное событие (ES)",                         IT_LOG_ES);
	m_pEB     = createTreeItem(m_pTrace, "Начало действия (EB)",                           IT_LOG_EB);
	m_pEF     = createTreeItem(m_pTrace, "Окончание действия (EF)",                        IT_LOG_EF);
	m_pEI     = createTreeItem(m_pTrace, "Нерегулярное событие (EI)",                      IT_LOG_EI);
	m_pER     = createTreeItem(m_pTrace, "Продукционное правило (ER)",                     IT_LOG_ER);
	m_pRC     = createTreeItem(m_pTrace, "Создание ресурса (RC)",                          IT_LOG_RC);
	m_pRE     = createTreeItem(m_pTrace, "Удаление ресурса (RE)",                          IT_LOG_RE);
	m_pRK     = createTreeItem(m_pTrace, "Изменение состояния ресурса (RK)",               IT_LOG_RK);
	m_pV      = createTreeItem(m_pTrace, "Трассировка индекса (V)",                        IT_LOG_V);
	m_pStatus = createTreeItem(m_pTrace, "Статус окончания моделирования ($Status)",       IT_LOG_STATUS);
	m_pDPS    = createTreeItem(m_pTrace, "Статистика по поиску на графе (DPS)",            IT_LOG_DPS);
	m_pSB     = createTreeItem(m_pTrace, "Начало поиска (SB)",                             IT_LOG_SB);
	m_pSO     = createTreeItem(m_pTrace, "Трассировка раскрываемой вершины (SO)",          IT_LOG_SO);
	m_pSTN    = createTreeItem(m_pTrace, "Признак вершины (STN)",                          IT_LOG_STN);
	m_pSTD    = createTreeItem(m_pTrace, "Признак вершины (STD)",                          IT_LOG_STD);
	m_pSTR    = createTreeItem(m_pTrace, "Признак вершины (STR)",                          IT_LOG_STR);
	m_pSRC    = createTreeItem(m_pTrace, "Создание ресурса (при поиске) (SRC)",            IT_LOG_SRC);
	m_pSRE    = createTreeItem(m_pTrace, "Удаление ресурса (при поиске) (SRE)",            IT_LOG_SRE);
	m_pSRK    = createTreeItem(m_pTrace, "Изменение состояния ресурса (при поиске) (SRK)", IT_LOG_SRK);
	m_pSD     = createTreeItem(m_pTrace, "Трассировка решения (SD)",                       IT_LOG_SD);
	m_pSES    = createTreeItem(m_pTrace, "Завершение поиска (SES)",                        IT_LOG_SES);
	m_pSEN    = createTreeItem(m_pTrace, "Завершение поиска (SEN)",                        IT_LOG_SEN);
	m_pSEM    = createTreeItem(m_pTrace, "Завершение поиска (SEM)",                        IT_LOG_SEM);
	m_pSEF    = createTreeItem(m_pTrace, "Завершение поиска (SEF)",                        IT_LOG_SEF);
	m_pSEU    = createTreeItem(m_pTrace, "Завершение поиска (SEU)",                        IT_LOG_SEU);

	m_pPlainTextResult = createTreeItem(m_pResult, "Исходный текст", IT_EDITOR_PLAINTEXT);
	m_pVariableResult  = createTreeItem(m_pResult, "Переменная",     IT_EDITOR_IDENTIFICATOR);
	m_pKeywordResult   = createTreeItem(m_pResult, "Ключевое слово", IT_EDITOR_KEYWORD);
	m_pNumberResult    = createTreeItem(m_pResult, "Число",          IT_EDITOR_NUMBER);
	m_pStringResult    = createTreeItem(m_pResult, "Строка",         IT_EDITOR_STRING);
	m_pOperatorResult  = createTreeItem(m_pResult, "Оператор",       IT_EDITOR_OPERATOR);
	m_pCaretResult     = createTreeItem(m_pResult, "Каретка",        IT_EDITOR_CARET);
	m_pSelectionResult = createTreeItem(m_pResult, "Выделение",      IT_EDITOR_TEXTSELECTION);
	m_pBookmarkResult  = createTreeItem(m_pResult, "Закладка",       IT_EDITOR_BOOKMARK);

	m_pTextSearch           = createTreeItem(m_pSearch, "Текст",             IT_BUILD_TEXT);
	m_pStringSearch         = createTreeItem(m_pSearch, "Строка для поиска", IT_FIND_SEARCHTEXT);
	m_pSelectedStringSearch = createTreeItem(m_pSearch, "Выделенная строка", IT_BUILD_SELECTEDLINE);
	m_pCaretSearch          = createTreeItem(m_pSearch, "Каретка",           IT_EDITOR_CARET);
	m_pSelectionSearch      = createTreeItem(m_pSearch, "Выделение",         IT_EDITOR_TEXTSELECTION);
	m_pBookmarkSearch       = createTreeItem(m_pSearch, "Закладка",          IT_EDITOR_BOOKMARK);

	m_pAxis   = createTreeItem(m_pChart, "Ось",       IT_CHART_AXIS);
	m_pTitle  = createTreeItem(m_pChart, "Заголовок", IT_CHART_TITLE);
	m_pLegend = createTreeItem(m_pChart, "Легенда",   IT_CHART_LEGEND);
	m_pGraph  = createTreeItem(m_pChart, "График",    IT_CHART_CHART);
	m_pTime   = createTreeItem(m_pChart, "Время",     IT_CHART_TIME);

	m_pEdgingColor     = createTreeItem(m_pAnimation, "Цвет окантовки",               IT_FRAME_BORDER);
	m_pBackgroundColor = createTreeItem(m_pAnimation, "Цвет фона за пределами кадра", IT_FRAME_BACKGROUND);

	treeWidget->setCurrentItem(m_pRoot);
}

PTR(QTreeWidgetItem) ViewPreferences::createTreeItem(PTR(QTreeWidgetItem) parent, CREF(QString) name, ItemType itemType)
{
	PTR(QTreeWidgetItem) item = new QTreeWidgetItem(parent);
	item->setText(0, name);
	item->setData(0, Qt::UserRole, QVariant(itemType));
	return item;
}

void ViewPreferences::insertColors(QComboBox* colorBox)
{
	insertColor(QColor(0x00, 0x80, 0x00), "Green",   colorBox);
	insertColor(QColor(0x00, 0x00, 0x80), "Navy",    colorBox);
	insertColor(QColor(0x80, 0x80, 0x80), "Grey",    colorBox);
	insertColor(QColor(0x80, 0x00, 0x80), "Purple",  colorBox);
	insertColor(QColor(0xFF, 0x00, 0x00), "Red",     colorBox);
	insertColor(QColor(0x00, 0xFF, 0x00), "Lime",    colorBox);
	insertColor(QColor(0x00, 0x00, 0x00), "Black",   colorBox);
	insertColor(QColor(0xFF, 0xFF, 0xFF), "White",   colorBox);
	insertColor(QColor(0x80, 0x80, 0x00), "Olive",   colorBox);
	insertColor(QColor(0xC0, 0xC0, 0xC0), "Silver",  colorBox);
	insertColor(QColor(0x80, 0x00, 0x00), "Maroon",  colorBox);
	insertColor(QColor(0x00, 0x80, 0x80), "Teal",    colorBox);
	insertColor(QColor(0xFF, 0xFF, 0x00), "Yellow",  colorBox);
	insertColor(QColor(0x00, 0x00, 0xFF), "Blue",    colorBox);
	insertColor(QColor(0xFF, 0x00, 0xFF), "Fushcia", colorBox);
	insertColor(QColor(0x00, 0xFF, 0xFF), "Aqua",    colorBox);
}

void ViewPreferences::insertColor(const QColor& color, const QString& colorName, QComboBox* colorBox)
{
	colorBox->addItem(colorName);
	int size = colorBox ->style()->pixelMetric(QStyle::PM_SmallIconSize);
	QPixmap pixmap(size,size-5);
	pixmap.fill(color);
	QRect rBorder(0,0,size-1,size-6);
	QPainter p(&pixmap);
	QPen pen(Qt::black, 1, Qt::SolidLine);
	p.setPen(pen);
	p.drawRect(rBorder);
	colorBox->setItemData(colorBox->findText(colorName), color, Qt::UserRole);
	colorBox->setItemIcon(colorBox->findText(colorName), QIcon(pixmap));
}

PTR(ViewPreferences::StyleProperty) ViewPreferences::getStyleProperty()
{
	PTR(StyleItem) item = getStyleItem();
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
	g_pApp->getStyle()->style_editor  = style_editor;
	g_pApp->getStyle()->style_build   = style_build;
	g_pApp->getStyle()->style_debug   = style_debug;
	g_pApp->getStyle()->style_trace   = style_trace;
	g_pApp->getStyle()->style_results = style_results;
	g_pApp->getStyle()->style_find    = style_find;
	g_pApp->getStyle()->style_frame   = style_frame;
	g_pApp->getStyle()->style_chart   = style_chart;
	g_pApp->setFileAssociationSetup(m_setup);
	g_pApp->setFileAssociationCheckInFuture(m_checkInFuture);
	g_pApp->setOpenLastProject(m_openLastProject);
	g_pApp->setShowCaptionFullName(m_showFullName);
	g_pApp->getStyle()->updateAllStyles();
	updatePluginList();
	g_pApp->getPluginLoader().setPluginInfoList(*m_pPluginInfoList);
}

void ViewPreferences::checkAllData()
{
	bool setupFlag           = m_setup           == g_pApp->getFileAssociationSetup() ? true : false;
	bool checkInFutureFlag   = m_checkInFuture   == g_pApp->getFileAssociationCheckInFuture() ? true : false;
	bool openLastProjectFlag = m_openLastProject == g_pApp->getOpenLastProject() ? true : false;
	bool showFullNameFlag    = m_showFullName    == g_pApp->getShowCaptionFullName() ? true : false;

	bool style_editor_flag  = style_editor  == g_pApp->getStyle()->style_editor ? true : false;
	bool style_build_flag   = style_build   == g_pApp->getStyle()->style_build ? true : false;
	bool style_debug_flag   = style_debug   == g_pApp->getStyle()->style_debug ? true : false;
	bool style_trace_flag   = style_trace   == g_pApp->getStyle()->style_trace ? true : false;
	bool style_results_flag = style_results == g_pApp->getStyle()->style_results ? true : false;
	bool style_find_flag    = style_find    == g_pApp->getStyle()->style_find ? true : false;
	bool style_chart_flag   = style_chart   == g_pApp->getStyle()->style_chart ? true : false;
	bool style_frame_flag   = style_frame   == g_pApp->getStyle()->style_frame ? true : false;

	if(setupFlag
		&& checkInFutureFlag
		&& openLastProjectFlag
		&& showFullNameFlag
		&& style_editor_flag
		&& style_build_flag
		&& style_debug_flag
		&& style_trace_flag
		&& style_results_flag
		&& style_find_flag
		&& style_chart_flag
		&& style_frame_flag)
		buttonApply->setEnabled(false);
	else
		buttonApply->setEnabled(true);
}

void ViewPreferences::keyPressEvent(QKeyEvent* pEvent)
{
	if (QKeySequence(pEvent->key()) == QKeySequence::HelpContents)
	{
		onHelpContext();
	}
}

void ViewPreferences::deletePlugin()
{
	onStopPlugin();
	const std::vector<int> rows = selectedRows();
	for(int i = rows.size()-1; i >= 0; i--)
	{
		const int current = rows[i];
		{
			const LPPluginInfo plgnInfo = getPluginInfoFromTable(current);
			QPluginLoader* loaderPtr = plgnInfo->getLoader();
			if (loaderPtr)
			{
				loaderPtr->unload();
				QFile::remove(loaderPtr->fileName());
			}
			m_pPluginInfoList->remove(plgnInfo);
			pluginInfoTable->removeRow(current);
		}
	}
	updateButtonsState();
}

void ViewPreferences::onStartPlugin()
{
	const std::vector<int> rows = selectedRows();
	for(int i = rows.size()-1; i >= 0; i--)
	{
		const int current = rows[i];
		const LPPluginInfo plgnInfo = getPluginInfoFromTable(current);
		if (!plgnInfo->isActive() && plgnInfo->isAvailable())
		{
			g_pApp->getPluginLoader().startPlugin(plgnInfo);
		}
	}
	pluginInfoTable->setFocus();
	updateButtonsState();
}

void ViewPreferences::onStopPlugin()
{
	const std::vector<int> rows = selectedRows();
	for(int i = rows.size()-1; i >= 0; i--)
	{
		const int current  = rows[i];
		const LPPluginInfo plgnInfo = getPluginInfoFromTable(current);
		if (plgnInfo->isActive() && plgnInfo->isAvailable())
		{
			g_pApp->getPluginLoader().stopPlugin(plgnInfo);
		}
	}
	updateButtonsState();
}

void ViewPreferences::populateRow(const LPPluginInfo& plgInfo)
{
	int currentRow = pluginInfoTable->rowCount();
	pluginInfoTable->setRowCount(currentRow + 1);
	QTableWidgetItem* pNameItem (new QTableWidgetItem);
	QTableWidgetItem* pCheckItem(new QTableWidgetItem);
	pNameItem ->setText(plgInfo->getName());
	pNameItem ->setData(Qt::UserRole,QVariant::fromValue(plgInfo));
	pCheckItem->setFlags     (pCheckItem->flags() | Qt::ItemIsUserCheckable);
	pCheckItem->setCheckState(plgInfo->getAutoload() ? Qt::Checked : Qt::Unchecked);
	pluginInfoTable->setItem(currentRow, 0, pNameItem);
	pluginInfoTable->setItem(currentRow, 1, new QTableWidgetItem(plgInfo->getVersion()));
	pluginInfoTable->setItem(currentRow, 2, new QTableWidgetItem(plgInfo->getAuthor ()));
	pluginInfoTable->setItem(currentRow, 3, pCheckItem);
}

void ViewPreferences::fillPluginInfoTable()
{
	BOOST_FOREACH(const LPPluginInfo& pluginInfo, *m_pPluginInfoList)
	{
		populateRow(pluginInfo);
	}
}
void ViewPreferences::updateButtonsState()
{
	const std::vector<int> rows = selectedRows();
	bool allActive      = true;
	bool allInactive    = true;
	bool allUnavailable = true;
	for(int i = rows.size()-1; i >= 0; i--)
	{
		const int current = rows[i];
		const LPPluginInfo plgnInfo = getPluginInfoFromTable(current);
		if (plgnInfo->isAvailable())
		{
			allActive      = allActive   &&   plgnInfo->isActive() ;
			allInactive    = allInactive && !(plgnInfo->isActive());
			allUnavailable = false;
		}
	}
	buttonStartPlugin ->setEnabled(!allActive   && !allUnavailable);
	buttonStopPlugin  ->setEnabled(!allInactive && !allUnavailable);
	buttonDeletePlugin->setEnabled(!m_pPluginInfoList->empty());
}

ViewPreferences::IntVector ViewPreferences::selectedRows() const
{
	std::vector<int> sortedRows;
	BOOST_FOREACH(const QModelIndex& index, pluginInfoTable->selectionModel()->selectedRows())
	{
		sortedRows.push_back(index.row()); 
	}

	std::sort(sortedRows.begin(), sortedRows.end());
	return sortedRows;
}

LPPluginInfo ViewPreferences::getPluginInfoFromTable(int pluginRow) const
{
	const QTableWidgetItem* itm = pluginInfoTable->item(pluginRow,0);
	LPPluginInfo plgnInfo = itm->data(Qt::UserRole).value<LPPluginInfo>();
	return plgnInfo;
}

void ViewPreferences::updatePluginList()
{
	for (int itmRow=0; itmRow < pluginInfoTable->rowCount(); itmRow++)
	{
		LPPluginInfo plgnInfo = getPluginInfoFromTable(itmRow);
		bool autoLoadValue = pluginInfoTable->item(itmRow,3)->checkState() == Qt::Checked;
		plgnInfo->setAutoload(autoLoadValue);
	}
}