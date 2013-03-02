/*!
  \copyright (c) RDO-Team, 2003-2012
  \file      rdologedit.h
  \author    Урусов Андрей (rdo@rk9.bmstu.ru)
  \date      28.02.2003
  \brief     
  \indent    4T
*/

#ifndef _RDO_STUDIO_EDIT_CTRLS_RDOLOGEDIT_H_
#define _RDO_STUDIO_EDIT_CTRLS_RDOLOGEDIT_H_

// ----------------------------------------------------------------------- INCLUDES
// ----------------------------------------------------------------------- SYNOPSIS
#include "simulator/report/log_edit_line_info.h"
#include "simulator/service/rdosimwin.h"
#include "app/rdo_studio/edit_ctrls/rdobaseedit.h"
#include "app/rdo_studio/edit_ctrls/rdologeditstyle.h"
// --------------------------------------------------------------------------------

namespace rdo { namespace gui { namespace editor {

class RDOEditorEdit;

class Log: public EditBase
{
Q_OBJECT

public:
	typedef rdo::simulation::report::LogEditLineInfo LogEditLineInfo;
	typedef std::list<PTR(LogEditLineInfo)> LogEditLineInfoList;

	Log(PTR(QWidget) pParent);
	virtual ~Log();

	virtual void setEditorStyle(PTR(LogEditStyle) pStyle);

	void gotoNext();
	void gotoPrev();

	virtual void clearAll();
	virtual void appendLine(PTR(LogEditLineInfo) pLine);

protected:
	void  getLines        (REF(LogEditLineInfoList) pLines) const;
	rsint getCurrentLine  () const;
	rsint getSciMarkerLine() const;

	void  setCurrentLine  (rsint currentLine  );
	void  setSciMarkerLine(rsint sciMarkerLine);

protected:
	virtual void updateEdit(PTR(RDOEditorEdit) pEdit, CPTR(LogEditLineInfo) pLineInfo);

	void  clearLines     ();
	void  setSelectLine  (int line, CPTR(LogEditLineInfo) pLineInfo, rbool useScroll = false);
	void  clearSelectLine();
	rbool hasSelectLine  () const;

private:
	LogEditLineInfoList  m_lines;
	rsint                m_currentLine;
	rsint                m_sciMarkerLine;

private slots:
	void catchDoubleClick(int position, int line);
	void catchModified   ();
};

}}} // namespace rdo::gui::editor

#endif // _RDO_STUDIO_EDIT_CTRLS_RDOLOGEDIT_H_
