#include "stdafx.h"
#include "rdoeditoreditstyle.h"
#include "../edit_ctrls/sci/SciLexer.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

using namespace std;
using namespace rdoEditor;
using namespace rdoEditCtrl;

// ----------------------------------------------------------------------------
// ---------- RDOEditorEditTheme
// ----------------------------------------------------------------------------
RDOEditorEditTheme::RDOEditorEditTheme(): RDOBaseEditTheme()
{
	identifierColor = RGB( 0x00, 0x00, 0x00 );
	keywordColor    = RGB( 0x00, 0x00, 0x00 );
	functionsColor  = RGB( 0x00, 0x80, 0x00 );
	traceColor      = RGB( 0x80, 0x00, 0x00 );
	commentColor    = RGB( 0x00, 0x80, 0x00 );
	numberColor     = RGB( 0x00, 0x00, 0x80 );
	stringColor     = RGB( 0x80, 0x00, 0x80 );
	operatorColor   = RGB( 0x00, 0x00, 0x00 );

	errorBgColor = RGB( 0xFF, 0x80, 0x80 );

	identifierStyle = RDOFS_NONE;
	keywordStyle    = RDOFS_BOLD;
	functionsStyle  = RDOFS_BOLD;
	traceStyle      = RDOFS_BOLD;
	commentStyle    = RDOFS_ITALIC;
	numberStyle     = RDOFS_NONE;
	stringStyle     = RDOFS_NONE;
	operatorStyle   = RDOFS_NONE;
}

RDOEditorEditTheme::~RDOEditorEditTheme()
{
}

RDOEditorEditTheme& RDOEditorEditTheme::operator =( const RDOEditorEditTheme& theme )
{
	identifierColor = theme.identifierColor;
	keywordColor    = theme.keywordColor;
	functionsColor  = theme.functionsColor;
	traceColor      = theme.traceColor;
	commentColor    = theme.commentColor;
	numberColor     = theme.numberColor;
	stringColor     = theme.stringColor;
	operatorColor   = theme.operatorColor;

	errorBgColor = theme.errorBgColor;

	identifierStyle = theme.identifierStyle;
	keywordStyle    = theme.keywordStyle;
	functionsStyle  = theme.functionsStyle;
	traceStyle      = theme.traceStyle;
	commentStyle    = theme.commentStyle;
	numberStyle     = theme.numberStyle;
	stringStyle     = theme.stringStyle;
	operatorStyle   = theme.operatorStyle;

	return *this;
}

bool RDOEditorEditTheme::operator ==( const RDOEditorEditTheme& theme ) const
{
	return identifierColor == theme.identifierColor &&
	       keywordColor    == theme.keywordColor &&
	       functionsColor  == theme.functionsColor &&
	       traceColor      == theme.traceColor &&
	       commentColor    == theme.commentColor &&
	       numberColor     == theme.numberColor &&
	       stringColor     == theme.stringColor &&
	       operatorColor   == theme.operatorColor &&

	       errorBgColor == theme.errorBgColor &&

	       identifierStyle == theme.identifierStyle &&
	       keywordStyle    == theme.keywordStyle &&
	       functionsStyle  == theme.functionsStyle &&
	       traceStyle      == theme.traceStyle &&
	       commentStyle    == theme.commentStyle &&
	       numberStyle     == theme.numberStyle &&
	       stringStyle     == theme.stringStyle &&
	       operatorStyle   == theme.operatorStyle;
}

bool RDOEditorEditTheme::operator !=( const RDOEditorEditTheme& theme ) const
{
	return !(*this == theme);
}

