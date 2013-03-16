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
// -------------------- BuildStyle
// --------------------------------------------------------------------------------
class BuildStyle: public LogStyle
{
public:
	BuildStyle();
	~BuildStyle();

	BuildStyle& operator =( const BuildStyle& style );
	rbool operator ==( const BuildStyle& style ) const;
	rbool operator !=( const BuildStyle& style ) const;

	void loadStyle(QSettings& settings);
	void saveStyle(QSettings& settings) const;

	rbool warning;

	static BuildStyle getDefaultStyle();
	static BuildStyle getClassicStyle();
	static BuildStyle getTwilightStyle();
	static BuildStyle getOceanStyle();
};

QSettings& operator<< (QSettings& settings, const BuildStyle& style);
QSettings& operator>> (QSettings& settings,       BuildStyle& style);

}}} // namespace rdo::gui::editor

#endif // _RDO_STUDIO_EDITOR_BUILD_EDIT_STYLE_H_
