#include "stdafx.h"
#include "rdoeditorscilogstyle.h"
#include "sci/SciLexer.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

using namespace rdoEditor;

// ----------------------------------------------------------------------------
// ---------- RDOEditorSciLogTheme
// ----------------------------------------------------------------------------
RDOEditorSciLogTheme::RDOEditorSciLogTheme(): RDOEditorSciEditTheme()
{
	selectLineBgColor = RGB( 0x00, 0xC0, 0xEF );
}

RDOEditorSciLogTheme::~RDOEditorSciLogTheme()
{
}

RDOEditorSciLogTheme& RDOEditorSciLogTheme::operator =( const RDOEditorSciLogTheme& theme )
{
	selectLineBgColor = theme.selectLineBgColor;

	return *this;
}

bool RDOEditorSciLogTheme::operator ==( const RDOEditorSciLogTheme& theme ) const
{
	return selectLineBgColor == theme.selectLineBgColor;
}

bool RDOEditorSciLogTheme::operator !=( const RDOEditorSciLogTheme& theme ) const
{
	return !(*this == theme);
}

void RDOEditorSciLogTheme::load( string regPath )
{
	RDOEditorSciEditTheme::load( regPath );

	regPath += "theme";
	selectLineBgColor = AfxGetApp()->GetProfileInt( regPath.c_str(), "selectLineBgColor", selectLineBgColor );
}

void RDOEditorSciLogTheme::save( string regPath ) const
{
	RDOEditorSciEditTheme::save( regPath );

	regPath += "theme";
	AfxGetApp()->WriteProfileInt( regPath.c_str(), "selectLineBgColor", selectLineBgColor );
}

// ----------------------------------------------------------------------------
// ---------- RDOEditorSciLogStyle
// ----------------------------------------------------------------------------
RDOEditorSciLogStyle::RDOEditorSciLogStyle(): RDOEditorSciEditStyle()
{
}

RDOEditorSciLogStyle::~RDOEditorSciLogStyle()
{
}

void RDOEditorSciLogStyle::initTheme()
{
	theme = new RDOEditorSciLogTheme;
}

RDOEditorSciLogStyle& RDOEditorSciLogStyle::operator =( const RDOEditorSciLogStyle& style )
{
	RDOEditorSciEditStyle::operator=( style );
	if ( theme && style.theme ) *(RDOEditorSciLogTheme*)theme = *(RDOEditorSciLogTheme*)style.theme;

	return *this;
}

bool RDOEditorSciLogStyle::operator ==( const RDOEditorSciLogStyle& style ) const
{
	bool flag = RDOEditorSciEditStyle::operator==( style );
	if ( theme && style.theme && flag ) flag &= *(RDOEditorSciLogTheme*)theme == *(RDOEditorSciLogTheme*)style.theme;
	return flag;
}

bool RDOEditorSciLogStyle::operator !=( const RDOEditorSciLogStyle& style ) const
{
	return !(*this == style);
}
