#pragma once

// ----------------------------------------------------------------------- INCLUDES
// ----------------------------------------------------------------------- SYNOPSIS
#include "app/rdo_studio/src/editor/log_edit_style.h"
// --------------------------------------------------------------------------------

namespace rdo { namespace gui { namespace editor {

// --------------------------------------------------------------------------------
// -------------------- BuildStyle
// --------------------------------------------------------------------------------
class BuildStyle: public LogStyle
{
public:
	BuildStyle();
	~BuildStyle();

	BuildStyle& operator =( const BuildStyle& style );
	bool operator ==( const BuildStyle& style ) const;
	bool operator !=( const BuildStyle& style ) const;

	bool warning;

	static BuildStyle getDefaultStyle();
	static BuildStyle getClassicStyle();
	static BuildStyle getTwilightStyle();
	static BuildStyle getOceanStyle();
};

QSettings& operator<< (QSettings& settings, const BuildStyle& style);
QSettings& operator>> (QSettings& settings,       BuildStyle& style);

}}} // namespace rdo::gui::editor
