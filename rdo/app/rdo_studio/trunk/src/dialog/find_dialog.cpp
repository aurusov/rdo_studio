/*!
  \copyright (c) RDO-Team, 2003-2012
  \file      find_dialog.cpp
  \author    Урусов Андрей (rdo@rk9.bmstu.ru)
  \date      31.12.2012
  \brief     
  \indent    4T
*/

// ---------------------------------------------------------------------------- PCH
#include "app/rdo_studio/pch/stdpch.h"
// ----------------------------------------------------------------------- INCLUDES
// ----------------------------------------------------------------------- SYNOPSIS
#include "app/rdo_studio/src/dialog/find_dialog.h"
// --------------------------------------------------------------------------------

FindDialog::Settings::Settings()
	: matchCase     (false)
	, matchWholeWord(false)
	, searchDown    (true )
{}

FindDialog::Settings::Settings(CREF(Settings) settings)
	: what          (settings.what          )
	, matchCase     (settings.matchCase     )
	, matchWholeWord(settings.matchWholeWord)
	, searchDown    (settings.searchDown    )
{}

FindDialog::FindDialog(QWidget* pParent, const OnFindCallback& onFindCallback, OnCloseCallback& onCloseCallback)
	: QDialog(pParent, Qt::Dialog | Qt::CustomizeWindowHint | Qt::WindowTitleHint | Qt::WindowCloseButtonHint)
	, m_onFindCallback (onFindCallback )
	, m_onCloseCallback(onCloseCallback)
{
	setupUi(this);

	layout()->setSizeConstraint(QLayout::SetFixedSize);

	connect(findButton,    SIGNAL(clicked(bool)),              this, SLOT(onFindButton()));
	connect(cancelButton,  SIGNAL(clicked(bool)),              this, SLOT(reject()));
	connect(lineEdit,      SIGNAL(textEdited(const QString&)), this, SLOT(onWhatEdited(const QString&)));
	connect(matchCase,     SIGNAL(stateChanged(int)),          this, SLOT(onMatchCaseChanged(int)));
	connect(wholeWord,     SIGNAL(stateChanged(int)),          this, SLOT(onMatchWholeWordChanged(int)));
	connect(directionDown, SIGNAL(toggled(bool)),              this, SLOT(onDirectionDownToggled(bool)));

	setAttribute(Qt::WA_DeleteOnClose, true);
}

FindDialog::~FindDialog()
{
	m_onCloseCallback();
}

void FindDialog::setSettings(CREF(Settings) settings)
{
	m_settings = settings;

	lineEdit->setText(m_settings.what);
	lineEdit->setFocus();
	lineEdit->selectAll();
	matchCase->setChecked(m_settings.matchCase);
	wholeWord->setChecked(m_settings.matchWholeWord);
	if (m_settings.searchDown)
	{
		directionDown->setChecked(true);
	}
	else
	{
		directionUp->setChecked(true);
	}
}

void FindDialog::onFindButton()
{
	m_onFindCallback(m_settings);
}

void FindDialog::onWhatEdited(const QString& text)
{
	m_settings.what = text;
}

void FindDialog::onMatchCaseChanged(int value)
{
	m_settings.matchCase = value ? true : false;
}

void FindDialog::onMatchWholeWordChanged(int value)
{
	m_settings.matchWholeWord = value ? true : false;
}

void FindDialog::onDirectionDownToggled(bool checked)
{
	m_settings.searchDown = checked;
}
