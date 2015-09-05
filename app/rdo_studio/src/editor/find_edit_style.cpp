// ---------------------------------------------------------------------------- PCH
#include "app/rdo_studio/pch/editor_pch.h"
// ----------------------------------------------------------------------- INCLUDES
// ----------------------------------------------------------------------- SYNOPSIS
#include "app/rdo_studio/src/editor/find_edit_style.h"
#include "thirdparty/scintilla/include/SciLexer.h"
// --------------------------------------------------------------------------------

using namespace rdo::gui::editor;
using namespace rdo::gui::style;

// --------------------------------------------------------------------------------
// -------------------- FindStyle
// --------------------------------------------------------------------------------
FindStyle::FindStyle(): LogStyle()
{
    keywordColor = QColor(0x00, 0x00, 0x00);
    keywordStyle = StyleFont::Style::BOLD;
}

FindStyle::~FindStyle()
{}

FindStyle& FindStyle::operator =(const FindStyle& style)
{
    LogStyle::operator=(style);

    keywordColor = style.keywordColor;
    keywordStyle = style.keywordStyle;

    return *this;
}

bool FindStyle::operator ==(const FindStyle& style) const
{
    bool flag = LogStyle::operator==(style);

    if (flag)
        flag &= keywordColor == style.keywordColor &&
                keywordStyle == style.keywordStyle;

    return flag;
}

bool FindStyle::operator !=(const FindStyle& style) const
{
    return !(*this == style);
}

bool FindStyle::styleDefault(const int styleType) const
{
    return styleType == SCE_FIND_DEFAULT;
}

bool FindStyle::styleUsing(const int styleType) const
{
    return styleType == SCE_FIND_DEFAULT || styleType == SCE_FIND_KEYWORD;
}

bool FindStyle::styleBold(const int styleType) const
{
    switch (styleType)
    {
    case SCE_FIND_DEFAULT: return static_cast<int>(defaultStyle) & static_cast<int>(StyleFont::Style::BOLD) ? true : false;
    case SCE_FIND_KEYWORD: return static_cast<int>(keywordStyle) & static_cast<int>(StyleFont::Style::BOLD) ? true : false;
    }
    return false;
}

bool FindStyle::styleItalic(const int styleType) const
{
    switch (styleType)
    {
    case SCE_FIND_DEFAULT: return static_cast<int>(defaultStyle) & static_cast<int>(StyleFont::Style::ITALIC) ? true : false;
    case SCE_FIND_KEYWORD: return static_cast<int>(keywordStyle) & static_cast<int>(StyleFont::Style::ITALIC) ? true : false;
    }
    return false;
}

std::string FindStyle::styleFGColorToHEX(const int styleType) const
{
    switch (styleType)
    {
    case SCE_FIND_DEFAULT: return colorToHEX(defaultColor);
    case SCE_FIND_KEYWORD: return colorToHEX(keywordColor);
    }
    return LogStyle::styleFGColorToHEX(styleType);
}

FindStyle FindStyle::getDefaultStyle()
{
    FindStyle style;
    return style;
}

FindStyle FindStyle::getClassicStyle()
{
    FindStyle style;
    *static_cast<LogStyle*>(&style) = LogStyle::getClassicStyle();

    style.keywordColor = QColor(0xFF, 0xFF, 0xFF);
    style.keywordStyle = StyleFont::Style::NONE;

    return style;
}

FindStyle FindStyle::getTwilightStyle()
{
    FindStyle style;
    *static_cast<LogStyle*>(&style) = LogStyle::getTwilightStyle();

    style.keywordColor = QColor(0x00, 0xFF, 0xFF);
    style.keywordStyle = StyleFont::Style::BOLD;

    return style;
}

FindStyle FindStyle::getOceanStyle()
{
    FindStyle style;
    *static_cast<LogStyle*>(&style) = LogStyle::getOceanStyle();

    style.keywordColor = QColor(0x00, 0x00, 0x00);
    style.keywordStyle = StyleFont::Style::BOLD;

    return style;
}

namespace rdo { namespace gui { namespace editor {

QSettings& operator<< (QSettings& settings, const FindStyle& style)
{
    settings << static_cast<const LogStyle&>(style);

    settings.beginGroup("theme");
    settings.setValue("keyword_color", style.keywordColor.name());
    settings.setValue("keyword_style", static_cast<int>(style.keywordStyle));
    settings.endGroup();

    return settings;
}

QSettings& operator>> (QSettings& settings, FindStyle& style)
{
    settings >> static_cast<LogStyle&>(style);

    settings.beginGroup("theme");
    style.keywordColor = QColor(settings.value("keyword_color", style.keywordColor.name()).toString());
    style.keywordStyle = static_cast<StyleFont::Style>(settings.value("keyword_style", static_cast<int>(style.keywordStyle)).toInt());
    settings.endGroup();

    return settings;
}

}}} // namespace rdo::gui::editor
