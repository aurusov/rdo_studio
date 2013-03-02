/*!
  \copyright (c) RDO-Team, 2003-2012
  \file      rdoeditorbaseeditstyle.h
  \author    Урусов Андрей (rdo@rk9.bmstu.ru)
  \date      14.03.2003
  \brief     
  \indent    4T
*/

#ifndef _RDO_STUDIO_RDO_EDIT_RDOEDITORBASEEDITSTYLE_H_
#define _RDO_STUDIO_RDO_EDIT_RDOEDITORBASEEDITSTYLE_H_

// ----------------------------------------------------------------------- INCLUDES
#include "utils/warning_disable.h"
#include <QColor>
#include "utils/warning_enable.h"
// ----------------------------------------------------------------------- SYNOPSIS
#include "app/rdo_studio/edit_ctrls/rdobaseeditstyle.h"
// --------------------------------------------------------------------------------

namespace rdo { namespace gui { namespace editor {

// --------------------------------------------------------------------------------
// -------------------- ParserTheme
// --------------------------------------------------------------------------------
class ParserTheme: public EditBaseTheme
{
public:
	ParserTheme();
	virtual ~ParserTheme();

	ParserTheme& operator =( const ParserTheme& theme );
	rbool operator ==( const ParserTheme& theme ) const;
	rbool operator !=( const ParserTheme& theme ) const;

	virtual void load(QSettings& settings);
	virtual void save(QSettings& settings) const;

	QColor identifierColor;
	QColor keywordColor;
	QColor functionsColor;
	QColor traceColor;
	QColor colorColor;
	QColor commentColor;
	QColor numberColor;
	QColor stringColor;
	QColor operatorColor;

	rdoStyle::RDOStyleFont::style identifierStyle;
	rdoStyle::RDOStyleFont::style keywordStyle;
	rdoStyle::RDOStyleFont::style functionsStyle;
	rdoStyle::RDOStyleFont::style traceStyle;
	rdoStyle::RDOStyleFont::style colorStyle;
	rdoStyle::RDOStyleFont::style commentStyle;
	rdoStyle::RDOStyleFont::style numberStyle;
	rdoStyle::RDOStyleFont::style stringStyle;
	rdoStyle::RDOStyleFont::style operatorStyle;

	virtual rbool styleDefault( const int styleType ) const;
	virtual rbool styleUsing( const int styleType ) const;
	virtual rbool styleBold( const int styleType = STYLE_DEFAULT ) const;
	virtual rbool styleItalic( const int styleType = STYLE_DEFAULT ) const;
	virtual tstring styleFGColorToHEX( const int styleType = STYLE_DEFAULT ) const;

	static ParserTheme getDefaultTheme();
	static ParserTheme getCppTheme();
	static ParserTheme getPascalTheme();
	static ParserTheme getHtmlTheme();
	static ParserTheme getClassicTheme();
	static ParserTheme getTwilightTheme();
	static ParserTheme getOceanTheme();
};

QSettings& operator<< (QSettings& settings, const ParserTheme& theme);
QSettings& operator>> (QSettings& settings,       ParserTheme& theme);

// --------------------------------------------------------------------------------
// -------------------- ParserStyle
// --------------------------------------------------------------------------------
class ParserStyle: public EditBaseStyle
{
public:
	ParserStyle();
	virtual ~ParserStyle();

	ParserStyle& operator =( const ParserStyle& style );
	rbool operator ==( const ParserStyle& style ) const;
	rbool operator !=( const ParserStyle& style ) const;
};

}}} // namespace rdo::gui::editor

#endif // _RDO_STUDIO_RDO_EDIT_RDOEDITORBASEEDITSTYLE_H_
