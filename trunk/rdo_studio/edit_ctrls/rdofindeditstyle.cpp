#include "stdafx.h"
#include "rdofindeditstyle.h"
#include "sci/SciLexer.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

using namespace std;
using namespace rdoEditCtrl;

// ----------------------------------------------------------------------------
// ---------- RDOLogEditTheme
// ----------------------------------------------------------------------------
RDOFindEditTheme::RDOFindEditTheme(): RDOLogEditTheme()
{
	defaultColor    = RGB( 0x00, 0x00, 0x00 );
	identifierColor = RGB( 0x00, 0x00, 0x00 );
	keywordColor    = RGB( 0x00, 0x00, 0x00 );

	defaultStyle    = RDOFS_NONE;
	identifierStyle = RDOFS_NONE;
	keywordStyle    = RDOFS_BOLD;
}

RDOFindEditTheme::~RDOFindEditTheme()
{
}

RDOFindEditTheme& RDOFindEditTheme::operator =( const RDOFindEditTheme& theme )
{
	identifierColor = theme.identifierColor;
	keywordColor    = theme.keywordColor;

	identifierStyle = theme.identifierStyle;
	keywordStyle    = theme.keywordStyle;

	return *this;
}

bool RDOFindEditTheme::operator ==( const RDOFindEditTheme& theme ) const
{
	return identifierColor == theme.identifierColor &&
	       keywordColor    == theme.keywordColor &&

	       identifierStyle == theme.identifierStyle &&
	       keywordStyle    == theme.keywordStyle;
}

bool RDOFindEditTheme::operator !=( const RDOFindEditTheme& theme ) const
{
	return !(*this == theme);
}

void RDOFindEditTheme::load( string regPath )
{
	RDOLogEditTheme::load( regPath );

	regPath += "theme";
	identifierColor = AfxGetApp()->GetProfileInt( regPath.c_str(), "identifierColor", identifierColor );
	keywordColor    = AfxGetApp()->GetProfileInt( regPath.c_str(), "keywordColor", keywordColor );
	identifierStyle = (RDOFontStyle)AfxGetApp()->GetProfileInt( regPath.c_str(), "identifierStyle", identifierStyle );
	keywordStyle    = (RDOFontStyle)AfxGetApp()->GetProfileInt( regPath.c_str(), "keywordStyle", keywordStyle );
}

void RDOFindEditTheme::save( string regPath ) const
{
	RDOLogEditTheme::save( regPath );

	regPath += "theme";
	AfxGetApp()->WriteProfileInt( regPath.c_str(), "identifierColor", identifierColor );
	AfxGetApp()->WriteProfileInt( regPath.c_str(), "keywordColor", keywordColor );
	AfxGetApp()->WriteProfileInt( regPath.c_str(), "identifierStyle", identifierStyle );
	AfxGetApp()->WriteProfileInt( regPath.c_str(), "keywordStyle", keywordStyle );
}

bool RDOFindEditTheme::styleDefault( const int styleType ) const
{
	return styleType == SCE_FIND_DEFAULT;
}

bool RDOFindEditTheme::styleUsing( const int styleType ) const
{
	return styleType == SCE_FIND_DEFAULT || styleType == SCE_FIND_IDENTIFIER || styleType == SCE_FIND_KEYWORD;
}

bool RDOFindEditTheme::styleBold( const int styleType ) const
{
	switch ( styleType ) {
		case SCE_FIND_DEFAULT   : return defaultStyle    & RDOFS_BOLD ? true : false;
		case SCE_FIND_IDENTIFIER: return identifierStyle & RDOFS_BOLD ? true : false;
		case SCE_FIND_KEYWORD   : return keywordStyle    & RDOFS_BOLD ? true : false;
	}
	return false;
}

bool RDOFindEditTheme::styleItalic( const int styleType ) const
{
	switch ( styleType ) {
		case SCE_FIND_DEFAULT   : return defaultStyle    & RDOFS_ITALIC ? true : false;
		case SCE_FIND_IDENTIFIER: return identifierStyle & RDOFS_ITALIC ? true : false;
		case SCE_FIND_KEYWORD   : return keywordStyle    & RDOFS_ITALIC ? true : false;
	}
	return false;
}

string RDOFindEditTheme::styleFGColorToHEX( const int styleType ) const
{
	switch ( styleType ) {
		case SCE_FIND_DEFAULT   : return colorToHEX( defaultColor );
		case SCE_FIND_IDENTIFIER: return colorToHEX( identifierColor );
		case SCE_FIND_KEYWORD   : return colorToHEX( keywordColor );
	}
	return RDOLogEditTheme::styleFGColorToHEX( styleType );
}

// ----------------------------------------------------------------------------
// ---------- RDOFindEditStyle
// ----------------------------------------------------------------------------
RDOFindEditStyle::RDOFindEditStyle(): RDOLogEditStyle()
{
}

RDOFindEditStyle::~RDOFindEditStyle()
{
}

void RDOFindEditStyle::initTheme()
{
	theme = new RDOFindEditTheme;
}

RDOFindEditStyle& RDOFindEditStyle::operator =( const RDOFindEditStyle& style )
{
	RDOLogEditStyle::operator=( style );
	if ( theme && style.theme ) *(RDOFindEditTheme*)theme = *(RDOFindEditTheme*)style.theme;

	return *this;
}

bool RDOFindEditStyle::operator ==( const RDOFindEditStyle& style ) const
{
	bool flag = RDOLogEditStyle::operator==( style );
	if ( theme && style.theme && flag ) flag &= *(RDOFindEditTheme*)theme == *(RDOFindEditTheme*)style.theme;
	return flag;
}

bool RDOFindEditStyle::operator !=( const RDOFindEditStyle& style ) const
{
	return !(*this == style);
}
