/*!
  \copyright (c) RDO-Team, 2003-2012
  \file      log_edit_style.h
  \author    Урусов Андрей (rdo@rk9.bmstu.ru)
  \date      28.02.2003
  \brief     
  \indent    4T
*/

#ifndef _RDO_STUDIO_EDITOR_LOG_EDIT_STYLE_H_
#define _RDO_STUDIO_EDITOR_LOG_EDIT_STYLE_H_

// ----------------------------------------------------------------------- INCLUDES
// ----------------------------------------------------------------------- SYNOPSIS
#include "app/rdo_studio/src/editor/edit_style.h"
// --------------------------------------------------------------------------------

namespace rdo { namespace gui { namespace editor {

// --------------------------------------------------------------------------------
// -------------------- LogTheme
// --------------------------------------------------------------------------------
class LogTheme: public EditTheme
{
public:
	LogTheme();
	virtual ~LogTheme();

	LogTheme& operator =( const LogTheme& theme );
	rbool operator ==( const LogTheme& theme ) const;
	rbool operator !=( const LogTheme& theme ) const;

	virtual void load(QSettings& settings);
	virtual void save(QSettings& settings) const;

	QColor selectLineBgColor;

	static LogTheme getDefaultTheme();
	static LogTheme getClassicTheme();
	static LogTheme getTwilightTheme();
	static LogTheme getOceanTheme();
};

QSettings& operator<< (QSettings& settings, const LogTheme& theme);
QSettings& operator>> (QSettings& settings,       LogTheme& theme);

// --------------------------------------------------------------------------------
// -------------------- LogStyle
// --------------------------------------------------------------------------------
class LogStyle: public EditStyle
{
public:
	LogStyle();
	virtual ~LogStyle();

	LogStyle& operator =( const LogStyle& style );
	rbool operator ==( const LogStyle& style ) const;
	rbool operator !=( const LogStyle& style ) const;
};

}}} // namespace rdo::gui::editor

#endif // _RDO_STUDIO_EDITOR_LOG_EDIT_STYLE_H_
