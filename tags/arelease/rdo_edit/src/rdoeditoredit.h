#ifndef RDOEDITOREDIT_H
#define RDOEDITOREDIT_H
#pragma once

#include "rdoeditoreditstyle.h"
#include "rdoEditorProject.h"
#include "sci/Scintilla.h"

// ----------------------------------------------------------------------------
// ---------- RDOEditorEdit
// ----------------------------------------------------------------------------
typedef long (*sciFunType)( long ptr, unsigned int iMessage, unsigned long wParam, long lParam );

class RDOEditorEdit: public CWnd
{
private:
	static int objectCount;

protected:

	virtual BOOL PreCreateWindow( CREATESTRUCT& cs );

	HWND       sciHWND;
	long       sciEditor;
	sciFunType sciFun;
	long sendEditor( unsigned int msg, unsigned long wParam = 0, long lParam = 0 ) const   { return sciFun( sciEditor, msg, wParam, lParam );               };
	long sendEditorString( unsigned int msg, unsigned long wParam, const char* str ) const { return sendEditor( msg, wParam, reinterpret_cast<long>(str) ); };

	CMenu* popupMenu;

	void defineMarker( int marker, int markerType, COLORREF fore, COLORREF back ) const;

	void setSelection( int anchor, int currentPos ) const { sendEditor( SCI_SETSEL, anchor, currentPos ); };
	CharacterRange getSelectionRange() const;
	void gotoLineEnsureVisible( int line ) const;
	void ensureRangeVisible( int posStart, int posEnd, bool enforcePolicy = true ) const;

	RDOEditorEditStyle style;

	int bufSelStart;

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
	void commentSelection() const;
	void completeWord() const;

	void expand( int& line, bool doExpand, bool force = false, int visLevels = 0, int level = -1 ) const;
	void foldChanged( int line, int levelNow, int levelPrev ) const;
	void toggleAllFolds() const;
	void foldMarginClick( int position, int modifiers ) const;

	int indentOfBlock( int line ) const;
	void setLineIndentation( int line, int indent ) const;
	void autoIndent() const;

	virtual BOOL OnNotify( WPARAM wParam, LPARAM lParam, LRESULT* pResult );

	afx_msg int OnCreate( LPCREATESTRUCT lpCreateStruct );
	afx_msg void OnSetFocus( CWnd *pOldWnd );
	afx_msg void OnSize( UINT nType, int cx, int cy );
	afx_msg void OnInitMenuPopup( CMenu* pPopupMenu, UINT nIndex, BOOL bSysMenu );
	afx_msg void OnContextMenu( CWnd* pWnd, CPoint pos );
	afx_msg void OnKeyDown( UINT nChar, UINT nRepCnt, UINT nFlags );

	afx_msg void OnSciCommand( UINT nID );
	afx_msg void OnCanUndo( CCmdUI* pCmdUI )     { pCmdUI->Enable( sendEditor(SCI_CANUNDO) );                             };
	afx_msg void OnCanRedo( CCmdUI* pCmdUI )     { pCmdUI->Enable( sendEditor(SCI_CANREDO) );                             };
	afx_msg void OnIsSelected( CCmdUI* pCmdUI )  { pCmdUI->Enable( isSelected() );                                        };
	afx_msg void OnCanPaste( CCmdUI* pCmdUI )    { pCmdUI->Enable( sendEditor(SCI_CANPASTE) );                            };
	afx_msg void OnCanDelete( CCmdUI* pCmdUI )   { pCmdUI->Enable( getCurrentPos() != getLength() || isSelected() );      };
	afx_msg void OnSelectAll( CCmdUI* pCmdUI )   { pCmdUI->Enable( !isEmpty() );                                          };

	afx_msg void OnFindReplace( UINT nID );
	afx_msg void OnFindNextPrev( UINT nID );
	afx_msg void OnFindNextPrevFast( UINT nID );
	afx_msg LRESULT OnFindReplaceMsg( WPARAM wParam, LPARAM lParam );
	afx_msg void OnUpdateFindNextPrev( CCmdUI* pCmdUI )                { pCmdUI->Enable( !findStr.IsEmpty() ); };

	afx_msg void OnInsertCommand( UINT nID );

