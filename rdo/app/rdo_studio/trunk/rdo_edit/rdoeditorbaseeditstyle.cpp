/*!
  \copyright (c) RDO-Team, 2003-2012
  \file      rdoeditorbaseeditstyle.cpp
  \author    Урусов Андрей (rdo@rk9.bmstu.ru)
  \date      14.03.2003
  \brief     
  \indent    4T
*/

// ---------------------------------------------------------------------------- PCH
#include "app/rdo_studio_mfc/pch/stdpch.h"
// ----------------------------------------------------------------------- INCLUDES
// ----------------------------------------------------------------------- SYNOPSIS
#include "app/rdo_studio_mfc/rdo_edit/rdoeditorbaseeditstyle.h"
#include "thirdparty/scintilla/include/SciLexer.h"
// --------------------------------------------------------------------------------

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

using namespace rdoEditor;
using namespace rdoStyle;
using namespace rdoEditCtrl;

// --------------------------------------------------------------------------------
// -------------------- RDOEditorBaseEditTheme
// --------------------------------------------------------------------------------
RDOEditorBaseEditTheme::RDOEditorBaseEditTheme(): RDOBaseEditTheme()
{
	defaultColor    = RGB( 0x80, 0x80, 0x80 );
	identifierColor = RGB( 0x00, 0x00, 0x00 );
	keywordColor    = RGB( 0x00, 0x00, 0x00 );
	functionsColor  = RGB( 0x00, 0x80, 0x00 );
	traceColor      = RGB( 0x80, 0x00, 0x00 );
	colorColor      = RGB( 0x09, 0x26, 0xB0 );
	commentColor    = RGB( 0x00, 0x80, 0x00 );
	numberColor     = RGB( 0x00, 0x00, 0x80 );
	stringColor     = RGB( 0x80, 0x00, 0x80 );
	operatorColor   = RGB( 0x00, 0x00, 0x00 );

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
	RDOBaseEditTheme::operator=( theme );

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
	rbool flag = RDOBaseEditTheme::operator==( theme );

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

void RDOEditorBaseEditTheme::load( tstring regPath )
{
	RDOBaseEditTheme::load( regPath );

	regPath += "theme";
	identifierColor        = AfxGetApp()->GetProfileInt( regPath.c_str(), "identifierColor", identifierColor );
	keywordColor           = AfxGetApp()->GetProfileInt( regPath.c_str(), "keywordColor", keywordColor );
	functionsColor         = AfxGetApp()->GetProfileInt( regPath.c_str(), "functionsColor", functionsColor );
	traceColor             = AfxGetApp()->GetProfileInt( regPath.c_str(), "traceColor", traceColor );
	colorColor             = AfxGetApp()->GetProfileInt( regPath.c_str(), "colorColor", colorColor );
	commentColor           = AfxGetApp()->GetProfileInt( regPath.c_str(), "commentColor", commentColor );
	numberColor            = AfxGetApp()->GetProfileInt( regPath.c_str(), "numberColor", numberColor );
	stringColor            = AfxGetApp()->GetProfileInt( regPath.c_str(), "stringColor", stringColor );
	operatorColor          = AfxGetApp()->GetProfileInt( regPath.c_str(), "operatorColor", operatorColor );
	identifierStyle        = static_cast<RDOStyleFont::style>(AfxGetApp()->GetProfileInt( regPath.c_str(), "identifierStyle", identifierStyle ));
	keywordStyle           = static_cast<RDOStyleFont::style>(AfxGetApp()->GetProfileInt( regPath.c_str(), "keywordStyle", keywordStyle ));
	functionsStyle         = static_cast<RDOStyleFont::style>(AfxGetApp()->GetProfileInt( regPath.c_str(), "functionsStyle", functionsStyle ));
	traceStyle             = static_cast<RDOStyleFont::style>(AfxGetApp()->GetProfileInt( regPath.c_str(), "traceStyle", traceStyle ));
	colorStyle             = static_cast<RDOStyleFont::style>(AfxGetApp()->GetProfileInt( regPath.c_str(), "colorStyle", colorStyle ));
	commentStyle           = static_cast<RDOStyleFont::style>(AfxGetApp()->GetProfileInt( regPath.c_str(), "commentStyle", commentStyle ));
	numberStyle            = static_cast<RDOStyleFont::style>(AfxGetApp()->GetProfileInt( regPath.c_str(), "numberStyle", numberStyle ));
	stringStyle            = static_cast<RDOStyleFont::style>(AfxGetApp()->GetProfileInt( regPath.c_str(), "stringStyle", stringStyle ));
	operatorStyle          = static_cast<RDOStyleFont::style>(AfxGetApp()->GetProfileInt( regPath.c_str(), "operatorStyle", operatorStyle ));
}

void RDOEditorBaseEditTheme::save( tstring regPath ) const
{
	RDOBaseEditTheme::save( regPath );

	regPath += "theme";
	AfxGetApp()->WriteProfileInt( regPath.c_str(), "identifierColor", identifierColor );
	AfxGetApp()->WriteProfileInt( regPath.c_str(), "keywordColor", keywordColor );
	AfxGetApp()->WriteProfileInt( regPath.c_str(), "functionsColor", functionsColor );
	AfxGetApp()->WriteProfileInt( regPath.c_str(), "traceColor", traceColor );
	AfxGetApp()->WriteProfileInt( regPath.c_str(), "colorColor", colorColor );
	AfxGetApp()->WriteProfileInt( regPath.c_str(), "commentColor", commentColor );
	AfxGetApp()->WriteProfileInt( regPath.c_str(), "numberColor", numberColor );
	AfxGetApp()->WriteProfileInt( regPath.c_str(), "stringColor", stringColor );
	AfxGetApp()->WriteProfileInt( regPath.c_str(), "operatorColor", operatorColor );
	AfxGetApp()->WriteProfileInt( regPath.c_str(), "identifierStyle", identifierStyle );
	AfxGetApp()->WriteProfileInt( regPath.c_str(), "keywordStyle", keywordStyle );
	AfxGetApp()->WriteProfileInt( regPath.c_str(), "functionsStyle", functionsStyle );
	AfxGetApp()->WriteProfileInt( regPath.c_str(), "traceColor", traceColor );
	AfxGetApp()->WriteProfileInt( regPath.c_str(), "colorColor", colorColor );
	AfxGetApp()->WriteProfileInt( regPath.c_str(), "commentStyle", commentStyle );
	AfxGetApp()->WriteProfileInt( regPath.c_str(), "numberStyle", numberStyle );
	AfxGetApp()->WriteProfileInt( regPath.c_str(), "stringStyle", stringStyle );
	AfxGetApp()->WriteProfileInt( regPath.c_str(), "operatorStyle", operatorStyle );
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
	return RDOBaseEditTheme::styleFGColorToHEX( styleType );
}

RDOEditorBaseEditTheme RDOEditorBaseEditTheme::getDefaultTheme()
{
	RDOEditorBaseEditTheme theme;
	return theme;
}

RDOEditorBaseEditTheme RDOEditorBaseEditTheme::getCppTheme()
{
	RDOEditorBaseEditTheme theme;
	*static_cast<RDOBaseEditTheme*>(&theme) = RDOBaseEditTheme::getDefaultTheme();

	theme.identifierColor = RGB( 0x00, 0x00, 0x00 );
	theme.keywordColor    = RGB( 0x00, 0x00, 0xFF );
	theme.functionsColor  = RGB( 0x00, 0x00, 0xFF );
	theme.traceColor      = RGB( 0x00, 0x00, 0xFF );
	theme.colorColor      = RGB( 0x00, 0x00, 0xFF );
	theme.commentColor    = RGB( 0x00, 0x80, 0x00 );
	theme.numberColor     = RGB( 0x00, 0x00, 0x00 );
	theme.stringColor     = RGB( 0x00, 0x00, 0x00 );
	theme.operatorColor   = RGB( 0x00, 0x00, 0x00 );

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
	*static_cast<RDOBaseEditTheme*>(&theme) = RDOBaseEditTheme::getDefaultTheme();

	theme.identifierColor = RGB( 0x00, 0x00, 0x00 );
	theme.keywordColor    = RGB( 0x00, 0x00, 0x00 );
	theme.functionsColor  = RGB( 0x00, 0x00, 0x00 );
	theme.traceColor      = RGB( 0x00, 0x00, 0x00 );
	theme.colorColor      = RGB( 0x00, 0x00, 0x00 );
	theme.commentColor    = RGB( 0x00, 0x00, 0x80 );
	theme.numberColor     = RGB( 0x00, 0x00, 0x00 );
	theme.stringColor     = RGB( 0x00, 0x00, 0x00 );
	theme.operatorColor   = RGB( 0x00, 0x00, 0x00 );

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
	*static_cast<RDOBaseEditTheme*>(&theme) = RDOBaseEditTheme::getDefaultTheme();

	theme.identifierColor = RGB( 0x00, 0x00, 0x00 );
	theme.keywordColor    = RGB( 0x80, 0x00, 0x80 );
	theme.functionsColor  = RGB( 0xFF, 0x00, 0x00 );
	theme.traceColor      = RGB( 0xFF, 0x00, 0x00 );
	theme.colorColor      = RGB( 0xFF, 0x00, 0x00 );
	theme.commentColor    = RGB( 0x00, 0x80, 0x00 );
	theme.numberColor     = RGB( 0x00, 0x00, 0xFF );
	theme.stringColor     = RGB( 0x00, 0x00, 0xFF );
	theme.operatorColor   = RGB( 0x00, 0x00, 0x00 );

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
	*static_cast<RDOBaseEditTheme*>(&theme) = RDOBaseEditTheme::getClassicTheme();

	theme.identifierColor = RGB( 0xFF, 0xFF, 0x00 );
	theme.keywordColor    = RGB( 0xFF, 0xFF, 0xFF );
	theme.functionsColor  = RGB( 0xFF, 0xFF, 0xFF );
	theme.traceColor      = RGB( 0xFF, 0xFF, 0xFF );
	theme.colorColor      = RGB( 0xFF, 0xFF, 0xFF );
	theme.commentColor    = RGB( 0xC0, 0xC0, 0xC0 );
	theme.numberColor     = RGB( 0xFF, 0xFF, 0x00 );
	theme.stringColor     = RGB( 0xFF, 0xFF, 0x00 );
	theme.operatorColor   = RGB( 0xFF, 0xFF, 0x00 );

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
	*static_cast<RDOBaseEditTheme*>(&theme) = RDOBaseEditTheme::getTwilightTheme();

	theme.identifierColor = RGB( 0xFF, 0xFF, 0xFF );
	theme.keywordColor    = RGB( 0x00, 0xFF, 0xFF );
	theme.functionsColor  = RGB( 0xFF, 0xFF, 0x00 );
	theme.traceColor      = RGB( 0x00, 0xFF, 0x00 );
	theme.colorColor      = RGB( 0x00, 0xFF, 0x00 );
	theme.commentColor    = RGB( 0xC0, 0xC0, 0xC0 );
	theme.numberColor     = RGB( 0xC0, 0xC0, 0xC0 );
	theme.stringColor     = RGB( 0xFF, 0xFF, 0xFF );
	theme.operatorColor   = RGB( 0xFF, 0xFF, 0xFF );

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
	*static_cast<RDOBaseEditTheme*>(&theme) = RDOBaseEditTheme::getOceanTheme();

	theme.identifierColor = RGB( 0x00, 0x00, 0xFF );
	theme.keywordColor    = RGB( 0x00, 0x00, 0x00 );
	theme.functionsColor  = RGB( 0x00, 0x00, 0x00 );
	theme.traceColor      = RGB( 0x00, 0x00, 0x00 );
	theme.colorColor      = RGB( 0x00, 0x00, 0x00 );
	theme.commentColor    = RGB( 0x00, 0x80, 0x80 );
	theme.numberColor     = RGB( 0x00, 0x00, 0xFF );
	theme.stringColor     = RGB( 0x00, 0x00, 0xFF );
	theme.operatorColor   = RGB( 0x00, 0x00, 0xFF );

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

// --------------------------------------------------------------------------------
// -------------------- RDOEditorBaseEditStyle
// --------------------------------------------------------------------------------
RDOEditorBaseEditStyle::RDOEditorBaseEditStyle(): RDOBaseEditStyle()
{
}

RDOEditorBaseEditStyle::~RDOEditorBaseEditStyle()
{
}

void RDOEditorBaseEditStyle::initTheme()
{
	theme = new RDOEditorBaseEditTheme;
}

RDOEditorBaseEditStyle& RDOEditorBaseEditStyle::operator =( const RDOEditorBaseEditStyle& style )
{
	RDOBaseEditStyle::operator=( style );
	if ( theme && style.theme ) *static_cast<RDOEditorBaseEditTheme*>(theme) = *static_cast<RDOEditorBaseEditTheme*>(style.theme);

	return *this;
}

rbool RDOEditorBaseEditStyle::operator ==( const RDOEditorBaseEditStyle& style ) const
{
	rbool flag = RDOBaseEditStyle::operator==( style );
	if ( theme && style.theme && flag ) flag &= *static_cast<RDOEditorBaseEditTheme*>(theme) == *static_cast<RDOEditorBaseEditTheme*>(style.theme);
	return flag;
}

rbool RDOEditorBaseEditStyle::operator !=( const RDOEditorBaseEditStyle& style ) const
{
	return !(*this == style);
}
