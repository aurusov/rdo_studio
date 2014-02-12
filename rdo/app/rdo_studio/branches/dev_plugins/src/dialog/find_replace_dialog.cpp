/*!
  \copyright (c) RDO-Team, 2003-2013
  \file      find_replace_dialog.cpp
  \author    Романов Ярослав (robot.xet@gmail.com)
  \date      11.01.2013
  \brief     
  \indent    4T
*/

// ---------------------------------------------------------------------------- PCH
#include "app/rdo_studio/pch/stdpch.h"
// ----------------------------------------------------------------------- INCLUDES
// ----------------------------------------------------------------------- SYNOPSIS
#include "app/rdo_studio/src/dialog/find_replace_dialog.h"
// --------------------------------------------------------------------------------

FindReplaceDialog::Settings::Settings()
{}

FindReplaceDialog::Settings::Settings(CREF(Settings) settings)
	: FindDialog::Settings(settings)
	, byWhat(settings.byWhat)
{}

FindReplaceDialog::FindReplaceDialog(QWidget* pParent, const OnFindCallback& onFindCallback, const OnFindCallback& onReplaceCallback, const OnFindCallback& onReplaceAllCallback, const OnCloseCallback& onCloseCallback)
	: QDialog(pParent, Qt::Dialog | Qt::CustomizeWindowHint | Qt::WindowTitleHint | Qt::WindowCloseButtonHint)
	, m_onFindCallback      (onFindCallback      )
	, m_onReplaceCallback   (onReplaceCallback   )
	, m_onReplaceAllCallback(onReplaceAllCallback)
	, m_onCloseCallback     (onCloseCallback     )
{
	setupUi(this);

	layout()->setSizeConstraint(QLayout::SetFixedSize);

	connect(findButton,       SIGNAL(clicked()),                  this, SLOT(onFindButton()));
	connect(replaceButton,    SIGNAL(clicked()),                  this, SLOT(onReplaceButton()));
	connect(replaceAllButton, SIGNAL(clicked()),                  this, SLOT(onReplaceAllButton()));
	connect(whatLineEdit,     SIGNAL(textEdited(const QString&)), this, SLOT(onWhatEdited(const QString&)));
	connect(byWhatLineEdit,   SIGNAL(textEdited(const QString&)), this, SLOT(onByWhatEdited(const QString&)));
	connect(matchCase,        SIGNAL(stateChanged(int)),          this, SLOT(onMatchCaseChanged(int)));
	connect(wholeWord,        SIGNAL(stateChanged(int)),          this, SLOT(onMatchWholeWordChanged(int)));
	connect(directionDown,    SIGNAL(toggled(bool)),              this, SLOT(onDirectionDownToggled(bool)));

	setAttribute(Qt::WA_DeleteOnClose, true);
}

FindReplaceDialog::~FindReplaceDialog()
{
	m_onCloseCallback();
}

void FindReplaceDialog::setSettings(CREF(Settings) settings)
{
	m_settings = settings;

	whatLineEdit->setText(m_settings.what);
	whatLineEdit->setFocus();
	whatLineEdit->selectAll();
	byWhatLineEdit->setText(m_settings.byWhat);
	matchCase->setChecked(m_settings.matchCase);
	wholeWord->setChecked(m_settings.matchWholeWord);
}

void FindReplaceDialog::onFindButton()
{
	m_onFindCallback(m_settings);
}

void FindReplaceDialog::onReplaceButton()
{
	m_onReplaceCallback(m_settings);
}

void FindReplaceDialog::onReplaceAllButton()
{
	m_onReplaceAllCallback(m_settings);
}

void FindReplaceDialog::onWhatEdited(const QString& text)
{
	m_settings.what = text;
}

void FindReplaceDialog::onByWhatEdited(const QString& text)
{
	m_settings.byWhat = text;
}

void FindReplaceDialog::onMatchCaseChanged(int value)
{
	m_settings.matchCase = value ? true : false;
}

void FindReplaceDialog::onMatchWholeWordChanged(int value)
{
	m_settings.matchWholeWord = value ? true : false;
}

void FindReplaceDialog::onDirectionDownToggled(bool checked)
{
	m_settings.searchDown = checked;
}
