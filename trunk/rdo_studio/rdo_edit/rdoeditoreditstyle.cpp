#include "stdafx.h"
#include "rdoeditoreditstyle.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

using namespace std;
using namespace rdoEditor;
using namespace rdoEditCtrl;

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

RDOEditorEditTheme& RDOEditorEditTheme::operator =( const RDOEditorBaseEditTheme& theme )
{
	RDOEditorBaseEditTheme::operator=( theme );
	return *this;
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

RDOEditorEditTheme RDOEditorEditTheme::getDefaultTheme()
{
	RDOEditorEditTheme theme;
	return theme;
}

RDOEditorEditTheme RDOEditorEditTheme::getCppTheme()
{
	RDOEditorEditTheme theme;
	theme = RDOEditorBaseEditTheme::getCppTheme();

	theme.foldFgColor  = RGB( 0xFF, 0xFF, 0xFF );
	theme.foldBgColor  = RGB( 0x00, 0x00, 0x00 );
	theme.errorBgColor = RGB( 0xFF, 0x80, 0x80 );

	theme.foldStyle = RDOFOLDS_PLUS;

	return theme;
}

RDOEditorEditTheme RDOEditorEditTheme::getPascalTheme()
{
	RDOEditorEditTheme theme;
	theme = RDOEditorBaseEditTheme::getPascalTheme();

	theme.foldFgColor  = RGB( 0xFF, 0xFF, 0xFF );
	theme.foldBgColor  = RGB( 0x00, 0x00, 0x00 );
	theme.errorBgColor = RGB( 0xFF, 0x80, 0x80 );

	theme.foldStyle = RDOFOLDS_PLUS;

	return theme;
}

RDOEditorEditTheme RDOEditorEditTheme::getHtmlTheme()
{
	RDOEditorEditTheme theme;
	theme = RDOEditorBaseEditTheme::getHtmlTheme();

	theme.foldFgColor  = RGB( 0xFF, 0xFF, 0xFF );
	theme.foldBgColor  = RGB( 0x00, 0x00, 0x00 );
	theme.errorBgColor = RGB( 0xFF, 0x80, 0x80 );

	theme.foldStyle = RDOFOLDS_PLUS;

	return theme;
}

RDOEditorEditTheme RDOEditorEditTheme::getClassicTheme()
{
	RDOEditorEditTheme theme;
	theme = RDOEditorBaseEditTheme::getClassicTheme();

	theme.foldFgColor  = RGB( 0xFF, 0xFF, 0xFF );
	theme.foldBgColor  = RGB( 0x00, 0x00, 0x00 );
	theme.errorBgColor = RGB( 0xFF, 0x00, 0x80 );

	theme.foldStyle = RDOFOLDS_PLUS;

	return theme;
}

RDOEditorEditTheme RDOEditorEditTheme::getTwilightTheme()
{
	RDOEditorEditTheme theme;
	theme = RDOEditorBaseEditTheme::getTwilightTheme();

	theme.foldFgColor  = RGB( 0xFF, 0xFF, 0xFF );
	theme.foldBgColor  = RGB( 0x00, 0x00, 0x00 );
	theme.errorBgColor = RGB( 0xFF, 0x80, 0x80 );

	theme.foldStyle = RDOFOLDS_PLUS;

	return theme;
}

RDOEditorEditTheme RDOEditorEditTheme::getOceanTheme()
{
	RDOEditorEditTheme theme;
	theme = RDOEditorBaseEditTheme::getOceanTheme();

	theme.foldFgColor  = RGB( 0xFF, 0xFF, 0xFF );
	theme.foldBgColor  = RGB( 0x00, 0x00, 0x00 );
	theme.errorBgColor = RGB( 0xFF, 0xFF, 0x00 );

	theme.foldStyle = RDOFOLDS_PLUS;

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
