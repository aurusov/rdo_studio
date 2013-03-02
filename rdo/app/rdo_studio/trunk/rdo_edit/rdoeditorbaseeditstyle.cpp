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
#include "app/rdo_studio/rdo_edit/rdoeditorbaseeditstyle.h"
#include "thirdparty/scintilla/include/SciLexer.h"
// --------------------------------------------------------------------------------

using namespace rdoEditor;
using namespace rdoStyle;
using namespace rdoEditCtrl;

// --------------------------------------------------------------------------------
// -------------------- RDOEditorBaseEditTheme
// --------------------------------------------------------------------------------
RDOEditorBaseEditTheme::RDOEditorBaseEditTheme(): EditBaseTheme()
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

	identifierStyle = RDOStyleFont::NONE;
	keywordStyle    = RDOStyleFont::BOLD;
	functionsStyle  = RDOStyleFont::BOLD;
	traceStyle      = RDOStyleFont::BOLD;
	colorStyle      = RDOStyleFont::BOLD;
	commentStyle    = RDOStyleFont::ITALIC;
	numberStyle     = RDOStyleFont::NONE;
	stringStyle     = RDOStyleFont::NONE;
	operatorStyle   = RDOStyleFont::NONE;
}

RDOEditorBaseEditTheme::~RDOEditorBaseEditTheme()
{
}

