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
// -------------------- RDOFindEditTheme
// --------------------------------------------------------------------------------
class RDOFindEditTheme: public LogEditTheme
{
public:
	RDOFindEditTheme();
	virtual ~RDOFindEditTheme();

	RDOFindEditTheme& operator =( const RDOFindEditTheme& theme );
	rbool operator ==( const RDOFindEditTheme& theme ) const;
	rbool operator !=( const RDOFindEditTheme& theme ) const;

	virtual void load(QSettings& settings);
	virtual void save(QSettings& settings) const;

	QColor keywordColor;

	rdoStyle::RDOStyleFont::style keywordStyle;

	virtual rbool styleDefault( const int styleType ) const;
	virtual rbool styleUsing( const int styleType ) const;
	virtual rbool styleBold( const int styleType = STYLE_DEFAULT ) const;
	virtual rbool styleItalic( const int styleType = STYLE_DEFAULT ) const;
	virtual tstring styleFGColorToHEX( const int styleType = STYLE_DEFAULT ) const;

	static RDOFindEditTheme getDefaultTheme();
	static RDOFindEditTheme getClassicTheme();
	static RDOFindEditTheme getTwilightTheme();
	static RDOFindEditTheme getOceanTheme();
};

QSettings& operator<< (QSettings& settings, const RDOFindEditTheme& theme);
QSettings& operator>> (QSettings& settings,       RDOFindEditTheme& theme);

// --------------------------------------------------------------------------------
// -------------------- RDOFindEditStyle
// --------------------------------------------------------------------------------
class RDOFindEditStyle: public LogEditStyle
{
public:
	RDOFindEditStyle();
	virtual ~RDOFindEditStyle();

	RDOFindEditStyle& operator =( const RDOFindEditStyle& style );
	rbool operator ==( const RDOFindEditStyle& style ) const;
	rbool operator !=( const RDOFindEditStyle& style ) const;
};

}}} // namespace rdo::gui::editor

#endif // _RDO_STUDIO_EDIT_CTRLS_RDOFINDEDITSTYLE_H_
