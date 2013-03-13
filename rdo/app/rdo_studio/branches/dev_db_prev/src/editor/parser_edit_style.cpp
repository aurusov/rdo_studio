/*!
  \copyright (c) RDO-Team, 2003-2012
  \file      rdoeditorbaseeditstyle.cpp
  \author    Урусов Андрей (rdo@rk9.bmstu.ru)
  \date      14.03.2003
  \brief     
  \indent    4T
*/

// ---------------------------------------------------------------------------- PCH
#include "app/rdo_studio/pch/stdpch.h"
// ----------------------------------------------------------------------- INCLUDES
// ----------------------------------------------------------------------- SYNOPSIS
#include "app/rdo_studio/src/editor/parser_edit_style.h"
#include "thirdparty/scintilla/include/SciLexer.h"
// --------------------------------------------------------------------------------

using namespace rdo::gui::style;
using namespace rdo::gui::editor;

// --------------------------------------------------------------------------------
// -------------------- ParserTheme
// --------------------------------------------------------------------------------
ParserTheme::ParserTheme(): EditTheme()
{
	defaultColor    = QColor( 0x80, 0x80, 0x80 );
	identifierColor = QColor( 0x00, 0x00, 0x00 );
	keywordColor    = QColor( 0x00, 0x00, 0x00 );
	functionsColor  = QColor( 0x00, 0x80, 0x00 );
	traceColor      = QColor( 0x80, 0x00, 0x00 );
	colorColor      = QColor( 0x09, 0x26, 0xB0 );
	commentColor    = QColor( 0x00, 0x80, 0x00 );
	numberColor     = QColor( 0x00, 0x00, 0x80 );
	stringColor     = QColor( 0x80, 0x00, 0x80 );
	operatorColor   = QColor( 0x00, 0x00, 0x00 );

	identifierStyle = StyleFont::NONE;
	keywordStyle    = StyleFont::BOLD;
	functionsStyle  = StyleFont::BOLD;
	traceStyle      = StyleFont::BOLD;
	colorStyle      = StyleFont::BOLD;
	commentStyle    = StyleFont::ITALIC;
	numberStyle     = StyleFont::NONE;
	stringStyle     = StyleFont::NONE;
	operatorStyle   = StyleFont::NONE;
}

ParserTheme::~ParserTheme()
{
}

ParserTheme& ParserTheme::operator =( const ParserTheme& theme )
{
	EditTheme::operator=( theme );

	identifierColor = theme.identifierColor;
	keywordColor    = theme.keywordColor;
	functionsColor  = theme.functionsColor;
	traceColor      = theme.traceColor;
	colorColor      = theme.colorColor;
	commentColor    = theme.commentColor;
	numberColor     = theme.numberColor;
	stringColor     = theme.stringColor;
	operatorColor   = theme.operatorColor;

	identifierStyle = theme.identifierStyle;
	keywordStyle    = theme.keywordStyle;
	functionsStyle  = theme.functionsStyle;
	traceStyle      = theme.traceStyle;
	colorStyle      = theme.colorStyle;
	commentStyle    = theme.commentStyle;
	numberStyle     = theme.numberStyle;
	stringStyle     = theme.stringStyle;
	operatorStyle   = theme.operatorStyle;

	return *this;
}

rbool ParserTheme::operator ==( const ParserTheme& theme ) const
{
	rbool flag = EditTheme::operator==( theme );

	if ( flag ) flag &= identifierColor == theme.identifierColor &&
	                    keywordColor    == theme.keywordColor &&
	                    functionsColor  == theme.functionsColor &&
	                    traceColor      == theme.traceColor &&
	                    colorColor      == theme.colorColor &&
	                    commentColor    == theme.commentColor &&
	                    numberColor     == theme.numberColor &&
	                    stringColor     == theme.stringColor &&
	                    operatorColor   == theme.operatorColor &&

	                    identifierStyle == theme.identifierStyle &&
	                    keywordStyle    == theme.keywordStyle &&
	                    functionsStyle  == theme.functionsStyle &&
	                    traceStyle      == theme.traceStyle &&
	                    colorStyle      == theme.colorStyle &&
	                    commentStyle    == theme.commentStyle &&
	                    numberStyle     == theme.numberStyle &&
	                    stringStyle     == theme.stringStyle &&
	                    operatorStyle   == theme.operatorStyle;
	return flag;
}

rbool ParserTheme::operator !=( const ParserTheme& theme ) const
{
	return !(*this == theme);
}

