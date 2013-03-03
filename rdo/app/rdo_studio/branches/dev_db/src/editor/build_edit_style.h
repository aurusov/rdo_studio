/*!
  \copyright (c) RDO-Team, 2003-2012
  \file      build_edit_style.h
  \author    Урусов Андрей (rdo@rk9.bmstu.ru)
  \date      25.02.2003
  \brief     
  \indent    4T
*/

#ifndef _RDO_STUDIO_EDITOR_BUILD_EDIT_STYLE_H_
#define _RDO_STUDIO_EDITOR_BUILD_EDIT_STYLE_H_

// ----------------------------------------------------------------------- INCLUDES
// ----------------------------------------------------------------------- SYNOPSIS
#include "app/rdo_studio/src/editor/log_edit_style.h"
// --------------------------------------------------------------------------------

namespace rdo { namespace gui { namespace editor {

// --------------------------------------------------------------------------------
// -------------------- BuildTheme
// --------------------------------------------------------------------------------
class BuildTheme: public LogTheme
{
public:
	BuildTheme();
	virtual ~BuildTheme();

	BuildTheme& operator =( const BuildTheme& theme );
	rbool operator ==( const BuildTheme& theme ) const;
	rbool operator !=( const BuildTheme& theme ) const;

	virtual void load(QSettings& settings);
	virtual void save(QSettings& settings) const;

	rbool warning;

	static BuildTheme getDefaultTheme();
	static BuildTheme getClassicTheme();
	static BuildTheme getTwilightTheme();
	static BuildTheme getOceanTheme();
};

QSettings& operator<< (QSettings& settings, const BuildTheme& theme);
QSettings& operator>> (QSettings& settings,       BuildTheme& theme);

// --------------------------------------------------------------------------------
// -------------------- BuildStyle
// --------------------------------------------------------------------------------
class BuildStyle: public LogStyle
{
public:
	BuildStyle();
	virtual ~BuildStyle();

	BuildStyle& operator =( const BuildStyle& style );
	rbool operator ==( const BuildStyle& style ) const;
	rbool operator !=( const BuildStyle& style ) const;
};

}}} // namespace rdo::gui::editor

#endif // _RDO_STUDIO_EDITOR_BUILD_EDIT_STYLE_H_
