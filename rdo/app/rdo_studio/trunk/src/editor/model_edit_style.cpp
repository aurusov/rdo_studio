/*!
  \copyright (c) RDO-Team, 2003-2012
  \file      model_edit_style.cpp
  \author    Урусов Андрей (rdo@rk9.bmstu.ru)
  \date      20.02.2003
  \brief     
  \indent    4T
*/

// ---------------------------------------------------------------------------- PCH
#include "app/rdo_studio/pch/stdpch.h"
// ----------------------------------------------------------------------- INCLUDES
// ----------------------------------------------------------------------- SYNOPSIS
#include "app/rdo_studio/src/editor/model_edit_style.h"
// --------------------------------------------------------------------------------

using namespace rdo::gui::editor;

// --------------------------------------------------------------------------------
// -------------------- ModelTheme
// --------------------------------------------------------------------------------
ModelTheme::ModelTheme(): ParserTheme()
{
	foldFgColor = QColor( 0xFF, 0xFF, 0xFF );
	foldBgColor = QColor( 0x00, 0x00, 0x00 );

	errorBgColor = QColor( 0xFF, 0x80, 0x80 );

	foldStyle   = RDOFOLDS_PLUS;
	commentFold = false;
}

ModelTheme::~ModelTheme()
{}

ModelTheme& ModelTheme::operator =( const ModelTheme& theme )
{
	ParserTheme::operator=( theme );

	foldFgColor = theme.foldFgColor;
	foldBgColor = theme.foldBgColor;

	errorBgColor = theme.errorBgColor;

	foldStyle   = theme.foldStyle;
	commentFold = theme.commentFold;

	return *this;
}

rbool ModelTheme::operator ==( const ModelTheme& theme ) const
{
	rbool flag = ParserTheme::operator==( theme );

	if ( flag ) flag &= foldFgColor == theme.foldFgColor &&
	                    foldBgColor == theme.foldBgColor &&

	                    errorBgColor == theme.errorBgColor &&

	                    foldStyle   == theme.foldStyle &&
	                    commentFold == theme.commentFold;
	return flag;
}

rbool ModelTheme::operator !=( const ModelTheme& theme ) const
{
	return !(*this == theme);
}

void ModelTheme::load(QSettings& settings)
{
	ParserTheme::load(settings);

	settings >> *this;
}

void ModelTheme::save(QSettings& settings ) const
{
	ParserTheme::save(settings);

	settings << *this;
}

ModelTheme ModelTheme::getDefaultTheme()
{
	ModelTheme theme;
	return theme;
}

ModelTheme ModelTheme::getCppTheme()
{
	ModelTheme theme;
	*static_cast<ParserTheme*>(&theme) = ParserTheme::getCppTheme();

	theme.foldFgColor  = QColor( 0xFF, 0xFF, 0xFF );
	theme.foldBgColor  = QColor( 0x00, 0x00, 0x00 );
	theme.errorBgColor = QColor( 0xFF, 0x80, 0x80 );

	theme.foldStyle   = RDOFOLDS_PLUS;
	theme.commentFold = false;

	return theme;
}

ModelTheme ModelTheme::getPascalTheme()
{
	ModelTheme theme;
	*static_cast<ParserTheme*>(&theme) = ParserTheme::getPascalTheme();

	theme.foldFgColor  = QColor( 0xFF, 0xFF, 0xFF );
	theme.foldBgColor  = QColor( 0x00, 0x00, 0x00 );
	theme.errorBgColor = QColor( 0xFF, 0x80, 0x80 );

	theme.foldStyle   = RDOFOLDS_PLUS;
	theme.commentFold = false;

	return theme;
}

ModelTheme ModelTheme::getHtmlTheme()
{
	ModelTheme theme;
	*static_cast<ParserTheme*>(&theme) = ParserTheme::getHtmlTheme();

	theme.foldFgColor  = QColor( 0xFF, 0xFF, 0xFF );
	theme.foldBgColor  = QColor( 0x00, 0x00, 0x00 );
	theme.errorBgColor = QColor( 0xFF, 0x80, 0x80 );

	theme.foldStyle   = RDOFOLDS_PLUS;
	theme.commentFold = false;

	return theme;
}

