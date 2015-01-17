#pragma once

// ----------------------------------------------------------------------- INCLUDES
// ----------------------------------------------------------------------- SYNOPSIS
#include "app/rdo_studio/src/editor/log_edit_style.h"
// --------------------------------------------------------------------------------

namespace rdo { namespace gui { namespace editor {

// --------------------------------------------------------------------------------
// -------------------- FindStyle
// --------------------------------------------------------------------------------
class FindStyle: public LogStyle
{
public:
    FindStyle();
    ~FindStyle();

    FindStyle& operator =(const FindStyle& style);
    bool operator ==(const FindStyle& style) const;
    bool operator !=(const FindStyle& style) const;

    virtual bool styleDefault(const int styleType) const;
    virtual bool styleUsing(const int styleType) const;
    virtual bool styleBold(const int styleType = STYLE_DEFAULT) const;
    virtual bool styleItalic(const int styleType = STYLE_DEFAULT) const;
    virtual std::string styleFGColorToHEX(const int styleType = STYLE_DEFAULT) const;

    static FindStyle getDefaultStyle();
    static FindStyle getClassicStyle();
    static FindStyle getTwilightStyle();
    static FindStyle getOceanStyle();

    QColor keywordColor;

    style::StyleFont::Style keywordStyle;
};

QSettings& operator<< (QSettings& settings, const FindStyle& style);
QSettings& operator>> (QSettings& settings,       FindStyle& style);

}}} // namespace rdo::gui::editor
