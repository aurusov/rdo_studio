#pragma once

// ----------------------------------------------------------------------- INCLUDES
#include "utils/src/common/warning_disable.h"
#include <QWidget>
#include <QMenu>
#include "utils/src/common/warning_enable.h"
// ----------------------------------------------------------------------- SYNOPSIS
#include "app/rdo_studio/src/editor/parser_edit.h"
#include "app/rdo_studio/src/editor/model_edit_style.h"
#include "app/rdo_studio/src/editor/log_edit.h"
// --------------------------------------------------------------------------------

namespace rdo { namespace gui { namespace editor {

class Model: public Parser
{
Q_OBJECT

public:
	Model(QWidget* pParent, QWidget* pView = NULL);
	virtual ~Model();

	void setEditorStyle(ModelStyle* pStyle);

	void setErrorLine(int line = -1);

	const Log* getLog() const;
	void setLog(Log& log);

	void setCanClearErrorLine(bool value);

private:
	typedef  Parser  super;

	QWidget*  m_pView;
	Log*      m_pLog;
	QMenu*    m_pPopupMenu;
	int       m_sciFoldMarginID;
	int       m_sciMarkerError;
	bool      m_canClearErrorLine;

	void expand         (int& line, bool doExpand, bool force = false, int visLevels = 0, int level = -1) const;
	void foldChanged    (int line, int levelNow, int levelPrev) const;
	void foldMarginClick(int position, int modifiers) const;

	void  clearErrorLine();
	bool  hasErrorLine  () const;

	virtual void contextMenuEvent(QContextMenuEvent* pEvent);
	virtual void onUpdateActions (bool activated);
	virtual void onHelpContext   ();

private slots:
	void onEditCommentSelection() const;
	void onEditCompleteWord    ();
	void onToggleAllFolds      () const;
	void onToggleCurrentFold   () const;
	void onGotoNext            ();
	void onGotoPrev            ();
	void onInsertCommand       (QObject* pObject);

	void catchModified   (int modificationType, int position, int length, int linesAdded, const QByteArray& bytes, int line, int foldLevelNow, int foldLevelPrev);
	void catchMarginClick(int position, int modifiers, int margin);
};

}}} // namespace rdo::gui::editor