void ParserTheme::load(QSettings& settings)
{
	EditTheme::load(settings);

	settings >> *this;
}

void ParserTheme::save(QSettings& settings) const
{
	EditTheme::save(settings);

	settings << *this;
}

rbool ParserTheme::styleDefault( const int styleType ) const
{
	return styleType == SCE_RDO_DEFAULT;
}

rbool ParserTheme::styleUsing( const int styleType ) const
{
	return styleType == SCE_RDO_DEFAULT      || styleType == SCE_RDO_IDENTIFIER   ||
	       styleType == SCE_RDO_KEYWORD      || styleType == SCE_RDO_FUNCTION     ||
	       styleType == SCE_RDO_TRACE        || styleType == SCE_RDO_COMMENT_CPP  ||
	       styleType == SCE_RDO_COMMENT_LINE || styleType == SCE_RDO_NUMBER       ||
	       styleType == SCE_RDO_STRING       || styleType == SCE_RDO_OPERATOR     ||
	       styleType == SCE_RDO_FRAME_COLOR;
}

rbool ParserTheme::styleBold( const int styleType ) const
{
	switch ( styleType ) {
		case SCE_RDO_DEFAULT     : return defaultStyle    & StyleFont::BOLD ? true : false;
		case SCE_RDO_IDENTIFIER  : return identifierStyle & StyleFont::BOLD ? true : false;
		case SCE_RDO_KEYWORD     : return keywordStyle    & StyleFont::BOLD ? true : false;
		case SCE_RDO_FUNCTION    : return functionsStyle  & StyleFont::BOLD ? true : false;
		case SCE_RDO_TRACE       : return traceStyle      & StyleFont::BOLD ? true : false;
		case SCE_RDO_FRAME_COLOR : return colorStyle      & StyleFont::BOLD ? true : false;
		case SCE_RDO_COMMENT_CPP : return commentStyle    & StyleFont::BOLD ? true : false;
		case SCE_RDO_COMMENT_LINE: return commentStyle    & StyleFont::BOLD ? true : false;
		case SCE_RDO_NUMBER      : return numberStyle     & StyleFont::BOLD ? true : false;
		case SCE_RDO_STRING      : return stringStyle     & StyleFont::BOLD ? true : false;
		case SCE_RDO_OPERATOR    : return operatorStyle   & StyleFont::BOLD ? true : false;
	}
	return false;
}

rbool ParserTheme::styleItalic( const int styleType ) const
{
	switch ( styleType ) {
		case SCE_RDO_DEFAULT     : return defaultStyle    & StyleFont::ITALIC ? true : false;
		case SCE_RDO_IDENTIFIER  : return identifierStyle & StyleFont::ITALIC ? true : false;
		case SCE_RDO_KEYWORD     : return keywordStyle    & StyleFont::ITALIC ? true : false;
		case SCE_RDO_FUNCTION    : return functionsStyle  & StyleFont::ITALIC ? true : false;
		case SCE_RDO_TRACE       : return traceStyle      & StyleFont::ITALIC ? true : false;
		case SCE_RDO_FRAME_COLOR : return colorStyle      & StyleFont::ITALIC ? true : false;
		case SCE_RDO_COMMENT_CPP : return commentStyle    & StyleFont::ITALIC ? true : false;
		case SCE_RDO_COMMENT_LINE: return commentStyle    & StyleFont::ITALIC ? true : false;
		case SCE_RDO_NUMBER      : return numberStyle     & StyleFont::ITALIC ? true : false;
		case SCE_RDO_STRING      : return stringStyle     & StyleFont::ITALIC ? true : false;
		case SCE_RDO_OPERATOR    : return operatorStyle   & StyleFont::ITALIC ? true : false;
	}
	return false;
}

tstring ParserTheme::styleFGColorToHEX( const int styleType ) const
{
	switch ( styleType ) {
		case SCE_RDO_DEFAULT     : return colorToHEX( defaultColor );
		case SCE_RDO_IDENTIFIER  : return colorToHEX( identifierColor );
		case SCE_RDO_KEYWORD     : return colorToHEX( keywordColor );
		case SCE_RDO_FUNCTION    : return colorToHEX( functionsColor );
		case SCE_RDO_TRACE       : return colorToHEX( traceColor );
		case SCE_RDO_FRAME_COLOR : return colorToHEX( colorColor );
		case SCE_RDO_COMMENT_CPP : return colorToHEX( commentColor );
		case SCE_RDO_COMMENT_LINE: return colorToHEX( commentColor );
		case SCE_RDO_NUMBER      : return colorToHEX( numberColor );
		case SCE_RDO_STRING      : return colorToHEX( stringColor );
		case SCE_RDO_OPERATOR    : return colorToHEX( operatorColor );
	}
	return EditTheme::styleFGColorToHEX( styleType );
}

