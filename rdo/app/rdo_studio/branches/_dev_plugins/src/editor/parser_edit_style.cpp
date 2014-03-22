/*!
  \copyright (c) RDO-Team, 2003-2012
  \file      parser_edit_style.cpp
  \author    Урусов Андрей (rdo@rk9.bmstu.ru)
  \date      14.03.2003
  \brief     
  \indent    4T
*/

// ---------------------------------------------------------------------------- PCH
#include "app/rdo_studio/pch/editor_pch.h"
// ----------------------------------------------------------------------- INCLUDES
// ----------------------------------------------------------------------- SYNOPSIS
#include "app/rdo_studio/src/editor/parser_edit_style.h"
#include "thirdparty/scintilla/include/SciLexer.h"
// --------------------------------------------------------------------------------

using namespace rdo::gui::style;
using namespace rdo::gui::editor;

// --------------------------------------------------------------------------------
// -------------------- ParserStyle
// --------------------------------------------------------------------------------
ParserStyle::ParserStyle(): EditStyle()
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

ParserStyle::~ParserStyle()
{}

ParserStyle& ParserStyle::operator =( const ParserStyle& style )
{
	EditStyle::operator=( style );

	identifierColor = style.identifierColor;
	keywordColor    = style.keywordColor;
	functionsColor  = style.functionsColor;
	traceColor      = style.traceColor;
	colorColor      = style.colorColor;
	commentColor    = style.commentColor;
	numberColor     = style.numberColor;
	stringColor     = style.stringColor;
	operatorColor   = style.operatorColor;

	identifierStyle = style.identifierStyle;
	keywordStyle    = style.keywordStyle;
	functionsStyle  = style.functionsStyle;
	traceStyle      = style.traceStyle;
	colorStyle      = style.colorStyle;
	commentStyle    = style.commentStyle;
	numberStyle     = style.numberStyle;
	stringStyle     = style.stringStyle;
	operatorStyle   = style.operatorStyle;

	return *this;
}

bool ParserStyle::operator ==( const ParserStyle& style ) const
{
	
	return EditStyle::operator==( style ) &&
		identifierColor == style.identifierColor &&
		keywordColor    == style.keywordColor &&
		functionsColor  == style.functionsColor &&
		traceColor      == style.traceColor &&
		colorColor      == style.colorColor &&
		commentColor    == style.commentColor &&
		numberColor     == style.numberColor &&
		stringColor     == style.stringColor &&
		operatorColor   == style.operatorColor &&

		identifierStyle == style.identifierStyle &&
		keywordStyle    == style.keywordStyle &&
		functionsStyle  == style.functionsStyle &&
		traceStyle      == style.traceStyle &&
		colorStyle      == style.colorStyle &&
		commentStyle    == style.commentStyle &&
		numberStyle     == style.numberStyle &&
		stringStyle     == style.stringStyle &&
		operatorStyle   == style.operatorStyle;
}

bool ParserStyle::operator !=( const ParserStyle& style ) const
{
	return !(*this == style);
}

bool ParserStyle::styleDefault( const int styleType ) const
{
	return styleType == SCE_RDO_DEFAULT;
}

bool ParserStyle::styleUsing( const int styleType ) const
{
	return
		styleType == SCE_RDO_DEFAULT      || styleType == SCE_RDO_IDENTIFIER   ||
		styleType == SCE_RDO_KEYWORD      || styleType == SCE_RDO_FUNCTION     ||
		styleType == SCE_RDO_TRACE        || styleType == SCE_RDO_COMMENT_CPP  ||
		styleType == SCE_RDO_COMMENT_LINE || styleType == SCE_RDO_NUMBER       ||
		styleType == SCE_RDO_STRING       || styleType == SCE_RDO_OPERATOR     ||
		styleType == SCE_RDO_FRAME_COLOR;
}

bool ParserStyle::styleBold( const int styleType ) const
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

bool ParserStyle::styleItalic( const int styleType ) const
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

