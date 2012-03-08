/*!
  \copyright (c) RDO-Team, 2003-2012
  \file      rdobaseedit.h
  \author    Урусов Андрей (rdo@rk9.bmstu.ru)
  \date      28.02.2003
  \brief     
  \indent    4T
*/

#ifndef _RDO_STUDIO_MFC_EDIT_CTRLS_RDOBASEEDIT_H_
#define _RDO_STUDIO_MFC_EDIT_CTRLS_RDOBASEEDIT_H_

// ----------------------------------------------------------------------- PLATFORM
// ----------------------------------------------------------------------- INCLUDES
#include <vector>
// ----------------------------------------------------------------------- SYNOPSIS
#include "utils/rdostream.h"
#include "app/rdo_studio_mfc/edit_ctrls/rdobaseeditstyle.h"
// --------------------------------------------------------------------------------

namespace rdoEditCtrl {

// --------------------------------------------------------------------------------
// -------------------- RDOBaseEdit
// --------------------------------------------------------------------------------
typedef long (*sciFunType)( long ptr, unsigned int iMessage, unsigned long wParam, long lParam );

class RDOBaseEdit;

typedef std::vector< RDOBaseEdit* >                 RDOBaseEditList;
typedef std::vector< RDOBaseEdit* >::const_iterator RDOBaseEditListIterator;

class RDOBaseEditGroup {
private:
	RDOBaseEditList list;

public:
	bool bMatchCase;
	bool bMatchWholeWord;
	bool bSearchDown;
	tstring findStr;
	tstring replaceStr;

	RDOBaseEditGroup():
		bMatchCase( false ),
		bMatchWholeWord( false ),
		bSearchDown( true ),
		findStr( "" ),
		replaceStr( "" )
	{
	}
	void insert( RDOBaseEdit* edit ) {
		list.insert( list.end(), edit );
	}
	RDOBaseEditListIterator begin() const { return list.begin(); }
	RDOBaseEditListIterator end()   const { return list.end();   }
};

class RDOBaseEdit: public CWnd
{
private:
	static int objectCount;

	int markerCount;

protected:
	bool GUI_ID_EDIT_UNDO;
	bool GUI_ID_EDIT_REDO;
	bool GUI_ID_EDIT_CUT;
	bool GUI_IS_SELECTED;
	bool GUI_IS_EMPTY;
	bool GUI_IS_READONLY;
	bool GUI_IS_MODIFY;
	bool GUI_HAS_BOOKMARK;
	bool GUI_ID_VIEW_WHITESPACE;
	bool GUI_ID_VIEW_ENDOFLINE;

	HWND       sciHWND;
	long       sciEditor;
	sciFunType sciFun;
	long sendEditor( unsigned int msg, unsigned long wParam = 0, long lParam = 0 ) const   { return sciFun( sciEditor, msg, wParam, lParam );               };
	long sendEditorString( unsigned int msg, unsigned long wParam, const char* str ) const { return sendEditor( msg, wParam, reinterpret_cast<long>(str) ); };

	CMenu* popupMenu;

	int sci_MARKER_BOOKMARK;
	int getNewMarker();
	void defineMarker( int marker, int markerType, COLORREF fore, COLORREF back ) const;

	void setSelection( int anchor, int currentPos ) const { sendEditor( SCI_SETSEL, anchor, currentPos ); };
	CharacterRange getSelectionRange() const;
	void gotoLineEnsureVisible( int line ) const;
	void ensureRangeVisible( int posStart, int posEnd, bool enforcePolicy = true ) const;

	RDOBaseEditStyle* style;
	RDOBaseEditGroup* group;

	int  firstFoundPos;
	bool bHaveFound;
	void findNext( REF(tstring) findWhat, const bool searchDown = true, const bool matchCase = false, const bool matchWholeWord = false );
	void replace( REF(tstring) findWhat, REF(tstring) replaceWhat, const bool searchDown = true, const bool matchCase = false, const bool matchWholeWord = false );
	void replaceAll( REF(tstring) findWhat, REF(tstring) replaceWhat, const bool matchCase = false, const bool matchWholeWord = false );

	void copyAsRTF();

