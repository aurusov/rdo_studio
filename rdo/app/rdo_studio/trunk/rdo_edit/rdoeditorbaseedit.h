/*!
  \copyright (c) RDO-Team, 2003-2012
  \file      rdoeditorbaseedit.h
  \author    Урусов Андрей (rdo@rk9.bmstu.ru)
  \date      14.03.2003
  \brief     
  \indent    4T
*/

#ifndef _RDO_STUDIO_RDO_EDIT_RDOEDITORBASEEDIT_H_
#define _RDO_STUDIO_RDO_EDIT_RDOEDITORBASEEDIT_H_

// ----------------------------------------------------------------------- INCLUDES
// ----------------------------------------------------------------------- SYNOPSIS
#include "app/rdo_studio/edit_ctrls/rdobaseedit.h"
#include "app/rdo_studio/rdo_edit/rdoeditoreditstyle.h"
// --------------------------------------------------------------------------------

namespace rdo { namespace gui { namespace editor {

class Parser: public Edit
{
public:
	Parser(PTR(QWidget) pParent);
	virtual ~Parser();

	void setEditorStyle(PTR(ParserStyle) pStyle);

	void replaceCurrent(CREF(tstring) str, int changePosValue = -1) const;

protected:
	tstring kw0;
	tstring kw1;
	tstring kw2;
	tstring kw3;
	tstring getAllKW() const;

	static tstring convertToLexer(CREF(tstring) kw);

private:
	typedef  Edit  super;
};

}}} // namespace rdo::gui::editor

#endif // _RDO_STUDIO_RDO_EDIT_RDOEDITORBASEEDIT_H_
