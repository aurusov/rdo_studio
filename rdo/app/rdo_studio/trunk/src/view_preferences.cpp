/*!
  \copyright (c) RDO-Team, 2003-2012
  \file      app/rdo_studio_mfc/src/view_preferences.cpp
  \author    Романов Ярослав (robot.xet@gmail.com)
  \date      27.11.2012
  \brief     
  \indent    4T
*/

// ---------------------------------------------------------------------------- PCH
#include "app/rdo_studio_mfc/pch/stdpch.h"
// ----------------------------------------------------------------------- INCLUDES
// ----------------------------------------------------------------------- SYNOPSIS
#include "app/rdo_studio_mfc/src/view_preferences.h"
#include "app/rdo_studio_mfc/res/build_version.h"
#include "app/rdo_studio_mfc/src/application.h"
// --------------------------------------------------------------------------------

ViewPreferences::ViewPreferences(PTR(QWidget) pParent)
	: QDialog(pParent),
	  editor()
{
	setupUi(this);

	//Вкладка "Редактор"
	connect(buttonOk, SIGNAL(clicked()), this, SLOT(okButtonClicked()));
	connect(buttonCancel, SIGNAL(clicked()), this, SLOT(cancelButtonClicked()));
	connect(buttonApply, SIGNAL(clicked()), this, SLOT(applyButtonClicked()));
	connect(buttonHelp, SIGNAL(clicked()), this, SLOT(helpButtonClicked()));

	connect(checkBoxCodeCompUse, SIGNAL(stateChanged(int)), this, SLOT(codeCompUseChanged(int)));
	connect(radioButtonFullList, SIGNAL(toggled(bool)), this, SLOT(codeCompShowFullListChanged(bool)));
	connect(radioButtonNearestWords, SIGNAL(toggled(bool)), this, SLOT(codeCompShowFullListChanged(bool)));
	connect(checkBoxMarginFold, SIGNAL(stateChanged(int)), this, SLOT(marginFoldChanged(int)));
	connect(checkBoxMarginBookmark, SIGNAL(stateChanged(int)), this, SLOT(marginBookmarkChanged(int)));
	connect(checkBoxMarginLineNum, SIGNAL(stateChanged(int)), this, SLOT(marginLineNumberChanged(int)));
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
	editor.setCodeCompUse(state);
	
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
		editor.setCodeCompShowFullList(true);
	if(radioButtonNearestWords->isChecked())
		editor.setCodeCompShowFullList(false);
}

void ViewPreferences::marginFoldChanged(int state)
{
	editor.setMarginFold(state);
}

void ViewPreferences::marginBookmarkChanged(int state)
{
	editor.setMarginBookmark(state);
}

void ViewPreferences::marginLineNumberChanged(int state)
{
	editor.setMarginLineNumber(state);
}

RDOStudioPreferencesEditor::RDOStudioPreferencesEditor()
	: m_codecompUse         (FALSE),
	  m_codecompShowFullList(-1   ),
	  m_marginFold          (FALSE),
	  m_marginBookmark      (FALSE),
	  m_marginLineNumber    (FALSE)
{}

RDOStudioPreferencesEditor::~RDOStudioPreferencesEditor()
{}

BOOL RDOStudioPreferencesEditor::getCodeCompUse() const
{
	return m_codecompUse;
}

int  RDOStudioPreferencesEditor::getCodeCompShowFullList() const
{
	return m_codecompShowFullList;
}

BOOL RDOStudioPreferencesEditor::getMarginFold() const
{
	return m_marginFold;
}

BOOL RDOStudioPreferencesEditor::getMarginBookmark() const
{
	return m_marginBookmark;
}

BOOL RDOStudioPreferencesEditor::getMarginLineNumber() const
{
	return m_marginLineNumber;
}

void RDOStudioPreferencesEditor::setCodeCompUse(BOOL m_value)
{
	m_codecompUse = m_value;
}

void RDOStudioPreferencesEditor::setCodeCompShowFullList(int m_value )
{
	m_codecompShowFullList = m_value;
}

void RDOStudioPreferencesEditor::setMarginFold(BOOL m_value)
{
	m_marginFold = m_value;
}

void RDOStudioPreferencesEditor::setMarginBookmark(BOOL m_value)
{
	m_marginBookmark = m_value;
}

void RDOStudioPreferencesEditor::setMarginLineNumber(BOOL m_value)
{
	m_marginLineNumber = m_value;
}