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

void RDOEditorEditTheme::load(QSettings& settings)
{
	RDOEditorBaseEditTheme::load(settings);

	settings >> *this;
}

void RDOEditorEditTheme::save(QSettings& settings ) const
{
	RDOEditorBaseEditTheme::save(settings);

	settings << *this;
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

namespace rdoEditor
{

QSettings& operator<< (QSettings& settings, const RDOEditorEditTheme& theme)
{
	settings.setValue("fold_fg_color", theme.foldFgColor.name());
	settings.setValue("fold_bg_color", theme.foldBgColor.name());
	settings.setValue("error_bg_color", theme.errorBgColor.name());
	settings.setValue("fold_style", theme.foldStyle);
	settings.setValue("comment_fold", theme.commentFold);

	return settings;
}

QSettings& operator>> (QSettings& settings, RDOEditorEditTheme& theme)
{
	theme.foldFgColor  = QColor(settings.value("fold_fg_color", theme.foldFgColor.name()).toString());
	theme.foldBgColor  = QColor(settings.value("fold_bg_color", theme.foldBgColor.name()).toString());
	theme.errorBgColor = QColor(settings.value("error_bg_color", theme.errorBgColor.name()).toString());
	theme.foldStyle    = (RDOFoldStyle)settings.value("fold_style", theme.foldStyle).toInt();
	theme.commentFold  = settings.value("comment_fold", theme.commentFold).toBool() ? true : false;

	return settings;
}

} // namespace rdoEditor

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

void RDOEditorEditAutoComplete::load(QSettings& settings)
{
	settings >> *this;
}

void RDOEditorEditAutoComplete::save(QSettings& settings) const
{
	settings << *this;
}

namespace rdoEditor
{

QSettings& operator<< (QSettings& settings, const RDOEditorEditAutoComplete& auto_complete)
{
	settings.setValue("use_auto_complete", auto_complete.useAutoComplete);
	settings.setValue("show_full_list", auto_complete.showFullList);

	return settings;
}

QSettings& operator>> (QSettings& settings, RDOEditorEditAutoComplete& auto_complete)
{
	auto_complete.useAutoComplete = settings.value("use_auto_complete", auto_complete.useAutoComplete).toBool() ? true : false;
	auto_complete.showFullList    = settings.value("show_full_list", auto_complete.showFullList).toBool() ? true : false;

	return settings;
}

} // namespace rdoEditor

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

void RDOEditorEditMargin::load(QSettings& settings)
{
	settings >> *this;
}

void RDOEditorEditMargin::save(QSettings& settings) const
{
	settings << *this;
}

namespace rdoEditor
{

QSettings& operator<< (QSettings& settings, const RDOEditorEditMargin& margin)
{
	settings.setValue("fold", margin.fold);
	settings.setValue("bookmark", margin.bookmark);
	settings.setValue("line_number", margin.lineNumber);

	return settings;
}

QSettings& operator>> (QSettings& settings, RDOEditorEditMargin& margin)
{
	margin.fold       = settings.value("fold", margin.fold).toBool() ? true : false;
	margin.bookmark   = settings.value("bookmark", margin.bookmark).toBool() ? true : false;
	margin.lineNumber = settings.value("line_number", margin.lineNumber).toBool() ? true : false;

	return settings;
}

} // namespace rdoEditor

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
	theme = new RDOEditorEditTheme();
}

RDOEditorEditStyle::~RDOEditorEditStyle()
{
	if ( autoComplete ) { delete autoComplete; autoComplete = NULL; };
	if ( margin )       { delete margin;       margin = NULL; };
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

void RDOEditorEditStyle::init( CREF(QString) _groupName )
{
	RDOEditorBaseEditStyle::init( _groupName );
}

rbool RDOEditorEditStyle::load()
{
	if ( RDOEditorBaseEditStyle::load() ) {
		QSettings settings;
		settings.beginGroup(groupName + "auto_complete");
		if (autoComplete) autoComplete->load(settings);
		settings.endGroup();
		settings.beginGroup(groupName + "margin");
		if (margin)       margin->load(settings);
		settings.endGroup();
		return true;
	}
	return false;
}

rbool RDOEditorEditStyle::save() const
{
	if ( RDOEditorBaseEditStyle::save() ) {
		QSettings settings;
		settings.beginGroup(groupName + "auto_complete");
		if ( autoComplete ) autoComplete->save(settings);
		settings.endGroup();
		settings.beginGroup(groupName + "margin");
		if ( margin )       margin->save(settings);
		settings.endGroup();
		return true;
	}
	return false;
}