/*!
  \copyright (c) RDO-Team, 2003-2012
  \file      rdoeditoredit.h
  \author    ������ ������ (rdo@rk9.bmstu.ru)
  \date      20.02.2003
  \brief     
  \indent    4T
*/

#ifndef _RDO_STUDIO_MFC_RDO_EDIT_RDOEDITOREDIT_H_
#define _RDO_STUDIO_MFC_RDO_EDIT_RDOEDITOREDIT_H_

// ----------------------------------------------------------------------- INCLUDES
// ----------------------------------------------------------------------- SYNOPSIS
#include "app/rdo_studio_mfc/rdo_edit/rdoeditorbaseedit.h"
#include "app/rdo_studio_mfc/rdo_edit/rdoeditoreditstyle.h"
#include "app/rdo_studio_mfc/edit_ctrls/rdologedit.h"
// --------------------------------------------------------------------------------

class RDOStudioEditBaseView;

namespace rdoEditor {

// --------------------------------------------------------------------------------
// -------------------- RDOEditorEdit
// --------------------------------------------------------------------------------
class RDOEditorEdit: public RDOEditorBaseEdit
{
private:
	int sci_FOLDMARGIN_ID;
	int sci_MARKER_ERROR;

protected:
	int bufSelStart;
	RDOStudioEditBaseView* view;

	rdoEditCtrl::RDOLogEdit* log;

	void expand( int& line, rbool doExpand, rbool force = false, int visLevels = 0, int level = -1 ) const;
	void foldChanged( int line, int levelNow, int levelPrev ) const;
	void toggleCurrentFold() const;
	void toggleAllFolds() const;
	void foldMarginClick( int position, int modifiers ) const;

	void commentSelection() const;
	void completeWord();

	void onBufferAppend( const int bufIndex );
	void onBufferEdit( const int bufIndex );

	rbool canClearErrorLine;
	void  clearErrorLine();
	rbool hasErrorLine  () const;

private:
	virtual BOOL OnNotify( WPARAM wParam, LPARAM lParam, LRESULT* pResult );

	afx_msg int OnCreate( LPCREATESTRUCT lpCreateStruct );
	afx_msg void OnEditCommentSelection();
	afx_msg void OnEditCompleteWord();
	afx_msg void OnInsertBuffer1Paste();
	afx_msg void OnInsertBuffer2Paste();
	afx_msg void OnInsertBuffer3Paste();
	afx_msg void OnInsertBuffer4Paste();
	afx_msg void OnInsertBuffer1Append();
	afx_msg void OnInsertBuffer2Append();
	afx_msg void OnInsertBuffer3Append();
	afx_msg void OnInsertBuffer4Append();
	afx_msg void OnInsertBuffer1Edit();
	afx_msg void OnInsertBuffer2Edit();
	afx_msg void OnInsertBuffer3Edit();
	afx_msg void OnInsertBuffer4Edit();
	afx_msg void OnInsertBuffer1Clear();
	afx_msg void OnInsertBuffer2Clear();
	afx_msg void OnInsertBuffer3Clear();
	afx_msg void OnInsertBuffer4Clear();
	afx_msg void OnUndateBuffer1Paste( CCmdUI* pCmdUI );
	afx_msg void OnUndateBuffer2Paste( CCmdUI* pCmdUI );
	afx_msg void OnUndateBuffer3Paste( CCmdUI* pCmdUI );
	afx_msg void OnUndateBuffer4Paste( CCmdUI* pCmdUI );
	afx_msg void OnUndateBufferAppend( CCmdUI* pCmdUI );
	afx_msg void OnUpdateInsertBufferEdit(CCmdUI* pCmdUI);
	afx_msg void OnUndateBuffer1Clear( CCmdUI* pCmdUI );
	afx_msg void OnUndateBuffer2Clear( CCmdUI* pCmdUI );
	afx_msg void OnUndateBuffer3Clear( CCmdUI* pCmdUI );
	afx_msg void OnUndateBuffer4Clear( CCmdUI* pCmdUI );
	afx_msg void OnGotoNext();
	afx_msg void OnGotoPrev();
	afx_msg void OnUpdateGotoNext(CCmdUI* pCmdUI);
	afx_msg void OnUpdateGotoPrev(CCmdUI* pCmdUI);
	afx_msg void OnToggleAllFolds();
	afx_msg void OnToggleCurrentFold();
	afx_msg void OnUpdateFold(CCmdUI* pCmdUI);
	afx_msg void OnHelpKeyword();
	afx_msg void OnInsertCommand( UINT nID );
	DECLARE_MESSAGE_MAP()

public:
	RDOEditorEdit( RDOStudioEditBaseView* _view = NULL );
	virtual ~RDOEditorEdit();

	void setEditorStyle( RDOEditorEditStyle* _style );

	void setErrorLine( int line = -1 );

	void setLog( rdoEditCtrl::RDOLogEdit& _log );
	const rdoEditCtrl::RDOLogEdit* getLog() const;

	void setCanClearErrorLine( rbool value ) { canClearErrorLine = value; }
};

}; // namespace rdoEditor

#endif // _RDO_STUDIO_MFC_RDO_EDIT_RDOEDITOREDIT_H_