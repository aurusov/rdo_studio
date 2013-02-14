/*!
  \copyright (c) RDO-Team, 2003-2012
  \file      rdoeditoreditstyle.cpp
  \author    Урусов Андрей (rdo@rk9.bmstu.ru)
  \date      20.02.2003
  \brief     
  \indent    4T
*/

// ---------------------------------------------------------------------------- PCH
#include "app/rdo_studio/pch/stdpch.h"
// ----------------------------------------------------------------------- INCLUDES
// ----------------------------------------------------------------------- SYNOPSIS
#include "app/rdo_studio/rdo_edit/rdoeditoreditstyle.h"
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
	foldFgColor = QColor( 0xFF, 0xFF, 0xFF );
	foldBgColor = QColor( 0x00, 0x00, 0x00 );

	errorBgColor = QColor( 0xFF, 0x80, 0x80 );

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

void RDOEditorEditTheme::load( QString regPath )
{
	RDOEditorBaseEditTheme::load( regPath );

	QSettings settings;
	regPath.append("theme\\");
	foldFgColor  = QColor(settings.value(QString(regPath + "fold_fg_color"), foldFgColor.name()).toString());
	foldBgColor  = QColor(settings.value(QString(regPath + "fold_bg_color"), foldBgColor.name()).toString());
	errorBgColor = QColor(settings.value(QString(regPath + "error_bg_color"), errorBgColor.name()).toString());
	foldStyle    = (RDOFoldStyle)settings.value(QString(regPath + "fold_style"), foldStyle).toInt();
	commentFold  = settings.value(QString(regPath + "comment_fold"), commentFold).toBool() ? true : false;
}

void RDOEditorEditTheme::save( QString regPath ) const
{
	RDOEditorBaseEditTheme::save( regPath );

	QSettings settings;
	regPath.append("theme\\");
	settings.setValue(QString(regPath + "fold_fg_color"), foldFgColor.name());
	settings.setValue(QString(regPath + "fold_bg_color"), foldBgColor.name());
	settings.setValue(QString(regPath + "error_bg_color"), errorBgColor.name());
	settings.setValue(QString(regPath + "fold_style"), foldStyle);
	settings.setValue(QString(regPath + "comment_fold"), commentFold);
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

	theme.foldFgColor  = QColor( 0xFF, 0xFF, 0xFF );
	theme.foldBgColor  = QColor( 0x00, 0x00, 0x00 );
	theme.errorBgColor = QColor( 0xFF, 0x80, 0x80 );

	theme.foldStyle   = RDOFOLDS_PLUS;
	theme.commentFold = false;

	return theme;
}

RDOEditorEditTheme RDOEditorEditTheme::getPascalTheme()
{
	RDOEditorEditTheme theme;
	*static_cast<RDOEditorBaseEditTheme*>(&theme) = RDOEditorBaseEditTheme::getPascalTheme();

	theme.foldFgColor  = QColor( 0xFF, 0xFF, 0xFF );
	theme.foldBgColor  = QColor( 0x00, 0x00, 0x00 );
	theme.errorBgColor = QColor( 0xFF, 0x80, 0x80 );

	theme.foldStyle   = RDOFOLDS_PLUS;
	theme.commentFold = false;

	return theme;
}

RDOEditorEditTheme RDOEditorEditTheme::getHtmlTheme()
{
	RDOEditorEditTheme theme;
	*static_cast<RDOEditorBaseEditTheme*>(&theme) = RDOEditorBaseEditTheme::getHtmlTheme();

	theme.foldFgColor  = QColor( 0xFF, 0xFF, 0xFF );
	theme.foldBgColor  = QColor( 0x00, 0x00, 0x00 );
	theme.errorBgColor = QColor( 0xFF, 0x80, 0x80 );

	theme.foldStyle   = RDOFOLDS_PLUS;
	theme.commentFold = false;

	return theme;
}

RDOEditorEditTheme RDOEditorEditTheme::getClassicTheme()
{
	RDOEditorEditTheme theme;
	*static_cast<RDOEditorBaseEditTheme*>(&theme) = RDOEditorBaseEditTheme::getClassicTheme();

	theme.foldFgColor  = QColor( 0xFF, 0xFF, 0xFF );
	theme.foldBgColor  = QColor( 0x00, 0x00, 0x00 );
	theme.errorBgColor = QColor( 0xE6, 0x05, 0xF8 );

	theme.foldStyle   = RDOFOLDS_PLUS;
	theme.commentFold = false;

	return theme;
}

RDOEditorEditTheme RDOEditorEditTheme::getTwilightTheme()
{
	RDOEditorEditTheme theme;
	*static_cast<RDOEditorBaseEditTheme*>(&theme) = RDOEditorBaseEditTheme::getTwilightTheme();

	theme.foldFgColor  = QColor( 0xFF, 0xFF, 0xFF );
	theme.foldBgColor  = QColor( 0x00, 0x00, 0x00 );
	theme.errorBgColor = QColor( 0xFF, 0x80, 0x80 );

	theme.foldStyle   = RDOFOLDS_PLUS;
	theme.commentFold = false;

	return theme;
}

RDOEditorEditTheme RDOEditorEditTheme::getOceanTheme()
{
	RDOEditorEditTheme theme;
	*static_cast<RDOEditorBaseEditTheme*>(&theme) = RDOEditorBaseEditTheme::getOceanTheme();

	theme.foldFgColor  = QColor( 0xFF, 0xFF, 0xFF );
	theme.foldBgColor  = QColor( 0x00, 0x00, 0x00 );
	theme.errorBgColor = QColor( 0xFF, 0xFF, 0x00 );

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

void RDOEditorEditAutoComplete::load( QString regPath )
{
	QSettings settings;
	regPath.append("auto_complete\\");
	useAutoComplete = settings.value(QString(regPath + "use_auto_complete"), useAutoComplete).toBool() ? true : false;
	showFullList    = settings.value(QString(regPath + "show_full_list"), showFullList).toBool() ? true : false;
}

void RDOEditorEditAutoComplete::save( QString regPath ) const
{
	QSettings settings;
	regPath.append("auto_complete\\");
	settings.setValue(QString(regPath + "use_auto_complete"), useAutoComplete);
	settings.setValue(QString(regPath + "show_full_list"), showFullList);
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

void RDOEditorEditMargin::load( QString regPath )
{
	QSettings settings;
	regPath.append("margin\\");
	fold       = settings.value(QString(regPath + "fold"), fold).toBool() ? true : false;
	bookmark   = settings.value(QString(regPath + "bookmark"), bookmark).toBool() ? true : false;
	lineNumber = settings.value(QString(regPath + "line_number"), lineNumber).toBool() ? true : false;
}

void RDOEditorEditMargin::save( QString regPath ) const
{
	QSettings settings;
	regPath.append("margin\\");
	settings.setValue(QString(regPath + "fold"), fold);
	settings.setValue(QString(regPath + "bookmark"), bookmark);
	settings.setValue(QString(regPath + "line_number"), lineNumber);
}

// --------------------------------------------------------------------------------
// -------------------- RDOEditorEditStyle
// --------------------------------------------------------------------------------
RDOEditorEditStyle::RDOEditorEditStyle():
	RDOEditorBaseEditStyle(),
	autoComplete( NULL ),
	margin( NULL )
{
	autoComplete = new RDOEditorEditAutoComplete();
	margin = new RDOEditorEditMargin();
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

void RDOEditorEditStyle::init( CREF(QString) _regPath )
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