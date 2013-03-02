/*!
  \copyright (c) RDO-Team, 2003-2012
  \file      rdoeditoredit.h
  \author    Урусов Андрей (rdo@rk9.bmstu.ru)
  \date      20.02.2003
  \brief     
  \indent    4T
*/

#ifndef _RDO_STUDIO_RDO_EDIT_RDOEDITOREDIT_H_
#define _RDO_STUDIO_RDO_EDIT_RDOEDITOREDIT_H_

// ----------------------------------------------------------------------- INCLUDES
#include "utils/warning_disable.h"
#include <QWidget>
#include <QMenu>
#include "utils/warning_enable.h"
// ----------------------------------------------------------------------- SYNOPSIS
#include "app/rdo_studio/rdo_edit/rdoeditorbaseedit.h"
#include "app/rdo_studio/rdo_edit/rdoeditoreditstyle.h"
#include "app/rdo_studio/edit_ctrls/rdologedit.h"
// --------------------------------------------------------------------------------

namespace rdo { namespace gui { namespace editor {

class Model: public Parser
{
Q_OBJECT

public:
	Model(PTR(QWidget) pParent, PTR(QWidget) pView = NULL);
	virtual ~Model();

	void setEditorStyle(PTR(ModelStyle) pStyle);

	void setErrorLine(int line = -1);

	CPTR(Log) getLog() const;
	void setLog(REF(Log) log);

	void setCanClearErrorLine(rbool value);

private:
	typedef  Parser  super;

	QWidget*  m_pView;
	Log*      m_pLog;
	QMenu*    m_pPopupMenu;
	int       m_sciFoldMarginID;
	int       m_sciMarkerError;
	rbool     m_canClearErrorLine;

	void expand         (int& line, rbool doExpand, rbool force = false, int visLevels = 0, int level = -1) const;
	void foldChanged    (int line, int levelNow, int levelPrev) const;
	void foldMarginClick(int position, int modifiers) const;

	void  clearErrorLine();
	rbool hasErrorLine  () const;

	virtual void mousePressEvent(QMouseEvent* pEvent);
	virtual void onUpdateActions(rbool activated);
	virtual void onHelpContext  ();

private slots:
	void onEditCommentSelection() const;
	void onEditCompleteWord    ();
	void onToggleAllFolds      () const;
	void onToggleCurrentFold   () const;
	void onGotoNext            ();
	void onGotoPrev            ();
	void onInsertCommand       (QObject* pObject);

	void catchModified   (int modificationType, int position, int length, int linesAdded, const QByteArray& bytes, int line, int foldLevelNow, int foldLevelPrev);
	void catchMarginClick(int position, int modifiers, int margin);
};

}}} // namespace rdo::gui::editor

#endif // _RDO_STUDIO_RDO_EDIT_RDOEDITOREDIT_H_
