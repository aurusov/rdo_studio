/*!
  \copyright (c) RDO-Team, 2003-2012
  \file      parser_edit_style.h
  \author    Урусов Андрей (rdo@rk9.bmstu.ru)
  \date      14.03.2003
  \brief     
  \indent    4T
*/

#ifndef _RDO_STUDIO_RDO_EDIT_RDOEDITORBASEEDITSTYLE_H_
#define _RDO_STUDIO_RDO_EDIT_RDOEDITORBASEEDITSTYLE_H_

// ----------------------------------------------------------------------- INCLUDES
#include "utils/src/common/warning_disable.h"
#include <QColor>
#include "utils/src/common/warning_enable.h"
// ----------------------------------------------------------------------- SYNOPSIS
#include "app/rdo_studio/src/editor/edit_style.h"
// --------------------------------------------------------------------------------

namespace rdo { namespace gui { namespace editor {

// --------------------------------------------------------------------------------
// -------------------- ParserStyle
// --------------------------------------------------------------------------------
class ParserStyle: public EditStyle
{
public:
	ParserStyle();
	~ParserStyle();

	ParserStyle& operator =( const ParserStyle& style );
	bool operator ==( const ParserStyle& style ) const;
	bool operator !=( const ParserStyle& style ) const;

	virtual bool styleDefault( const int styleType ) const;
	virtual bool styleUsing( const int styleType ) const;
	virtual bool styleBold( const int styleType = STYLE_DEFAULT ) const;
	virtual bool styleItalic( const int styleType = STYLE_DEFAULT ) const;
	virtual std::string styleFGColorToHEX( const int styleType = STYLE_DEFAULT ) const;

	static ParserStyle getDefaultStyle();
	static ParserStyle getCppStyle();
	static ParserStyle getPascalStyle();
	static ParserStyle getHtmlStyle();
	static ParserStyle getClassicStyle();
	static ParserStyle getTwilightStyle();
	static ParserStyle getOceanStyle();

	QColor identifierColor;
	QColor keywordColor;
	QColor functionsColor;
	QColor traceColor;
	QColor colorColor;
	QColor commentColor;
	QColor numberColor;
	QColor stringColor;
	QColor operatorColor;

	style::StyleFont::style identifierStyle;
	style::StyleFont::style keywordStyle;
	style::StyleFont::style functionsStyle;
	style::StyleFont::style traceStyle;
	style::StyleFont::style colorStyle;
	style::StyleFont::style commentStyle;
	style::StyleFont::style numberStyle;
	style::StyleFont::style stringStyle;
	style::StyleFont::style operatorStyle;
};

QSettings& operator<< (QSettings& settings, const ParserStyle& style);
QSettings& operator>> (QSettings& settings,       ParserStyle& style);

}}} // namespace rdo::gui::editor

#endif // _RDO_STUDIO_RDO_EDIT_RDOEDITORBASEEDITSTYLE_H_
