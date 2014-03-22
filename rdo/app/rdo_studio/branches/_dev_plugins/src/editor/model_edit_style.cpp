/*!
  \copyright (c) RDO-Team, 2003-2012
  \file      model_edit_style.cpp
  \author    Урусов Андрей (rdo@rk9.bmstu.ru)
  \date      20.02.2003
  \brief     
  \indent    4T
*/

// ---------------------------------------------------------------------------- PCH
#include "app/rdo_studio/pch/model_pch.h"
// ----------------------------------------------------------------------- INCLUDES
// ----------------------------------------------------------------------- SYNOPSIS
#include "app/rdo_studio/src/editor/model_edit_style.h"
// --------------------------------------------------------------------------------

using namespace rdo::gui::editor;

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

bool ModelAutoComplete::operator ==( const ModelAutoComplete& autoComplete ) const
{
	return useAutoComplete == autoComplete.useAutoComplete &&
	       showFullList    == autoComplete.showFullList;
}

bool ModelAutoComplete::operator !=( const ModelAutoComplete& autoComplete ) const
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

bool ModelMargin::operator ==( const ModelMargin& margin ) const
{
	return fold       == margin.fold &&
	       bookmark   == margin.bookmark &&
	       lineNumber == margin.lineNumber;
}

bool ModelMargin::operator !=( const ModelMargin& margin ) const
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
	autoComplete(),
	margin()
{
	foldFgColor = QColor( 0xFF, 0xFF, 0xFF );
	foldBgColor = QColor( 0x00, 0x00, 0x00 );

	errorBgColor = QColor( 0xFF, 0x80, 0x80 );

	foldStyle   = F_PLUS;
	commentFold = false;
}

ModelStyle::~ModelStyle()
{
}

ModelStyle& ModelStyle::operator =( const ModelStyle& style )
{
	ParserStyle::operator=( style );
	
	foldFgColor = style.foldFgColor;
	foldBgColor = style.foldBgColor;

	errorBgColor = style.errorBgColor;

	foldStyle   = style.foldStyle;
	commentFold = style.commentFold;
	autoComplete = style.autoComplete;
	margin       = style.margin;

	return *this;
}

bool ModelStyle::operator ==( const ModelStyle& style ) const
{
	bool flag = ParserStyle::operator==( style );
	flag &= autoComplete == style.autoComplete;
	flag &= margin       == style.margin;
	if ( flag ) flag &= foldFgColor == style.foldFgColor &&
		foldBgColor == style.foldBgColor &&
		errorBgColor == style.errorBgColor &&
		foldStyle   == style.foldStyle &&
		commentFold == style.commentFold;
	return flag;
}

bool ModelStyle::operator !=( const ModelStyle& style ) const
{
	return !(*this == style);
}

ModelStyle ModelStyle::getDefaultStyle()
{
	ModelStyle style;
	return style;
}

ModelStyle ModelStyle::getCppStyle()
{
	ModelStyle style;
	*static_cast<ParserStyle*>(&style) = ParserStyle::getCppStyle();

	style.foldFgColor  = QColor( 0xFF, 0xFF, 0xFF );
	style.foldBgColor  = QColor( 0x00, 0x00, 0x00 );
	style.errorBgColor = QColor( 0xFF, 0x80, 0x80 );

	style.foldStyle   = F_PLUS;
	style.commentFold = false;

	return style;
}

ModelStyle ModelStyle::getPascalStyle()
{
	ModelStyle style;
	*static_cast<ParserStyle*>(&style) = ParserStyle::getPascalStyle();

	style.foldFgColor  = QColor( 0xFF, 0xFF, 0xFF );
	style.foldBgColor  = QColor( 0x00, 0x00, 0x00 );
	style.errorBgColor = QColor( 0xFF, 0x80, 0x80 );

	style.foldStyle   = F_PLUS;
	style.commentFold = false;

	return style;
}

ModelStyle ModelStyle::getHtmlStyle()
{
	ModelStyle style;
	*static_cast<ParserStyle*>(&style) = ParserStyle::getHtmlStyle();

	style.foldFgColor  = QColor( 0xFF, 0xFF, 0xFF );
	style.foldBgColor  = QColor( 0x00, 0x00, 0x00 );
	style.errorBgColor = QColor( 0xFF, 0x80, 0x80 );

	style.foldStyle   = F_PLUS;
	style.commentFold = false;

	return style;
}

ModelStyle ModelStyle::getClassicStyle()
{
	ModelStyle style;
	*static_cast<ParserStyle*>(&style) = ParserStyle::getClassicStyle();

	style.foldFgColor  = QColor( 0xFF, 0xFF, 0xFF );
	style.foldBgColor  = QColor( 0x00, 0x00, 0x00 );
	style.errorBgColor = QColor( 0xE6, 0x05, 0xF8 );

	style.foldStyle   = F_PLUS;
	style.commentFold = false;

	return style;
}

ModelStyle ModelStyle::getTwilightStyle()
{
	ModelStyle style;
	*static_cast<ParserStyle*>(&style) = ParserStyle::getTwilightStyle();

	style.foldFgColor  = QColor( 0xFF, 0xFF, 0xFF );
	style.foldBgColor  = QColor( 0x00, 0x00, 0x00 );
	style.errorBgColor = QColor( 0xFF, 0x80, 0x80 );

	style.foldStyle   = F_PLUS;
	style.commentFold = false;

	return style;
}

ModelStyle ModelStyle::getOceanStyle()
{
	ModelStyle style;
	*static_cast<ParserStyle*>(&style) = ParserStyle::getOceanStyle();

	style.foldFgColor  = QColor( 0xFF, 0xFF, 0xFF );
	style.foldBgColor  = QColor( 0x00, 0x00, 0x00 );
	style.errorBgColor = QColor( 0xFF, 0xFF, 0x00 );

	style.foldStyle   = F_PLUS;
	style.commentFold = false;

	return style;
}

namespace rdo { namespace gui { namespace editor {

QSettings& operator<< (QSettings& settings, const ModelStyle& style)
{
	settings << static_cast<const ParserStyle&>(style);

	settings.beginGroup("theme");
	settings.setValue("fold_fg_color", style.foldFgColor.name());
	settings.setValue("fold_bg_color", style.foldBgColor.name());
	settings.setValue("error_bg_color", style.errorBgColor.name());
	settings.setValue("fold_style", style.foldStyle);
	settings.setValue("comment_fold", style.commentFold);
	settings.endGroup();

	settings.beginGroup("auto_complete");
	settings << style.autoComplete;
	settings.endGroup();

	settings.beginGroup("margin");
	settings << style.margin;
	settings.endGroup();


	return settings;
}

QSettings& operator>> (QSettings& settings, ModelStyle& style)
{
	settings >> static_cast<ParserStyle&>(style);

	settings.beginGroup("theme");
	style.foldFgColor  = QColor(settings.value("fold_fg_color", style.foldFgColor.name()).toString());
	style.foldBgColor  = QColor(settings.value("fold_bg_color", style.foldBgColor.name()).toString());
	style.errorBgColor = QColor(settings.value("error_bg_color", style.errorBgColor.name()).toString());
	style.foldStyle    = (ModelStyle::Fold)settings.value("fold_style", style.foldStyle).toInt();
	style.commentFold  = settings.value("comment_fold", style.commentFold).toBool() ? true : false;
	settings.endGroup();

	settings.beginGroup("auto_complete");
	settings >> style.autoComplete;
	settings.endGroup();

	settings.beginGroup("margin");
	settings >> style.margin;
	settings.endGroup();

	return settings;
}

}}} // namespace rdo::gui::editor