tstring ParserStyle::styleFGColorToHEX( const int styleType ) const
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
	return EditStyle::styleFGColorToHEX( styleType );
}

ParserStyle ParserStyle::getDefaultStyle()
{
	ParserStyle style;
	return style;
}

ParserStyle ParserStyle::getCppStyle()
{
	ParserStyle style;
	*static_cast<EditStyle*>(&style) = EditStyle::getDefaultStyle();

	style.identifierColor = QColor( 0x00, 0x00, 0x00 );
	style.keywordColor    = QColor( 0x00, 0x00, 0xFF );
	style.functionsColor  = QColor( 0x00, 0x00, 0xFF );
	style.traceColor      = QColor( 0x00, 0x00, 0xFF );
	style.colorColor      = QColor( 0x00, 0x00, 0xFF );
	style.commentColor    = QColor( 0x00, 0x80, 0x00 );
	style.numberColor     = QColor( 0x00, 0x00, 0x00 );
	style.stringColor     = QColor( 0x00, 0x00, 0x00 );
	style.operatorColor   = QColor( 0x00, 0x00, 0x00 );

	style.identifierStyle = StyleFont::NONE;
	style.keywordStyle    = StyleFont::NONE;
	style.functionsStyle  = StyleFont::NONE;
	style.traceStyle      = StyleFont::NONE;
	style.colorStyle      = StyleFont::NONE;
	style.commentStyle    = StyleFont::NONE;
	style.numberStyle     = StyleFont::NONE;
	style.stringStyle     = StyleFont::NONE;
	style.operatorStyle   = StyleFont::NONE;

	return style;
}

ParserStyle ParserStyle::getPascalStyle()
{
	ParserStyle style;
	*static_cast<EditStyle*>(&style) = EditStyle::getDefaultStyle();

	style.identifierColor = QColor( 0x00, 0x00, 0x00 );
	style.keywordColor    = QColor( 0x00, 0x00, 0x00 );
	style.functionsColor  = QColor( 0x00, 0x00, 0x00 );
	style.traceColor      = QColor( 0x00, 0x00, 0x00 );
	style.colorColor      = QColor( 0x00, 0x00, 0x00 );
	style.commentColor    = QColor( 0x00, 0x00, 0x80 );
	style.numberColor     = QColor( 0x00, 0x00, 0x00 );
	style.stringColor     = QColor( 0x00, 0x00, 0x00 );
	style.operatorColor   = QColor( 0x00, 0x00, 0x00 );

	style.identifierStyle = StyleFont::NONE;
	style.keywordStyle    = StyleFont::BOLD;
	style.functionsStyle  = StyleFont::BOLD;
	style.traceStyle      = StyleFont::BOLD;
	style.colorStyle      = StyleFont::BOLD;
	style.commentStyle    = StyleFont::ITALIC;
	style.numberStyle     = StyleFont::NONE;
	style.stringStyle     = StyleFont::NONE;
	style.operatorStyle   = StyleFont::NONE;

	return style;
}

ParserStyle ParserStyle::getHtmlStyle()
{
	ParserStyle style;
	*static_cast<EditStyle*>(&style) = EditStyle::getDefaultStyle();

	style.identifierColor = QColor( 0x00, 0x00, 0x00 );
	style.keywordColor    = QColor( 0x80, 0x00, 0x80 );
	style.functionsColor  = QColor( 0xFF, 0x00, 0x00 );
	style.traceColor      = QColor( 0xFF, 0x00, 0x00 );
	style.colorColor      = QColor( 0xFF, 0x00, 0x00 );
	style.commentColor    = QColor( 0x00, 0x80, 0x00 );
	style.numberColor     = QColor( 0x00, 0x00, 0xFF );
	style.stringColor     = QColor( 0x00, 0x00, 0xFF );
	style.operatorColor   = QColor( 0x00, 0x00, 0x00 );

	style.identifierStyle = StyleFont::NONE;
	style.keywordStyle    = StyleFont::NONE;
	style.functionsStyle  = StyleFont::NONE;
	style.traceStyle      = StyleFont::NONE;
	style.colorStyle      = StyleFont::NONE;
	style.commentStyle    = StyleFont::NONE;
	style.numberStyle     = StyleFont::NONE;
	style.stringStyle     = StyleFont::NONE;
	style.operatorStyle   = StyleFont::NONE;

	return style;
}

