#ifndef RDOEDITORSCIEDIT_H
#define RDOEDITORSCIEDIT_H
#pragma once

#include "rdoeditorscieditstyle.h"
#include "sci/Scintilla.h"

using namespace std;

namespace rdoEditor {

// ----------------------------------------------------------------------------
// ---------- RDOEditorSciEdit
// ----------------------------------------------------------------------------
typedef long (*sciFunType)( long ptr, unsigned int iMessage, unsigned long wParam, long lParam );

class RDOEditorSciEdit;
typedef vector< RDOEditorSciEdit* >           RDOEditorSciEditList;
typedef vector< RDOEditorSciEdit* >::iterator RDOEditorSciEditListIterator;

class RDOEditorSciEdit: public CWnd
{
private:
	static int objectCount;

	int markerCount;

protected:
	HWND       sciHWND;
	long       sciEditor;
	sciFunType sciFun;
	long sendEditor( unsigned int msg, unsigned long wParam = 0, long lParam = 0 ) const   { return sciFun( sciEditor, msg, wParam, lParam );               };
	long sendEditorString( unsigned int msg, unsigned long wParam, const char* str ) const { return sendEditor( msg, wParam, reinterpret_cast<long>(str) ); };

	CMenu* popupMenu;

	int sci_MARKER_BOOKMARK;
	int sci_FOLDMARGIN_ID;
	int getNewMarker();
	void defineMarker( int marker, int markerType, COLORREF fore, COLORREF back ) const;

	void setSelection( int anchor, int currentPos ) const { sendEditor( SCI_SETSEL, anchor, currentPos ); };
	CharacterRange getSelectionRange() const;
	void gotoLineEnsureVisible( int line ) const;
	void ensureRangeVisible( int posStart, int posEnd, bool enforcePolicy = true ) const;

	RDOEditorSciEditStyle* style;
	RDOEditorSciEditList*  group;

	int  firstFoundPos;
	bool bHaveFound;
	bool bSearchDown;
	bool bMatchCase;
	bool bMatchWholeWord;
	CString findStr;
	void findNext( CString& findWhat, const bool searchDown = true, const bool matchCase = false, const bool matchWholeWord = false );
	void replace( CString& findWhat, CString& replaceWhat, const bool searchDown = true, const bool matchCase = false, const bool matchWholeWord = false );
	void replaceAll( CString& findWhat, CString& replaceWhat, const bool matchCase = false, const bool matchWholeWord = false );

	void copyAsRTF();

	void expand( int& line, bool doExpand, bool force = false, int visLevels = 0, int level = -1 ) const;
	void foldChanged( int line, int levelNow, int levelPrev ) const;
	void toggleAllFolds() const;
	void foldMarginClick( int position, int modifiers ) const;

	int indentOfBlock( int line ) const;
	void setLineIndentation( int line, int indent ) const;
	void autoIndent() const;

	//{{AFX_MSG(RDOEditorSciEdit)
	afx_msg int OnCreate( LPCREATESTRUCT lpCreateStruct );
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
	afx_msg void OnViewToggleCurrentFold();
	afx_msg void OnViewToggleAllFolds();
	afx_msg void OnUpdateFold( CCmdUI* pCmdUI );
	afx_msg void OnBookmarkToggle();
	afx_msg void OnBookmarkNext();
	afx_msg void OnBookmarkPrev();
	afx_msg void OnBookmarkClearAll();
	afx_msg void OnHasBookmarks( CCmdUI* pCmdUI );
	afx_msg void OnViewWhiteSpace();
	afx_msg void OnViewEndOfLine();
	afx_msg void OnViewLineNumberMargin();
	afx_msg void OnViewMargin();
	afx_msg void OnViewFoldMargin();
	afx_msg void OnViewZoomIn();
	afx_msg void OnViewZoomOut();
	afx_msg void OnViewZoomReset();
	afx_msg void OnUpdateWhiteSpace( CCmdUI* pCmdUI );
	afx_msg void OnUpdateEndOfLine( CCmdUI* pCmdUI );
	afx_msg void OnUpdateViewLineNumberMargin( CCmdUI* pCmdUI );
	afx_msg void OnUpdateViewMargin( CCmdUI* pCmdUI );
	afx_msg void OnUpdateViewFoldMargin( CCmdUI* pCmdUI );
	afx_msg void OnUpdateZoomIn( CCmdUI *pCmdUI );
	afx_msg void OnUpdateZoomOut( CCmdUI *pCmdUI );
	afx_msg void OnUpdateZoomReset( CCmdUI *pCmdUI );
	afx_msg void OnIsSelected(CCmdUI* pCmdUI);
	//}}AFX_MSG
	afx_msg LRESULT OnFindReplaceMsg( WPARAM wParam, LPARAM lParam );
	DECLARE_MESSAGE_MAP()

