/*!
  \copyright (c) RDO-Team, 2003-2012
  \file      rdologedit.h
  \author    Урусов Андрей (rdo@rk9.bmstu.ru)
  \date      28.02.2003
  \brief     
  \indent    4T
*/

#ifndef _RDO_STUDIO_MFC_EDIT_CTRLS_RDOLOGEDIT_H_
#define _RDO_STUDIO_MFC_EDIT_CTRLS_RDOLOGEDIT_H_

// ----------------------------------------------------------------------- INCLUDES
// ----------------------------------------------------------------------- SYNOPSIS
#include "simulator/service/rdosimwin.h"
#include "app/rdo_studio_mfc/edit_ctrls/rdobaseedit.h"
#include "app/rdo_studio_mfc/edit_ctrls/rdologeditstyle.h"
// --------------------------------------------------------------------------------

namespace rdoEditor {
class RDOEditorEdit;
}

namespace rdoEditCtrl {

// --------------------------------------------------------------------------------
// -------------------- RDOLogEditLineInfo
// --------------------------------------------------------------------------------
class RDOLogEditLineInfo
{
public:
	typedef  rdo::service::simulation::RDOSyntaxMessage  RDOSyntaxMessage;

	explicit RDOLogEditLineInfo(CREF(RDOSyntaxMessage) message);
	explicit RDOLogEditLineInfo(CREF(tstring)          message);
	virtual ~RDOLogEditLineInfo();

	virtual tstring getMessage() const;
	rbool  isSimpleTextMessage() const;

	RDOFileType getFileType  () const;
	int         getLineNumber() const;
	int         getPosInLine () const;
	int         getPosInLog  () const;
	tstring     getText      () const;

	RDOSyntaxMessage::Type      getMessageType() const;
	RDOSyntaxMessage::ErrorCode getErrorCode  () const;

	void setPosInLog(int posInLog);

private:
	RDOSyntaxMessage  m_message;
	rsint             m_posInLog;
	rbool             m_simpleTextMessage;
};

// --------------------------------------------------------------------------------
// -------------------- RDOLogEdit
// --------------------------------------------------------------------------------
class RDOLogEdit: public RDOBaseEdit
{
public:
	typedef  std::list<PTR(RDOLogEditLineInfo)>  RDOLogEditLineInfoList;

public:
	RDOLogEdit();
	virtual ~RDOLogEdit();

	virtual void setEditorStyle(PTR(RDOLogEditStyle) style);

	virtual void gotoNext();
	virtual void gotoPrev();

	virtual void clearAll();
	virtual void appendLine(PTR(RDOLogEditLineInfo) line);

protected:
	void  getLines        (REF(RDOLogEditLineInfoList) lines) const;
	rsint getCurrentLine  () const;
	rsint getSciMarkerLine() const;

	void  setCurrentLine  (rsint currentLine  );
	void  setSciMarkerLine(rsint sciMarkerLine);

protected:
	virtual void updateEdit( PTR(rdoEditor::RDOEditorEdit) edit, CPTR(RDOLogEditLineInfo) lineInfo );

	void  clearLines     ();
	void  setSelectLine  ();
	void  setSelectLine  (int line, CPTR(RDOLogEditLineInfo) lineInfo, rbool useScroll = false);
	void  clearSelectLine();
	rbool hasSelectLine  () const;

	virtual BOOL OnNotify  (WPARAM wParam, LPARAM lParam, LRESULT* pResult);
	afx_msg int  OnCreate  (LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnGotoNext();
	afx_msg void OnGotoPrev();
	DECLARE_MESSAGE_MAP()

private:
	RDOLogEditLineInfoList  m_lines;
	rsint                   m_currentLine;
	rsint                   m_sciMarkerLine;
};

}; // namespace rdoEditCtrl

#endif // _RDO_STUDIO_MFC_EDIT_CTRLS_RDOLOGEDIT_H_
