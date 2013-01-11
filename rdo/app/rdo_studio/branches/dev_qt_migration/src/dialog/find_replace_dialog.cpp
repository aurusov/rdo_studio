/*!
  \copyright (c) RDO-Team, 2003-2013
  \file      find_replace_dialog.cpp
  \author    Романов Ярослав (robot.xet@gmail.com)
  \date      11.01.2013
  \brief     
  \indent    4T
*/

// ---------------------------------------------------------------------------- PCH
#include "app/rdo_studio_mfc/pch/stdpch.h"
// ----------------------------------------------------------------------- INCLUDES
// ----------------------------------------------------------------------- SYNOPSIS
#include "app/rdo_studio_mfc/src/dialog/find_replace_dialog.h"
// --------------------------------------------------------------------------------

FindReplaceDialog::Settings::Settings()
: what          ("")
, byWhat        ("")
, matchCase     (false)
, matchWholeWord(false)
{}

FindReplaceDialog::Settings::Settings(CREF(Settings) settings)
: what          (settings.what          )
, byWhat        (settings.byWhat        )
, matchCase     (settings.matchCase     )
, matchWholeWord(settings.matchWholeWord)
{}

FindReplaceDialog::FindReplaceDialog(PTR(QWidget) pParent, CREF(OnFindCallback) onFindCallback, CREF(OnCloseCallback) onCloseCallback)
: QDialog(pParent, Qt::Dialog | Qt::CustomizeWindowHint | Qt::WindowTitleHint | Qt::WindowCloseButtonHint)
, m_onFindCallback (onFindCallback )
, m_onCloseCallback(onCloseCallback)
{
	setupUi(this);

	layout()->setSizeConstraint(QLayout::SetFixedSize);

	connect(findButton,     SIGNAL(clicked()), this, SLOT(onFindButton()));
	connect(whatLineEdit,   SIGNAL(textEdited(const QString&)), this, SLOT(onWhatEdited(const QString&)));
	connect(byWhatLineEdit, SIGNAL(textEdited(const QString&)), this, SLOT(onByWhatEdited(const QString&)));
	connect(matchCase,      SIGNAL(stateChanged(int)),          this, SLOT(onMatchCaseChanged(int)));
	connect(wholeWord,      SIGNAL(stateChanged(int)),          this, SLOT(onMatchWholeWordChanged(int)));
}

FindReplaceDialog::~FindReplaceDialog()
{
	m_onCloseCallback();
}

void FindReplaceDialog::setSettings(CREF(Settings) settings)
{
	m_settings = settings;

	whatLineEdit->setText(QString::fromStdString(m_settings.what));
	byWhatLineEdit->setText(QString::fromStdString(m_settings.byWhat));
	matchCase->setChecked(m_settings.matchCase);
	wholeWord->setChecked(m_settings.matchWholeWord);
}

void FindReplaceDialog::onFindButton()
{
	m_onFindCallback(m_settings);
}

void FindReplaceDialog::onWhatEdited(const QString& text)
{
	m_settings.what = text.toStdString();
}

void FindReplaceDialog::onByWhatEdited(const QString& text)
{
	m_settings.byWhat = text.toStdString();
}


void FindReplaceDialog::onMatchCaseChanged(int value)
{
	m_settings.matchCase = value ? true : false;
}

void FindReplaceDialog::onMatchWholeWordChanged(int value)
{
	m_settings.matchWholeWord = value ? true : false;
}