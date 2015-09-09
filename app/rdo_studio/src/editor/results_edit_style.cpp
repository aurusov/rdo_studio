// ---------------------------------------------------------------------------- PCH
#include "app/rdo_studio/pch/editor_pch.h"
// ----------------------------------------------------------------------- INCLUDES
// ----------------------------------------------------------------------- SYNOPSIS
#include "app/rdo_studio/src/editor/results_edit_style.h"
// --------------------------------------------------------------------------------

using namespace rdo::gui::style;
using namespace rdo::gui::editor;

ResultsStyle::ResultsStyle(): ParserStyle()
{}

ResultsStyle::~ResultsStyle()
{}

ResultsStyle ResultsStyle::getDefaultStyle()
{
    ResultsStyle style;
    return style;
}

ResultsStyle ResultsStyle::getCppStyle()
{
    ResultsStyle style;
    *static_cast<ParserStyle*>(&style) = ParserStyle::getDefaultStyle();

    style.identifierColor = QColor( 0x00, 0x00, 0x00 );
    style.keywordColor    = QColor( 0x00, 0x00, 0xFF );
    style.functionsColor  = QColor( 0x00, 0x00, 0xFF );
    style.traceColor      = QColor( 0x00, 0x00, 0xFF );
    style.colorColor      = QColor( 0x00, 0x00, 0xFF );
    style.commentColor    = QColor( 0x00, 0x80, 0x00 );
    style.numberColor     = QColor( 0x00, 0x00, 0x00 );
    style.stringColor     = QColor( 0x00, 0x00, 0x00 );
    style.operatorColor   = QColor( 0x00, 0x00, 0x00 );

    style.identifierStyle = StyleFont::Style::NONE;
    style.keywordStyle    = StyleFont::Style::NONE;
    style.functionsStyle  = StyleFont::Style::NONE;
    style.traceStyle      = StyleFont::Style::NONE;
    style.colorStyle      = StyleFont::Style::NONE;
    style.commentStyle    = StyleFont::Style::NONE;
    style.numberStyle     = StyleFont::Style::NONE;
    style.stringStyle     = StyleFont::Style::NONE;
    style.operatorStyle   = StyleFont::Style::NONE;

    return style;
}

ResultsStyle ResultsStyle::getPascalStyle()
{
    ResultsStyle style;
    *static_cast<ParserStyle*>(&style) = ParserStyle::getDefaultStyle();

    style.identifierColor = QColor( 0x00, 0x00, 0x00 );
    style.keywordColor    = QColor( 0x00, 0x00, 0x00 );
    style.functionsColor  = QColor( 0x00, 0x00, 0x00 );
    style.traceColor      = QColor( 0x00, 0x00, 0x00 );
    style.colorColor      = QColor( 0x00, 0x00, 0x00 );
    style.commentColor    = QColor( 0x00, 0x00, 0x80 );
    style.numberColor     = QColor( 0x00, 0x00, 0x00 );
    style.stringColor     = QColor( 0x00, 0x00, 0x00 );
    style.operatorColor   = QColor( 0x00, 0x00, 0x00 );

    style.identifierStyle = StyleFont::Style::NONE;
    style.keywordStyle    = StyleFont::Style::BOLD;
    style.functionsStyle  = StyleFont::Style::BOLD;
    style.traceStyle      = StyleFont::Style::BOLD;
    style.colorStyle      = StyleFont::Style::BOLD;
    style.commentStyle    = StyleFont::Style::ITALIC;
    style.numberStyle     = StyleFont::Style::NONE;
    style.stringStyle     = StyleFont::Style::NONE;
    style.operatorStyle   = StyleFont::Style::NONE;

    return style;
}

ResultsStyle ResultsStyle::getHtmlStyle()
{
    ResultsStyle style;
    *static_cast<ParserStyle*>(&style) = ParserStyle::getDefaultStyle();

    style.identifierColor = QColor( 0x00, 0x00, 0x00 );
    style.keywordColor    = QColor( 0x80, 0x00, 0x80 );
    style.functionsColor  = QColor( 0xFF, 0x00, 0x00 );
    style.traceColor      = QColor( 0xFF, 0x00, 0x00 );
    style.colorColor      = QColor( 0xFF, 0x00, 0x00 );
    style.commentColor    = QColor( 0x00, 0x80, 0x00 );
    style.numberColor     = QColor( 0x00, 0x00, 0xFF );
    style.stringColor     = QColor( 0x00, 0x00, 0xFF );
    style.operatorColor   = QColor( 0x00, 0x00, 0x00 );

    style.identifierStyle = StyleFont::Style::NONE;
    style.keywordStyle    = StyleFont::Style::NONE;
    style.functionsStyle  = StyleFont::Style::NONE;
    style.traceStyle      = StyleFont::Style::NONE;
    style.colorStyle      = StyleFont::Style::NONE;
    style.commentStyle    = StyleFont::Style::NONE;
    style.numberStyle     = StyleFont::Style::NONE;
    style.stringStyle     = StyleFont::Style::NONE;
    style.operatorStyle   = StyleFont::Style::NONE;

    return style;
}