void RDOEditorEditTheme::load( string regPath )
{
	RDOBaseEditTheme::load( regPath );

	regPath += "theme";
	identifierColor        = AfxGetApp()->GetProfileInt( regPath.c_str(), "identifierColor", identifierColor );
	keywordColor           = AfxGetApp()->GetProfileInt( regPath.c_str(), "keywordColor", keywordColor );
	functionsColor         = AfxGetApp()->GetProfileInt( regPath.c_str(), "functionsColor", functionsColor );
	traceColor             = AfxGetApp()->GetProfileInt( regPath.c_str(), "traceColor", traceColor );
	commentColor           = AfxGetApp()->GetProfileInt( regPath.c_str(), "commentColor", commentColor );
	numberColor            = AfxGetApp()->GetProfileInt( regPath.c_str(), "numberColor", numberColor );
	stringColor            = AfxGetApp()->GetProfileInt( regPath.c_str(), "stringColor", stringColor );
	operatorColor          = AfxGetApp()->GetProfileInt( regPath.c_str(), "operatorColor", operatorColor );
	errorBgColor           = AfxGetApp()->GetProfileInt( regPath.c_str(), "errorBgColor", errorBgColor );
	identifierStyle        = static_cast<RDOFontStyle>(AfxGetApp()->GetProfileInt( regPath.c_str(), "identifierStyle", identifierStyle ));
	keywordStyle           = static_cast<RDOFontStyle>(AfxGetApp()->GetProfileInt( regPath.c_str(), "keywordStyle", keywordStyle ));
	functionsStyle         = static_cast<RDOFontStyle>(AfxGetApp()->GetProfileInt( regPath.c_str(), "functionsStyle", functionsStyle ));
	traceStyle             = static_cast<RDOFontStyle>(AfxGetApp()->GetProfileInt( regPath.c_str(), "traceStyle", traceStyle ));
	commentStyle           = static_cast<RDOFontStyle>(AfxGetApp()->GetProfileInt( regPath.c_str(), "commentStyle", commentStyle ));
	numberStyle            = static_cast<RDOFontStyle>(AfxGetApp()->GetProfileInt( regPath.c_str(), "numberStyle", numberStyle ));
	stringStyle            = static_cast<RDOFontStyle>(AfxGetApp()->GetProfileInt( regPath.c_str(), "stringStyle", stringStyle ));
	operatorStyle          = static_cast<RDOFontStyle>(AfxGetApp()->GetProfileInt( regPath.c_str(), "operatorStyle", operatorStyle ));
}

void RDOEditorEditTheme::save( string regPath ) const
{
	RDOBaseEditTheme::save( regPath );

	regPath += "theme";
	AfxGetApp()->WriteProfileInt( regPath.c_str(), "identifierColor", identifierColor );
	AfxGetApp()->WriteProfileInt( regPath.c_str(), "keywordColor", keywordColor );
	AfxGetApp()->WriteProfileInt( regPath.c_str(), "functionsColor", functionsColor );
	AfxGetApp()->WriteProfileInt( regPath.c_str(), "traceColor", traceColor );
	AfxGetApp()->WriteProfileInt( regPath.c_str(), "commentColor", commentColor );
	AfxGetApp()->WriteProfileInt( regPath.c_str(), "numberColor", numberColor );
	AfxGetApp()->WriteProfileInt( regPath.c_str(), "stringColor", stringColor );
	AfxGetApp()->WriteProfileInt( regPath.c_str(), "operatorColor", operatorColor );
	AfxGetApp()->WriteProfileInt( regPath.c_str(), "errorBgColor", errorBgColor );
	AfxGetApp()->WriteProfileInt( regPath.c_str(), "identifierStyle", identifierStyle );
	AfxGetApp()->WriteProfileInt( regPath.c_str(), "keywordStyle", keywordStyle );
	AfxGetApp()->WriteProfileInt( regPath.c_str(), "functionsStyle", functionsStyle );
	AfxGetApp()->WriteProfileInt( regPath.c_str(), "traceStyle", traceStyle );
	AfxGetApp()->WriteProfileInt( regPath.c_str(), "commentStyle", commentStyle );
	AfxGetApp()->WriteProfileInt( regPath.c_str(), "numberStyle", numberStyle );
	AfxGetApp()->WriteProfileInt( regPath.c_str(), "stringStyle", stringStyle );
	AfxGetApp()->WriteProfileInt( regPath.c_str(), "operatorStyle", operatorStyle );
}

bool RDOEditorEditTheme::styleDefault( const int styleType ) const
{
	return styleType == SCE_RDO_DEFAULT;
}