	int indentOfBlock( int line ) const;
	void setLineIndentation( int line, int indent ) const;
	void autoIndent() const;

	void updateAllGUI();
	void updateBookmarksGUI();

protected:
	afx_msg int OnCreate( LPCREATESTRUCT lpCreateStruct );
	afx_msg void OnIsSelected(CCmdUI* pCmdUI);
	virtual BOOL OnNotify( WPARAM wParam, LPARAM lParam, LRESULT* pResult );

private:
	virtual BOOL PreCreateWindow( CREATESTRUCT& cs );
	virtual BOOL OnCommand(WPARAM wParam, LPARAM lParam);

	afx_msg void OnSetFocus( CWnd *pOldWnd );
	afx_msg void OnSize( UINT nType, int cx, int cy );
	afx_msg void OnInitMenuPopup( CMenu* pPopupMenu, UINT nIndex, BOOL bSysMenu );
	afx_msg void OnContextMenu( CWnd* pWnd, CPoint pos );
	afx_msg void OnEditUndo();
	afx_msg void OnEditRedo();
	afx_msg void OnEditCut();
	afx_msg void OnEditCopy();
	afx_msg void OnEditPaste();
	afx_msg void OnEditClear();
	afx_msg void OnEditCopyAsRTF();
	afx_msg void OnEditSelectAll();
	afx_msg void OnEditUpperCase();
	afx_msg void OnEditLowerCase();
	afx_msg void OnUpdateEditUndo(CCmdUI* pCmdUI);
	afx_msg void OnUpdateEditRedo(CCmdUI* pCmdUI);
	afx_msg void OnUpdateEditCut(CCmdUI* pCmdUI);
	afx_msg void OnUpdateEditPaste(CCmdUI* pCmdUI);
	afx_msg void OnUpdateEditClear(CCmdUI* pCmdUI);
	afx_msg void OnSelectAll( CCmdUI* pCmdUI );
	afx_msg void OnSearchFind();
	afx_msg void OnSearchReplace();
	afx_msg void OnSearchFindNext();
	afx_msg void OnSearchFindPrevious();
	afx_msg void OnSearchFindNextFast();
	afx_msg void OnSearchFindPreviousFast();
	afx_msg void OnUpdateSearchFindNextPrev(CCmdUI* pCmdUI);
	afx_msg void OnUpdateSearchFind(CCmdUI* pCmdUI);
	afx_msg void OnUpdateSearchReplace(CCmdUI* pCmdUI);
	afx_msg void OnBookmarkToggle();
	afx_msg void OnBookmarkNext();
	afx_msg void OnBookmarkPrev();
	afx_msg void OnBookmarkClearAll();
	afx_msg void OnHasBookmarks( CCmdUI* pCmdUI );
	afx_msg void OnViewWhiteSpace();
	afx_msg void OnViewEndOfLine();
	afx_msg void OnViewZoomIn();
	afx_msg void OnViewZoomOut();
	afx_msg void OnViewZoomReset();
	afx_msg void OnUpdateWhiteSpace( CCmdUI* pCmdUI );
	afx_msg void OnUpdateEndOfLine( CCmdUI* pCmdUI );
	afx_msg void OnUpdateZoomIn( CCmdUI *pCmdUI );
	afx_msg void OnUpdateZoomOut( CCmdUI *pCmdUI );
	afx_msg void OnUpdateZoomReset( CCmdUI *pCmdUI );
	afx_msg void OnSearchGotoLine();
	afx_msg LRESULT OnFindReplaceMsg( WPARAM wParam, LPARAM lParam );
	DECLARE_MESSAGE_MAP()

public:
	RDOBaseEdit();
	virtual ~RDOBaseEdit();

	HWND getSCIHWND() const { return sciHWND; }

	const RDOBaseEditStyle* getEditorStyle() const         { return style; };
	void setEditorStyle( RDOBaseEditStyle* _style );

	void setGroup( RDOBaseEditGroup* _group );
	void setPopupMenu( CMenu* const value )                { popupMenu = value; };

