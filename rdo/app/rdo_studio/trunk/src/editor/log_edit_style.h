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
// -------------------- LogStyle
// --------------------------------------------------------------------------------
class LogStyle: public EditStyle
{
public:
	LogStyle();
	~LogStyle();

	LogStyle& operator =( const LogStyle& style );
	rbool operator ==( const LogStyle& style ) const;
	rbool operator !=( const LogStyle& style ) const;

	void loadStyle(QSettings& settings);
	void saveStyle(QSettings& settings) const;

	static LogStyle getDefaultStyle();
	static LogStyle getClassicStyle();
	static LogStyle getTwilightStyle();
	static LogStyle getOceanStyle();

	QColor selectLineBgColor;
};

QSettings& operator<< (QSettings& settings, const LogStyle& style);
QSettings& operator>> (QSettings& settings,       LogStyle& style);

}}} // namespace rdo::gui::editor

#endif // _RDO_STUDIO_EDITOR_LOG_EDIT_STYLE_H_
