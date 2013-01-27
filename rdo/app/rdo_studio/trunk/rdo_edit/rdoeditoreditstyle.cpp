/*!
  \copyright (c) RDO-Team, 2003-2012
  \file      rdoeditoreditstyle.cpp
  \author    Урусов Андрей (rdo@rk9.bmstu.ru)
  \date      20.02.2003
  \brief     
  \indent    4T
*/

// ---------------------------------------------------------------------------- PCH
#include "app/rdo_studio_mfc/pch/stdpch.h"
// ----------------------------------------------------------------------- INCLUDES
// ----------------------------------------------------------------------- SYNOPSIS
#include "app/rdo_studio_mfc/rdo_edit/rdoeditoreditstyle.h"
// --------------------------------------------------------------------------------

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

using namespace rdoEditor;
using namespace rdoEditCtrl;

// --------------------------------------------------------------------------------
// -------------------- RDOEditorEditTheme
// --------------------------------------------------------------------------------
RDOEditorEditTheme::RDOEditorEditTheme(): RDOEditorBaseEditTheme()
{
	foldFgColor = RGB( 0xFF, 0xFF, 0xFF );
	foldBgColor = RGB( 0x00, 0x00, 0x00 );

	errorBgColor = RGB( 0xFF, 0x80, 0x80 );

	foldStyle   = RDOFOLDS_PLUS;
	commentFold = false;
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

	foldStyle   = theme.foldStyle;
	commentFold = theme.commentFold;

	return *this;
}

rbool RDOEditorEditTheme::operator ==( const RDOEditorEditTheme& theme ) const
{
	rbool flag = RDOEditorBaseEditTheme::operator==( theme );

	if ( flag ) flag &= foldFgColor == theme.foldFgColor &&
	                    foldBgColor == theme.foldBgColor &&

	                    errorBgColor == theme.errorBgColor &&

	                    foldStyle   == theme.foldStyle &&
	                    commentFold == theme.commentFold;
	return flag;
}

rbool RDOEditorEditTheme::operator !=( const RDOEditorEditTheme& theme ) const
{
	return !(*this == theme);
}

void RDOEditorEditTheme::load( tstring regPath )
{
	RDOEditorBaseEditTheme::load( regPath );

	regPath += "theme";
	foldFgColor  = AfxGetApp()->GetProfileInt( regPath.c_str(), "foldFgColor", foldFgColor );
	foldBgColor  = AfxGetApp()->GetProfileInt( regPath.c_str(), "foldBgColor", foldBgColor );
	errorBgColor = AfxGetApp()->GetProfileInt( regPath.c_str(), "errorBgColor", errorBgColor );
	foldStyle    = (RDOFoldStyle)AfxGetApp()->GetProfileInt( regPath.c_str(), "foldStyle", foldStyle );
	commentFold  = AfxGetApp()->GetProfileInt( regPath.c_str(), "commentFold", commentFold ) ? true : false;
}

void RDOEditorEditTheme::save( tstring regPath ) const
{
	RDOEditorBaseEditTheme::save( regPath );

	regPath += "theme";
	AfxGetApp()->WriteProfileInt( regPath.c_str(), "foldFgColor", foldFgColor );
	AfxGetApp()->WriteProfileInt( regPath.c_str(), "foldBgColor", foldBgColor );
	AfxGetApp()->WriteProfileInt( regPath.c_str(), "errorBgColor", errorBgColor );
	AfxGetApp()->WriteProfileInt( regPath.c_str(), "foldStyle", foldStyle );
	AfxGetApp()->WriteProfileInt( regPath.c_str(), "commentFold", commentFold );
}

RDOEditorEditTheme RDOEditorEditTheme::getDefaultTheme()
{
	RDOEditorEditTheme theme;
	return theme;
}

RDOEditorEditTheme RDOEditorEditTheme::getCppTheme()
{
	RDOEditorEditTheme theme;
	*static_cast<RDOEditorBaseEditTheme*>(&theme) = RDOEditorBaseEditTheme::getCppTheme();

	theme.foldFgColor  = RGB( 0xFF, 0xFF, 0xFF );
	theme.foldBgColor  = RGB( 0x00, 0x00, 0x00 );
	theme.errorBgColor = RGB( 0xFF, 0x80, 0x80 );

	theme.foldStyle   = RDOFOLDS_PLUS;
	theme.commentFold = false;

	return theme;
}

