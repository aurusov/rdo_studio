/*!
  \copyright (c) RDO-Team, 2003-2012
  \file      find_dialog.cpp
  \author    Урусов Андрей (rdo@rk9.bmstu.ru)
  \date      31.12.2012
  \brief     
  \indent    4T
*/

// ---------------------------------------------------------------------------- PCH
#include "app/rdo_studio_mfc/pch/stdpch.h"
// ----------------------------------------------------------------------- INCLUDES
// ----------------------------------------------------------------------- SYNOPSIS
#include "app/rdo_studio_mfc/src/dialog/find_dialog.h"
// --------------------------------------------------------------------------------

FindDialog::Settings::Settings()
	: what          ("")
	, matchCase     (false)
	, matchWholeWord(false)
	, searchDown    (true )
{}

FindDialog::Settings::Settings(CREF(Settings) settings)
	: what          (settings.what          )
	, matchCase     (settings.matchCase     )
	, matchWholeWord(settings.matchWholeWord)
	, searchDown    (settings.searchDown    )
{}

FindDialog::FindDialog(PTR(QWidget) pParent, REF(Settings) settings, CREF(OnFindCallback) onFindCallback)
	: QDialog(pParent, Qt::Dialog | Qt::CustomizeWindowHint | Qt::WindowTitleHint | Qt::WindowCloseButtonHint)
	, m_settings(settings)
	, m_onFindCallback(onFindCallback)
{
	setupUi(this);

	lineEdit->setText(QString::fromStdString(settings.what));
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

	connect(findButton,    SIGNAL(clicked(bool)),              this, SLOT(onFindButton(bool)));
	connect(cancelButton,  SIGNAL(clicked(bool)),              this, SLOT(reject()));
	connect(lineEdit,      SIGNAL(textEdited(const QString&)), this, SLOT(onWhatEdited(const QString&)));
	connect(matchCase,     SIGNAL(stateChanged(int)),          this, SLOT(onMatchCaseChanged(int)));
	connect(wholeWord,     SIGNAL(stateChanged(int)),          this, SLOT(onMatchWholeWordChanged(int)));
	connect(directionDown, SIGNAL(toggled(bool)),              this, SLOT(onDirectionDownToggled(bool)));

	if (pParent)
	{
//		move(pParent->frameGeometry().center() - frameGeometry().center());
	}
}

void FindDialog::onFindButton(rbool)
{
	m_onFindCallback();
}

void FindDialog::onWhatEdited(const QString& text)
{
	m_settings.what = text.toStdString();
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