bool RDOEditorEditTheme::styleUsing( const int styleType ) const
{
	return styleType == SCE_RDO_DEFAULT || styleType == SCE_RDO_IDENTIFIER ||
	       styleType == SCE_RDO_KEYWORD || styleType == SCE_RDO_FUNCTION ||
	       styleType == SCE_RDO_TRACE   || styleType == SCE_RDO_COMMENT ||
	       styleType == SCE_RDO_NUMBER  || styleType == SCE_RDO_STRING ||
	       styleType == SCE_RDO_OPERATOR;
}

bool RDOEditorEditTheme::styleBold( const int styleType ) const
{
	switch ( styleType ) {
		case SCE_RDO_DEFAULT   : return defaultStyle    & RDOFS_BOLD ? true : false;
		case SCE_RDO_IDENTIFIER: return identifierStyle & RDOFS_BOLD ? true : false;
		case SCE_RDO_KEYWORD   : return keywordStyle    & RDOFS_BOLD ? true : false;
		case SCE_RDO_FUNCTION  : return functionsStyle  & RDOFS_BOLD ? true : false;
		case SCE_RDO_TRACE     : return traceStyle      & RDOFS_BOLD ? true : false;
		case SCE_RDO_COMMENT   : return commentStyle    & RDOFS_BOLD ? true : false;
		case SCE_RDO_NUMBER    : return numberStyle     & RDOFS_BOLD ? true : false;
		case SCE_RDO_STRING    : return stringStyle     & RDOFS_BOLD ? true : false;
		case SCE_RDO_OPERATOR  : return operatorStyle   & RDOFS_BOLD ? true : false;
	}
	return false;
}

bool RDOEditorEditTheme::styleItalic( const int styleType ) const
{
	switch ( styleType ) {
		case SCE_RDO_DEFAULT   : return defaultStyle    & RDOFS_ITALIC ? true : false;
		case SCE_RDO_IDENTIFIER: return identifierStyle & RDOFS_ITALIC ? true : false;
		case SCE_RDO_KEYWORD   : return keywordStyle    & RDOFS_ITALIC ? true : false;
		case SCE_RDO_FUNCTION  : return functionsStyle  & RDOFS_ITALIC ? true : false;
		case SCE_RDO_TRACE     : return traceStyle      & RDOFS_ITALIC ? true : false;
		case SCE_RDO_COMMENT   : return commentStyle    & RDOFS_ITALIC ? true : false;
		case SCE_RDO_NUMBER    : return numberStyle     & RDOFS_ITALIC ? true : false;
		case SCE_RDO_STRING    : return stringStyle     & RDOFS_ITALIC ? true : false;
		case SCE_RDO_OPERATOR  : return operatorStyle   & RDOFS_ITALIC ? true : false;
	}
	return false;
}

string RDOEditorEditTheme::styleFGColorToHEX( const int styleType ) const
{
	switch ( styleType ) {
		case SCE_RDO_DEFAULT   : return colorToHEX( defaultColor );
		case SCE_RDO_IDENTIFIER: return colorToHEX( identifierColor );
		case SCE_RDO_KEYWORD   : return colorToHEX( keywordColor );
		case SCE_RDO_FUNCTION  : return colorToHEX( functionsColor );
		case SCE_RDO_TRACE     : return colorToHEX( traceColor );
		case SCE_RDO_COMMENT   : return colorToHEX( commentColor );
		case SCE_RDO_NUMBER    : return colorToHEX( numberColor );
		case SCE_RDO_STRING    : return colorToHEX( stringColor );
		case SCE_RDO_OPERATOR  : return colorToHEX( operatorColor );
	}
	return RDOBaseEditTheme::styleFGColorToHEX( styleType );
}

RDOEditorEditTheme RDOEditorEditTheme::getDefaultTheme()
{
	RDOEditorEditTheme theme;
	return theme;
}