RDOEditorBaseEditTheme& RDOEditorBaseEditTheme::operator =( const RDOEditorBaseEditTheme& theme )
{
	EditBaseTheme::operator=( theme );

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

rbool RDOEditorBaseEditTheme::operator ==( const RDOEditorBaseEditTheme& theme ) const
{
	rbool flag = EditBaseTheme::operator==( theme );

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

rbool RDOEditorBaseEditTheme::operator !=( const RDOEditorBaseEditTheme& theme ) const
{
	return !(*this == theme);
}

void RDOEditorBaseEditTheme::load(QSettings& settings)
{
	EditBaseTheme::load(settings);

	settings >> *this;
}

void RDOEditorBaseEditTheme::save(QSettings& settings) const
{
	EditBaseTheme::save(settings);

	settings << *this;
}

rbool RDOEditorBaseEditTheme::styleDefault( const int styleType ) const
{
	return styleType == SCE_RDO_DEFAULT;
}

rbool RDOEditorBaseEditTheme::styleUsing( const int styleType ) const
{
	return styleType == SCE_RDO_DEFAULT      || styleType == SCE_RDO_IDENTIFIER   ||
	       styleType == SCE_RDO_KEYWORD      || styleType == SCE_RDO_FUNCTION     ||
	       styleType == SCE_RDO_TRACE        || styleType == SCE_RDO_COMMENT_CPP  ||
	       styleType == SCE_RDO_COMMENT_LINE || styleType == SCE_RDO_NUMBER       ||
	       styleType == SCE_RDO_STRING       || styleType == SCE_RDO_OPERATOR     ||
	       styleType == SCE_RDO_FRAME_COLOR;
}

rbool RDOEditorBaseEditTheme::styleBold( const int styleType ) const
{
	switch ( styleType ) {
		case SCE_RDO_DEFAULT     : return defaultStyle    & RDOStyleFont::BOLD ? true : false;
		case SCE_RDO_IDENTIFIER  : return identifierStyle & RDOStyleFont::BOLD ? true : false;
		case SCE_RDO_KEYWORD     : return keywordStyle    & RDOStyleFont::BOLD ? true : false;
		case SCE_RDO_FUNCTION    : return functionsStyle  & RDOStyleFont::BOLD ? true : false;
		case SCE_RDO_TRACE       : return traceStyle      & RDOStyleFont::BOLD ? true : false;
		case SCE_RDO_FRAME_COLOR : return colorStyle      & RDOStyleFont::BOLD ? true : false;
		case SCE_RDO_COMMENT_CPP : return commentStyle    & RDOStyleFont::BOLD ? true : false;
		case SCE_RDO_COMMENT_LINE: return commentStyle    & RDOStyleFont::BOLD ? true : false;
		case SCE_RDO_NUMBER      : return numberStyle     & RDOStyleFont::BOLD ? true : false;
		case SCE_RDO_STRING      : return stringStyle     & RDOStyleFont::BOLD ? true : false;
		case SCE_RDO_OPERATOR    : return operatorStyle   & RDOStyleFont::BOLD ? true : false;
	}
	return false;
}

rbool RDOEditorBaseEditTheme::styleItalic( const int styleType ) const
{
	switch ( styleType ) {
		case SCE_RDO_DEFAULT     : return defaultStyle    & RDOStyleFont::ITALIC ? true : false;
		case SCE_RDO_IDENTIFIER  : return identifierStyle & RDOStyleFont::ITALIC ? true : false;
		case SCE_RDO_KEYWORD     : return keywordStyle    & RDOStyleFont::ITALIC ? true : false;
		case SCE_RDO_FUNCTION    : return functionsStyle  & RDOStyleFont::ITALIC ? true : false;
		case SCE_RDO_TRACE       : return traceStyle      & RDOStyleFont::ITALIC ? true : false;
		case SCE_RDO_FRAME_COLOR : return colorStyle      & RDOStyleFont::ITALIC ? true : false;
		case SCE_RDO_COMMENT_CPP : return commentStyle    & RDOStyleFont::ITALIC ? true : false;
		case SCE_RDO_COMMENT_LINE: return commentStyle    & RDOStyleFont::ITALIC ? true : false;
		case SCE_RDO_NUMBER      : return numberStyle     & RDOStyleFont::ITALIC ? true : false;
		case SCE_RDO_STRING      : return stringStyle     & RDOStyleFont::ITALIC ? true : false;
		case SCE_RDO_OPERATOR    : return operatorStyle   & RDOStyleFont::ITALIC ? true : false;
	}
	return false;
}

tstring RDOEditorBaseEditTheme::styleFGColorToHEX( const int styleType ) const
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
	return EditBaseTheme::styleFGColorToHEX( styleType );
}

RDOEditorBaseEditTheme RDOEditorBaseEditTheme::getDefaultTheme()
{
	RDOEditorBaseEditTheme theme;
	return theme;
}

RDOEditorBaseEditTheme RDOEditorBaseEditTheme::getCppTheme()
{
	RDOEditorBaseEditTheme theme;
	*static_cast<EditBaseTheme*>(&theme) = EditBaseTheme::getDefaultTheme();

	theme.identifierColor = QColor( 0x00, 0x00, 0x00 );
	theme.keywordColor    = QColor( 0x00, 0x00, 0xFF );
	theme.functionsColor  = QColor( 0x00, 0x00, 0xFF );
	theme.traceColor      = QColor( 0x00, 0x00, 0xFF );
	theme.colorColor      = QColor( 0x00, 0x00, 0xFF );
	theme.commentColor    = QColor( 0x00, 0x80, 0x00 );
	theme.numberColor     = QColor( 0x00, 0x00, 0x00 );
	theme.stringColor     = QColor( 0x00, 0x00, 0x00 );
	theme.operatorColor   = QColor( 0x00, 0x00, 0x00 );

	theme.identifierStyle = RDOStyleFont::NONE;
	theme.keywordStyle    = RDOStyleFont::NONE;
	theme.functionsStyle  = RDOStyleFont::NONE;
	theme.traceStyle      = RDOStyleFont::NONE;
	theme.colorStyle      = RDOStyleFont::NONE;
	theme.commentStyle    = RDOStyleFont::NONE;
	theme.numberStyle     = RDOStyleFont::NONE;
	theme.stringStyle     = RDOStyleFont::NONE;
	theme.operatorStyle   = RDOStyleFont::NONE;

	return theme;
}

RDOEditorBaseEditTheme RDOEditorBaseEditTheme::getPascalTheme()
{
	RDOEditorBaseEditTheme theme;
	*static_cast<EditBaseTheme*>(&theme) = EditBaseTheme::getDefaultTheme();

	theme.identifierColor = QColor( 0x00, 0x00, 0x00 );
	theme.keywordColor    = QColor( 0x00, 0x00, 0x00 );
	theme.functionsColor  = QColor( 0x00, 0x00, 0x00 );
	theme.traceColor      = QColor( 0x00, 0x00, 0x00 );
	theme.colorColor      = QColor( 0x00, 0x00, 0x00 );
	theme.commentColor    = QColor( 0x00, 0x00, 0x80 );
	theme.numberColor     = QColor( 0x00, 0x00, 0x00 );
	theme.stringColor     = QColor( 0x00, 0x00, 0x00 );
	theme.operatorColor   = QColor( 0x00, 0x00, 0x00 );

	theme.identifierStyle = RDOStyleFont::NONE;
	theme.keywordStyle    = RDOStyleFont::BOLD;
	theme.functionsStyle  = RDOStyleFont::BOLD;
	theme.traceStyle      = RDOStyleFont::BOLD;
	theme.colorStyle      = RDOStyleFont::BOLD;
	theme.commentStyle    = RDOStyleFont::ITALIC;
	theme.numberStyle     = RDOStyleFont::NONE;
	theme.stringStyle     = RDOStyleFont::NONE;
	theme.operatorStyle   = RDOStyleFont::NONE;

	return theme;
}

RDOEditorBaseEditTheme RDOEditorBaseEditTheme::getHtmlTheme()
{
	RDOEditorBaseEditTheme theme;
	*static_cast<EditBaseTheme*>(&theme) = EditBaseTheme::getDefaultTheme();

	theme.identifierColor = QColor( 0x00, 0x00, 0x00 );
	theme.keywordColor    = QColor( 0x80, 0x00, 0x80 );
	theme.functionsColor  = QColor( 0xFF, 0x00, 0x00 );
	theme.traceColor      = QColor( 0xFF, 0x00, 0x00 );
	theme.colorColor      = QColor( 0xFF, 0x00, 0x00 );
	theme.commentColor    = QColor( 0x00, 0x80, 0x00 );
	theme.numberColor     = QColor( 0x00, 0x00, 0xFF );
	theme.stringColor     = QColor( 0x00, 0x00, 0xFF );
	theme.operatorColor   = QColor( 0x00, 0x00, 0x00 );

	theme.identifierStyle = RDOStyleFont::NONE;
	theme.keywordStyle    = RDOStyleFont::NONE;
	theme.functionsStyle  = RDOStyleFont::NONE;
	theme.traceStyle      = RDOStyleFont::NONE;
	theme.colorStyle      = RDOStyleFont::NONE;
	theme.commentStyle    = RDOStyleFont::NONE;
	theme.numberStyle     = RDOStyleFont::NONE;
	theme.stringStyle     = RDOStyleFont::NONE;
	theme.operatorStyle   = RDOStyleFont::NONE;

	return theme;
}

RDOEditorBaseEditTheme RDOEditorBaseEditTheme::getClassicTheme()
{
	RDOEditorBaseEditTheme theme;
	*static_cast<EditBaseTheme*>(&theme) = EditBaseTheme::getClassicTheme();

	theme.identifierColor = QColor( 0xFF, 0xFF, 0x00 );
	theme.keywordColor    = QColor( 0xFF, 0xFF, 0xFF );
	theme.functionsColor  = QColor( 0xFF, 0xFF, 0xFF );
	theme.traceColor      = QColor( 0xFF, 0xFF, 0xFF );
	theme.colorColor      = QColor( 0xFF, 0xFF, 0xFF );
	theme.commentColor    = QColor( 0xC0, 0xC0, 0xC0 );
	theme.numberColor     = QColor( 0xFF, 0xFF, 0x00 );
	theme.stringColor     = QColor( 0xFF, 0xFF, 0x00 );
	theme.operatorColor   = QColor( 0xFF, 0xFF, 0x00 );

	theme.identifierStyle = RDOStyleFont::NONE;
	theme.keywordStyle    = RDOStyleFont::NONE;
	theme.functionsStyle  = RDOStyleFont::NONE;
	theme.traceStyle      = RDOStyleFont::NONE;
	theme.colorStyle      = RDOStyleFont::NONE;
	theme.commentStyle    = RDOStyleFont::NONE;
	theme.numberStyle     = RDOStyleFont::NONE;
	theme.stringStyle     = RDOStyleFont::NONE;
	theme.operatorStyle   = RDOStyleFont::NONE;

	return theme;
}

RDOEditorBaseEditTheme RDOEditorBaseEditTheme::getTwilightTheme()
{
	RDOEditorBaseEditTheme theme;
	*static_cast<EditBaseTheme*>(&theme) = EditBaseTheme::getTwilightTheme();

	theme.identifierColor = QColor( 0xFF, 0xFF, 0xFF );
	theme.keywordColor    = QColor( 0x00, 0xFF, 0xFF );
	theme.functionsColor  = QColor( 0xFF, 0xFF, 0x00 );
	theme.traceColor      = QColor( 0x00, 0xFF, 0x00 );
	theme.colorColor      = QColor( 0x00, 0xFF, 0x00 );
	theme.commentColor    = QColor( 0xC0, 0xC0, 0xC0 );
	theme.numberColor     = QColor( 0xC0, 0xC0, 0xC0 );
	theme.stringColor     = QColor( 0xFF, 0xFF, 0xFF );
	theme.operatorColor   = QColor( 0xFF, 0xFF, 0xFF );

	theme.identifierStyle = RDOStyleFont::NONE;
	theme.keywordStyle    = RDOStyleFont::BOLD;
	theme.functionsStyle  = RDOStyleFont::BOLD;
	theme.traceStyle      = static_cast<RDOStyleFont::style>(RDOStyleFont::BOLD | RDOStyleFont::ITALIC);
	theme.colorStyle      = RDOStyleFont::BOLD;
	theme.commentStyle    = RDOStyleFont::ITALIC;
	theme.numberStyle     = RDOStyleFont::NONE;
	theme.stringStyle     = RDOStyleFont::NONE;
	theme.operatorStyle   = RDOStyleFont::NONE;

	return theme;
}

RDOEditorBaseEditTheme RDOEditorBaseEditTheme::getOceanTheme()
{
	RDOEditorBaseEditTheme theme;
	*static_cast<EditBaseTheme*>(&theme) = EditBaseTheme::getOceanTheme();

	theme.identifierColor = QColor( 0x00, 0x00, 0xFF );
	theme.keywordColor    = QColor( 0x00, 0x00, 0x00 );
	theme.functionsColor  = QColor( 0x00, 0x00, 0x00 );
	theme.traceColor      = QColor( 0x00, 0x00, 0x00 );
	theme.colorColor      = QColor( 0x00, 0x00, 0x00 );
	theme.commentColor    = QColor( 0x00, 0x80, 0x80 );
	theme.numberColor     = QColor( 0x00, 0x00, 0xFF );
	theme.stringColor     = QColor( 0x00, 0x00, 0xFF );
	theme.operatorColor   = QColor( 0x00, 0x00, 0xFF );

	theme.identifierStyle = RDOStyleFont::NONE;
	theme.keywordStyle    = RDOStyleFont::BOLD;
	theme.functionsStyle  = RDOStyleFont::BOLD;
	theme.traceStyle      = RDOStyleFont::BOLD;
	theme.colorStyle      = RDOStyleFont::BOLD;
	theme.commentStyle    = RDOStyleFont::ITALIC;
	theme.numberStyle     = RDOStyleFont::NONE;
	theme.stringStyle     = RDOStyleFont::NONE;
	theme.operatorStyle   = RDOStyleFont::NONE;

	return theme;
}

namespace rdoEditor
{

QSettings& operator<< (QSettings& settings, const RDOEditorBaseEditTheme& theme)
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

QSettings& operator>> (QSettings& settings, RDOEditorBaseEditTheme& theme)
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

	theme.identifierStyle        = static_cast<RDOStyleFont::style>(settings.value("identifier_style", theme.identifierStyle).toInt());
	theme.keywordStyle           = static_cast<RDOStyleFont::style>(settings.value("keyword_style", theme.keywordStyle).toInt());
	theme.functionsStyle         = static_cast<RDOStyleFont::style>(settings.value("functions_style", theme.functionsStyle).toInt());
	theme.traceStyle             = static_cast<RDOStyleFont::style>(settings.value("trace_style", theme.traceStyle).toInt());
	theme.colorStyle             = static_cast<RDOStyleFont::style>(settings.value("color_style", theme.colorStyle).toInt());
	theme.commentStyle           = static_cast<RDOStyleFont::style>(settings.value("comment_style", theme.commentStyle).toInt());
	theme.numberStyle            = static_cast<RDOStyleFont::style>(settings.value("number_style", theme.numberStyle).toInt());
	theme.stringStyle            = static_cast<RDOStyleFont::style>(settings.value("string_style", theme.stringStyle).toInt());
	theme.operatorStyle          = static_cast<RDOStyleFont::style>(settings.value("operator_style", theme.operatorStyle).toInt());
	
	return settings;
}

} // namespace rdoEditor

