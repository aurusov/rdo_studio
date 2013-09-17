/*!
  \copyright (c) RDO-Team, 2003-2012
  \file      find_dialog.h
  \author    Урусов Андрей (rdo@rk9.bmstu.ru)
  \date      31.12.2012
  \brief     
  \indent    4T
*/

#ifndef _RDO_STUDIO_DIALOG_FIND_DIALOG_H_
#define _RDO_STUDIO_DIALOG_FIND_DIALOG_H_

// ----------------------------------------------------------------------- INCLUDES
#include "utils/src/common/warning_disable.h"
#include <boost/function.hpp>
#include <QDialog>
#include "ui_find_dialog.h"
#include "utils/src/common/warning_enable.h"
// ----------------------------------------------------------------------- SYNOPSIS
// --------------------------------------------------------------------------------

class FindDialog
	: public  QDialog
	, private Ui::FindDialog
{
Q_OBJECT

public:
	struct Settings
	{
		QString what;
		bool    matchCase;
		bool    matchWholeWord;
		bool    searchDown;

		Settings();
		Settings(CREF(Settings) settings);
	};

	typedef  boost::function<void (const Settings&)>  OnFindCallback;
	typedef  boost::function<void ()>                 OnCloseCallback;

	FindDialog(QWidget* pParent, const OnFindCallback& onFindCallback, const OnCloseCallback& onCloseCallback);
	virtual ~FindDialog();

	void setSettings(CREF(Settings) settings);

private:
	Settings         m_settings;
	OnFindCallback   m_onFindCallback;
	OnCloseCallback  m_onCloseCallback;

private slots:
	void onFindButton           ();
	void onWhatEdited           (const QString& text);
	void onMatchCaseChanged     (int value);
	void onMatchWholeWordChanged(int value);
	void onDirectionDownToggled (bool checked);
};

#endif // _RDO_STUDIO_DIALOG_FIND_DIALOG_H_
