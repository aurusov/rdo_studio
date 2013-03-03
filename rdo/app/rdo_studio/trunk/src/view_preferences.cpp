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
#include "ui/qt/headers/validator/int_validator.h"
// --------------------------------------------------------------------------------

using namespace rdo::simulation::report;
using namespace rdo::gui::editor;
using namespace rdo::gui::style;
using namespace rdo::gui::tracer;

rbool ViewPreferences::null_wordwrap      = false;
rbool ViewPreferences::null_horzscrollbar = true;
rbool ViewPreferences::null_warning       = true;
rbool ViewPreferences::null_commentfold   = false;
EditTheme::Bookmark ViewPreferences::null_bookmarkstyle = EditTheme::B_NONE;
ModelTheme::Fold    ViewPreferences::null_foldstyle     = ModelTheme::F_NONE;
QColor ViewPreferences::null_fg_color = QColor(0x00, 0x00, 0x00);
QColor ViewPreferences::null_bg_color = QColor(0xFF, 0xFF, 0xFF);

ViewPreferences::ViewPreferences(PTR(QWidget) pParent)
	: QDialog(pParent)
	, all_font_size(-1)
	, all_font_name("")
	, null_font_style(StyleFont::NONE)
	, all_fg_color(0x00, 0x00, 0x00)
	, all_bg_color(0xFF, 0xFF, 0xFF)
{
	setupUi(this);

	connect(buttonOk, SIGNAL(clicked()), this, SLOT(onOkButton()));
	connect(buttonCancel, SIGNAL(clicked()), this, SLOT(onCancelButton()));
	connect(buttonApply, SIGNAL(clicked()), this, SLOT(onApplyButton()));
	
	m_setup           = g_pApp->getFileAssociationSetup();
	m_checkInFuture   = g_pApp->getFileAssociationCheckInFuture();
	m_openLastProject = g_pApp->getOpenLastProject();
	m_showFullName    = g_pApp->getShowCaptionFullName();

	style_editor.init();
	style_build.init();
	style_debug.init();
	style_trace.init();
	style_results.init();
	style_find.init();
	style_chart.init();
	style_frame.init();

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
	tabSizeLineEdit->setValidator(new rdo::gui::IntValidator(1, 100, this));
	tabSizeLineEdit->setText(QString::number(style_editor.tab->tabSize));
	indentSizeLineEdit->setValidator(new rdo::gui::IntValidator(1, 100, this));
	indentSizeLineEdit->setText(QString::number(style_editor.tab->indentSize));
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

	bookmarkComboBox->addItem("Нет",           EditTheme::B_NONE);
	bookmarkComboBox->addItem("Круг",          EditTheme::B_CIRCLE);
	bookmarkComboBox->addItem("Прямоугольник", EditTheme::B_RECT);
	bookmarkComboBox->addItem("Овал",          EditTheme::B_ROUNDRECT);
	bookmarkComboBox->addItem("Стрелка",       EditTheme::B_ARROW);

	foldComboBox->addItem("Нет",             ModelTheme::F_NONE);
	foldComboBox->addItem("Плюс",            ModelTheme::F_PLUS);
	foldComboBox->addItem("Плюс + линия",    ModelTheme::F_PLUSCONNECTED);
	foldComboBox->addItem("Стрелка",         ModelTheme::F_ARROW);
	foldComboBox->addItem("Стрелка + линия", ModelTheme::F_ARROWCONNECTED);
	foldComboBox->addItem("Квадрат + линия", ModelTheme::F_BOXCONNECTED);
	foldComboBox->addItem("Круг + линия",    ModelTheme::F_CIRCLECONNECTED);

	boldCheckBox->setEnabled(false);
	italicCheckBox->setEnabled(false);
	underlineCheckBox->setEnabled(false);

	horzIndentLineEdit->setValidator(new rdo::gui::IntValidator(1, 100, this));
	horzIndentLineEdit->setText(QString::number(style_trace.borders->horzBorder));
	vertIndentLineEdit->setValidator(new rdo::gui::IntValidator(1, 100, this));
	vertIndentLineEdit->setText(QString::number(style_trace.borders->vertBorder));

	tickWidthLineEdit->setValidator(new rdo::gui::IntValidator(1, 100, this));
	tickWidthLineEdit->setText(QString::number(style_chart.pFontsTicks->tickWidth));

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

void ViewPreferences::onCodeCompUse(int state)
{
	style_editor.autoComplete->useAutoComplete = state == Qt::Checked ? true : false;
	
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
		style_editor.autoComplete->showFullList = true;
	if(radioButtonNearestWords->isChecked())
		style_editor.autoComplete->showFullList = false;
	checkAllData();
}

void ViewPreferences::onMarginFold(int state)
{
	style_editor.margin->fold = state == Qt::Checked ? true : false;
	checkAllData();
}

void ViewPreferences::onMarginBookmark(int state)
{
	style_editor.margin->bookmark = state == Qt::Checked ? true : false;
	checkAllData();
}

void ViewPreferences::onMarginLineNumber(int state)
{
	style_editor.margin->lineNumber = state == Qt::Checked ? true : false;
	checkAllData();
}

void ViewPreferences::onUseTabSymbol(int state)
{
	style_editor.tab->useTabs = state == Qt::Checked ? true : false;
	checkAllData();
}

void ViewPreferences::onIndentAsTab(int state)
{
	style_editor.tab->tabIndents = state == Qt::Checked ? true : false;
	checkAllData();
}

void ViewPreferences::onAutoIndent(int state)
{
	style_editor.tab->autoIndent = state == Qt::Checked ? true : false;
	checkAllData();
}

void ViewPreferences::onEraseWithTab(bool state)
{
	UNUSED(state);

	if(eraseWithTabRadioButton->isChecked())
		style_editor.tab->backspaceUntabs = true;
	if(eraseWithIndentRadioButton->isChecked())
		style_editor.tab->backspaceUntabs = false;
	checkAllData();
}

void ViewPreferences::onTabSize(const QString& text)
{
	style_editor.tab->tabSize = text.toInt();
	checkAllData();
}

void ViewPreferences::onIndentSize(const QString& text)
{
	style_editor.tab->indentSize = text.toInt();
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
		style_editor.font->size  = size;
		style_build.font->size   = size;
		style_debug.font->size   = size;
		style_trace.font->size   = size;
		style_results.font->size = size;
		style_find.font->size    = size;
		style_chart.font->size   = size;
		style_frame.font->size   = size;
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
		style_chart.font->size   = size;
		break;
	case IT_FRAME:
		style_frame.font->size   = size;
		break;
	}

	updatePreview();
}