ParserStyle ParserStyle::getClassicStyle()
{
	ParserStyle style;
	*static_cast<EditStyle*>(&style) = EditStyle::getClassicStyle();

	style.identifierColor = QColor( 0xFF, 0xFF, 0x00 );
	style.keywordColor    = QColor( 0xFF, 0xFF, 0xFF );
	style.functionsColor  = QColor( 0xFF, 0xFF, 0xFF );
	style.traceColor      = QColor( 0xFF, 0xFF, 0xFF );
	style.colorColor      = QColor( 0xFF, 0xFF, 0xFF );
	style.commentColor    = QColor( 0xC0, 0xC0, 0xC0 );
	style.numberColor     = QColor( 0xFF, 0xFF, 0x00 );
	style.stringColor     = QColor( 0xFF, 0xFF, 0x00 );
	style.operatorColor   = QColor( 0xFF, 0xFF, 0x00 );

	style.identifierStyle = StyleFont::NONE;
	style.keywordStyle    = StyleFont::NONE;
	style.functionsStyle  = StyleFont::NONE;
	style.traceStyle      = StyleFont::NONE;
	style.colorStyle      = StyleFont::NONE;
	style.commentStyle    = StyleFont::NONE;
	style.numberStyle     = StyleFont::NONE;
	style.stringStyle     = StyleFont::NONE;
	style.operatorStyle   = StyleFont::NONE;

	return style;
}

ParserStyle ParserStyle::getTwilightStyle()
{
	ParserStyle style;
	*static_cast<EditStyle*>(&style) = EditStyle::getTwilightStyle();

	style.identifierColor = QColor( 0xFF, 0xFF, 0xFF );
	style.keywordColor    = QColor( 0x00, 0xFF, 0xFF );
	style.functionsColor  = QColor( 0xFF, 0xFF, 0x00 );
	style.traceColor      = QColor( 0x00, 0xFF, 0x00 );
	style.colorColor      = QColor( 0x00, 0xFF, 0x00 );
	style.commentColor    = QColor( 0xC0, 0xC0, 0xC0 );
	style.numberColor     = QColor( 0xC0, 0xC0, 0xC0 );
	style.stringColor     = QColor( 0xFF, 0xFF, 0xFF );
	style.operatorColor   = QColor( 0xFF, 0xFF, 0xFF );

	style.identifierStyle = StyleFont::NONE;
	style.keywordStyle    = StyleFont::BOLD;
	style.functionsStyle  = StyleFont::BOLD;
	style.traceStyle      = static_cast<StyleFont::style>(StyleFont::BOLD | StyleFont::ITALIC);
	style.colorStyle      = StyleFont::BOLD;
	style.commentStyle    = StyleFont::ITALIC;
	style.numberStyle     = StyleFont::NONE;
	style.stringStyle     = StyleFont::NONE;
	style.operatorStyle   = StyleFont::NONE;

	return style;
}

ParserStyle ParserStyle::getOceanStyle()
{
	ParserStyle style;
	*static_cast<EditStyle*>(&style) = EditStyle::getOceanStyle();

	style.identifierColor = QColor( 0x00, 0x00, 0xFF );
	style.keywordColor    = QColor( 0x00, 0x00, 0x00 );
	style.functionsColor  = QColor( 0x00, 0x00, 0x00 );
	style.traceColor      = QColor( 0x00, 0x00, 0x00 );
	style.colorColor      = QColor( 0x00, 0x00, 0x00 );
	style.commentColor    = QColor( 0x00, 0x80, 0x80 );
	style.numberColor     = QColor( 0x00, 0x00, 0xFF );
	style.stringColor     = QColor( 0x00, 0x00, 0xFF );
	style.operatorColor   = QColor( 0x00, 0x00, 0xFF );

	style.identifierStyle = StyleFont::NONE;
	style.keywordStyle    = StyleFont::BOLD;
	style.functionsStyle  = StyleFont::BOLD;
	style.traceStyle      = StyleFont::BOLD;
	style.colorStyle      = StyleFont::BOLD;
	style.commentStyle    = StyleFont::ITALIC;
	style.numberStyle     = StyleFont::NONE;
	style.stringStyle     = StyleFont::NONE;
	style.operatorStyle   = StyleFont::NONE;

	return style;
}

