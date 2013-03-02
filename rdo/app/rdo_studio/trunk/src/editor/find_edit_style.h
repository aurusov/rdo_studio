/*!
  \copyright (c) RDO-Team, 2003-2012
  \file      find_edit_style.h
  \author    Урусов Андрей (rdo@rk9.bmstu.ru)
  \date      28.02.2003
  \brief     
  \indent    4T
*/

#ifndef _RDO_STUDIO_EDITOR_FIND_EDIT_STYLE_H_
#define _RDO_STUDIO_EDITOR_FIND_EDIT_STYLE_H_

// ----------------------------------------------------------------------- INCLUDES
// ----------------------------------------------------------------------- SYNOPSIS
#include "app/rdo_studio/src/editor/log_edit_style.h"
// --------------------------------------------------------------------------------

namespace rdo { namespace gui { namespace editor {

// --------------------------------------------------------------------------------
// -------------------- FindTheme
// --------------------------------------------------------------------------------
class FindTheme: public LogTheme
{
public:
	FindTheme();
	virtual ~FindTheme();

	FindTheme& operator =( const FindTheme& theme );
	rbool operator ==( const FindTheme& theme ) const;
	rbool operator !=( const FindTheme& theme ) const;

	virtual void load(QSettings& settings);
	virtual void save(QSettings& settings) const;

	QColor keywordColor;

	style::StyleFont::style keywordStyle;

	virtual rbool styleDefault( const int styleType ) const;
	virtual rbool styleUsing( const int styleType ) const;
	virtual rbool styleBold( const int styleType = STYLE_DEFAULT ) const;
	virtual rbool styleItalic( const int styleType = STYLE_DEFAULT ) const;
	virtual tstring styleFGColorToHEX( const int styleType = STYLE_DEFAULT ) const;

	static FindTheme getDefaultTheme();
	static FindTheme getClassicTheme();
	static FindTheme getTwilightTheme();
	static FindTheme getOceanTheme();
};

QSettings& operator<< (QSettings& settings, const FindTheme& theme);
QSettings& operator>> (QSettings& settings,       FindTheme& theme);

// --------------------------------------------------------------------------------
// -------------------- FindStyle
// --------------------------------------------------------------------------------
class FindStyle: public LogStyle
{
public:
	FindStyle();
	virtual ~FindStyle();

	FindStyle& operator =( const FindStyle& style );
	rbool operator ==( const FindStyle& style ) const;
	rbool operator !=( const FindStyle& style ) const;
};

}}} // namespace rdo::gui::editor

#endif // _RDO_STUDIO_EDITOR_FIND_EDIT_STYLE_H_
