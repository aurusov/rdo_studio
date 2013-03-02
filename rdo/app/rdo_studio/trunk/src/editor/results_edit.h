/*!
  \copyright (c) RDO-Team, 2003-2012
  \file      rdoeditorresults.h
  \author    Урусов Андрей (rdo@rk9.bmstu.ru)
  \date      14.03.2003
  \brief     
  \indent    4T
*/

#ifndef _RDO_STUDIO_RDO_EDIT_RDOEDITORRESULTS_H_
#define _RDO_STUDIO_RDO_EDIT_RDOEDITORRESULTS_H_

// ----------------------------------------------------------------------- INCLUDES
// ----------------------------------------------------------------------- SYNOPSIS
#include "app/rdo_studio/rdo_edit/rdoeditorbaseedit.h"
#include "app/rdo_studio/rdo_edit/rdoeditorresultsstyle.h"
#include "app/rdo_studio/edit_ctrls/editwithreadonlypopupmenu.h"
// --------------------------------------------------------------------------------

namespace rdo { namespace gui { namespace editor {

class Results
	: public Parser
	, public PopupMenu
{
public:
	Results(PTR(QWidget) pParent);
	virtual ~Results();

	void setEditorStyle(PTR(ResultsStyle) pStyle);

private:
	typedef  Parser  super;

	virtual void mousePressEvent(QMouseEvent* pEvent);
	virtual void onHelpContext();
};

}}} // namespace rdo::gui::editor

#endif // _RDO_STUDIO_RDO_EDIT_RDOEDITORRESULTS_H_