namespace rdo { namespace gui { namespace editor {

QSettings& operator<< (QSettings& settings, const ParserStyle& style)
{
	settings << static_cast<const EditStyle&>(style);

	settings.beginGroup("theme");	
	settings.setValue("identifier_color", style.identifierColor.name());
	settings.setValue("keyword_color", style.keywordColor.name());
	settings.setValue("functions_color", style.functionsColor.name());
	settings.setValue("trace_color", style.traceColor.name());
	settings.setValue("color_color", style.colorColor.name());
	settings.setValue("comment_color", style.commentColor.name());
	settings.setValue("number_color", style.numberColor.name());
	settings.setValue("string_color", style.stringColor.name());
	settings.setValue("operator_color", style.operatorColor.name());

	settings.setValue("identifier_style", style.identifierStyle);
	settings.setValue("keyword_style", style.keywordStyle);
	settings.setValue("functions_style", style.functionsStyle);
	settings.setValue("trace_style", style.traceStyle);
	settings.setValue("color_style", style.colorStyle);
	settings.setValue("comment_style", style.commentStyle);
	settings.setValue("number_style", style.numberStyle);
	settings.setValue("string_style", style.stringStyle);
	settings.setValue("operator_style", style.operatorStyle);
	settings.endGroup();

	return settings;
}

QSettings& operator>> (QSettings& settings, ParserStyle& style)
{
	settings >> static_cast<EditStyle&>(style);

	settings.beginGroup("theme");
	style.identifierColor        = QColor(settings.value("identifier_color", style.identifierColor.name()).toString());
	style.keywordColor           = QColor(settings.value("keyword_color", style.keywordColor.name()).toString());
	style.functionsColor         = QColor(settings.value("functions_color", style.functionsColor.name()).toString());
	style.traceColor             = QColor(settings.value("trace_color", style.traceColor.name()).toString());
	style.colorColor             = QColor(settings.value("color_color", style.colorColor.name()).toString());
	style.commentColor           = QColor(settings.value("comment_color", style.commentColor.name()).toString());
	style.numberColor            = QColor(settings.value("number_color", style.numberColor.name()).toString());
	style.stringColor            = QColor(settings.value("string_color", style.stringColor.name()).toString());
	style.operatorColor          = QColor(settings.value("operator_color", style.operatorColor.name()).toString());

	style.identifierStyle        = static_cast<StyleFont::style>(settings.value("identifier_style", style.identifierStyle).toInt());
	style.keywordStyle           = static_cast<StyleFont::style>(settings.value("keyword_style", style.keywordStyle).toInt());
	style.functionsStyle         = static_cast<StyleFont::style>(settings.value("functions_style", style.functionsStyle).toInt());
	style.traceStyle             = static_cast<StyleFont::style>(settings.value("trace_style", style.traceStyle).toInt());
	style.colorStyle             = static_cast<StyleFont::style>(settings.value("color_style", style.colorStyle).toInt());
	style.commentStyle           = static_cast<StyleFont::style>(settings.value("comment_style", style.commentStyle).toInt());
	style.numberStyle            = static_cast<StyleFont::style>(settings.value("number_style", style.numberStyle).toInt());
	style.stringStyle            = static_cast<StyleFont::style>(settings.value("string_style", style.stringStyle).toInt());
	style.operatorStyle          = static_cast<StyleFont::style>(settings.value("operator_style", style.operatorStyle).toInt());
	settings.endGroup();

	return settings;
}

}}} // namespace rdo::gui::editor
