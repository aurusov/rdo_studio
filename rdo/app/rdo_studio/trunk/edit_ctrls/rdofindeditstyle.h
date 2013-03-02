/*!
  \copyright (c) RDO-Team, 2003-2012
  \file      rdofindeditstyle.h
  \author    Урусов Андрей (rdo@rk9.bmstu.ru)
  \date      28.02.2003
  \brief     
  \indent    4T
*/

#ifndef _RDO_STUDIO_EDIT_CTRLS_RDOFINDEDITSTYLE_H_
#define _RDO_STUDIO_EDIT_CTRLS_RDOFINDEDITSTYLE_H_

// ----------------------------------------------------------------------- INCLUDES
// ----------------------------------------------------------------------- SYNOPSIS
#include "app/rdo_studio/edit_ctrls/rdologeditstyle.h"
// --------------------------------------------------------------------------------

namespace rdo { namespace gui { namespace editor {

// --------------------------------------------------------------------------------
// -------------------- FindTheme
// --------------------------------------------------------------------------------
class FindTheme: public LogEditTheme
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

	rdoStyle::RDOStyleFont::style keywordStyle;

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
class FindStyle: public LogEditStyle
{
public:
	FindStyle();
	virtual ~FindStyle();

	FindStyle& operator =( const FindStyle& style );
	rbool operator ==( const FindStyle& style ) const;
	rbool operator !=( const FindStyle& style ) const;
};

}}} // namespace rdo::gui::editor

#endif // _RDO_STUDIO_EDIT_CTRLS_RDOFINDEDITSTYLE_H_