void ViewPreferences::onFontType(int index)
{
	UNUSED(index);

	QString name = fontComboBox->currentFont().family();
	QByteArray text = name.toLocal8Bit();

	switch(getStyleItem()->type)
	{
	case IT_ROOT:
		all_font_name = name.toLocal8Bit().constData();
		style_editor.font->name  = text.constData();
		style_build.font->name   = text.constData();
		style_debug.font->name   = text.constData();
		style_trace.font->name   = text.constData();
		style_results.font->name = text.constData();
		style_find.font->name    = text.constData();
		style_chart.font->name   = text.constData();
		style_frame.font->name   = text.constData();
		break;
	case IT_EDITOR:
		style_editor.font->name = text.constData();
		break;
	case IT_BUILD:
		style_build.font->name = text.constData();
		break;
	case IT_DEBUG:
		style_debug.font->name   = text.constData();
		break;
	case IT_LOG:
		style_trace.font->name = text.constData();
		break;
	case IT_RESULT:
		style_results.font->name = text.constData();
		break;
	case IT_FIND:
		style_find.font->name    = text.constData();
		break;
	case IT_CHART:
		style_chart.font->name   = text.constData();
		break;
	case IT_FRAME:
		style_frame.font->name   = text.constData();
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
		}
	}
	updatePreview();
}

void ViewPreferences::onBookmark(int index)
{
	PTR(StyleItem) item = getStyleItem();
	item->bookmarkstyle = static_cast<EditTheme::Bookmark>(index);
	updatePreview();
}

void ViewPreferences::onFold(int index)
{
	PTR(StyleItem) item = getStyleItem();
	item->foldstyle = static_cast<ModelTheme::Fold>(index);
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
	style_trace.borders->horzBorder = text.toInt();
	updatePreview();
}

