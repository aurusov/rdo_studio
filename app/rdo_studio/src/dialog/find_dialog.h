#pragma once

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
		Settings(const Settings& settings);
	};

	typedef  boost::function<void (const Settings&)>  OnFindCallback;
	typedef  boost::function<void ()>                 OnCloseCallback;

	FindDialog(QWidget* pParent, const OnFindCallback& onFindCallback, const OnCloseCallback& onCloseCallback);
	virtual ~FindDialog();

	void setSettings(const Settings& settings);

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
