/*!
  \copyright (c) RDO-Team, 2003-2012
  \file      rdoeditoredit.h
  \author    Урусов Андрей (rdo@rk9.bmstu.ru)
  \date      20.02.2003
  \brief     
  \indent    4T
*/

#ifndef _RDO_STUDIO_MFC_RDO_EDIT_RDOEDITOREDIT_H_
#define _RDO_STUDIO_MFC_RDO_EDIT_RDOEDITOREDIT_H_

// ----------------------------------------------------------------------- INCLUDES
#include <QtGui/qwidget.h>
// ----------------------------------------------------------------------- SYNOPSIS
#include "app/rdo_studio_mfc/rdo_edit/rdoeditorbaseedit.h"
#include "app/rdo_studio_mfc/rdo_edit/rdoeditoreditstyle.h"
#include "app/rdo_studio_mfc/edit_ctrls/rdologedit.h"
// --------------------------------------------------------------------------------

namespace rdoEditor {

// --------------------------------------------------------------------------------
// -------------------- RDOEditorEdit
// --------------------------------------------------------------------------------
class RDOEditorEdit: public RDOEditorBaseEdit
{
Q_OBJECT

private:
	int sci_FOLDMARGIN_ID;
	int sci_MARKER_ERROR;

protected:
	QWidget* view;

	rdoEditCtrl::LogEdit* log;

	void expand( int& line, rbool doExpand, rbool force = false, int visLevels = 0, int level = -1 ) const;
	void foldChanged( int line, int levelNow, int levelPrev ) const;
	void foldMarginClick( int position, int modifiers ) const;

	rbool canClearErrorLine;
	void  clearErrorLine();
	rbool hasErrorLine  () const;

private:
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

public:
	RDOEditorEdit(PTR(QWidget) pParent, PTR(QWidget) pView = NULL);
	virtual ~RDOEditorEdit();

	void setEditorStyle(PTR(RDOEditorEditStyle) pStyle);

	void setErrorLine(int line = -1);

	CPTR(rdoEditCtrl::LogEdit) getLog() const;
	void setLog(REF(rdoEditCtrl::LogEdit) log);

	void setCanClearErrorLine( rbool value ) { canClearErrorLine = value; }

private slots:
	void catchModified     (int modificationType, int position, int length, int linesAdded, const QByteArray& bytes, int line, int foldLevelNow, int foldLevelPrev);
	void catchMarginClick  (int position, int modifiers, int margin);
};

} // namespace rdoEditor

#endif // _RDO_STUDIO_MFC_RDO_EDIT_RDOEDITOREDIT_H_