ParserTheme ParserTheme::getDefaultTheme()
{
	ParserTheme theme;
	return theme;
}

ParserTheme ParserTheme::getCppTheme()
{
	ParserTheme theme;
	*static_cast<EditTheme*>(&theme) = EditTheme::getDefaultTheme();

	theme.identifierColor = QColor( 0x00, 0x00, 0x00 );
	theme.keywordColor    = QColor( 0x00, 0x00, 0xFF );
	theme.functionsColor  = QColor( 0x00, 0x00, 0xFF );
	theme.traceColor      = QColor( 0x00, 0x00, 0xFF );
	theme.colorColor      = QColor( 0x00, 0x00, 0xFF );
	theme.commentColor    = QColor( 0x00, 0x80, 0x00 );
	theme.numberColor     = QColor( 0x00, 0x00, 0x00 );
	theme.stringColor     = QColor( 0x00, 0x00, 0x00 );
	theme.operatorColor   = QColor( 0x00, 0x00, 0x00 );

	theme.identifierStyle = StyleFont::NONE;
	theme.keywordStyle    = StyleFont::NONE;
	theme.functionsStyle  = StyleFont::NONE;
	theme.traceStyle      = StyleFont::NONE;
	theme.colorStyle      = StyleFont::NONE;
	theme.commentStyle    = StyleFont::NONE;
	theme.numberStyle     = StyleFont::NONE;
	theme.stringStyle     = StyleFont::NONE;
	theme.operatorStyle   = StyleFont::NONE;

	return theme;
}

ParserTheme ParserTheme::getPascalTheme()
{
	ParserTheme theme;
	*static_cast<EditTheme*>(&theme) = EditTheme::getDefaultTheme();

	theme.identifierColor = QColor( 0x00, 0x00, 0x00 );
	theme.keywordColor    = QColor( 0x00, 0x00, 0x00 );
	theme.functionsColor  = QColor( 0x00, 0x00, 0x00 );
	theme.traceColor      = QColor( 0x00, 0x00, 0x00 );
	theme.colorColor      = QColor( 0x00, 0x00, 0x00 );
	theme.commentColor    = QColor( 0x00, 0x00, 0x80 );
	theme.numberColor     = QColor( 0x00, 0x00, 0x00 );
	theme.stringColor     = QColor( 0x00, 0x00, 0x00 );
	theme.operatorColor   = QColor( 0x00, 0x00, 0x00 );

	theme.identifierStyle = StyleFont::NONE;
	theme.keywordStyle    = StyleFont::BOLD;
	theme.functionsStyle  = StyleFont::BOLD;
	theme.traceStyle      = StyleFont::BOLD;
	theme.colorStyle      = StyleFont::BOLD;
	theme.commentStyle    = StyleFont::ITALIC;
	theme.numberStyle     = StyleFont::NONE;
	theme.stringStyle     = StyleFont::NONE;
	theme.operatorStyle   = StyleFont::NONE;

	return theme;
}

ParserTheme ParserTheme::getHtmlTheme()
{
	ParserTheme theme;
	*static_cast<EditTheme*>(&theme) = EditTheme::getDefaultTheme();

	theme.identifierColor = QColor( 0x00, 0x00, 0x00 );
	theme.keywordColor    = QColor( 0x80, 0x00, 0x80 );
	theme.functionsColor  = QColor( 0xFF, 0x00, 0x00 );
	theme.traceColor      = QColor( 0xFF, 0x00, 0x00 );
	theme.colorColor      = QColor( 0xFF, 0x00, 0x00 );
	theme.commentColor    = QColor( 0x00, 0x80, 0x00 );
	theme.numberColor     = QColor( 0x00, 0x00, 0xFF );
	theme.stringColor     = QColor( 0x00, 0x00, 0xFF );
	theme.operatorColor   = QColor( 0x00, 0x00, 0x00 );

	theme.identifierStyle = StyleFont::NONE;
	theme.keywordStyle    = StyleFont::NONE;
	theme.functionsStyle  = StyleFont::NONE;
	theme.traceStyle      = StyleFont::NONE;
	theme.colorStyle      = StyleFont::NONE;
	theme.commentStyle    = StyleFont::NONE;
	theme.numberStyle     = StyleFont::NONE;
	theme.stringStyle     = StyleFont::NONE;
	theme.operatorStyle   = StyleFont::NONE;

	return theme;
}