ModelTheme ModelTheme::getClassicTheme()
{
	ModelTheme theme;
	*static_cast<ParserTheme*>(&theme) = ParserTheme::getClassicTheme();

	theme.foldFgColor  = QColor( 0xFF, 0xFF, 0xFF );
	theme.foldBgColor  = QColor( 0x00, 0x00, 0x00 );
	theme.errorBgColor = QColor( 0xE6, 0x05, 0xF8 );

	theme.foldStyle   = RDOFOLDS_PLUS;
	theme.commentFold = false;

	return theme;
}

ModelTheme ModelTheme::getTwilightTheme()
{
	ModelTheme theme;
	*static_cast<ParserTheme*>(&theme) = ParserTheme::getTwilightTheme();

	theme.foldFgColor  = QColor( 0xFF, 0xFF, 0xFF );
	theme.foldBgColor  = QColor( 0x00, 0x00, 0x00 );
	theme.errorBgColor = QColor( 0xFF, 0x80, 0x80 );

	theme.foldStyle   = RDOFOLDS_PLUS;
	theme.commentFold = false;

	return theme;
}

ModelTheme ModelTheme::getOceanTheme()
{
	ModelTheme theme;
	*static_cast<ParserTheme*>(&theme) = ParserTheme::getOceanTheme();

	theme.foldFgColor  = QColor( 0xFF, 0xFF, 0xFF );
	theme.foldBgColor  = QColor( 0x00, 0x00, 0x00 );
	theme.errorBgColor = QColor( 0xFF, 0xFF, 0x00 );

	theme.foldStyle   = RDOFOLDS_PLUS;
	theme.commentFold = false;

	return theme;
}

namespace rdo { namespace gui { namespace editor {

QSettings& operator<< (QSettings& settings, const ModelTheme& theme)
{
	settings.setValue("fold_fg_color", theme.foldFgColor.name());
	settings.setValue("fold_bg_color", theme.foldBgColor.name());
	settings.setValue("error_bg_color", theme.errorBgColor.name());
	settings.setValue("fold_style", theme.foldStyle);
	settings.setValue("comment_fold", theme.commentFold);

	return settings;
}

QSettings& operator>> (QSettings& settings, ModelTheme& theme)
{
	theme.foldFgColor  = QColor(settings.value("fold_fg_color", theme.foldFgColor.name()).toString());
	theme.foldBgColor  = QColor(settings.value("fold_bg_color", theme.foldBgColor.name()).toString());
	theme.errorBgColor = QColor(settings.value("error_bg_color", theme.errorBgColor.name()).toString());
	theme.foldStyle    = (RDOFoldStyle)settings.value("fold_style", theme.foldStyle).toInt();
	theme.commentFold  = settings.value("comment_fold", theme.commentFold).toBool() ? true : false;

	return settings;
}

}}} // namespace rdo::gui::editor

// --------------------------------------------------------------------------------
// -------------------- ModelAutoComplete
// --------------------------------------------------------------------------------
ModelAutoComplete::ModelAutoComplete()
{
	useAutoComplete = true;
	showFullList    = true;
}

ModelAutoComplete::~ModelAutoComplete()
{}

ModelAutoComplete& ModelAutoComplete::operator =( const ModelAutoComplete& autoComplete )
{
	useAutoComplete = autoComplete.useAutoComplete;
	showFullList    = autoComplete.showFullList;

	return *this;
}

rbool ModelAutoComplete::operator ==( const ModelAutoComplete& autoComplete ) const
{
	return useAutoComplete == autoComplete.useAutoComplete &&
	       showFullList    == autoComplete.showFullList;
}

rbool ModelAutoComplete::operator !=( const ModelAutoComplete& autoComplete ) const
{
	return !(*this == autoComplete);
}

void ModelAutoComplete::load(QSettings& settings)
{
	settings >> *this;
}

void ModelAutoComplete::save(QSettings& settings) const
{
	settings << *this;
}

