/*!
  \copyright (c) RDO-Team, 2003-2012
  \file      results_edit.h
  \author    Урусов Андрей (rdo@rk9.bmstu.ru)
  \date      14.03.2003
  \brief     
  \indent    4T
*/

#ifndef _RDO_STUDIO_EDITOR_PARSER_EDIT_H_
#define _RDO_STUDIO_EDITOR_PARSER_EDIT_H_

// ----------------------------------------------------------------------- INCLUDES
// ----------------------------------------------------------------------- SYNOPSIS
#include "app/rdo_studio/src/editor/parser_edit.h"
#include "app/rdo_studio/src/editor/results_edit_style.h"
#include "app/rdo_studio/src/editor/edit_popup_menu.h"
// --------------------------------------------------------------------------------

namespace rdo { namespace gui { namespace editor {

class Results
	: public Parser
	, public PopupMenu
{
public:
	Results(QWidget* pParent);
	virtual ~Results();

	void setEditorStyle(ResultsStyle* pStyle);

private:
	typedef  Parser  super;

	virtual void contextMenuEvent(QContextMenuEvent* pEvent);
	virtual void onHelpContext();
};

}}} // namespace rdo::gui::editor

#endif // _RDO_STUDIO_EDITOR_PARSER_EDIT_H_