ParserTheme ParserTheme::getClassicTheme()
{
	ParserTheme theme;
	*static_cast<EditTheme*>(&theme) = EditTheme::getClassicTheme();

	theme.identifierColor = QColor( 0xFF, 0xFF, 0x00 );
	theme.keywordColor    = QColor( 0xFF, 0xFF, 0xFF );
	theme.functionsColor  = QColor( 0xFF, 0xFF, 0xFF );
	theme.traceColor      = QColor( 0xFF, 0xFF, 0xFF );
	theme.colorColor      = QColor( 0xFF, 0xFF, 0xFF );
	theme.commentColor    = QColor( 0xC0, 0xC0, 0xC0 );
	theme.numberColor     = QColor( 0xFF, 0xFF, 0x00 );
	theme.stringColor     = QColor( 0xFF, 0xFF, 0x00 );
	theme.operatorColor   = QColor( 0xFF, 0xFF, 0x00 );

	theme.identifierStyle = StyleFont::NONE;
	theme.keywordStyle    = StyleFont::NONE;
	theme.functionsStyle  = StyleFont::NONE;
	theme.traceStyle      = StyleFont::NONE;
	theme.colorStyle      = StyleFont::NONE;
	theme.commentStyle    = StyleFont::NONE;
	theme.numberStyle     = StyleFont::NONE;
	theme.stringStyle     = StyleFont::NONE;
	theme.operatorStyle   = StyleFont::NONE;

	return theme;
}

ParserTheme ParserTheme::getTwilightTheme()
{
	ParserTheme theme;
	*static_cast<EditTheme*>(&theme) = EditTheme::getTwilightTheme();

	theme.identifierColor = QColor( 0xFF, 0xFF, 0xFF );
	theme.keywordColor    = QColor( 0x00, 0xFF, 0xFF );
	theme.functionsColor  = QColor( 0xFF, 0xFF, 0x00 );
	theme.traceColor      = QColor( 0x00, 0xFF, 0x00 );
	theme.colorColor      = QColor( 0x00, 0xFF, 0x00 );
	theme.commentColor    = QColor( 0xC0, 0xC0, 0xC0 );
	theme.numberColor     = QColor( 0xC0, 0xC0, 0xC0 );
	theme.stringColor     = QColor( 0xFF, 0xFF, 0xFF );
	theme.operatorColor   = QColor( 0xFF, 0xFF, 0xFF );

	theme.identifierStyle = StyleFont::NONE;
	theme.keywordStyle    = StyleFont::BOLD;
	theme.functionsStyle  = StyleFont::BOLD;
	theme.traceStyle      = static_cast<StyleFont::style>(StyleFont::BOLD | StyleFont::ITALIC);
	theme.colorStyle      = StyleFont::BOLD;
	theme.commentStyle    = StyleFont::ITALIC;
	theme.numberStyle     = StyleFont::NONE;
	theme.stringStyle     = StyleFont::NONE;
	theme.operatorStyle   = StyleFont::NONE;

	return theme;
}

ParserTheme ParserTheme::getOceanTheme()
{
	ParserTheme theme;
	*static_cast<EditTheme*>(&theme) = EditTheme::getOceanTheme();

	theme.identifierColor = QColor( 0x00, 0x00, 0xFF );
	theme.keywordColor    = QColor( 0x00, 0x00, 0x00 );
	theme.functionsColor  = QColor( 0x00, 0x00, 0x00 );
	theme.traceColor      = QColor( 0x00, 0x00, 0x00 );
	theme.colorColor      = QColor( 0x00, 0x00, 0x00 );
	theme.commentColor    = QColor( 0x00, 0x80, 0x80 );
	theme.numberColor     = QColor( 0x00, 0x00, 0xFF );
	theme.stringColor     = QColor( 0x00, 0x00, 0xFF );
	theme.operatorColor   = QColor( 0x00, 0x00, 0xFF );

	theme.identifierStyle = StyleFont::NONE;
	theme.keywordStyle    = StyleFont::BOLD;
	theme.functionsStyle  = StyleFont::BOLD;
	theme.traceStyle      = StyleFont::BOLD;
	theme.colorStyle      = StyleFont::BOLD;
	theme.commentStyle    = StyleFont::ITALIC;
	theme.numberStyle     = StyleFont::NONE;
	theme.stringStyle     = StyleFont::NONE;
	theme.operatorStyle   = StyleFont::NONE;

	return theme;
}