namespace rdo { namespace gui { namespace editor {

QSettings& operator<< (QSettings& settings, const ModelAutoComplete& auto_complete)
{
	settings.setValue("use_auto_complete", auto_complete.useAutoComplete);
	settings.setValue("show_full_list", auto_complete.showFullList);

	return settings;
}

QSettings& operator>> (QSettings& settings, ModelAutoComplete& auto_complete)
{
	auto_complete.useAutoComplete = settings.value("use_auto_complete", auto_complete.useAutoComplete).toBool() ? true : false;
	auto_complete.showFullList    = settings.value("show_full_list", auto_complete.showFullList).toBool() ? true : false;

	return settings;
}

}}} // namespace rdo::gui::editor

// --------------------------------------------------------------------------------
// -------------------- ModelMargin
// --------------------------------------------------------------------------------
ModelMargin::ModelMargin()
{
	fold       = true;
	bookmark   = true;
	lineNumber = false;
}

ModelMargin::~ModelMargin()
{}

ModelMargin& ModelMargin::operator =( const ModelMargin& margin )
{
	fold       = margin.fold;
	bookmark   = margin.bookmark;
	lineNumber = margin.lineNumber;

	return *this;
}

rbool ModelMargin::operator ==( const ModelMargin& margin ) const
{
	return fold       == margin.fold &&
	       bookmark   == margin.bookmark &&
	       lineNumber == margin.lineNumber;
}

rbool ModelMargin::operator !=( const ModelMargin& margin ) const
{
	return !(*this == margin);
}

void ModelMargin::load(QSettings& settings)
{
	settings >> *this;
}

void ModelMargin::save(QSettings& settings) const
{
	settings << *this;
}

namespace rdo { namespace gui { namespace editor {

QSettings& operator<< (QSettings& settings, const ModelMargin& margin)
{
	settings.setValue("fold", margin.fold);
	settings.setValue("bookmark", margin.bookmark);
	settings.setValue("line_number", margin.lineNumber);

	return settings;
}

QSettings& operator>> (QSettings& settings, ModelMargin& margin)
{
	margin.fold       = settings.value("fold", margin.fold).toBool() ? true : false;
	margin.bookmark   = settings.value("bookmark", margin.bookmark).toBool() ? true : false;
	margin.lineNumber = settings.value("line_number", margin.lineNumber).toBool() ? true : false;

	return settings;
}

}}} // namespace rdo::gui::editor

// --------------------------------------------------------------------------------
// -------------------- ModelStyle
// --------------------------------------------------------------------------------
ModelStyle::ModelStyle():
	ParserStyle(),
	autoComplete( NULL ),
	margin( NULL )
{
	autoComplete = new ModelAutoComplete();
	margin = new ModelMargin();
	theme = new ModelTheme();
}

ModelStyle::~ModelStyle()
{
	if ( autoComplete ) { delete autoComplete; autoComplete = NULL; };
	if ( margin )       { delete margin;       margin = NULL; };
}

ModelStyle& ModelStyle::operator =( const ModelStyle& style )
{
	ParserStyle::operator=( style );
	if ( theme        && style.theme )        *static_cast<ModelTheme*>(theme) = *static_cast<ModelTheme*>(style.theme);
	if ( autoComplete && style.autoComplete ) *autoComplete = *style.autoComplete;
	if ( margin       && style.margin )       *margin       = *style.margin;

	return *this;
}

rbool ModelStyle::operator ==( const ModelStyle& style ) const
{
	rbool flag = ParserStyle::operator==( style );
	if ( theme        && style.theme        && flag ) flag &= *static_cast<ModelTheme*>(theme) == *static_cast<ModelTheme*>(style.theme);
	if ( autoComplete && style.autoComplete && flag ) flag &= *autoComplete == *style.autoComplete;
	if ( margin       && style.margin       && flag ) flag &= *margin       == *style.margin;
	return flag;
}

rbool ModelStyle::operator !=( const ModelStyle& style ) const
{
	return !(*this == style);
}

void ModelStyle::init( CREF(QString) _groupName )
{
	ParserStyle::init( _groupName );
}

rbool ModelStyle::load()
{
	if ( ParserStyle::load() ) {
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

rbool ModelStyle::save() const
{
	if ( ParserStyle::save() ) {
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