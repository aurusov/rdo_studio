#pragma once

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

    static ResultsStyle getDefaultStyle();
    static ResultsStyle getCppStyle();
    static ResultsStyle getPascalStyle();
    static ResultsStyle getHtmlStyle();
    static ResultsStyle getClassicStyle();
    static ResultsStyle getTwilightStyle();
    static ResultsStyle getOceanStyle();
};

}}} // namespace rdo::gui::editor
