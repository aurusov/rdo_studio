#ifndef _RDO_STUDIO_EDITOR_LOG_EDIT_H_
#define _RDO_STUDIO_EDITOR_LOG_EDIT_H_

// ----------------------------------------------------------------------- INCLUDES
// ----------------------------------------------------------------------- SYNOPSIS
#include "simulator/report/src/log_edit_line_info.h"
#include "simulator/service/src/simulator.h"
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
	typedef std::list<LogEditLineInfo*> LogEditLineInfoList;

	Log(QWidget* pParent);
	virtual ~Log();

	virtual void setEditorStyle(LogStyle* pStyle);

	void gotoNext();
	void gotoPrev();

	virtual void clearAll();
	virtual void appendLine(LogEditLineInfo* pLine);

protected:
	void getLines(LogEditLineInfoList& pLines) const;
	int getCurrentLine() const;
	int getSciMarkerLine() const;

	void setCurrentLine(int currentLine);
	void setSciMarkerLine(int sciMarkerLine);

protected:
	virtual void updateEdit(Model* pEdit, const LogEditLineInfo* pLineInfo);

	void  clearLines     ();
	void  setSelectLine  (int line, const LogEditLineInfo* pLineInfo, bool useScroll = false);
	void  clearSelectLine();
	bool  hasSelectLine  () const;

private:
	LogEditLineInfoList m_lines;
	int m_currentLine;
	int m_sciMarkerLine;

private slots:
	void catchDoubleClick(int position, int line);
	void catchModified   ();
};

}}} // namespace rdo::gui::editor

#endif // _RDO_STUDIO_EDITOR_LOG_EDIT_H_
