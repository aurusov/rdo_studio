#include "stdafx.h"
#include "rdoeditoreditstyle.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

using namespace std;
using namespace rdoEditor;

// ----------------------------------------------------------------------------
// ---------- RDOEditorEditTheme
// ----------------------------------------------------------------------------
RDOEditorEditTheme::RDOEditorEditTheme(): RDOEditorBaseEditTheme()
{
	foldFgColor = RGB( 0xFF, 0xFF, 0xFF );
	foldBgColor = RGB( 0x00, 0x00, 0x00 );

	errorBgColor = RGB( 0xFF, 0x80, 0x80 );

	foldStyle = RDOFOLDS_PLUS;
}

RDOEditorEditTheme::~RDOEditorEditTheme()
{
}

RDOEditorEditTheme& RDOEditorEditTheme::operator =( const RDOEditorEditTheme& theme )
{
	RDOEditorBaseEditTheme::operator=( theme );

	foldFgColor = theme.foldFgColor;
	foldBgColor = theme.foldBgColor;

	errorBgColor = theme.errorBgColor;

	foldStyle = theme.foldStyle;

	return *this;
}

bool RDOEditorEditTheme::operator ==( const RDOEditorEditTheme& theme ) const
{
	bool flag = RDOEditorBaseEditTheme::operator==( theme );

	if ( flag ) flag &= foldFgColor == theme.foldFgColor &&
	                    foldBgColor == theme.foldBgColor &&

	                    errorBgColor == theme.errorBgColor &&

	                    foldStyle == theme.foldStyle;
	return flag;
}

bool RDOEditorEditTheme::operator !=( const RDOEditorEditTheme& theme ) const
{
	return !(*this == theme);
}

void RDOEditorEditTheme::load( string regPath )
{
	RDOEditorBaseEditTheme::load( regPath );

	regPath += "theme";
	foldFgColor  = AfxGetApp()->GetProfileInt( regPath.c_str(), "foldFgColor", foldFgColor );
	foldBgColor  = AfxGetApp()->GetProfileInt( regPath.c_str(), "foldBgColor", foldBgColor );
	errorBgColor = AfxGetApp()->GetProfileInt( regPath.c_str(), "errorBgColor", errorBgColor );
	foldStyle    = (RDOFoldStyle)AfxGetApp()->GetProfileInt( regPath.c_str(), "foldStyle", foldStyle );
}

void RDOEditorEditTheme::save( string regPath ) const
{
	RDOEditorBaseEditTheme::save( regPath );

	regPath += "theme";
	AfxGetApp()->WriteProfileInt( regPath.c_str(), "foldFgColor", foldFgColor );
	AfxGetApp()->WriteProfileInt( regPath.c_str(), "foldBgColor", foldBgColor );
	AfxGetApp()->WriteProfileInt( regPath.c_str(), "errorBgColor", errorBgColor );
	AfxGetApp()->WriteProfileInt( regPath.c_str(), "foldStyle", foldStyle );
}

/*
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
*/
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
// ---------- RDOEditorEditMargin
// ----------------------------------------------------------------------------
RDOEditorEditMargin::RDOEditorEditMargin()
{
	fold       = true;
	bookmark   = true;
	lineNumber = false;
}

RDOEditorEditMargin::~RDOEditorEditMargin()
{
}

RDOEditorEditMargin& RDOEditorEditMargin::operator =( const RDOEditorEditMargin& margin )
{
	fold       = margin.fold;
	bookmark   = margin.bookmark;
	lineNumber = margin.lineNumber;

	return *this;
}

bool RDOEditorEditMargin::operator ==( const RDOEditorEditMargin& margin ) const
{
	return fold       == margin.fold &&
	       bookmark   == margin.bookmark &&
	       lineNumber == margin.lineNumber;
}

bool RDOEditorEditMargin::operator !=( const RDOEditorEditMargin& margin ) const
{
	return !(*this == margin);
}

