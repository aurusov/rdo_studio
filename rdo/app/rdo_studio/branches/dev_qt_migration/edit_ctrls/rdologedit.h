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
#include "simulator/report/log_edit_line_info.h"
#include "simulator/service/rdosimwin.h"
#include "app/rdo_studio_mfc/edit_ctrls/rdobaseedit.h"
#include "app/rdo_studio_mfc/edit_ctrls/rdologeditstyle.h"
// --------------------------------------------------------------------------------

namespace rdoEditor {
class RDOEditorEdit;
}

namespace rdoEditCtrl {

// --------------------------------------------------------------------------------
// -------------------- RDOLogEdit
// --------------------------------------------------------------------------------
class RDOLogEdit: public RDOBaseEdit
{
	Q_OBJECT
public:
	typedef rdo::simulation::report::LogEditLineInfo RDOLogEditLineInfo;
	typedef std::list<PTR(RDOLogEditLineInfo)> RDOLogEditLineInfoList;

public:
	RDOLogEdit(PTR(QWidget) pParent);
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

	afx_msg void OnGotoNext();
	afx_msg void OnGotoPrev();

private:
	RDOLogEditLineInfoList  m_lines;
	rsint                   m_currentLine;
	rsint                   m_sciMarkerLine;

private slots:
	void catchDoubleClick();
	void catchModified();
};

}; // namespace rdoEditCtrl

#endif // _RDO_STUDIO_MFC_EDIT_CTRLS_RDOLOGEDIT_H_