// --------------------------------------------------------------------------------
// -------------------- RDOEditorBaseEditStyle
// --------------------------------------------------------------------------------
RDOEditorBaseEditStyle::RDOEditorBaseEditStyle(): EditBaseStyle()
{
	theme = new RDOEditorBaseEditTheme();
}

RDOEditorBaseEditStyle::~RDOEditorBaseEditStyle()
{
}


RDOEditorBaseEditStyle& RDOEditorBaseEditStyle::operator =( const RDOEditorBaseEditStyle& style )
{
	EditBaseStyle::operator=( style );
	if ( theme && style.theme ) *static_cast<RDOEditorBaseEditTheme*>(theme) = *static_cast<RDOEditorBaseEditTheme*>(style.theme);

	return *this;
}

rbool RDOEditorBaseEditStyle::operator ==( const RDOEditorBaseEditStyle& style ) const
{
	rbool flag = EditBaseStyle::operator==( style );
	if ( theme && style.theme && flag ) flag &= *static_cast<RDOEditorBaseEditTheme*>(theme) == *static_cast<RDOEditorBaseEditTheme*>(style.theme);
	return flag;
}

rbool RDOEditorBaseEditStyle::operator !=( const RDOEditorBaseEditStyle& style ) const
{
	return !(*this == style);
}