RDOEditorEditTheme RDOEditorEditTheme::getPascalTheme()
{
	RDOEditorEditTheme theme;
	*static_cast<RDOEditorBaseEditTheme*>(&theme) = RDOEditorBaseEditTheme::getPascalTheme();

	theme.foldFgColor  = RGB( 0xFF, 0xFF, 0xFF );
	theme.foldBgColor  = RGB( 0x00, 0x00, 0x00 );
	theme.errorBgColor = RGB( 0xFF, 0x80, 0x80 );

	theme.foldStyle   = RDOFOLDS_PLUS;
	theme.commentFold = false;

	return theme;
}

RDOEditorEditTheme RDOEditorEditTheme::getHtmlTheme()
{
	RDOEditorEditTheme theme;
	*static_cast<RDOEditorBaseEditTheme*>(&theme) = RDOEditorBaseEditTheme::getHtmlTheme();

	theme.foldFgColor  = RGB( 0xFF, 0xFF, 0xFF );
	theme.foldBgColor  = RGB( 0x00, 0x00, 0x00 );
	theme.errorBgColor = RGB( 0xFF, 0x80, 0x80 );

	theme.foldStyle   = RDOFOLDS_PLUS;
	theme.commentFold = false;

	return theme;
}

RDOEditorEditTheme RDOEditorEditTheme::getClassicTheme()
{
	RDOEditorEditTheme theme;
	*static_cast<RDOEditorBaseEditTheme*>(&theme) = RDOEditorBaseEditTheme::getClassicTheme();

	theme.foldFgColor  = RGB( 0xFF, 0xFF, 0xFF );
	theme.foldBgColor  = RGB( 0x00, 0x00, 0x00 );
	theme.errorBgColor = RGB( 0xE6, 0x05, 0xF8 );

	theme.foldStyle   = RDOFOLDS_PLUS;
	theme.commentFold = false;

	return theme;
}

RDOEditorEditTheme RDOEditorEditTheme::getTwilightTheme()
{
	RDOEditorEditTheme theme;
	*static_cast<RDOEditorBaseEditTheme*>(&theme) = RDOEditorBaseEditTheme::getTwilightTheme();

	theme.foldFgColor  = RGB( 0xFF, 0xFF, 0xFF );
	theme.foldBgColor  = RGB( 0x00, 0x00, 0x00 );
	theme.errorBgColor = RGB( 0xFF, 0x80, 0x80 );

	theme.foldStyle   = RDOFOLDS_PLUS;
	theme.commentFold = false;

	return theme;
}

RDOEditorEditTheme RDOEditorEditTheme::getOceanTheme()
{
	RDOEditorEditTheme theme;
	*static_cast<RDOEditorBaseEditTheme*>(&theme) = RDOEditorBaseEditTheme::getOceanTheme();

	theme.foldFgColor  = RGB( 0xFF, 0xFF, 0xFF );
	theme.foldBgColor  = RGB( 0x00, 0x00, 0x00 );
	theme.errorBgColor = RGB( 0xFF, 0xFF, 0x00 );

	theme.foldStyle   = RDOFOLDS_PLUS;
	theme.commentFold = false;

	return theme;
}

// --------------------------------------------------------------------------------
// -------------------- RDOEditorEditAutoComplete
// --------------------------------------------------------------------------------
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

rbool RDOEditorEditAutoComplete::operator ==( const RDOEditorEditAutoComplete& autoComplete ) const
{
	return useAutoComplete == autoComplete.useAutoComplete &&
	       showFullList    == autoComplete.showFullList;
}

rbool RDOEditorEditAutoComplete::operator !=( const RDOEditorEditAutoComplete& autoComplete ) const
{
	return !(*this == autoComplete);
}

void RDOEditorEditAutoComplete::load( tstring regPath )
{
	regPath += "autoComplete";
	useAutoComplete = AfxGetApp()->GetProfileInt( regPath.c_str(), "useAutoComplete", useAutoComplete ) ? true : false;
	showFullList    = AfxGetApp()->GetProfileInt( regPath.c_str(), "showFullList", showFullList ) ? true : false;
}

void RDOEditorEditAutoComplete::save( tstring regPath ) const
{
	regPath += "autoComplete";
	AfxGetApp()->WriteProfileInt( regPath.c_str(), "useAutoComplete", useAutoComplete );
	AfxGetApp()->WriteProfileInt( regPath.c_str(), "showFullList", showFullList );
}