void RDOEditorEditMargin::load( string regPath )
{
	regPath += "margin";
	fold       = AfxGetApp()->GetProfileInt( regPath.c_str(), "fold", fold ) ? true : false;
	bookmark   = AfxGetApp()->GetProfileInt( regPath.c_str(), "bookmark", bookmark ) ? true : false;
	lineNumber = AfxGetApp()->GetProfileInt( regPath.c_str(), "lineNumber", lineNumber ) ? true : false;
}

void RDOEditorEditMargin::save( string regPath ) const
{
	regPath += "margin";
	AfxGetApp()->WriteProfileInt( regPath.c_str(), "fold", fold );
	AfxGetApp()->WriteProfileInt( regPath.c_str(), "bookmark", bookmark );
	AfxGetApp()->WriteProfileInt( regPath.c_str(), "lineNumber", lineNumber );
}

// ----------------------------------------------------------------------------
// ---------- RDOEditorEditStyle
// ----------------------------------------------------------------------------
RDOEditorEditStyle::RDOEditorEditStyle():
	RDOEditorBaseEditStyle(),
	autoComplete( NULL ),
	buffer( NULL ),
	margin( NULL )
{
}

RDOEditorEditStyle::~RDOEditorEditStyle()
{
	if ( autoComplete ) { delete autoComplete; autoComplete = NULL; };
	if ( buffer )       { delete buffer;       buffer = NULL; };
	if ( margin )       { delete margin;       margin = NULL; };
}

void RDOEditorEditStyle::initTheme()
{
	theme = new RDOEditorEditTheme;
}

void RDOEditorEditStyle::initAutoComplete()
{
	autoComplete = new RDOEditorEditAutoComplete;
}

void RDOEditorEditStyle::initBuffer()
{
	buffer = new RDOEditorEditBuffer;
}

void RDOEditorEditStyle::initMargin()
{
	margin = new RDOEditorEditMargin;
}

RDOEditorEditStyle& RDOEditorEditStyle::operator =( const RDOEditorEditStyle& style )
{
	RDOEditorBaseEditStyle::operator=( style );
	if ( theme        && style.theme )        *static_cast<RDOEditorEditTheme*>(theme) = *static_cast<RDOEditorEditTheme*>(style.theme);
	if ( autoComplete && style.autoComplete ) *autoComplete = *style.autoComplete;
	if ( buffer       && style.buffer )       *buffer       = *style.buffer;
	if ( margin       && style.margin )       *margin       = *style.margin;

	return *this;
}

bool RDOEditorEditStyle::operator ==( const RDOEditorEditStyle& style ) const
{
	bool flag = RDOEditorBaseEditStyle::operator==( style );
	if ( theme        && style.theme        && flag ) flag &= *static_cast<RDOEditorEditTheme*>(theme) == *static_cast<RDOEditorEditTheme*>(style.theme);
	if ( autoComplete && style.autoComplete && flag ) flag &= *autoComplete == *style.autoComplete;
	if ( buffer       && style.buffer       && flag ) flag &= *buffer       == *style.buffer;
	if ( margin       && style.margin       && flag ) flag &= *margin       == *style.margin;
	return flag;
}

bool RDOEditorEditStyle::operator !=( const RDOEditorEditStyle& style ) const
{
	return !(*this == style);
}

void RDOEditorEditStyle::init( const string& _regPath )
{
	RDOEditorBaseEditStyle::init( _regPath );
	initAutoComplete();
	initBuffer();
	initMargin();
}

bool RDOEditorEditStyle::load()
{
	if ( RDOEditorBaseEditStyle::load() ) {
		if ( autoComplete ) autoComplete->load( regPath );
		if ( buffer )       buffer->load( regPath );
		if ( margin )       margin->load( regPath );
		return true;
	}
	return false;
}

bool RDOEditorEditStyle::save() const
{
	if ( RDOEditorBaseEditStyle::save() ) {
		if ( autoComplete ) autoComplete->save(  regPath );
		if ( buffer )       buffer->save(  regPath );
		if ( margin )       margin->save(  regPath );
		return true;
	}
	return false;
}