	afx_msg void OnBufferPaste( UINT nID );
	afx_msg void OnUndateBuffer1Paste( CCmdUI* pCmdUI )  { pCmdUI->Enable( !rdoEditorProject.buf1.IsEmpty() ); };
	afx_msg void OnUndateBuffer2Paste( CCmdUI* pCmdUI )  { pCmdUI->Enable( !rdoEditorProject.buf2.IsEmpty() ); };
	afx_msg void OnUndateBuffer3Paste( CCmdUI* pCmdUI )  { pCmdUI->Enable( !rdoEditorProject.buf3.IsEmpty() ); };
	afx_msg void OnUndateBuffer4Paste( CCmdUI* pCmdUI )  { pCmdUI->Enable( !rdoEditorProject.buf4.IsEmpty() ); };
	afx_msg void OnBufferAppend( UINT nID );
	afx_msg void OnUndateBufferAppend( CCmdUI* pCmdUI )  { pCmdUI->Enable( isSelected() || getCurrentPos() != getLength() ); };
	afx_msg void OnBufferEdit( UINT nID );
	afx_msg void OnBufferClear( UINT nID );
	afx_msg void OnUndateBuffer1Clear( CCmdUI* pCmdUI )  { pCmdUI->Enable( !rdoEditorProject.buf1.IsEmpty() ); };
	afx_msg void OnUndateBuffer2Clear( CCmdUI* pCmdUI )  { pCmdUI->Enable( !rdoEditorProject.buf2.IsEmpty() ); };
	afx_msg void OnUndateBuffer3Clear( CCmdUI* pCmdUI )  { pCmdUI->Enable( !rdoEditorProject.buf3.IsEmpty() ); };
	afx_msg void OnUndateBuffer4Clear( CCmdUI* pCmdUI )  { pCmdUI->Enable( !rdoEditorProject.buf4.IsEmpty() ); };

	afx_msg void OnHelpKeyword();

	DECLARE_MESSAGE_MAP()

public:
	RDOEditorEdit();
	virtual ~RDOEditorEdit();

	const RDOEditorEditStyle& getEditorStyle() const       { return style;      };
	void setEditorStyle( const RDOEditorEditStyle& style );

	void setPopupMenu( CMenu* const value )                { popupMenu = value; };

	bool isEmpty() const                                   { return getLength() == 0;                                                         };
	bool isSelected() const                                { return sendEditor( SCI_GETSELECTIONSTART ) != sendEditor( SCI_GETSELECTIONEND ); };
	bool isOverwrite() const                               { return sendEditor( SCI_GETOVERTYPE ) ? true : false;                             };

	bool isModify() const                                  { return sendEditor( SCI_GETMODIFY ) ? true : false; };
	void setModifyFalse() const                            { sendEditor( SCI_SETSAVEPOINT ); clearUndoBuffer(); };

	void clearAll() const;
	void clearUndoBuffer() const                           { sendEditor( SCI_EMPTYUNDOBUFFER ); };

	void replaceCurrent( const CString str, const int changePosValue = -1 ) const;

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

	void setErrorLine( int line = -1 );
	void clearErrorLine();
	bool hasErrorLine() const;

	int getLength() const                       { return sendEditor( SCI_GETLENGTH );                           };
	int getLineCount() const                    { return sendEditor( SCI_GETLINECOUNT );                        };
	int getCurrentPos() const                   { return sendEditor( SCI_GETCURRENTPOS );                       };
	void setCurrentPos( const int value ) const;
	void setCurrentPos( const int line, int pos_in_line, const bool convert_rdo_tab = false ) const;
	int getCurrentLineNumber() const            { return sendEditor( SCI_LINEFROMPOSITION, getCurrentPos() );   };
	int getCurrentColumnNumber() const          { return sendEditor( SCI_GETCOLUMN, getCurrentPos() );          };

	CString getCurrentWord() const;
	CString getSelection() const;
	CString getCurrentOrSelectedWord() const;

	void load( CFile& file ) const;
	void save( CFile& file ) const;
	void saveAsRTF( CFile& file, int start = 0, int end = -1 ) const;
};

#endif // RDOEDITOREDIT_H