// --------------------------------------------------------------------------------
// -------------------- RDOEditorEditMargin
// --------------------------------------------------------------------------------
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

rbool RDOEditorEditMargin::operator ==( const RDOEditorEditMargin& margin ) const
{
	return fold       == margin.fold &&
	       bookmark   == margin.bookmark &&
	       lineNumber == margin.lineNumber;
}

rbool RDOEditorEditMargin::operator !=( const RDOEditorEditMargin& margin ) const
{
	return !(*this == margin);
}

void RDOEditorEditMargin::load( tstring regPath )
{
	regPath += "margin";
	fold       = AfxGetApp()->GetProfileInt( regPath.c_str(), "fold", fold ) ? true : false;
	bookmark   = AfxGetApp()->GetProfileInt( regPath.c_str(), "bookmark", bookmark ) ? true : false;
	lineNumber = AfxGetApp()->GetProfileInt( regPath.c_str(), "lineNumber", lineNumber ) ? true : false;
}

void RDOEditorEditMargin::save( tstring regPath ) const
{
	regPath += "margin";
	AfxGetApp()->WriteProfileInt( regPath.c_str(), "fold", fold );
	AfxGetApp()->WriteProfileInt( regPath.c_str(), "bookmark", bookmark );
	AfxGetApp()->WriteProfileInt( regPath.c_str(), "lineNumber", lineNumber );
}

// --------------------------------------------------------------------------------
// -------------------- RDOEditorEditStyle
// --------------------------------------------------------------------------------
RDOEditorEditStyle::RDOEditorEditStyle():
	RDOEditorBaseEditStyle(),
	autoComplete( NULL ),
	margin( NULL )
{
}

RDOEditorEditStyle::~RDOEditorEditStyle()
{
	if ( autoComplete ) { delete autoComplete; autoComplete = NULL; };
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

void RDOEditorEditStyle::initMargin()
{
	margin = new RDOEditorEditMargin;
}

RDOEditorEditStyle& RDOEditorEditStyle::operator =( const RDOEditorEditStyle& style )
{
	RDOEditorBaseEditStyle::operator=( style );
	if ( theme        && style.theme )        *static_cast<RDOEditorEditTheme*>(theme) = *static_cast<RDOEditorEditTheme*>(style.theme);
	if ( autoComplete && style.autoComplete ) *autoComplete = *style.autoComplete;
	if ( margin       && style.margin )       *margin       = *style.margin;

	return *this;
}

rbool RDOEditorEditStyle::operator ==( const RDOEditorEditStyle& style ) const
{
	rbool flag = RDOEditorBaseEditStyle::operator==( style );
	if ( theme        && style.theme        && flag ) flag &= *static_cast<RDOEditorEditTheme*>(theme) == *static_cast<RDOEditorEditTheme*>(style.theme);
	if ( autoComplete && style.autoComplete && flag ) flag &= *autoComplete == *style.autoComplete;
	if ( margin       && style.margin       && flag ) flag &= *margin       == *style.margin;
	return flag;
}

rbool RDOEditorEditStyle::operator !=( const RDOEditorEditStyle& style ) const
{
	return !(*this == style);
}

void RDOEditorEditStyle::init( CREF(tstring) _regPath )
{
	RDOEditorBaseEditStyle::init( _regPath );
	initAutoComplete();
	initMargin();
}

rbool RDOEditorEditStyle::load()
{
	if ( RDOEditorBaseEditStyle::load() ) {
		if ( autoComplete ) autoComplete->load( regPath );
		if ( margin )       margin->load( regPath );
		return true;
	}
	return false;
}

rbool RDOEditorEditStyle::save() const
{
	if ( RDOEditorBaseEditStyle::save() ) {
		if ( autoComplete ) autoComplete->save(  regPath );
		if ( margin )       margin->save(  regPath );
		return true;
	}
	return false;
}

template <class CallbackFun>
void RDOEditorEditStyle::attachSubscriber(const CallbackFun& sub)
{
	subscriber.connect(sub);
	sub(*this);
}

template <class CallbackFun>
void RDOEditorEditStyle::detachSubscriber(const CallbackFun& sub)
{
	subscriber.disconnect(sub);
}