RDOEditorEditTheme RDOEditorEditTheme::getCppTheme()
{
	RDOEditorEditTheme theme;

	theme.defaultColor    = RGB( 0x00, 0x00, 0x00 );
	theme.identifierColor = RGB( 0x00, 0x00, 0x00 );
	theme.keywordColor    = RGB( 0x00, 0x00, 0xFF );
	theme.functionsColor  = RGB( 0x00, 0x00, 0xFF );
	theme.traceColor      = RGB( 0x00, 0x00, 0xFF );
	theme.commentColor    = RGB( 0x00, 0x80, 0x00 );
	theme.numberColor     = RGB( 0x00, 0x00, 0x00 );
	theme.stringColor     = RGB( 0x00, 0x00, 0x00 );
	theme.operatorColor   = RGB( 0x00, 0x00, 0x00 );
	theme.backgroundColor = RGB( 0xFF, 0xFF, 0xFF );

	theme.caretColor       = RGB( 0x00, 0x00, 0x00 );
	theme.selectionBgColor = RGB( 0xC0, 0xC0, 0xC0 );
	theme.bookmarkFgColor  = RGB( 0x00, 0x00, 0x00 );
	theme.bookmarkBgColor  = RGB( 0x00, 0xFF, 0xFF );
	theme.foldFgColor      = RGB( 0xFF, 0xFF, 0xFF );
	theme.foldBgColor      = RGB( 0x00, 0x00, 0x00 );
	theme.errorBgColor     = RGB( 0xFF, 0x80, 0x80 );

	theme.defaultStyle    = RDOFS_NONE;
	theme.identifierStyle = RDOFS_NONE;
	theme.keywordStyle    = RDOFS_NONE;
	theme.functionsStyle  = RDOFS_NONE;
	theme.traceStyle      = RDOFS_NONE;
	theme.commentStyle    = RDOFS_NONE;
	theme.numberStyle     = RDOFS_NONE;
	theme.stringStyle     = RDOFS_NONE;
	theme.operatorStyle   = RDOFS_NONE;

	theme.bookmarkStyle = RDOBOOKMARKS_CIRCLE;
	theme.foldStyle     = RDOFOLDS_PLUS;

	return theme;
}

RDOEditorEditTheme RDOEditorEditTheme::getPascalTheme()
{
	RDOEditorEditTheme theme;

	theme.defaultColor    = RGB( 0x00, 0x00, 0x00 );
	theme.identifierColor = RGB( 0x00, 0x00, 0x00 );
	theme.keywordColor    = RGB( 0x00, 0x00, 0x00 );
	theme.functionsColor  = RGB( 0x00, 0x00, 0x00 );
	theme.traceColor      = RGB( 0x00, 0x00, 0x00 );
	theme.commentColor    = RGB( 0x00, 0x00, 0x80 );
	theme.numberColor     = RGB( 0x00, 0x00, 0x00 );
	theme.stringColor     = RGB( 0x00, 0x00, 0x00 );
	theme.operatorColor   = RGB( 0x00, 0x00, 0x00 );
	theme.backgroundColor = RGB( 0xFF, 0xFF, 0xFF );

	theme.caretColor       = RGB( 0x00, 0x00, 0x00 );
	theme.selectionBgColor = RGB( 0xC0, 0xC0, 0xC0 );
	theme.bookmarkFgColor  = RGB( 0x00, 0x00, 0x00 );
	theme.bookmarkBgColor  = RGB( 0x00, 0xFF, 0xFF );
	theme.foldFgColor      = RGB( 0xFF, 0xFF, 0xFF );
	theme.foldBgColor      = RGB( 0x00, 0x00, 0x00 );
	theme.errorBgColor     = RGB( 0xFF, 0x80, 0x80 );

	theme.defaultStyle    = RDOFS_NONE;
	theme.identifierStyle = RDOFS_NONE;
	theme.keywordStyle    = RDOFS_BOLD;
	theme.functionsStyle  = RDOFS_BOLD;
	theme.traceStyle      = RDOFS_BOLD;
	theme.commentStyle    = RDOFS_ITALIC;
	theme.numberStyle     = RDOFS_NONE;
	theme.stringStyle     = RDOFS_NONE;
	theme.operatorStyle   = RDOFS_NONE;

	theme.bookmarkStyle = RDOBOOKMARKS_CIRCLE;
	theme.foldStyle     = RDOFOLDS_PLUS;

	return theme;
}

