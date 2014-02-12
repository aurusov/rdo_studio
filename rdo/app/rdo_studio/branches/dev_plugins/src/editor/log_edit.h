/*!
  \copyright (c) RDO-Team, 2003-2012
  \file      log_edit.h
  \author    Урусов Андрей (rdo@rk9.bmstu.ru)
  \date      28.02.2003
  \brief     
  \indent    4T
*/

#ifndef _RDO_STUDIO_EDITOR_LOG_EDIT_H_
#define _RDO_STUDIO_EDITOR_LOG_EDIT_H_

// ----------------------------------------------------------------------- INCLUDES
// ----------------------------------------------------------------------- SYNOPSIS
#include "simulator/report/log_edit_line_info.h"
#include "simulator/service/rdosimwin.h"
#include "app/rdo_studio/src/editor/edit.h"
#include "app/rdo_studio/src/editor/log_edit_style.h"
// --------------------------------------------------------------------------------

namespace rdo { namespace gui { namespace editor {

class Model;

class Log: public Edit
{
Q_OBJECT

public:
	typedef rdo::simulation::report::LogEditLineInfo LogEditLineInfo;
	typedef std::list<PTR(LogEditLineInfo)> LogEditLineInfoList;

	Log(QWidget* pParent);
	virtual ~Log();

	virtual void setEditorStyle(PTR(LogStyle) pStyle);

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
	virtual void updateEdit(PTR(Model) pEdit, CPTR(LogEditLineInfo) pLineInfo);

	void  clearLines     ();
	void  setSelectLine  (int line, CPTR(LogEditLineInfo) pLineInfo, bool useScroll = false);
	void  clearSelectLine();
	bool  hasSelectLine  () const;

private:
	LogEditLineInfoList  m_lines;
	rsint                m_currentLine;
	rsint                m_sciMarkerLine;

private slots:
	void catchDoubleClick(int position, int line);
	void catchModified   ();
};

}}} // namespace rdo::gui::editor

#endif // _RDO_STUDIO_EDITOR_LOG_EDIT_H_