ResultsStyle ResultsStyle::getClassicStyle()
{
    ResultsStyle style;
    *static_cast<ParserStyle*>(&style) = ParserStyle::getClassicStyle();

    style.identifierColor = QColor( 0xFF, 0xFF, 0x00 );
    style.keywordColor    = QColor( 0xFF, 0xFF, 0xFF );
    style.functionsColor  = QColor( 0xFF, 0xFF, 0xFF );
    style.traceColor      = QColor( 0xFF, 0xFF, 0xFF );
    style.colorColor      = QColor( 0xFF, 0xFF, 0xFF );
    style.commentColor    = QColor( 0xC0, 0xC0, 0xC0 );
    style.numberColor     = QColor( 0xFF, 0xFF, 0x00 );
    style.stringColor     = QColor( 0xFF, 0xFF, 0x00 );
    style.operatorColor   = QColor( 0xFF, 0xFF, 0x00 );

    style.identifierStyle = StyleFont::Style::NONE;
    style.keywordStyle    = StyleFont::Style::NONE;
    style.functionsStyle  = StyleFont::Style::NONE;
    style.traceStyle      = StyleFont::Style::NONE;
    style.colorStyle      = StyleFont::Style::NONE;
    style.commentStyle    = StyleFont::Style::NONE;
    style.numberStyle     = StyleFont::Style::NONE;
    style.stringStyle     = StyleFont::Style::NONE;
    style.operatorStyle   = StyleFont::Style::NONE;

    return style;
}

ResultsStyle ResultsStyle::getTwilightStyle()
{
    ResultsStyle style;
    *static_cast<ParserStyle*>(&style) = ParserStyle::getTwilightStyle();

    style.identifierColor = QColor( 0xFF, 0xFF, 0xFF );
    style.keywordColor    = QColor( 0x00, 0xFF, 0xFF );
    style.functionsColor  = QColor( 0xFF, 0xFF, 0x00 );
    style.traceColor      = QColor( 0x00, 0xFF, 0x00 );
    style.colorColor      = QColor( 0x00, 0xFF, 0x00 );
    style.commentColor    = QColor( 0xC0, 0xC0, 0xC0 );
    style.numberColor     = QColor( 0xC0, 0xC0, 0xC0 );
    style.stringColor     = QColor( 0xFF, 0xFF, 0xFF );
    style.operatorColor   = QColor( 0xFF, 0xFF, 0xFF );

    style.identifierStyle = StyleFont::Style::NONE;
    style.keywordStyle    = StyleFont::Style::BOLD;
    style.functionsStyle  = StyleFont::Style::BOLD;
    style.traceStyle      = static_cast<StyleFont::Style>(static_cast<int>(StyleFont::Style::BOLD) | static_cast<int>(StyleFont::Style::ITALIC));
    style.colorStyle      = StyleFont::Style::BOLD;
    style.commentStyle    = StyleFont::Style::ITALIC;
    style.numberStyle     = StyleFont::Style::NONE;
    style.stringStyle     = StyleFont::Style::NONE;
    style.operatorStyle   = StyleFont::Style::NONE;

    return style;
}

ResultsStyle ResultsStyle::getOceanStyle()
{
    ResultsStyle style;
    *static_cast<ParserStyle*>(&style) = ParserStyle::getOceanStyle();

    style.identifierColor = QColor( 0x00, 0x00, 0xFF );
    style.keywordColor    = QColor( 0x00, 0x00, 0x00 );
    style.functionsColor  = QColor( 0x00, 0x00, 0x00 );
    style.traceColor      = QColor( 0x00, 0x00, 0x00 );
    style.colorColor      = QColor( 0x00, 0x00, 0x00 );
    style.commentColor    = QColor( 0x00, 0x80, 0x80 );
    style.numberColor     = QColor( 0x00, 0x00, 0xFF );
    style.stringColor     = QColor( 0x00, 0x00, 0xFF );
    style.operatorColor   = QColor( 0x00, 0x00, 0xFF );

    style.identifierStyle = StyleFont::Style::NONE;
    style.keywordStyle    = StyleFont::Style::BOLD;
    style.functionsStyle  = StyleFont::Style::BOLD;
    style.traceStyle      = StyleFont::Style::BOLD;
    style.colorStyle      = StyleFont::Style::BOLD;
    style.commentStyle    = StyleFont::Style::ITALIC;
    style.numberStyle     = StyleFont::Style::NONE;
    style.stringStyle     = StyleFont::Style::NONE;
    style.operatorStyle   = StyleFont::Style::NONE;

    return style;
}