RDOEditorEditTheme RDOEditorEditTheme::getHtmlTheme()
{
	RDOEditorEditTheme theme;

	theme.defaultColor    = RGB( 0x00, 0x00, 0x00 );
	theme.identifierColor = RGB( 0x00, 0x00, 0x00 );
	theme.keywordColor    = RGB( 0x80, 0x00, 0x80 );
	theme.functionsColor  = RGB( 0xFF, 0x00, 0x00 );
	theme.traceColor      = RGB( 0xFF, 0x00, 0x00 );
	theme.commentColor    = RGB( 0x00, 0x80, 0x00 );
	theme.numberColor     = RGB( 0x00, 0x00, 0xFF );
	theme.stringColor     = RGB( 0x00, 0x00, 0xFF );
	theme.operatorColor   = RGB( 0x00, 0x00, 0x00 );
	theme.backgroundColor = RGB( 0xFF, 0xFF, 0xFF );

	theme.caretColor       = RGB( 0x00, 0x00, 0x00 );
	theme.selectionBgColor = RGB( 0xC0, 0xC0, 0xC0 );
	theme.bookmarkFgColor  = RGB( 0x00, 0x00, 0x00 );
	theme.bookmarkBgColor  = RGB( 0x00, 0xFF, 0xFF );
	theme.foldFgColor      = RGB( 0xFF, 0xFF, 0xFF );
	theme.foldBgColor      = RGB( 0x00, 0x00, 0x00 );
	theme.errorBgColor     = RGB( 0xFF, 0x80, 0x80 );

	theme.defaultStyle    = RDOFS_NONE;
	theme.identifierStyle = RDOFS_NONE;
	theme.keywordStyle    = RDOFS_NONE;
	theme.functionsStyle  = RDOFS_NONE;
	theme.traceStyle      = RDOFS_NONE;
	theme.commentStyle    = RDOFS_NONE;
	theme.numberStyle     = RDOFS_NONE;
	theme.stringStyle     = RDOFS_NONE;
	theme.operatorStyle   = RDOFS_NONE;

	theme.bookmarkStyle = RDOBOOKMARKS_CIRCLE;
	theme.foldStyle     = RDOFOLDS_PLUS;

	return theme;
}

RDOEditorEditTheme RDOEditorEditTheme::getClassicTheme()
{
	RDOEditorEditTheme theme;

	theme.defaultColor    = RGB( 0xFF, 0xFF, 0x00 );
	theme.identifierColor = RGB( 0xFF, 0xFF, 0x00 );
	theme.keywordColor    = RGB( 0xFF, 0xFF, 0xFF );
	theme.functionsColor  = RGB( 0xFF, 0xFF, 0xFF );
	theme.traceColor      = RGB( 0xFF, 0xFF, 0xFF );
	theme.commentColor    = RGB( 0xC0, 0xC0, 0xC0 );
	theme.numberColor     = RGB( 0xFF, 0xFF, 0x00 );
	theme.stringColor     = RGB( 0xFF, 0xFF, 0x00 );
	theme.operatorColor   = RGB( 0xFF, 0xFF, 0x00 );
	theme.backgroundColor = RGB( 0x00, 0x00, 0x80 );

	theme.caretColor       = RGB( 0xFF, 0xFF, 0x00 );
	theme.selectionBgColor = RGB( 0x00, 0x00, 0x40 );
	theme.bookmarkFgColor  = RGB( 0x00, 0x00, 0x00 );
	theme.bookmarkBgColor  = RGB( 0x00, 0xFF, 0xFF );
	theme.foldFgColor      = RGB( 0xFF, 0xFF, 0xFF );
	theme.foldBgColor      = RGB( 0x00, 0x00, 0x00 );
	theme.errorBgColor     = RGB( 0xFF, 0x00, 0x80 );

	theme.defaultStyle    = RDOFS_NONE;
	theme.identifierStyle = RDOFS_NONE;
	theme.keywordStyle    = RDOFS_NONE;
	theme.functionsStyle  = RDOFS_NONE;
	theme.traceStyle      = RDOFS_NONE;
	theme.commentStyle    = RDOFS_NONE;
	theme.numberStyle     = RDOFS_NONE;
	theme.stringStyle     = RDOFS_NONE;
	theme.operatorStyle   = RDOFS_NONE;

	theme.bookmarkStyle = RDOBOOKMARKS_CIRCLE;
	theme.foldStyle     = RDOFOLDS_PLUS;

	return theme;
}

