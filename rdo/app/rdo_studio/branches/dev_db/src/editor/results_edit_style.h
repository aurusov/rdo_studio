/*!
  \copyright (c) RDO-Team, 2003-2012
  \file      results_edit_style.h
  \author    Урусов Андрей (rdo@rk9.bmstu.ru)
  \date      14.03.2003
  \brief     
  \indent    4T
*/

#ifndef _RDO_STUDIO_EDITOR_RESULTS_EDIT_STYLE_H_
#define _RDO_STUDIO_EDITOR_RESULTS_EDIT_STYLE_H_

// ----------------------------------------------------------------------- INCLUDES
// ----------------------------------------------------------------------- SYNOPSIS
#include "app/rdo_studio/src/editor/parser_edit_style.h"
// --------------------------------------------------------------------------------

namespace rdo { namespace gui { namespace editor {

class ResultsStyle: public ParserStyle
{
public:
	ResultsStyle();
	~ResultsStyle();
};

}}} // namespace rdo::gui::editor

#endif // _RDO_STUDIO_EDITOR_RESULTS_EDIT_STYLE_H_
