/*!
  \copyright (c) RDO-Team, 2003-2012
  \file      edit.h
  \author    Урусов Андрей (rdo@rk9.bmstu.ru)
  \date      28.02.2003
  \brief     
  \indent    4T
*/

#ifndef _RDO_STUDIO_EDITOR_EDIT_H_
#define _RDO_STUDIO_EDITOR_EDIT_H_

// ----------------------------------------------------------------------- PLATFORM
// ----------------------------------------------------------------------- INCLUDES
#include "utils/warning_disable.h"
#include <vector>
#include <boost/function.hpp>
#include <QColor>
#include "thirdparty/scintilla/qt/ScintillaEditBase/ScintillaEditBase.h"
#include "utils/warning_enable.h"
// ----------------------------------------------------------------------- SYNOPSIS
#include "utils/rdostream.h"
#include "app/rdo_studio/src/editor/edit_style.h"
#include "app/rdo_studio/src/action_activator/action_activator.h"
#include "app/rdo_studio/src/dialog/find_dialog.h"
#include "app/rdo_studio/src/dialog/find_replace_dialog.h"
// --------------------------------------------------------------------------------

namespace rdo { namespace gui { namespace editor {

class Edit
	: public ScintillaEditBase
	, public ActionActivator
{
Q_OBJECT

private:
	typedef  boost::function<void (Edit*)>         this_method;
	typedef  boost::function<rbool (const Edit*)>  this_predicate;

public:
	Edit(QWidget* pParent);
	virtual ~Edit();

	//! @todo Вынести класс в отдельный модуль
	class Group
	{
	public:
		typedef std::vector<PTR(Edit)> List;

		rbool   bMatchCase;
		rbool   bMatchWholeWord;
		rbool   bSearchDown;
		QString findStr;
		QString replaceStr;

		Group();

		void insert(PTR(Edit) pEdit);

		List::const_iterator begin() const;
		List::const_iterator end  () const;
		List::const_iterator next (CREF(List::const_iterator) it) const;
		List::const_iterator prev (CREF(List::const_iterator) it) const;

		void                 for_each(CREF(this_method)    fun) const;
		List::const_iterator find_if (CREF(this_predicate) fun) const;

	private:
		List m_list;
	};

	const EditStyle* getEditorStyle() const;
	void setEditorStyle(EditStyle* pStyle);

	void setGroup(PTR(Group) pGroup);

	rbool isEmpty() const                                  { return getLength() == 0;                                                     }
	rbool isSelected() const                               { return sendEditor(SCI_GETSELECTIONSTART) != sendEditor(SCI_GETSELECTIONEND); }

	rbool isModify() const                                 { return sendEditor(SCI_GETMODIFY) ? true : false; }
	void  setModifyFalse()                                 { sendEditor(SCI_SETSAVEPOINT); }

	virtual void clearAll();
	void clearUndoBuffer() const                           { sendEditor(SCI_EMPTYUNDOBUFFER); }

	rbool isReadOnly() const                               { return sendEditor(SCI_GETREADONLY) ? true : false; }
	void setReadOnly(const rbool value)                    { sendEditor(SCI_SETREADONLY, value); }

	void appendText(CREF(QString) str) const;

	int getZoom() const                                    { return sendEditor(SCI_GETZOOM); }
	void setZoom(const int value) const                    { sendEditor(SCI_SETZOOM, value); }
	void zoomIn() const                                    { sendEditor(SCI_ZOOMIN);         }
	void zoomOut() const                                   { sendEditor(SCI_ZOOMOUT);        }
	void resetZoom() const                                 { sendEditor(SCI_SETZOOM, 0);     }

	rbool bookmarkToggle  (int line = -1) const;
	rbool bookmarkNext    (rbool canLoop = true, rbool fromCurrentLine = true) const;
	rbool bookmarkPrev    (rbool canLoop = true, rbool fromCurrentLine = true) const;
	void  bookmarkClearAll() const;
	rbool hasBookmarks    () const;

	int getLength() const                         { return sendEditor(SCI_GETLENGTH);                  }
	int getLineCount() const                      { return sendEditor(SCI_GETLINECOUNT);               }
	int getCurrentPos() const                     { return sendEditor(SCI_GETCURRENTPOS);              }
	int getPositionFromLine(const int line) const { return sendEditor(SCI_POSITIONFROMLINE, line);     }
	int getLineFromPosition(const int pos) const  { return sendEditor(SCI_LINEFROMPOSITION, pos);      }
	int isEndOfWord(int pos) const                { return sendEditor(SCI_WORDENDPOSITION, pos, true); }
	void setCurrentPos (const int value) const;
	void setCurrentPos (const int line, int pos_in_line, const rbool convert_rdo_tab = false) const;
	rbool isLineVisible(const int line) const;
	void scrollToLine  (const int line) const;
	void scrollToLine2 (const int line) const;
	void scrollToCarret() const;
	void horzScrollToCurrentPos() const;

	tstring getCurrentWord() const;
	tstring getSelection  () const;
	tstring getCurrentOrSelectedWord() const;
	QString getWordForFind() const;

	int findPos(CREF(QString) findWhat, const int startFromLine = 0, const rbool matchCase = false, const rbool matchWholeWord = false) const;
	tstring getLine(const int line) const;

	void load(rdo::stream& stream);
	void save(rdo::stream& stream) const;
	tstring saveAsRTF(int start, int end) const;

protected:
	EditStyle* m_pStyle;

	long sendEditor      (ruint msg, unsigned long wParam = 0, long lParam = 0) const { return super::send (msg, wParam, lParam); }
	long sendEditorString(ruint msg, unsigned long wParam, const char* str)     const { return super::sends(msg, wParam, str);    }
	long sendEditorString(ruint msg, unsigned long wParam, CREF(QString) str)   const;

	int  getNewMarker();
	void defineMarker(int marker, int markerType, QColor fore, QColor back) const;

	CharacterRange getSelectionRange() const;
	void setSelection(int anchor, int currentPos) const { sendEditor(SCI_SETSEL, anchor, currentPos); }

	int getCurrentLineNumber  () const { return getLineFromPosition(getCurrentPos());       }
	int getCurrentColumnNumber() const { return sendEditor(SCI_GETCOLUMN, getCurrentPos()); }

	virtual void onUpdateActions(rbool activated);

	static ruint convertColor(CREF(QColor) color);

protected slots:
	        void onUpdateEditGUI();
	virtual void onHelpContext  () = 0;

private:
	typedef  ScintillaEditBase  super;

	PTR(Group) m_pGroup;
	int        m_sciMarkerBookmark;
	int        m_firstFoundPos;
	rbool      m_haveFound;

	void gotoLineEnsureVisible(int line) const;
	void ensureRangeVisible(int posStart, int posEnd, rbool enforcePolicy = true) const;

	void findNext  (CREF(QString) findWhat, rbool searchDown, rbool matchCase, rbool matchWholeWord);
	void replace   (CREF(QString) findWhat, CREF(QString) replaceWhat, rbool searchDown, rbool matchCase, rbool matchWholeWord);
	void replaceAll(CREF(QString) findWhat, CREF(QString) replaceWhat, rbool matchCase, rbool matchWholeWord);

	int  indentOfBlock     (int line) const;
	void setLineIndentation(int line, int indent) const;
	void autoIndent        () const;

	rbool isViewWhiteSpace () const;
	void  setViewWhiteSpace(rbool value);

	rbool isViewEndOfLine () const;
	void  setViewEndOfLine(rbool value);

	void onSearchBookmarkNextPrev(
		const boost::function<rbool (const Edit*, rbool, rbool)>& nextPrevFun,
		const boost::function<Group::List::const_iterator (const Group::List::const_iterator& it)>& nextPrevGroup
	) const;

	int   m_markerCount;

	FindDialog*          m_pFindDialog;
	FindDialog::Settings m_findSettings;

	FindReplaceDialog*          m_pFindReplaceDialog;
	FindReplaceDialog::Settings m_findReplaceSettings;

	void onFindDlgFind(CREF(FindDialog::Settings) settings);
	void onFindDlgClose();

	void onFindReplaceDlgFind      (CREF(FindReplaceDialog::Settings) settings);
	void onFindReplaceDlgReplace   (CREF(FindReplaceDialog::Settings) settings);
	void onFindReplaceDlgReplaceAll(CREF(FindReplaceDialog::Settings) settings);
	void onFindReplaceDlgClose     ();
	void showFindWarning           (CREF(QString) findWhat);

	void updateActionFind(rbool activated);

	void  methodOfGroup   (CREF(this_method)    fun);
	rbool predicateOfGroup(CREF(this_predicate) fun) const;

	virtual void focusInEvent (QFocusEvent* pEvent);
	virtual void focusOutEvent(QFocusEvent* pEvent);

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
	void onEditUpperCase();
	void onEditLowerCase();

	void onSearchFind               ();
	void onSearchFindNext           ();
	void onSearchFindPrevious       ();
	void onSearchFindNextCurrent    ();
	void onSearchFindPreviousCurrent();
	void onSearchReplace            ();
	void onSearchBookmarkToggle     ();
	void onSearchBookmarkNext       () const;
	void onSearchBookmarkPrev       () const;
	void onSearchBookmarkClearAll   ();
	void onSearchGotoLine           ();

	void onViewZoomChanged   (int zoom);
	void onViewZoomInc       ();
	void onViewZoomDec       ();
	void onViewZoomReset     ();
	void onViewShowWhiteSpace();
	void onViewShowEndOfLine ();

	void onCopyAsRTF(QMimeData* pMimeData);

	void onUpdateModify();

signals:
	void modifyChanged(bool value);
};

}}} // namespace rdo::gui::editor

#endif // _RDO_STUDIO_EDITOR_EDIT_H_