RDOEditorEditTheme RDOEditorEditTheme::getTwilightTheme()
{
	RDOEditorEditTheme theme;

	theme.defaultColor    = RGB( 0xFF, 0xFF, 0xFF );
	theme.identifierColor = RGB( 0xFF, 0xFF, 0xFF );
	theme.keywordColor    = RGB( 0x00, 0xFF, 0xFF );
	theme.functionsColor  = RGB( 0xFF, 0xFF, 0x00 );
	theme.traceColor      = RGB( 0x00, 0xFF, 0x00 );
	theme.commentColor    = RGB( 0xC0, 0xC0, 0xC0 );
	theme.numberColor     = RGB( 0xC0, 0xC0, 0xC0 );
	theme.stringColor     = RGB( 0xFF, 0xFF, 0xFF );
	theme.operatorColor   = RGB( 0xFF, 0xFF, 0xFF );
	theme.backgroundColor = RGB( 0x00, 0x00, 0x00 );

	theme.caretColor       = RGB( 0xFF, 0xFF, 0xFF );
	theme.selectionBgColor = RGB( 0x70, 0x70, 0x70 );
	theme.bookmarkFgColor  = RGB( 0x00, 0x00, 0x00 );
	theme.bookmarkBgColor  = RGB( 0x00, 0xFF, 0xFF );
	theme.foldFgColor      = RGB( 0xFF, 0xFF, 0xFF );
	theme.foldBgColor      = RGB( 0x00, 0x00, 0x00 );
	theme.errorBgColor     = RGB( 0xFF, 0x80, 0x80 );

	theme.defaultStyle    = RDOFS_NONE;
	theme.identifierStyle = RDOFS_NONE;
	theme.keywordStyle    = RDOFS_BOLD;
	theme.functionsStyle  = RDOFS_BOLD;
	theme.traceStyle      = static_cast<RDOFontStyle>(RDOFS_BOLD | RDOFS_ITALIC);
	theme.commentStyle    = RDOFS_ITALIC;
	theme.numberStyle     = RDOFS_NONE;
	theme.stringStyle     = RDOFS_NONE;
	theme.operatorStyle   = RDOFS_NONE;

	theme.bookmarkStyle = RDOBOOKMARKS_CIRCLE;
	theme.foldStyle     = RDOFOLDS_PLUS;

	return theme;
}

RDOEditorEditTheme RDOEditorEditTheme::getOceanTheme()
{
	RDOEditorEditTheme theme;

	theme.defaultColor    = RGB( 0x00, 0x00, 0xFF );
	theme.identifierColor = RGB( 0x00, 0x00, 0xFF );
	theme.keywordColor    = RGB( 0x00, 0x00, 0x00 );
	theme.functionsColor  = RGB( 0x00, 0x00, 0x00 );
	theme.traceColor      = RGB( 0x00, 0x00, 0x00 );
	theme.commentColor    = RGB( 0x00, 0x80, 0x80 );
	theme.numberColor     = RGB( 0x00, 0x00, 0xFF );
	theme.stringColor     = RGB( 0x00, 0x00, 0xFF );
	theme.operatorColor   = RGB( 0x00, 0x00, 0xFF );
	theme.backgroundColor = RGB( 0x00, 0xFF, 0xFF );

	theme.caretColor       = RGB( 0x00, 0x00, 0x00 );
	theme.selectionBgColor = RGB( 0xC0, 0xC0, 0xD0 );
	theme.bookmarkFgColor  = RGB( 0x00, 0x00, 0x00 );
	theme.bookmarkBgColor  = RGB( 0x00, 0xFF, 0xFF );
	theme.foldFgColor      = RGB( 0xFF, 0xFF, 0xFF );
	theme.foldBgColor      = RGB( 0x00, 0x00, 0x00 );
	theme.errorBgColor     = RGB( 0xFF, 0xFF, 0x00 );

	theme.defaultStyle    = RDOFS_NONE;
	theme.identifierStyle = RDOFS_NONE;
	theme.keywordStyle    = RDOFS_BOLD;
	theme.functionsStyle  = RDOFS_BOLD;
	theme.traceStyle      = RDOFS_BOLD;
	theme.commentStyle    = RDOFS_ITALIC;
	theme.numberStyle     = RDOFS_NONE;
	theme.stringStyle     = RDOFS_NONE;
	theme.operatorStyle   = RDOFS_NONE;

	theme.bookmarkStyle = RDOBOOKMARKS_CIRCLE;
	theme.foldStyle     = RDOFOLDS_PLUS;

	return theme;
}

