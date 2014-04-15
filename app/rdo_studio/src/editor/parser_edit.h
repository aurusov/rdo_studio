/*!
  \copyright (c) RDO-Team, 2003-2012
  \file      parser_edit.h
  \author    Урусов Андрей (rdo@rk9.bmstu.ru)
  \date      14.03.2003
  \brief     
  \indent    4T
*/

#ifndef _RDO_STUDIO_RDO_EDIT_RDOEDITORBASEEDIT_H_
#define _RDO_STUDIO_RDO_EDIT_RDOEDITORBASEEDIT_H_

// ----------------------------------------------------------------------- INCLUDES
// ----------------------------------------------------------------------- SYNOPSIS
#include "app/rdo_studio/src/editor/edit.h"
#include "app/rdo_studio/src/editor/model_edit_style.h"
// --------------------------------------------------------------------------------

namespace rdo { namespace gui { namespace editor {

class Parser: public Edit
{
public:
	Parser(QWidget* pParent);
	virtual ~Parser();

	void setEditorStyle(ParserStyle* pStyle);

	void replaceCurrent(CREF(QString) str, int changePosValue = -1) const;

protected:
	std::string kw0;
	std::string kw1;
	std::string kw2;
	std::string kw3;
	std::string getAllKW() const;

	static std::string convertToLexer(CREF(std::string) kw);

private:
	typedef  Edit  super;
};

}}} // namespace rdo::gui::editor

#endif // _RDO_STUDIO_RDO_EDIT_RDOEDITORBASEEDIT_H_
