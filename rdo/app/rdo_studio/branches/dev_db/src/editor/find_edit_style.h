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

	virtual void loadStyle(QSettings& settings);
	virtual void saveStyle(QSettings& settings) const;

	virtual rbool styleDefault( const int styleType ) const;
	virtual rbool styleUsing( const int styleType ) const;
	virtual rbool styleBold( const int styleType = STYLE_DEFAULT ) const;
	virtual rbool styleItalic( const int styleType = STYLE_DEFAULT ) const;
	virtual tstring styleFGColorToHEX( const int styleType = STYLE_DEFAULT ) const;

	static FindStyle getDefaultStyle();
	static FindStyle getClassicStyle();
	static FindStyle getTwilightStyle();
	static FindStyle getOceanStyle();

	QColor keywordColor;

	style::StyleFont::style keywordStyle;
};

QSettings& operator<< (QSettings& settings, const FindStyle& style);
QSettings& operator>> (QSettings& settings,       FindStyle& style);

}}} // namespace rdo::gui::editor

#endif // _RDO_STUDIO_EDITOR_FIND_EDIT_STYLE_H_
