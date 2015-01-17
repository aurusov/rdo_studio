// ---------------------------------------------------------------------------- PCH
#include "app/rdo_studio/pch/editor_pch.h"
// ----------------------------------------------------------------------- INCLUDES
// ----------------------------------------------------------------------- SYNOPSIS
#include "app/rdo_studio/src/editor/parser_edit_style.h"
#include "thirdparty/scintilla/include/SciLexer.h"
// --------------------------------------------------------------------------------

using namespace rdo::gui::style;
using namespace rdo::gui::editor;

// --------------------------------------------------------------------------------
// -------------------- ParserStyle
// --------------------------------------------------------------------------------
ParserStyle::ParserStyle(): EditStyle()
{
    defaultColor    = QColor(0x80, 0x80, 0x80);
    identifierColor = QColor(0x00, 0x00, 0x00);
    keywordColor    = QColor(0x00, 0x00, 0x00);
    functionsColor  = QColor(0x00, 0x80, 0x00);
    traceColor      = QColor(0x80, 0x00, 0x00);
    colorColor      = QColor(0x09, 0x26, 0xB0);
    commentColor    = QColor(0x00, 0x80, 0x00);
    numberColor     = QColor(0x00, 0x00, 0x80);
    stringColor     = QColor(0x80, 0x00, 0x80);
    operatorColor   = QColor(0x00, 0x00, 0x00);

    identifierStyle = StyleFont::Style::NONE;
    keywordStyle    = StyleFont::Style::BOLD;
    functionsStyle  = StyleFont::Style::BOLD;
    traceStyle      = StyleFont::Style::BOLD;
    colorStyle      = StyleFont::Style::BOLD;
    commentStyle    = StyleFont::Style::ITALIC;
    numberStyle     = StyleFont::Style::NONE;
    stringStyle     = StyleFont::Style::NONE;
    operatorStyle   = StyleFont::Style::NONE;
}

ParserStyle::~ParserStyle()
{}

ParserStyle& ParserStyle::operator =(const ParserStyle& style)
{
    EditStyle::operator=(style);

    identifierColor = style.identifierColor;
    keywordColor    = style.keywordColor;
    functionsColor  = style.functionsColor;
    traceColor      = style.traceColor;
    colorColor      = style.colorColor;
    commentColor    = style.commentColor;
    numberColor     = style.numberColor;
    stringColor     = style.stringColor;
    operatorColor   = style.operatorColor;

    identifierStyle = style.identifierStyle;
    keywordStyle    = style.keywordStyle;
    functionsStyle  = style.functionsStyle;
    traceStyle      = style.traceStyle;
    colorStyle      = style.colorStyle;
    commentStyle    = style.commentStyle;
    numberStyle     = style.numberStyle;
    stringStyle     = style.stringStyle;
    operatorStyle   = style.operatorStyle;

    return *this;
}

bool ParserStyle::operator ==(const ParserStyle& style) const
{

    return EditStyle::operator==(style) &&
        identifierColor == style.identifierColor &&
        keywordColor    == style.keywordColor &&
        functionsColor  == style.functionsColor &&
        traceColor      == style.traceColor &&
        colorColor      == style.colorColor &&
        commentColor    == style.commentColor &&
        numberColor     == style.numberColor &&
        stringColor     == style.stringColor &&
        operatorColor   == style.operatorColor &&

        identifierStyle == style.identifierStyle &&
        keywordStyle    == style.keywordStyle &&
        functionsStyle  == style.functionsStyle &&
        traceStyle      == style.traceStyle &&
        colorStyle      == style.colorStyle &&
        commentStyle    == style.commentStyle &&
        numberStyle     == style.numberStyle &&
        stringStyle     == style.stringStyle &&
        operatorStyle   == style.operatorStyle;
}

bool ParserStyle::operator !=(const ParserStyle& style) const
{
    return !(*this == style);
}

bool ParserStyle::styleDefault(const int styleType) const
{
    return styleType == SCE_RDO_DEFAULT;
}

bool ParserStyle::styleUsing(const int styleType) const
{
    return
        styleType == SCE_RDO_DEFAULT      || styleType == SCE_RDO_IDENTIFIER   ||
        styleType == SCE_RDO_KEYWORD      || styleType == SCE_RDO_FUNCTION     ||
        styleType == SCE_RDO_TRACE        || styleType == SCE_RDO_COMMENT_CPP  ||
        styleType == SCE_RDO_COMMENT_LINE || styleType == SCE_RDO_NUMBER       ||
        styleType == SCE_RDO_STRING       || styleType == SCE_RDO_OPERATOR     ||
        styleType == SCE_RDO_FRAME_COLOR;
}