	bool isEmpty() const                                   { return getLength() == 0;                                                         };
	bool isSelected() const                                { return sendEditor( SCI_GETSELECTIONSTART ) != sendEditor( SCI_GETSELECTIONEND ); };
	bool isOverwrite() const                               { return sendEditor( SCI_GETOVERTYPE ) ? true : false;                             };

	void updateEditGUI();
	bool isModify() const                                  { return GUI_IS_MODIFY;                                  };
	void setModifyFalse()                                  { GUI_IS_MODIFY = false; sendEditor( SCI_SETSAVEPOINT ); };

	virtual void clearAll();
	void clearUndoBuffer() const                           { sendEditor( SCI_EMPTYUNDOBUFFER ); };

	bool isReadOnly() const                                { return sendEditor( SCI_GETREADONLY ) ? true : false;           };
	void setReadOnly( const bool value )                   { GUI_IS_READONLY = value; sendEditor( SCI_SETREADONLY, value ); };

	bool isViewWhiteSpace() const                          { return sendEditor( SCI_GETVIEWWS ) != SCWS_INVISIBLE;                                                     };
	void setViewWhiteSpace( const bool value )             { GUI_ID_VIEW_WHITESPACE = value; sendEditor( SCI_SETVIEWWS, value ? SCWS_VISIBLEALWAYS : SCWS_INVISIBLE ); };

	bool isViewEndOfLine() const                           { return sendEditor( SCI_GETVIEWEOL ) ? true : false;                 };
	void setEndOfLine( const bool value )                  { GUI_ID_VIEW_ENDOFLINE = value; sendEditor( SCI_SETVIEWEOL, value ); };

	void appendText( CREF(tstring) str ) const;

	int getZoom() const                                    { return sendEditor( SCI_GETZOOM ); };
	void setZoom( const int value ) const                  { sendEditor( SCI_SETZOOM, value ); };
	void zoomIn() const                                    { sendEditor( SCI_ZOOMIN );         };
	void zoomOut() const                                   { sendEditor( SCI_ZOOMOUT );        };
	void resetZoom() const                                 { sendEditor( SCI_SETZOOM, 0 );     };

	bool bookmarkToggle( int line = -1 ) const;
	bool bookmarkNext( const bool canLoop = true, const bool fromCurrentLine = true, bool* wasLoop = NULL ) const;
	bool bookmarkPrev( const bool canLoop = true, const bool fromCurrentLine = true, bool* wasLoop = NULL ) const;
	void bookmarkClearAll() const;
	bool hasBookmarks() const;

	int getLength() const                           { return sendEditor( SCI_GETLENGTH );                  };
	int getLineCount() const                        { return sendEditor( SCI_GETLINECOUNT );               };
	int getCurrentPos() const                       { return sendEditor( SCI_GETCURRENTPOS );              };
	int getPositionFromLine( const int line ) const { return sendEditor( SCI_POSITIONFROMLINE, line );     };
	int getLineFromPosition( const int pos ) const  { return sendEditor( SCI_LINEFROMPOSITION, pos );      };
	void setCurrentPos( const int value ) const;
	void setCurrentPos( const int line, int pos_in_line, const bool convert_rdo_tab = false ) const;
	int getCurrentLineNumber() const                { return getLineFromPosition( getCurrentPos() );       };
	int getCurrentColumnNumber() const              { return sendEditor( SCI_GETCOLUMN, getCurrentPos() ); };
	bool isLineVisible( const int line ) const;
	void scrollToLine( const int line ) const;
	void scrollToLine2( const int line ) const;
	void scrollToCarret() const;
	void horzScrollToCurrentPos() const;

	tstring getCurrentWord() const;
	tstring getSelection() const;
	tstring getCurrentOrSelectedWord() const;
	tstring getWordForFind() const;

	int findPos( REF(tstring) findWhat, const int startFromLine = 0, const bool matchCase = false, const bool matchWholeWord = false ) const;
	tstring getLine( const int line ) const;

	void load( rdo::stream& stream );
	void save( rdo::stream& stream ) const;
	void saveAsRTF( CFile& file, int start = 0, int end = -1 ) const;
};

}; // namespace rdoEditCtrl

#endif // _RDO_STUDIO_MFC_EDIT_CTRLS_RDOBASEEDIT_H_
