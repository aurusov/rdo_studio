/*!
  \copyright (c) RDO-Team, 2003-2013
  \file      find_replace_dialog.h
  \author    Романов Ярослав (robot.xet@gmail.com)
  \date      11.01.2013
  \brief     
  \indent    4T
  */

#ifndef _RDO_STUDIO_MFC_DIALOG_FIND_REPLACE_DIALOG_H_
#define _RDO_STUDIO_MFC_DIALOG_FIND_REPLACE_DIALOG_H_

// ----------------------------------------------------------------------- INCLUDES
#include <boost/function.hpp>
#include <QtGui/qdialog.h>
// ----------------------------------------------------------------------- SYNOPSIS
#include "app/rdo_studio_mfc/projects/common/bin/rdo_studio/generated/ui_find_replace_dialog.h"
// --------------------------------------------------------------------------------

class FindReplaceDialog
	: public QDialog
	, public Ui::FindReplaceDialog
{
Q_OBJECT

public:
	struct Settings
	{
		tstring what;
		tstring byWhat;
		rbool   matchCase;
		rbool   searchDown;
		rbool   matchWholeWord;

		Settings();
		Settings(CREF(Settings) settings);
	};

	typedef  boost::function<void (const Settings&)>  OnFindCallback;
	typedef  boost::function<void ()>                 OnCloseCallback;

	FindReplaceDialog(PTR(QWidget) pParent, CREF(OnFindCallback) onFindCallback, CREF(OnFindCallback) onReplaceCallback, CREF(OnFindCallback) onReplaceAllCallback, CREF(OnCloseCallback) onCloseCallback);
	virtual ~FindReplaceDialog();

	void setSettings(CREF(Settings) settings);

private:
	Settings         m_settings;
	OnFindCallback   m_onFindCallback;
	OnFindCallback   m_onReplaceCallback;
	OnFindCallback   m_onReplaceAllCallback;
	OnCloseCallback  m_onCloseCallback;

private slots:
	void onFindButton();
	void onReplaceButton();
	void onReplaceAllButton();
	void onWhatEdited           (const QString& text);
	void onByWhatEdited         (const QString& text);
	void onMatchCaseChanged     (int value);
	void onMatchWholeWordChanged(int value);
	void onDirectionDownToggled (bool checked);
};

#endif // _RDO_STUDIO_MFC_DIALOG_FIND_REPLACE_DIALOG_H_