bool ParserStyle::styleBold(const int styleType) const
{
    switch (styleType) {
        case SCE_RDO_DEFAULT     : return static_cast<int>(defaultStyle)    & static_cast<int>(StyleFont::Style::BOLD) ? true : false;
        case SCE_RDO_IDENTIFIER  : return static_cast<int>(identifierStyle) & static_cast<int>(StyleFont::Style::BOLD) ? true : false;
        case SCE_RDO_KEYWORD     : return static_cast<int>(keywordStyle)    & static_cast<int>(StyleFont::Style::BOLD) ? true : false;
        case SCE_RDO_FUNCTION    : return static_cast<int>(functionsStyle)  & static_cast<int>(StyleFont::Style::BOLD) ? true : false;
        case SCE_RDO_TRACE       : return static_cast<int>(traceStyle)      & static_cast<int>(StyleFont::Style::BOLD) ? true : false;
        case SCE_RDO_FRAME_COLOR : return static_cast<int>(colorStyle)      & static_cast<int>(StyleFont::Style::BOLD) ? true : false;
        case SCE_RDO_COMMENT_CPP : return static_cast<int>(commentStyle)    & static_cast<int>(StyleFont::Style::BOLD) ? true : false;
        case SCE_RDO_COMMENT_LINE: return static_cast<int>(commentStyle)    & static_cast<int>(StyleFont::Style::BOLD) ? true : false;
        case SCE_RDO_NUMBER      : return static_cast<int>(numberStyle)     & static_cast<int>(StyleFont::Style::BOLD) ? true : false;
        case SCE_RDO_STRING      : return static_cast<int>(stringStyle)     & static_cast<int>(StyleFont::Style::BOLD) ? true : false;
        case SCE_RDO_OPERATOR    : return static_cast<int>(operatorStyle)   & static_cast<int>(StyleFont::Style::BOLD) ? true : false;
    }
    return false;
}

bool ParserStyle::styleItalic(const int styleType) const
{
    switch (styleType) {
        case SCE_RDO_DEFAULT     : return static_cast<int>(defaultStyle)    & static_cast<int>(StyleFont::Style::ITALIC) ? true : false;
        case SCE_RDO_IDENTIFIER  : return static_cast<int>(identifierStyle) & static_cast<int>(StyleFont::Style::ITALIC) ? true : false;
        case SCE_RDO_KEYWORD     : return static_cast<int>(keywordStyle)    & static_cast<int>(StyleFont::Style::ITALIC) ? true : false;
        case SCE_RDO_FUNCTION    : return static_cast<int>(functionsStyle)  & static_cast<int>(StyleFont::Style::ITALIC) ? true : false;
        case SCE_RDO_TRACE       : return static_cast<int>(traceStyle)      & static_cast<int>(StyleFont::Style::ITALIC) ? true : false;
        case SCE_RDO_FRAME_COLOR : return static_cast<int>(colorStyle)      & static_cast<int>(StyleFont::Style::ITALIC) ? true : false;
        case SCE_RDO_COMMENT_CPP : return static_cast<int>(commentStyle)    & static_cast<int>(StyleFont::Style::ITALIC) ? true : false;
        case SCE_RDO_COMMENT_LINE: return static_cast<int>(commentStyle)    & static_cast<int>(StyleFont::Style::ITALIC) ? true : false;
        case SCE_RDO_NUMBER      : return static_cast<int>(numberStyle)     & static_cast<int>(StyleFont::Style::ITALIC) ? true : false;
        case SCE_RDO_STRING      : return static_cast<int>(stringStyle)     & static_cast<int>(StyleFont::Style::ITALIC) ? true : false;
        case SCE_RDO_OPERATOR    : return static_cast<int>(operatorStyle)   & static_cast<int>(StyleFont::Style::ITALIC) ? true : false;
    }
    return false;
}

std::string ParserStyle::styleFGColorToHEX(const int styleType) const
{
    switch (styleType) {
        case SCE_RDO_DEFAULT     : return colorToHEX(defaultColor);
        case SCE_RDO_IDENTIFIER  : return colorToHEX(identifierColor);
        case SCE_RDO_KEYWORD     : return colorToHEX(keywordColor);
        case SCE_RDO_FUNCTION    : return colorToHEX(functionsColor);
        case SCE_RDO_TRACE       : return colorToHEX(traceColor);
        case SCE_RDO_FRAME_COLOR : return colorToHEX(colorColor);
        case SCE_RDO_COMMENT_CPP : return colorToHEX(commentColor);
        case SCE_RDO_COMMENT_LINE: return colorToHEX(commentColor);
        case SCE_RDO_NUMBER      : return colorToHEX(numberColor);
        case SCE_RDO_STRING      : return colorToHEX(stringColor);
        case SCE_RDO_OPERATOR    : return colorToHEX(operatorColor);
    }
    return EditStyle::styleFGColorToHEX(styleType);
}

