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
#include "utils/warning_disable.h"
#include <boost/function.hpp>
#include <QDialog>
#include "app/rdo_studio/projects/common/bin/rdo_studio/generated/ui_find_dialog.h"
#include "utils/warning_enable.h"
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
		rbool   matchCase;
		rbool   matchWholeWord;
		rbool   searchDown;

		Settings();
		Settings(CREF(Settings) settings);
	};

	typedef  boost::function<void (const Settings&)>  OnFindCallback;
	typedef  boost::function<void ()>                 OnCloseCallback;

	FindDialog(PTR(QWidget) pParent, CREF(OnFindCallback) onFindCallback, CREF(OnCloseCallback) onCloseCallback);
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