void ViewPreferences::onVertIndent(const QString& text)
{
	style_trace.borders->vertBorder = text.toInt();
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
	style_chart.pFontsTicks->titleFontSize = titleComboBox->currentText().toInt();
	updatePreview();
}

void ViewPreferences::onLegendSize(int index)
{
	UNUSED(index);
	style_chart.pFontsTicks->legendFontSize = legendComboBox->currentText().toInt();
	updatePreview();
}

void ViewPreferences::onTickWidth(const QString& text)
{
	style_chart.pFontsTicks->tickWidth = text.toInt();
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
	useTabSymbolCheckBox->setCheckState(style_editor.tab->useTabs
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
	style_editor.tab->backspaceUntabs
		? eraseWithTabRadioButton->toggle()
		: eraseWithIndentRadioButton->toggle();

	updateStyleTab();
}

void ViewPreferences::updateStyleTab()
{
	PTR(StyleProperty) prop = getStyleProperty();
	QString fontName = QString::fromLocal8Bit(prop->item->font_name.c_str());
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
		if((all_font_size == style_editor.font->size)  &&
		   (all_font_size == style_debug.font->size)   &&
		   (all_font_size == style_build.font->size)   &&
		   (all_font_size == style_find.font->size)    &&
		   (all_font_size == style_frame.font->size)   &&
		   (all_font_size == style_results.font->size) &&
		   (all_font_size == style_trace.font->size)   &&
		   (all_font_size == style_chart.font->size)
		)
		{
			fontSizeComboBox->setCurrentIndex(fontSizeComboBox->findText(QString::number(prop->item->font_size)));
		}
		else
			fontSizeComboBox->setCurrentIndex(-1);
		
		if((all_font_name == style_editor.font->name)  &&
		   (all_font_name == style_debug.font->name)   &&
		   (all_font_name == style_build.font->name)   &&
		   (all_font_name == style_find.font->name)    &&
	       (all_font_name == style_frame.font->name)   &&
	       (all_font_name == style_results.font->name) &&
		   (all_font_name == style_trace.font->name)   &&
		   (all_font_name == style_chart.font->name)
		)
		{
			fontComboBox->setCurrentFont(QFont(QString::fromLocal8Bit(all_font_name.c_str())));
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
		vertIndentLineEdit->setText(QString::number(style_trace.borders->vertBorder));
		horzIndentLineEdit->setText(QString::number(style_trace.borders->horzBorder));
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
		titleComboBox->setCurrentIndex(titleComboBox->findText(QString::number(style_chart.pFontsTicks->titleFontSize)));
		legendComboBox->setCurrentIndex(legendComboBox->findText(QString::number(style_chart.pFontsTicks->legendFontSize)));
		tickWidthLineEdit->setText(QString::number(style_chart.pFontsTicks->tickWidth));
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
	
	//! todo падение при закрытии диалога
	//preview_trace->view().setStyle(&style_trace);
	//preview_trace->repaint();

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
	preview_editor->appendText(QString("{ comments }\n$Pattern pattern_name : operation trace\n$Relevant_resources\n  rel_res2  : res_type2     Keep    Keep\n  rel_res1  : res_type1     Create  NoChange\n$Time = Abs(rel_res2.par1 - rel_res2.par3)\n{...}\n$End\n\ntext [ 10, 20, ... = 'text' ]\n\n$Re levant_resources"));
	preview_editor->scrollToLine(0);
	preview_editor->setReadOnly(true);
	preview_editor->bookmarkToggle();
	preview_editor->setErrorLine(preview_editor->getLineCount() - 1);
	previewStackedWidget->addWidget(preview_editor);

	preview_build = new Build(previewStackedWidget->currentWidget());
	preview_build->setEditorStyle(&style_build);
	preview_build->appendLine(new BuildEditLineInfo(QString("Компиляция...").toLocal8Bit().constData()));
	preview_build->appendLine(new BuildEditLineInfo(rdo::simulation::report::FileMessage(QString("Неправильное значение параметра: 4").toLocal8Bit().constData(), rdoModelObjects::PAT, 40, 0)));
	preview_build->appendLine(new BuildEditLineInfo(QString("найдено ошибок: 1, предупреждений: 0").toLocal8Bit().constData()));
	preview_build->gotoNext();
	previewStackedWidget->addWidget(preview_build);

	preview_debug = new Debug(previewStackedWidget->currentWidget());
	preview_debug->setEditorStyle(&style_debug);
	preview_debug->appendLine("Получение структуры модели...ok\nМодель запущена");
	previewStackedWidget->addWidget(preview_debug);

	preview_trace = new rdo::gui::tracer::LogMainWnd(previewStackedWidget->currentWidget());
	preview_trace->view().setStyle(&g_pApp->getStyle()->style_trace);
	preview_trace->view().setFocusOnly(true);
	preview_trace->view().setText(QString("Простая строка\nES 0 3\nEB 0 1 1 2 2 1 2\nEF 0.335153 1 1 2 2 1 2\nEI 0.427752 1 1 2 1 2\nER 1.07933 2 2 3 1 3\nRC 0.427752 2 2 0 0.427752 0\nRE 0.335153 2 2\nRK 0.427752 1 1 1 1 1\nV  0.427752 1  1\n$Status = USER_BREAK    607.228\nDPS_C  1  1  1\nSB 0 1\nSO 1 0 0 0\nSTN 3 1 3 6 2 1 3 2  5 6\nSTD 4 3 7 11 1 1 4 2  5 6\nSTR 5 2 8 12 2 1 4 2  1 6\nSRC 0 1 1 1 4\nSRE 0 1 2 2 4\nSRK 0 1 2 2 2\nSD\nSES 0 0.065 397312 10 8 13 13 19\nSEN 0 0.065 397312 10 8 13 13 19\nSEM 0 0.065 397312 10 8 13 13 19\nSEF 0 0.065 397312 10 8 13 13 19\nSEU 0 0.065 397312 10 8 13 13 19").toLocal8Bit().constData());
	preview_trace->view().selectLine(0);
	previewStackedWidget->addWidget(preview_trace);

	preview_results = new Results(previewStackedWidget->currentWidget());
	preview_results->setEditorStyle(&style_results);
	preview_results->setReadOnly(false);
	preview_results->replaceCurrent(QString("Длина_очереди                2  194  0.675957  21.6506  0  4\r\nЗанятость_парикмахера         TRUE  96  0.877351  21.7041  0.0397544  0.918872\r\nВсего_обслужено              96\r\nПропускная_способность       1.99198\r\n").toLocal8Bit().constData(), 0);
	preview_results->setReadOnly(true);
	previewStackedWidget->addWidget(preview_results);

	preview_find = new Find(previewStackedWidget->currentWidget());
	preview_find->setEditorStyle(&style_find);
	preview_find->setKeyword("$Time");
	preview_find->appendLine(new LogEditLineInfo(QString("Поиск '$Time'...").toLocal8Bit().constData()));
	preview_find->appendLine(new LogEditLineInfo(rdo::simulation::report::FileMessage(QString("$Time = Равномерный(0.25, 0.75)").toLocal8Bit().constData(), rdoModelObjects::PAT, 3, 0)));
	preview_find->appendLine(new LogEditLineInfo(rdo::simulation::report::FileMessage(QString("$Time = Нормальный(0.45, 0.2)").toLocal8Bit().constData(), rdoModelObjects::PAT, 13, 0)));
	preview_find->appendLine(new LogEditLineInfo(QString("'2' раз было найдено.").toLocal8Bit().constData()));
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

	ModelTheme* editor_theme = static_cast<ModelTheme*>(style_editor.theme);
	item = new StyleItem(IT_EDITOR, style_editor.font->size, style_editor.font->name, style_editor.window->wordWrap, style_editor.window->showHorzScrollBar, editor_theme->bookmarkStyle, editor_theme->foldStyle, editor_theme->commentFold);
	item->properties.push_back(new StyleProperty(item, IT_EDITOR, editor_theme->identifierStyle, editor_theme->identifierColor, editor_theme->backgroundColor));
	item->properties.push_back(new StyleProperty(item, IT_EDITOR_PLAINTEXT, editor_theme->defaultStyle, editor_theme->defaultColor, null_bg_color, null_fg_color, editor_theme->backgroundColor));
	item->properties.push_back(new StyleProperty(item, IT_EDITOR_IDENTIFICATOR, editor_theme->identifierStyle, editor_theme->identifierColor, null_bg_color, null_fg_color, editor_theme->backgroundColor));
	item->properties.push_back(new StyleProperty(item, IT_EDITOR_KEYWORD, editor_theme->keywordStyle, editor_theme->keywordColor, null_bg_color, null_fg_color, editor_theme->backgroundColor));
	item->properties.push_back(new StyleProperty(item, IT_EDITOR_FUNCTION, editor_theme->functionsStyle, editor_theme->functionsColor, null_bg_color, null_fg_color, editor_theme->backgroundColor));
	item->properties.push_back(new StyleProperty(item, IT_EDITOR_TRACE, editor_theme->traceStyle, editor_theme->traceColor, null_bg_color, null_fg_color, editor_theme->backgroundColor));
	item->properties.push_back(new StyleProperty(item, IT_EDITOR_COLOR, editor_theme->colorStyle, editor_theme->colorColor, null_bg_color, null_fg_color, editor_theme->backgroundColor));
	item->properties.push_back(new StyleProperty(item, IT_EDITOR_COMMENT, editor_theme->commentStyle, editor_theme->commentColor, null_bg_color, null_fg_color, editor_theme->backgroundColor));
	item->properties.push_back(new StyleProperty(item, IT_EDITOR_NUMBER, editor_theme->numberStyle, editor_theme->numberColor, null_bg_color, null_fg_color, editor_theme->backgroundColor));
	item->properties.push_back(new StyleProperty(item, IT_EDITOR_STRING, editor_theme->stringStyle, editor_theme->stringColor, null_bg_color, null_fg_color, editor_theme->backgroundColor));
	item->properties.push_back(new StyleProperty(item, IT_EDITOR_OPERATOR, editor_theme->operatorStyle, editor_theme->operatorColor, null_bg_color, null_fg_color, editor_theme->backgroundColor));
	item->properties.push_back(new StyleProperty(item, IT_EDITOR_CARET, null_font_style, editor_theme->caretColor, null_bg_color));
	item->properties.push_back(new StyleProperty(item, IT_EDITOR_TEXTSELECTION, null_font_style, null_fg_color, editor_theme->selectionBgColor));
	item->properties.push_back(new StyleProperty(item, IT_EDITOR_BOOKMARK, null_font_style, editor_theme->bookmarkFgColor, editor_theme->bookmarkBgColor));
	item->properties.push_back(new StyleProperty(item, IT_EDITOR_FOLD, null_font_style, editor_theme->foldFgColor, editor_theme->foldBgColor));
	item->properties.push_back(new StyleProperty(item, IT_EDITOR_ERROR, null_font_style, null_fg_color, editor_theme->errorBgColor));
	style_list.push_back(item);

	BuildTheme* build_theme = static_cast<BuildTheme*>(style_build.theme);
	item = new StyleItem(IT_BUILD, style_build.font->size, style_build.font->name, style_build.window->wordWrap, style_build.window->showHorzScrollBar, null_bookmarkstyle, null_foldstyle, null_commentfold, build_theme->warning);
	item->properties.push_back(new StyleProperty(item, IT_BUILD, build_theme->defaultStyle, build_theme->defaultColor, build_theme->backgroundColor));
	item->properties.push_back(new StyleProperty(item, IT_BUILD_TEXT, build_theme->defaultStyle, build_theme->defaultColor, build_theme->backgroundColor));
	item->properties.push_back(new StyleProperty(item, IT_BUILD_SELECTEDLINE, null_font_style, null_fg_color, build_theme->selectLineBgColor));
	item->properties.push_back(new StyleProperty(item, IT_EDITOR_CARET, null_font_style, build_theme->caretColor, null_bg_color));
	item->properties.push_back(new StyleProperty(item, IT_EDITOR_TEXTSELECTION, null_font_style, null_fg_color, build_theme->selectionBgColor));
	item->properties.push_back(new StyleProperty(item, IT_EDITOR_BOOKMARK, null_font_style, null_fg_color, build_theme->bookmarkBgColor));
	style_list.push_back(item);

	EditTheme* debug_theme = static_cast<EditTheme*>(style_debug.theme);
	item = new StyleItem(IT_DEBUG, style_debug.font->size, style_debug.font->name, style_debug.window->wordWrap, style_debug.window->showHorzScrollBar);
	item->properties.push_back(new StyleProperty(item, IT_DEBUG, debug_theme->defaultStyle, debug_theme->defaultColor, debug_theme->backgroundColor));
	item->properties.push_back(new StyleProperty(item, IT_BUILD_TEXT, debug_theme->defaultStyle, debug_theme->defaultColor, debug_theme->backgroundColor));
	item->properties.push_back(new StyleProperty(item, IT_EDITOR_CARET, null_font_style, debug_theme->caretColor, null_bg_color));
	item->properties.push_back(new StyleProperty(item, IT_EDITOR_TEXTSELECTION, null_font_style, null_fg_color, debug_theme->selectionBgColor));
	item->properties.push_back(new StyleProperty(item, IT_EDITOR_BOOKMARK, null_font_style, null_fg_color, debug_theme->bookmarkBgColor));
	style_list.push_back(item);

	rdo::gui::tracer::LogTheme* trace_theme = style_trace.theme;
	item = new StyleItem(IT_LOG, style_trace.font->size, style_trace.font->name);
	item->properties.push_back(new StyleProperty(item, IT_LOG, trace_theme->style, trace_theme->defaultColor.foregroundColor, trace_theme->defaultColor.backgroundColor));
	item->properties.push_back(new StyleProperty(item, IT_LOG_ES, trace_theme->style, trace_theme->es.foregroundColor, trace_theme->es.backgroundColor));
	item->properties.push_back(new StyleProperty(item, IT_LOG_EB, trace_theme->style, trace_theme->eb.foregroundColor, trace_theme->eb.backgroundColor));
	item->properties.push_back(new StyleProperty(item, IT_LOG_EF, trace_theme->style, trace_theme->ef.foregroundColor, trace_theme->ef.backgroundColor));
	item->properties.push_back(new StyleProperty(item, IT_LOG_EI, trace_theme->style, trace_theme->ei.foregroundColor, trace_theme->ei.backgroundColor));
	item->properties.push_back(new StyleProperty(item, IT_LOG_ER, trace_theme->style, trace_theme->er.foregroundColor, trace_theme->er.backgroundColor));
	item->properties.push_back(new StyleProperty(item, IT_LOG_RC, trace_theme->style, trace_theme->rc.foregroundColor, trace_theme->rc.backgroundColor));
	item->properties.push_back(new StyleProperty(item, IT_LOG_RE, trace_theme->style, trace_theme->re.foregroundColor, trace_theme->re.backgroundColor));
	item->properties.push_back(new StyleProperty(item, IT_LOG_RK, trace_theme->style, trace_theme->rk.foregroundColor, trace_theme->rk.backgroundColor));
	item->properties.push_back(new StyleProperty(item, IT_LOG_V, trace_theme->style, trace_theme->v.foregroundColor, trace_theme->v.backgroundColor));
	item->properties.push_back(new StyleProperty(item, IT_LOG_STATUS, trace_theme->style, trace_theme->s.foregroundColor, trace_theme->s.backgroundColor));
	item->properties.push_back(new StyleProperty(item, IT_LOG_DPS, trace_theme->style, trace_theme->dps.foregroundColor, trace_theme->dps.backgroundColor));
	item->properties.push_back(new StyleProperty(item, IT_LOG_SB, trace_theme->style, trace_theme->sb.foregroundColor, trace_theme->sb.backgroundColor));
	item->properties.push_back(new StyleProperty(item, IT_LOG_SO, trace_theme->style, trace_theme->so.foregroundColor, trace_theme->so.backgroundColor));
	item->properties.push_back(new StyleProperty(item, IT_LOG_STN, trace_theme->style, trace_theme->stn.foregroundColor, trace_theme->stn.backgroundColor));
	item->properties.push_back(new StyleProperty(item, IT_LOG_STD, trace_theme->style, trace_theme->std.foregroundColor, trace_theme->std.backgroundColor));
	item->properties.push_back(new StyleProperty(item, IT_LOG_STR, trace_theme->style, trace_theme->str.foregroundColor, trace_theme->str.backgroundColor));
	item->properties.push_back(new StyleProperty(item, IT_LOG_SRC, trace_theme->style, trace_theme->src.foregroundColor, trace_theme->src.backgroundColor));
	item->properties.push_back(new StyleProperty(item, IT_LOG_SRE, trace_theme->style, trace_theme->sre.foregroundColor, trace_theme->sre.backgroundColor));
	item->properties.push_back(new StyleProperty(item, IT_LOG_SRK, trace_theme->style, trace_theme->srk.foregroundColor, trace_theme->srk.backgroundColor));
	item->properties.push_back(new StyleProperty(item, IT_LOG_SD, trace_theme->style, trace_theme->sd.foregroundColor, trace_theme->sd.backgroundColor));
	item->properties.push_back(new StyleProperty(item, IT_LOG_SES, trace_theme->style, trace_theme->ses.foregroundColor, trace_theme->ses.backgroundColor));
	item->properties.push_back(new StyleProperty(item, IT_LOG_SEN, trace_theme->style, trace_theme->sen.foregroundColor, trace_theme->sen.backgroundColor));
	item->properties.push_back(new StyleProperty(item, IT_LOG_SEM, trace_theme->style, trace_theme->sem.foregroundColor, trace_theme->sem.backgroundColor));
	item->properties.push_back(new StyleProperty(item, IT_LOG_SEF, trace_theme->style, trace_theme->sef.foregroundColor, trace_theme->sef.backgroundColor));
	item->properties.push_back(new StyleProperty(item, IT_LOG_SEU, trace_theme->style, trace_theme->seu.foregroundColor, trace_theme->seu.backgroundColor));
	style_list.push_back(item);

	ParserTheme* results_theme = static_cast<ParserTheme*>(style_results.theme);
	item = new StyleItem(IT_RESULT, style_results.font->size, style_results.font->name, style_results.window->wordWrap, style_results.window->showHorzScrollBar);
	item->properties.push_back(new StyleProperty(item, IT_RESULT, results_theme->identifierStyle, results_theme->identifierColor, results_theme->backgroundColor));
	item->properties.push_back(new StyleProperty(item, IT_EDITOR_PLAINTEXT, results_theme->defaultStyle, results_theme->defaultColor, null_bg_color, null_fg_color, results_theme->backgroundColor));
	item->properties.push_back(new StyleProperty(item, IT_EDITOR_IDENTIFICATOR, results_theme->identifierStyle, results_theme->identifierColor, null_bg_color, null_fg_color, results_theme->backgroundColor));
	item->properties.push_back(new StyleProperty(item, IT_EDITOR_KEYWORD, results_theme->keywordStyle, results_theme->keywordColor, null_bg_color, null_fg_color, results_theme->backgroundColor));
	item->properties.push_back(new StyleProperty(item, IT_EDITOR_NUMBER, results_theme->numberStyle, results_theme->numberColor, null_bg_color, null_fg_color, results_theme->backgroundColor));
	item->properties.push_back(new StyleProperty(item, IT_EDITOR_STRING, results_theme->stringStyle, results_theme->stringColor, null_bg_color, null_fg_color, results_theme->backgroundColor));
	item->properties.push_back(new StyleProperty(item, IT_EDITOR_OPERATOR, results_theme->operatorStyle, results_theme->operatorColor, null_bg_color, null_fg_color, results_theme->backgroundColor));
	item->properties.push_back(new StyleProperty(item, IT_EDITOR_CARET, null_font_style, results_theme->caretColor, null_bg_color));
	item->properties.push_back(new StyleProperty(item, IT_EDITOR_TEXTSELECTION, null_font_style, null_fg_color, results_theme->selectionBgColor));
	item->properties.push_back(new StyleProperty(item, IT_EDITOR_BOOKMARK, null_font_style, results_theme->bookmarkFgColor, results_theme->bookmarkBgColor));
	style_list.push_back(item);

	FindTheme* find_theme = static_cast<FindTheme*>(style_find.theme);
	item = new StyleItem(IT_FIND, style_find.font->size, style_find.font->name, style_find.window->wordWrap, style_find.window->showHorzScrollBar);
	item->properties.push_back(new StyleProperty(item, IT_FIND, find_theme->defaultStyle, find_theme->defaultColor, find_theme->backgroundColor));
	item->properties.push_back(new StyleProperty(item, IT_BUILD_TEXT, find_theme->defaultStyle, find_theme->defaultColor, find_theme->backgroundColor));
	item->properties.push_back(new StyleProperty(item, IT_FIND_SEARCHTEXT, find_theme->keywordStyle, find_theme->keywordColor, null_bg_color, null_fg_color, find_theme->backgroundColor));
	item->properties.push_back(new StyleProperty(item, IT_BUILD_SELECTEDLINE, null_font_style, null_fg_color, find_theme->selectLineBgColor));
	item->properties.push_back(new StyleProperty(item, IT_EDITOR_CARET, null_font_style, find_theme->caretColor, null_bg_color));
	item->properties.push_back(new StyleProperty(item, IT_EDITOR_TEXTSELECTION, null_font_style, null_fg_color, find_theme->selectionBgColor));
	item->properties.push_back(new StyleProperty(item, IT_EDITOR_BOOKMARK, null_font_style, null_fg_color, find_theme->bookmarkBgColor));
	style_list.push_back(item);

	rdo::gui::tracer::ChartViewTheme* chart_theme = static_cast<rdo::gui::tracer::ChartViewTheme*>(style_chart.theme);
	item = new StyleItem(IT_CHART, style_chart.font->size, style_chart.font->name);
	item->properties.push_back(new StyleProperty(item, IT_CHART, chart_theme->defaultStyle, chart_theme->defaultColor, chart_theme->backgroundColor));
	item->properties.push_back(new StyleProperty(item, IT_CHART_AXIS, chart_theme->defaultStyle, chart_theme->axisFgColor, null_bg_color, null_fg_color, chart_theme->backgroundColor));
	item->properties.push_back(new StyleProperty(item, IT_CHART_TITLE, chart_theme->titleStyle, chart_theme->titleFGColor, null_bg_color, null_fg_color, chart_theme->backgroundColor));
	item->properties.push_back(new StyleProperty(item, IT_CHART_LEGEND, chart_theme->legendStyle, chart_theme->legendFgColor, null_bg_color, null_fg_color, chart_theme->backgroundColor));
	item->properties.push_back(new StyleProperty(item, IT_CHART_CHART, null_font_style, null_fg_color, chart_theme->chartBgColor));
	item->properties.push_back(new StyleProperty(item, IT_CHART_TIME, null_font_style, null_fg_color, chart_theme->timeBgColor));
	style_list.push_back(item);

	rdo::gui::frame::FrameTheme* frame_theme = static_cast<rdo::gui::frame::FrameTheme*>(style_frame.theme);
	item = new StyleItem(IT_FRAME, style_frame.font->size, style_frame.font->name);
	item->properties.push_back(new StyleProperty(item, IT_FRAME, frame_theme->defaultStyle, frame_theme->defaultColor, frame_theme->backgroundColor));
	item->properties.push_back(new StyleProperty(item, IT_FRAME_BORDER, frame_theme->defaultStyle, frame_theme->defaultColor, null_bg_color));
	item->properties.push_back(new StyleProperty(item, IT_FRAME_BACKGROUND, frame_theme->defaultStyle, null_fg_color, frame_theme->backgroundColor));
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
}

void ViewPreferences::checkAllData()
{
	rbool setupFlag           = m_setup           == g_pApp->getFileAssociationSetup() ? true : false;
	rbool checkInFutureFlag   = m_checkInFuture   == g_pApp->getFileAssociationCheckInFuture() ? true : false;
	rbool openLastProjectFlag = m_openLastProject == g_pApp->getOpenLastProject() ? true : false;
	rbool showFullNameFlag    = m_showFullName    == g_pApp->getShowCaptionFullName() ? true : false;

	rbool style_editor_flag  = style_editor  == g_pApp->getStyle()->style_editor ? true : false;
	rbool style_build_flag   = style_build   == g_pApp->getStyle()->style_build ? true : false;
	rbool style_debug_flag   = style_debug   == g_pApp->getStyle()->style_debug ? true : false;
	rbool style_trace_flag   = style_trace   == g_pApp->getStyle()->style_trace ? true : false;
	rbool style_results_flag = style_results == g_pApp->getStyle()->style_results ? true : false;
	rbool style_find_flag    = style_find    == g_pApp->getStyle()->style_find ? true : false;
	rbool style_chart_flag   = style_chart   == g_pApp->getStyle()->style_chart ? true : false;
	rbool style_frame_flag   = style_frame   == g_pApp->getStyle()->style_frame ? true : false;

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