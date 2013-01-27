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
// --------------------------------------------------------------------------------

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

	connect(treeWidget, SIGNAL(itemClicked(QTreeWidgetItem*, int)), this, SLOT(onTreeWidgetItemActivated(QTreeWidgetItem*, int)));
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
}