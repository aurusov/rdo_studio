#pragma once

// ----------------------------------------------------------------------- INCLUDES
#include "utils/src/common/warning_disable.h"
#include <QColor>
#include "utils/src/common/warning_enable.h"
// ----------------------------------------------------------------------- SYNOPSIS
#include "app/rdo_studio/src/editor/edit_style.h"
// --------------------------------------------------------------------------------

namespace rdo { namespace gui { namespace editor {

// --------------------------------------------------------------------------------
// -------------------- ParserStyle
// --------------------------------------------------------------------------------
class ParserStyle: public EditStyle
{
public:
    ParserStyle();
    ~ParserStyle();

    ParserStyle& operator =(const ParserStyle& style);
    bool operator ==(const ParserStyle& style) const;
    bool operator !=(const ParserStyle& style) const;

    virtual bool styleDefault(const int styleType) const;
    virtual bool styleUsing(const int styleType) const;
    virtual bool styleBold(const int styleType = STYLE_DEFAULT) const;
    virtual bool styleItalic(const int styleType = STYLE_DEFAULT) const;
    virtual std::string styleFGColorToHEX(const int styleType = STYLE_DEFAULT) const;

    static ParserStyle getDefaultStyle();
    static ParserStyle getCppStyle();
    static ParserStyle getPascalStyle();
    static ParserStyle getHtmlStyle();
    static ParserStyle getClassicStyle();
    static ParserStyle getTwilightStyle();
    static ParserStyle getOceanStyle();

    QColor identifierColor;
    QColor keywordColor;
    QColor functionsColor;
    QColor traceColor;
    QColor colorColor;
    QColor commentColor;
    QColor numberColor;
    QColor stringColor;
    QColor operatorColor;

    style::StyleFont::Style identifierStyle;
    style::StyleFont::Style keywordStyle;
    style::StyleFont::Style functionsStyle;
    style::StyleFont::Style traceStyle;
    style::StyleFont::Style colorStyle;
    style::StyleFont::Style commentStyle;
    style::StyleFont::Style numberStyle;
    style::StyleFont::Style stringStyle;
    style::StyleFont::Style operatorStyle;
};

QSettings& operator<< (QSettings& settings, const ParserStyle& style);
QSettings& operator>> (QSettings& settings,       ParserStyle& style);

}}} // namespace rdo::gui::editor
