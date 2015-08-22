#pragma once

// ----------------------------------------------------------------------- INCLUDES
// ----------------------------------------------------------------------- SYNOPSIS
#include "app/rdo_studio/src/editor/edit_style.h"
// --------------------------------------------------------------------------------

namespace rdo { namespace gui { namespace editor {

// --------------------------------------------------------------------------------
// -------------------- LogStyle
// --------------------------------------------------------------------------------
class LogStyle: public EditStyle
{
public:
    LogStyle();
    ~LogStyle();

    LogStyle& operator =( const LogStyle& style );
    bool operator ==( const LogStyle& style ) const;
    bool operator !=( const LogStyle& style ) const;

    static LogStyle getDefaultStyle();
    static LogStyle getClassicStyle();
    static LogStyle getTwilightStyle();
    static LogStyle getOceanStyle();

    QColor selectLineBgColor;
};

QSettings& operator<< (QSettings& settings, const LogStyle& style);
QSettings& operator>> (QSettings& settings,       LogStyle& style);

}}} // namespace rdo::gui::editor
