#include "stdafx.h"
#include "rdobasectrlstyle.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

using namespace std;
using namespace rdoEditCtrl;

// ----------------------------------------------------------------------------
// ---------- RDOBaseCtrlFont
// ----------------------------------------------------------------------------
RDOBaseCtrlFont::RDOBaseCtrlFont()
{
	name         = "Courier New";
	size         = 10;
	codepage     = 0;
	characterSet = 1;
	if ( PRIMARYLANGID(GetSystemDefaultLangID()) == LANG_RUSSIAN ) {
		characterSet = RUSSIAN_CHARSET;
	}
}

RDOBaseCtrlFont::~RDOBaseCtrlFont()
{
}

RDOBaseCtrlFont& RDOBaseCtrlFont::operator =( const RDOBaseCtrlFont& font )
{
	name         = font.name;
	size         = font.size;
	codepage     = font.codepage;
	characterSet = font.characterSet;

	return *this;
}

bool RDOBaseCtrlFont::operator ==( const RDOBaseCtrlFont& font ) const
{
	return name         == font.name &&
	       size         == font.size &&
	       codepage     == font.codepage &&
	       characterSet == font.characterSet;
}

bool RDOBaseCtrlFont::operator !=( const RDOBaseCtrlFont& font ) const
{
	return !(*this == font);
}

void RDOBaseCtrlFont::load( string regPath )
{
	regPath += "font";
	name         = AfxGetApp()->GetProfileString( regPath.c_str(), "name", name.c_str() );
	size         = AfxGetApp()->GetProfileInt( regPath.c_str(), "size", size );
	codepage     = AfxGetApp()->GetProfileInt( regPath.c_str(), "codepage", codepage );
	characterSet = AfxGetApp()->GetProfileInt( regPath.c_str(), "characterSet", characterSet );
}

void RDOBaseCtrlFont::save( string regPath ) const
{
	regPath += "font";
	AfxGetApp()->WriteProfileString( regPath.c_str(), "name", name.c_str() );
	AfxGetApp()->WriteProfileInt( regPath.c_str(), "size", size );
	AfxGetApp()->WriteProfileInt( regPath.c_str(), "codepage", codepage );
	AfxGetApp()->WriteProfileInt( regPath.c_str(), "characterSet", characterSet );
}

// ----------------------------------------------------------------------------
// ---------- RDOBaseCtrlStyle
// ----------------------------------------------------------------------------
RDOBaseCtrlStyle::RDOBaseCtrlStyle():
	regPath( "" ),
	font( NULL )
{
}

RDOBaseCtrlStyle::~RDOBaseCtrlStyle()
{
	if ( font ) { delete font; font = NULL; };
}

void RDOBaseCtrlStyle::initFont()
{
	font = new RDOBaseCtrlFont;
}

RDOBaseCtrlStyle& RDOBaseCtrlStyle::operator =( const RDOBaseCtrlStyle& style )
{
	if ( font && style.font ) *font = *style.font;

	return *this;
}

bool RDOBaseCtrlStyle::operator ==( const RDOBaseCtrlStyle& style ) const
{
	bool flag = true;
	if ( font && style.font && flag ) flag &= *font == *style.font;
	return flag;
}

bool RDOBaseCtrlStyle::operator !=( const RDOBaseCtrlStyle& style ) const
{
	return !(*this == style);
}

void RDOBaseCtrlStyle::init( const string& _regPath )
{
	regPath = _regPath;
	trim( regPath );
	if ( !regPath.empty() ) {
		if ( regPath.find_last_of( '\\' ) != regPath.length() - 1 ) {
			regPath += '\\';
		}
		regPath = "style\\" + regPath;
	}
	initFont();
}

bool RDOBaseCtrlStyle::load()
{
	if ( !regPath.empty() ) {
		if ( font ) font->load( regPath );
		return true;
	}
	return false;
}

bool RDOBaseCtrlStyle::save() const
{
	if ( !regPath.empty() ) {
		if ( font ) font->save( regPath );
		return true;
	}
	return false;
}