namespace rdo { namespace gui { namespace editor {

QSettings& operator<< (QSettings& settings, const ParserTheme& theme)
{
	settings.setValue("identifier_color", theme.identifierColor.name());
	settings.setValue("keyword_color", theme.keywordColor.name());
	settings.setValue("functions_color", theme.functionsColor.name());
	settings.setValue("trace_color", theme.traceColor.name());
	settings.setValue("color_color", theme.colorColor.name());
	settings.setValue("comment_color", theme.commentColor.name());
	settings.setValue("number_color", theme.numberColor.name());
	settings.setValue("string_color", theme.stringColor.name());
	settings.setValue("operator_color", theme.operatorColor.name());

	settings.setValue("identifier_style", theme.identifierStyle);
	settings.setValue("keyword_style", theme.keywordStyle);
	settings.setValue("functions_style", theme.functionsStyle);
	settings.setValue("trace_style", theme.traceStyle);
	settings.setValue("color_style", theme.colorStyle);
	settings.setValue("comment_style", theme.commentStyle);
	settings.setValue("number_style", theme.numberStyle);
	settings.setValue("string_style", theme.stringStyle);
	settings.setValue("operator_style", theme.operatorStyle);

	return settings;
}

QSettings& operator>> (QSettings& settings, ParserTheme& theme)
{
	theme.identifierColor        = QColor(settings.value("identifier_color", theme.identifierColor.name()).toString());
	theme.keywordColor           = QColor(settings.value("keyword_color", theme.keywordColor.name()).toString());
	theme.functionsColor         = QColor(settings.value("functions_color", theme.functionsColor.name()).toString());
	theme.traceColor             = QColor(settings.value("trace_color", theme.traceColor.name()).toString());
	theme.colorColor             = QColor(settings.value("color_color", theme.colorColor.name()).toString());
	theme.commentColor           = QColor(settings.value("comment_color", theme.commentColor.name()).toString());
	theme.numberColor            = QColor(settings.value("number_color", theme.numberColor.name()).toString());
	theme.stringColor            = QColor(settings.value("string_color", theme.stringColor.name()).toString());
	theme.operatorColor          = QColor(settings.value("operator_color", theme.operatorColor.name()).toString());

	theme.identifierStyle        = static_cast<StyleFont::style>(settings.value("identifier_style", theme.identifierStyle).toInt());
	theme.keywordStyle           = static_cast<StyleFont::style>(settings.value("keyword_style", theme.keywordStyle).toInt());
	theme.functionsStyle         = static_cast<StyleFont::style>(settings.value("functions_style", theme.functionsStyle).toInt());
	theme.traceStyle             = static_cast<StyleFont::style>(settings.value("trace_style", theme.traceStyle).toInt());
	theme.colorStyle             = static_cast<StyleFont::style>(settings.value("color_style", theme.colorStyle).toInt());
	theme.commentStyle           = static_cast<StyleFont::style>(settings.value("comment_style", theme.commentStyle).toInt());
	theme.numberStyle            = static_cast<StyleFont::style>(settings.value("number_style", theme.numberStyle).toInt());
	theme.stringStyle            = static_cast<StyleFont::style>(settings.value("string_style", theme.stringStyle).toInt());
	theme.operatorStyle          = static_cast<StyleFont::style>(settings.value("operator_style", theme.operatorStyle).toInt());
	
	return settings;
}

}}} // namespace rdo::gui::editor

// --------------------------------------------------------------------------------
// -------------------- ParserStyle
// --------------------------------------------------------------------------------
ParserStyle::ParserStyle(): EditStyle()
{
	theme = new ParserTheme();
}

ParserStyle::~ParserStyle()
{}

ParserStyle& ParserStyle::operator =( const ParserStyle& style )
{
	EditStyle::operator=( style );
	if ( theme && style.theme ) *static_cast<ParserTheme*>(theme) = *static_cast<ParserTheme*>(style.theme);

	return *this;
}

rbool ParserStyle::operator ==( const ParserStyle& style ) const
{
	rbool flag = EditStyle::operator==( style );
	if ( theme && style.theme && flag ) flag &= *static_cast<ParserTheme*>(theme) == *static_cast<ParserTheme*>(style.theme);
	return flag;
}

rbool ParserStyle::operator !=( const ParserStyle& style ) const
{
	return !(*this == style);
}