	//{{AFX_VIRTUAL(RDOEditorSciEdit)
	protected:
	virtual BOOL PreCreateWindow( CREATESTRUCT& cs );
	virtual BOOL OnNotify( WPARAM wParam, LPARAM lParam, LRESULT* pResult );
	virtual BOOL OnCommand(WPARAM wParam, LPARAM lParam);
	//}}AFX_VIRTUAL

public:
	RDOEditorSciEdit();
	virtual ~RDOEditorSciEdit();

	const RDOEditorSciEditStyle* getEditorStyle() const       { return style; };
	virtual void setEditorStyle( RDOEditorSciEditStyle* style );

	void setGroup( RDOEditorSciEditList* _group );
	void setPopupMenu( CMenu* const value )                { popupMenu = value; };

	bool isEmpty() const                                   { return getLength() == 0;                                                         };
	bool isSelected() const                                { return sendEditor( SCI_GETSELECTIONSTART ) != sendEditor( SCI_GETSELECTIONEND ); };
	bool isOverwrite() const                               { return sendEditor( SCI_GETOVERTYPE ) ? true : false;                             };

	bool isModify() const                                  { return sendEditor( SCI_GETMODIFY ) ? true : false; };
	void setModifyFalse() const                            { sendEditor( SCI_SETSAVEPOINT ); clearUndoBuffer(); };

	void clearAll() const;
	void clearUndoBuffer() const                           { sendEditor( SCI_EMPTYUNDOBUFFER ); };

	void replaceCurrent( const CString str, const int changePosValue = -1 ) const;
	void appendText( const string& str ) const;

	bool isReadOnly() const                                { return sendEditor( SCI_GETREADONLY ) ? true : false;  };
	void setReadOnly( const bool value ) const             { sendEditor( SCI_SETREADONLY, value );                 };

	bool isViewWhiteSpace() const                          { return sendEditor( SCI_GETVIEWWS ) != SCWS_INVISIBLE;                     };
	void setViewWhiteSpace( const bool value ) const       { sendEditor( SCI_SETVIEWWS, value ? SCWS_VISIBLEALWAYS : SCWS_INVISIBLE ); };

	bool isViewEndOfLine() const                           { return sendEditor( SCI_GETVIEWEOL ) ? true : false; };
	void setEndOfLine( const bool value ) const            { sendEditor( SCI_SETVIEWEOL, value );                };

	bool isViewLineNumberMargin() const                    { return sendEditor( SCI_GETMARGINWIDTHN, 0 ) != 0;     };
	void setViewLineNumberMargin( const bool value ) const { sendEditor( SCI_SETMARGINWIDTHN, 0, value ? 40 : 0 ); };

	bool isViewMargin() const                              { return sendEditor( SCI_GETMARGINWIDTHN, 1 ) != 0;     };
	void setViewMargin( const bool value ) const           { sendEditor( SCI_SETMARGINWIDTHN, 1, value ? 16 : 0 ); };

	bool isViewFoldMargin() const                          { return sendEditor( SCI_GETMARGINWIDTHN, 2 ) != 0;     };
	void setViewFoldMargin( const bool value ) const       { sendEditor( SCI_SETMARGINWIDTHN, 2, value ? 16 : 0 ); };

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

	int getLength() const                       { return sendEditor( SCI_GETLENGTH );                           };
	int getLineCount() const                    { return sendEditor( SCI_GETLINECOUNT );                        };
	int getCurrentPos() const                   { return sendEditor( SCI_GETCURRENTPOS );                       };
	void setCurrentPos( const int value ) const;
	void setCurrentPos( const int line, int pos_in_line, const bool convert_rdo_tab = false ) const;
	int getCurrentLineNumber() const            { return sendEditor( SCI_LINEFROMPOSITION, getCurrentPos() );   };
	int getCurrentColumnNumber() const          { return sendEditor( SCI_GETCOLUMN, getCurrentPos() );          };
	bool isLineVisible( const int line ) const;
	void scrollToLine( const int line ) const;

	CString getCurrentWord() const;
	CString getSelection() const;
	CString getCurrentOrSelectedWord() const;

	int findLine( CString& findWhat, const int startFromLine, const bool matchCase = false, const bool matchWholeWord = false ) const;
	string getLine( const int line ) const;

	void load( strstream& stream ) const;
	void save( strstream& stream ) const;
	void saveAsRTF( CFile& file, int start = 0, int end = -1 ) const;
};

}; // namespace rdoEditor

//{{AFX_INSERT_LOCATION}}

#endif // RDOEDITORSCIEDIT_H
