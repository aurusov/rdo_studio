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
#include <QtGui/qmenu.h>
// ----------------------------------------------------------------------- SYNOPSIS
#include "utils/rdostream.h"
#include "app/rdo_studio_mfc/edit_ctrls/rdobaseeditstyle.h"
#include "app/rdo_studio_mfc/src/action_activator/action_activator.h"
#include "thirdparty/sci/qt/ScintillaEditBase/ScintillaEditBase.h"
// --------------------------------------------------------------------------------

namespace rdoEditCtrl {

// --------------------------------------------------------------------------------
// -------------------- RDOBaseEdit
// --------------------------------------------------------------------------------
class RDOBaseEdit;

typedef std::vector< RDOBaseEdit* >                 RDOBaseEditList;
typedef std::vector< RDOBaseEdit* >::const_iterator RDOBaseEditListIterator;

class RDOBaseEditGroup {
private:
	RDOBaseEditList list;

public:
	rbool bMatchCase;
	rbool bMatchWholeWord;
	rbool bSearchDown;
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

class RDOBaseEdit
	: public ScintillaEditBase
	, public ActionActivator
{
Q_OBJECT
private:
	typedef  ScintillaEditBase  super;
	int   markerCount;

protected:
	rbool GUI_HAS_BOOKMARK;
	rbool GUI_ID_VIEW_WHITESPACE;
	rbool GUI_ID_VIEW_ENDOFLINE;

	long sendEditor( unsigned int msg, unsigned long wParam = 0, long lParam = 0 ) const   { return super::send( msg, wParam, lParam );; };
	long sendEditorString( unsigned int msg, unsigned long wParam, const char* str ) const { return super::sends( msg, wParam, str ); };
	QMenu* popupMenu;

	int sci_MARKER_BOOKMARK;
	int getNewMarker();
	void defineMarker( int marker, int markerType, COLORREF fore, COLORREF back ) const;

	void setSelection( int anchor, int currentPos ) const { sendEditor( SCI_SETSEL, anchor, currentPos ); };
	CharacterRange getSelectionRange() const;
	void gotoLineEnsureVisible( int line ) const;
	void ensureRangeVisible( int posStart, int posEnd, rbool enforcePolicy = true ) const;

	RDOBaseEditStyle* style;
	RDOBaseEditGroup* group;

	int  firstFoundPos;
	rbool bHaveFound;
	void findNext( REF(tstring) findWhat, const rbool searchDown = true, const rbool matchCase = false, const rbool matchWholeWord = false );
	void replace( REF(tstring) findWhat, REF(tstring) replaceWhat, const rbool searchDown = true, const rbool matchCase = false, const rbool matchWholeWord = false );
	void replaceAll( REF(tstring) findWhat, REF(tstring) replaceWhat, const rbool matchCase = false, const rbool matchWholeWord = false );

	int indentOfBlock( int line ) const;
	void setLineIndentation( int line, int indent ) const;
	void autoIndent() const;

	void updateAllGUI();
	void updateBookmarksGUI();

protected:
	virtual void focusInEvent (QFocusEvent* pEvent);
	virtual void focusOutEvent(QFocusEvent* pEvent);
	virtual void onActivate   ();
	virtual void onDeactivate ();

	afx_msg void OnIsSelected(CCmdUI* pCmdUI);

protected slots:
	        void onUpdateEditGUI();
	virtual void onHelpContext  () = 0;

private:
	void updateActions(rbool activated);

	//! @todo qt
	//afx_msg void OnSetFocus( CWnd *pOldWnd );
	//afx_msg void OnContextMenu( CWnd* pWnd, CPoint pos );
	afx_msg void OnEditUpperCase();
	afx_msg void OnEditLowerCase();
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

private slots:
	void catchNeedShown(int position, int length);
	void catchCharAdded(int ch);

	void onEditUndo     ();
	void onEditRedo     ();
	void onEditCut      ();
	void onEditCopy     ();
	void onEditPaste    ();
	void onEditDel      ();
	void onEditSelectAll();

	void onCopyAsRTF(QMimeData* pMimeData);

	void onUpdateModify ();

public:
	RDOBaseEdit(PTR(QWidget) pParent);
	virtual ~RDOBaseEdit();

	const RDOBaseEditStyle* getEditorStyle() const         { return style; };
	void setEditorStyle( RDOBaseEditStyle* _style );

	void setGroup( RDOBaseEditGroup* _group );
	void setPopupMenu( QMenu* const value )                { popupMenu = value; };

	rbool isEmpty() const                                  { return getLength() == 0;                                                         };
	rbool isSelected() const                               { return sendEditor( SCI_GETSELECTIONSTART ) != sendEditor( SCI_GETSELECTIONEND ); };

	rbool isModify() const                                 { return sendEditor(SCI_GETMODIFY) ? true : false; };
	void  setModifyFalse()                                 { sendEditor(SCI_SETSAVEPOINT); };

	virtual void clearAll();
	void clearUndoBuffer() const                           { sendEditor( SCI_EMPTYUNDOBUFFER ); };

	rbool isReadOnly() const                               { return sendEditor( SCI_GETREADONLY ) ? true : false;           };
	void setReadOnly( const rbool value )                  { sendEditor( SCI_SETREADONLY, value ); };

	rbool isViewWhiteSpace() const                         { return sendEditor( SCI_GETVIEWWS ) != SCWS_INVISIBLE;                                                     };
	void setViewWhiteSpace( const rbool value )            { GUI_ID_VIEW_WHITESPACE = value; sendEditor( SCI_SETVIEWWS, value ? SCWS_VISIBLEALWAYS : SCWS_INVISIBLE ); };

	rbool isViewEndOfLine() const                          { return sendEditor( SCI_GETVIEWEOL ) ? true : false;                 };
	void setEndOfLine( const rbool value )                 { GUI_ID_VIEW_ENDOFLINE = value; sendEditor( SCI_SETVIEWEOL, value ); };

	void appendText( CREF(tstring) str ) const;

	int getZoom() const                                    { return sendEditor( SCI_GETZOOM ); };
	void setZoom( const int value ) const                  { sendEditor( SCI_SETZOOM, value ); };
	void zoomIn() const                                    { sendEditor( SCI_ZOOMIN );         };
	void zoomOut() const                                   { sendEditor( SCI_ZOOMOUT );        };
	void resetZoom() const                                 { sendEditor( SCI_SETZOOM, 0 );     };

	rbool bookmarkToggle( int line = -1 ) const;
	rbool bookmarkNext( const rbool canLoop = true, const rbool fromCurrentLine = true, rbool* wasLoop = NULL ) const;
	rbool bookmarkPrev( const rbool canLoop = true, const rbool fromCurrentLine = true, rbool* wasLoop = NULL ) const;
	void  bookmarkClearAll() const;
	rbool hasBookmarks() const;

	int getLength() const                           { return sendEditor( SCI_GETLENGTH );                  };
	int getLineCount() const                        { return sendEditor( SCI_GETLINECOUNT );               };
	int getCurrentPos() const                       { return sendEditor( SCI_GETCURRENTPOS );              };
	int getPositionFromLine( const int line ) const { return sendEditor( SCI_POSITIONFROMLINE, line );     };
	int getLineFromPosition( const int pos ) const  { return sendEditor( SCI_LINEFROMPOSITION, pos );      };
	void setCurrentPos( const int value ) const;
	void setCurrentPos( const int line, int pos_in_line, const rbool convert_rdo_tab = false ) const;
	rbool isLineVisible( const int line ) const;
	void scrollToLine( const int line ) const;
	void scrollToLine2( const int line ) const;
	void scrollToCarret() const;
	void horzScrollToCurrentPos() const;

	tstring getCurrentWord() const;
	tstring getSelection() const;
	tstring getCurrentOrSelectedWord() const;
	tstring getWordForFind() const;

	int findPos( REF(tstring) findWhat, const int startFromLine = 0, const rbool matchCase = false, const rbool matchWholeWord = false ) const;
	tstring getLine( const int line ) const;

	void load( rdo::stream& stream );
	void save( rdo::stream& stream ) const;
	tstring saveAsRTF(int start, int end) const;

signals:
	void modifyChanged(bool value);

protected:
	int getCurrentLineNumber  () const { return getLineFromPosition(getCurrentPos());       };
	int getCurrentColumnNumber() const { return sendEditor(SCI_GETCOLUMN, getCurrentPos()); };
};

} // namespace rdoEditCtrl

#endif // _RDO_STUDIO_MFC_EDIT_CTRLS_RDOBASEEDIT_H_
