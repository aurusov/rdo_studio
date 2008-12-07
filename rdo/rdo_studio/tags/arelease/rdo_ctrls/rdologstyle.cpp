#include "rdo_ctrls_pch.h"
#include "rdologstyle.h"

// ----------------------------------------------------------------------------
// ---------- RDOFont
// ----------------------------------------------------------------------------
RDOFont::RDOFont()
{
	name         = "Courier";
	size         = 10;
	style        = RDOFS_NONE;
	codepage     = 0;
	characterSet = 1;
	if ( PRIMARYLANGID(GetSystemDefaultLangID()) == LANG_RUSSIAN ) {
		characterSet = RUSSIAN_CHARSET;
	}
}

RDOFont::~RDOFont()
{
}

RDOFont& RDOFont::operator =( const RDOFont& font )
{
	name         = font.name;
	size         = font.size;
	style        = font.style;
	codepage     = font.codepage;
	characterSet = font.characterSet;

	return *this;
}

bool RDOFont::operator ==( const RDOFont& font ) const
{
	return name         == font.name &&
	       size         == font.size &&
		   style        == font.style &&
	       codepage     == font.codepage &&
	       characterSet == font.characterSet;
}

bool RDOFont::operator !=( const RDOFont& font ) const
{
	return !(*this == font);
}

void RDOFont::load()
{
	CWinApp* app = AfxGetApp();
	
	name         = app->GetProfileString( "logStyle\\font", "name", name );
	size         = app->GetProfileInt( "logStyle\\font", "size", size );
	style        = (RDOFontStyle)app->GetProfileInt( "logStyle\\font", "style", style );
	codepage     = app->GetProfileInt( "logStyle\\font", "codepage", codepage );
	characterSet = app->GetProfileInt( "logStyle\\font", "characterSet", characterSet );
}

void RDOFont::save() const
{
	CWinApp* app = AfxGetApp();

	app->WriteProfileString( "logStyle\\font", "name", name );
	app->WriteProfileInt( "logStyle\\font", "size", size );
	app->WriteProfileInt( "logStyle\\font", "style", style );
	app->WriteProfileInt( "logStyle\\font", "codepage", codepage );
	app->WriteProfileInt( "logStyle\\font", "characterSet", characterSet );
}

// ----------------------------------------------------------------------------
// ---------- RDOLogStyle
// ----------------------------------------------------------------------------
RDOLogStyle::RDOLogStyle() :
	vertBorder( 0 ),
	horzBorder( 0 )
{
}

RDOLogStyle::~RDOLogStyle()
{
}

bool RDOLogStyle::getItemColors( const int index, COLORREF& textColor, COLORREF& backColor ) const
{
	return false;
}

bool RDOLogStyle::getItemColors( const CString& item, COLORREF& textColor, COLORREF& backColor ) const
{
	return false;
}

RDOLogStyle& RDOLogStyle::operator =( const RDOLogStyle& style )
{
	font       = style.font;
	vertBorder = style.vertBorder;
	horzBorder = style.horzBorder;

	return *this;
}

bool RDOLogStyle::operator ==( const RDOLogStyle& style ) const
{
	return font       == style.font &&
	       vertBorder == style.vertBorder &&
	       horzBorder == style.horzBorder;
}

bool RDOLogStyle::operator !=( const RDOLogStyle& style ) const
{
	return !(*this == style);
}

RDOLogStyle RDOLogStyle::getDefaultStyle()
{
	RDOLogStyle style;
	return style;
}

void RDOLogStyle::load()
{
	font.load();

	CWinApp* app = AfxGetApp();
	vertBorder = app->GetProfileInt( "logStyle", "vertBorder", vertBorder );
	horzBorder = app->GetProfileInt( "logStyle", "horzBorder", horzBorder );
}

void RDOLogStyle::save() const
{
	font.save();

	CWinApp* app = AfxGetApp();
	app->WriteProfileInt( "logStyle", "vertBorder", vertBorder );
	app->WriteProfileInt( "logStyle", "horzBorder", horzBorder );
}
