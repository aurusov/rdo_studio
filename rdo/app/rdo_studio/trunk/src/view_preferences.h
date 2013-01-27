/*!
  \copyright (c) RDO-Team, 2003-2012
  \file      app/rdo_studio_mfc/src/view_preferences.h
  \author    Романов Ярослав (robot.xet@gmail.com)
  \date      27.11.2012
  \brief     
  \indent    4T
*/

#ifndef _RDO_STUDIO_VIEW_PREFERENCES_H_
#define _RDO_STUDIO_VIEW_PREFERENCES_H_

// ----------------------------------------------------------------------- INCLUDES
#include <QtGui/qdialog.h>
// ----------------------------------------------------------------------- SYNOPSIS
#include "app/rdo_studio_mfc/projects/common/bin/rdo_studio/generated/ui_view_preferenses.h"
#include "app/rdo_studio_mfc/src/options.h"
// --------------------------------------------------------------------------------

class RDOStudioPreferences
{
public:
	RDOStudioPreferences();
	virtual ~RDOStudioPreferences();

private:
	rdoEditor::RDOEditorEditStyle style_editor;

	//typedef boost::function<void (const RDOStudioPreferences& options)> NotifyCallback;
	//typedef std::list<NotifyCallback>  CallbackList;
};

class RDOStudioPreferencesEditor
{
public:
	RDOStudioPreferencesEditor();
	virtual ~RDOStudioPreferencesEditor();

private:
	BOOL m_codecompUse;
	int  m_codecompShowFullList;
	BOOL m_marginFold;
	BOOL m_marginBookmark;
	BOOL m_marginLineNumber;

public:

	BOOL getCodeCompUse         () const;
	int  getCodeCompShowFullList() const;
	BOOL getMarginFold          () const;
	BOOL getMarginBookmark      () const;
	BOOL getMarginLineNumber    () const;

	void setCodeCompUse         (BOOL m_value);
	void setCodeCompShowFullList(int  m_value);
	void setMarginFold          (BOOL m_value);
	void setMarginBookmark      (BOOL m_value);
	void setMarginLineNumber    (BOOL m_value);
};

class ViewPreferences: public QDialog, private Ui::ViewPreferencesDialog
{
Q_OBJECT

public:
	explicit ViewPreferences(PTR(QWidget) pParent = NULL);

private slots:

	void okButtonClicked();
	void cancelButtonClicked();
	void applyButtonClicked();
	void helpButtonClicked();

	void codeCompUseChanged(int state);
	void codeCompShowFullListChanged(bool state);
	void marginFoldChanged(int state);
	void marginBookmarkChanged(int state);
	void marginLineNumberChanged(int state);

private:
	RDOStudioPreferencesEditor editor;
};

#endif // _RDO_STUDIO_VIEW_PREFERENCES_H_