ParserStyle ParserStyle::getDefaultStyle()
{
    ParserStyle style;
    return style;
}

ParserStyle ParserStyle::getCppStyle()
{
    ParserStyle style;
    *static_cast<EditStyle*>(&style) = EditStyle::getDefaultStyle();

    style.identifierColor = QColor(0x00, 0x00, 0x00);
    style.keywordColor    = QColor(0x00, 0x00, 0xFF);
    style.functionsColor  = QColor(0x00, 0x00, 0xFF);
    style.traceColor      = QColor(0x00, 0x00, 0xFF);
    style.colorColor      = QColor(0x00, 0x00, 0xFF);
    style.commentColor    = QColor(0x00, 0x80, 0x00);
    style.numberColor     = QColor(0x00, 0x00, 0x00);
    style.stringColor     = QColor(0x00, 0x00, 0x00);
    style.operatorColor   = QColor(0x00, 0x00, 0x00);

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

ParserStyle ParserStyle::getPascalStyle()
{
    ParserStyle style;
    *static_cast<EditStyle*>(&style) = EditStyle::getDefaultStyle();

    style.identifierColor = QColor(0x00, 0x00, 0x00);
    style.keywordColor    = QColor(0x00, 0x00, 0x00);
    style.functionsColor  = QColor(0x00, 0x00, 0x00);
    style.traceColor      = QColor(0x00, 0x00, 0x00);
    style.colorColor      = QColor(0x00, 0x00, 0x00);
    style.commentColor    = QColor(0x00, 0x00, 0x80);
    style.numberColor     = QColor(0x00, 0x00, 0x00);
    style.stringColor     = QColor(0x00, 0x00, 0x00);
    style.operatorColor   = QColor(0x00, 0x00, 0x00);

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

ParserStyle ParserStyle::getHtmlStyle()
{
    ParserStyle style;
    *static_cast<EditStyle*>(&style) = EditStyle::getDefaultStyle();

    style.identifierColor = QColor(0x00, 0x00, 0x00);
    style.keywordColor    = QColor(0x80, 0x00, 0x80);
    style.functionsColor  = QColor(0xFF, 0x00, 0x00);
    style.traceColor      = QColor(0xFF, 0x00, 0x00);
    style.colorColor      = QColor(0xFF, 0x00, 0x00);
    style.commentColor    = QColor(0x00, 0x80, 0x00);
    style.numberColor     = QColor(0x00, 0x00, 0xFF);
    style.stringColor     = QColor(0x00, 0x00, 0xFF);
    style.operatorColor   = QColor(0x00, 0x00, 0x00);

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

ParserStyle ParserStyle::getClassicStyle()
{
    ParserStyle style;
    *static_cast<EditStyle*>(&style) = EditStyle::getClassicStyle();

    style.identifierColor = QColor(0xFF, 0xFF, 0x00);
    style.keywordColor    = QColor(0xFF, 0xFF, 0xFF);
    style.functionsColor  = QColor(0xFF, 0xFF, 0xFF);
    style.traceColor      = QColor(0xFF, 0xFF, 0xFF);
    style.colorColor      = QColor(0xFF, 0xFF, 0xFF);
    style.commentColor    = QColor(0xC0, 0xC0, 0xC0);
    style.numberColor     = QColor(0xFF, 0xFF, 0x00);
    style.stringColor     = QColor(0xFF, 0xFF, 0x00);
    style.operatorColor   = QColor(0xFF, 0xFF, 0x00);

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

ParserStyle ParserStyle::getTwilightStyle()
{
    ParserStyle style;
    *static_cast<EditStyle*>(&style) = EditStyle::getTwilightStyle();

    style.identifierColor = QColor(0xFF, 0xFF, 0xFF);
    style.keywordColor    = QColor(0x00, 0xFF, 0xFF);
    style.functionsColor  = QColor(0xFF, 0xFF, 0x00);
    style.traceColor      = QColor(0x00, 0xFF, 0x00);
    style.colorColor      = QColor(0x00, 0xFF, 0x00);
    style.commentColor    = QColor(0xC0, 0xC0, 0xC0);
    style.numberColor     = QColor(0xC0, 0xC0, 0xC0);
    style.stringColor     = QColor(0xFF, 0xFF, 0xFF);
    style.operatorColor   = QColor(0xFF, 0xFF, 0xFF);

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

ParserStyle ParserStyle::getOceanStyle()
{
    ParserStyle style;
    *static_cast<EditStyle*>(&style) = EditStyle::getOceanStyle();

    style.identifierColor = QColor(0x00, 0x00, 0xFF);
    style.keywordColor    = QColor(0x00, 0x00, 0x00);
    style.functionsColor  = QColor(0x00, 0x00, 0x00);
    style.traceColor      = QColor(0x00, 0x00, 0x00);
    style.colorColor      = QColor(0x00, 0x00, 0x00);
    style.commentColor    = QColor(0x00, 0x80, 0x80);
    style.numberColor     = QColor(0x00, 0x00, 0xFF);
    style.stringColor     = QColor(0x00, 0x00, 0xFF);
    style.operatorColor   = QColor(0x00, 0x00, 0xFF);

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

namespace rdo { namespace gui { namespace editor {

QSettings& operator<< (QSettings& settings, const ParserStyle& style)
{
    settings << static_cast<const EditStyle&>(style);

    settings.beginGroup("theme");
    settings.setValue("identifier_color", style.identifierColor.name());
    settings.setValue("keyword_color", style.keywordColor.name());
    settings.setValue("functions_color", style.functionsColor.name());
    settings.setValue("trace_color", style.traceColor.name());
    settings.setValue("color_color", style.colorColor.name());
    settings.setValue("comment_color", style.commentColor.name());
    settings.setValue("number_color", style.numberColor.name());
    settings.setValue("string_color", style.stringColor.name());
    settings.setValue("operator_color", style.operatorColor.name());

    settings.setValue("identifier_style", static_cast<int>(style.identifierStyle));
    settings.setValue("keyword_style", static_cast<int>(style.keywordStyle));
    settings.setValue("functions_style", static_cast<int>(style.functionsStyle));
    settings.setValue("trace_style", static_cast<int>(style.traceStyle));
    settings.setValue("color_style", static_cast<int>(style.colorStyle));
    settings.setValue("comment_style", static_cast<int>(style.commentStyle));
    settings.setValue("number_style", static_cast<int>(style.numberStyle));
    settings.setValue("string_style", static_cast<int>(style.stringStyle));
    settings.setValue("operator_style", static_cast<int>(style.operatorStyle));
    settings.endGroup();

    return settings;
}

QSettings& operator>> (QSettings& settings, ParserStyle& style)
{
    settings >> static_cast<EditStyle&>(style);

    settings.beginGroup("theme");
    style.identifierColor        = QColor(settings.value("identifier_color", style.identifierColor.name()).toString());
    style.keywordColor           = QColor(settings.value("keyword_color", style.keywordColor.name()).toString());
    style.functionsColor         = QColor(settings.value("functions_color", style.functionsColor.name()).toString());
    style.traceColor             = QColor(settings.value("trace_color", style.traceColor.name()).toString());
    style.colorColor             = QColor(settings.value("color_color", style.colorColor.name()).toString());
    style.commentColor           = QColor(settings.value("comment_color", style.commentColor.name()).toString());
    style.numberColor            = QColor(settings.value("number_color", style.numberColor.name()).toString());
    style.stringColor            = QColor(settings.value("string_color", style.stringColor.name()).toString());
    style.operatorColor          = QColor(settings.value("operator_color", style.operatorColor.name()).toString());

    style.identifierStyle        = static_cast<StyleFont::Style>(settings.value("identifier_style", static_cast<int>(style.identifierStyle)).toInt());
    style.keywordStyle           = static_cast<StyleFont::Style>(settings.value("keyword_style", static_cast<int>(style.keywordStyle)).toInt());
    style.functionsStyle         = static_cast<StyleFont::Style>(settings.value("functions_style", static_cast<int>(style.functionsStyle)).toInt());
    style.traceStyle             = static_cast<StyleFont::Style>(settings.value("trace_style", static_cast<int>(style.traceStyle)).toInt());
    style.colorStyle             = static_cast<StyleFont::Style>(settings.value("color_style", static_cast<int>(style.colorStyle)).toInt());
    style.commentStyle           = static_cast<StyleFont::Style>(settings.value("comment_style", static_cast<int>(style.commentStyle)).toInt());
    style.numberStyle            = static_cast<StyleFont::Style>(settings.value("number_style", static_cast<int>(style.numberStyle)).toInt());
    style.stringStyle            = static_cast<StyleFont::Style>(settings.value("string_style", static_cast<int>(style.stringStyle)).toInt());
    style.operatorStyle          = static_cast<StyleFont::Style>(settings.value("operator_style", static_cast<int>(style.operatorStyle)).toInt());
    settings.endGroup();

    return settings;
}

}}} // namespace rdo::gui::editor