// ----------------------------------------------------------------------------
// ---------- RDOEditorEditAutoComplete
// ----------------------------------------------------------------------------
RDOEditorEditAutoComplete::RDOEditorEditAutoComplete()
{
	useAutoComplete = true;
	showFullList    = true;
}

RDOEditorEditAutoComplete::~RDOEditorEditAutoComplete()
{
}

RDOEditorEditAutoComplete& RDOEditorEditAutoComplete::operator =( const RDOEditorEditAutoComplete& autoComplete )
{
	useAutoComplete = autoComplete.useAutoComplete;
	showFullList    = autoComplete.showFullList;

	return *this;
}

bool RDOEditorEditAutoComplete::operator ==( const RDOEditorEditAutoComplete& autoComplete ) const
{
	return useAutoComplete == autoComplete.useAutoComplete &&
	       showFullList    == autoComplete.showFullList;
}

bool RDOEditorEditAutoComplete::operator !=( const RDOEditorEditAutoComplete& autoComplete ) const
{
	return !(*this == autoComplete);
}

void RDOEditorEditAutoComplete::load( string regPath )
{
	regPath += "autoComplete";
	useAutoComplete = AfxGetApp()->GetProfileInt( regPath.c_str(), "useAutoComplete", useAutoComplete ) ? true : false;
	showFullList    = AfxGetApp()->GetProfileInt( regPath.c_str(), "showFullList", showFullList ) ? true : false;
}

void RDOEditorEditAutoComplete::save( string regPath ) const
{
	regPath += "autoComplete";
	AfxGetApp()->WriteProfileInt( regPath.c_str(), "useAutoComplete", useAutoComplete );
	AfxGetApp()->WriteProfileInt( regPath.c_str(), "showFullList", showFullList );
}

// ----------------------------------------------------------------------------
// ---------- RDOEditorEditBuffer
// ----------------------------------------------------------------------------
RDOEditorEditBuffer::RDOEditorEditBuffer()
{
	canClearBuffer   = true;
	clearBufferDelay = 7;
}

RDOEditorEditBuffer::~RDOEditorEditBuffer()
{
}

RDOEditorEditBuffer& RDOEditorEditBuffer::operator =( const RDOEditorEditBuffer& buffer )
{
	canClearBuffer   = buffer.canClearBuffer;
	clearBufferDelay = buffer.clearBufferDelay;

	return *this;
}

bool RDOEditorEditBuffer::operator ==( const RDOEditorEditBuffer& buffer ) const
{
	return canClearBuffer   == buffer.canClearBuffer &&
	       clearBufferDelay == buffer.clearBufferDelay;
}

bool RDOEditorEditBuffer::operator !=( const RDOEditorEditBuffer& buffer ) const
{
	return !(*this == buffer);
}

void RDOEditorEditBuffer::load( std::string regPath )
{
	regPath += "buffer";
	canClearBuffer   = AfxGetApp()->GetProfileInt( regPath.c_str(), "canClearBuffer", canClearBuffer ) ? true : false;
	clearBufferDelay = AfxGetApp()->GetProfileInt( regPath.c_str(), "clearBufferDelay", clearBufferDelay );
}

void RDOEditorEditBuffer::save( std::string regPath ) const
{
	regPath += "buffer";
	AfxGetApp()->WriteProfileInt( regPath.c_str(), "canClearBuffer", canClearBuffer );
	AfxGetApp()->WriteProfileInt( regPath.c_str(), "clearBufferDelay", clearBufferDelay );
}

// ----------------------------------------------------------------------------
// ---------- RDOEditorBaseStyle
// ----------------------------------------------------------------------------
RDOEditorBaseStyle::RDOEditorBaseStyle(): RDOBaseEditStyle()
{
}

RDOEditorBaseStyle::~RDOEditorBaseStyle()
{
}

void RDOEditorBaseStyle::initTheme()
{
	theme = new RDOEditorEditTheme;
}

RDOEditorBaseStyle& RDOEditorBaseStyle::operator =( const RDOEditorBaseStyle& style )
{
	RDOBaseEditStyle::operator=( style );
	if ( theme && style.theme ) *static_cast<RDOEditorEditTheme*>(theme) = *static_cast<RDOEditorEditTheme*>(style.theme);

	return *this;
}

bool RDOEditorBaseStyle::operator ==( const RDOEditorBaseStyle& style ) const
{
	bool flag = RDOBaseEditStyle::operator==( style );
	if ( theme && style.theme && flag ) flag &= *static_cast<RDOEditorEditTheme*>(theme) == *static_cast<RDOEditorEditTheme*>(style.theme);
	return flag;
}

bool RDOEditorBaseStyle::operator !=( const RDOEditorBaseStyle& style ) const
{
	return !(*this == style);
}

// ----------------------------------------------------------------------------
// ---------- RDOEditorResultsStyle
// ----------------------------------------------------------------------------
RDOEditorResultsStyle::RDOEditorResultsStyle(): RDOEditorBaseStyle()
{
}

RDOEditorResultsStyle::~RDOEditorResultsStyle()
{
}

// ----------------------------------------------------------------------------
// ---------- RDOEditorEditStyle
// ----------------------------------------------------------------------------
RDOEditorEditStyle::RDOEditorEditStyle():
	RDOEditorBaseStyle(),
	autoComplete( NULL ),
	buffer( NULL )
{
}

RDOEditorEditStyle::~RDOEditorEditStyle()
{
	if ( autoComplete ) { delete autoComplete; autoComplete = NULL; };
	if ( buffer )       { delete buffer;       buffer = NULL; };
}

void RDOEditorEditStyle::initAutoComplete()
{
	autoComplete = new RDOEditorEditAutoComplete;
}

void RDOEditorEditStyle::initBuffer()
{
	buffer = new RDOEditorEditBuffer;
}

RDOEditorEditStyle& RDOEditorEditStyle::operator =( const RDOEditorEditStyle& style )
{
	RDOEditorBaseStyle::operator=( style );
	if ( autoComplete && style.autoComplete ) *autoComplete = *style.autoComplete;
	if ( buffer       && style.buffer )       *buffer       = *style.buffer;

	return *this;
}

bool RDOEditorEditStyle::operator ==( const RDOEditorEditStyle& style ) const
{
	bool flag = RDOEditorBaseStyle::operator==( style );
	if ( autoComplete && style.autoComplete && flag ) flag &= *autoComplete == *style.autoComplete;
	if ( buffer       && style.buffer       && flag ) flag &= *buffer       == *style.buffer;
	return flag;
}

bool RDOEditorEditStyle::operator !=( const RDOEditorEditStyle& style ) const
{
	return !(*this == style);
}

void RDOEditorEditStyle::init( const string& _regPath )
{
	RDOEditorBaseStyle::init( _regPath );
	initAutoComplete();
	initBuffer();
}

void RDOEditorEditStyle::load()
{
	RDOEditorBaseStyle::load();
	if ( autoComplete ) autoComplete->load( regPath );
	if ( buffer )       buffer->load( regPath );
}

void RDOEditorEditStyle::save() const
{
	RDOEditorBaseStyle::save();
	if ( autoComplete ) autoComplete->save(  regPath );
	if ( buffer )       buffer->save(  regPath );
}
