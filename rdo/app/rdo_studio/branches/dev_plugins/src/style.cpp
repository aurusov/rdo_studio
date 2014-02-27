/*!
  \copyright (c) RDO-Team, 2003-2012
  \file      app/rdo_studio/src/style.cpp
  \author    Урусов Андрей (rdo@rk9.bmstu.ru)
  \date      27.03.2003
  \brief     
  \indent    4T
*/

// ---------------------------------------------------------------------------- PCH
#include "app/rdo_studio/pch/application_core_pch.h"
// ----------------------------------------------------------------------- INCLUDES
#include <QtGlobal>
// ----------------------------------------------------------------------- SYNOPSIS
#include "utils/src/debug/static_assert.h"
#include "app/rdo_studio/src/style.h"
#include "thirdparty/scintilla/include/Scintilla.h"
// --------------------------------------------------------------------------------

namespace rdo { namespace gui { namespace style {

// --------------------------------------------------------------------------------
// -------------------- StyleFont
// --------------------------------------------------------------------------------
StyleFont::StyleFont()
	: size(10)
{
#if defined(Q_OS_WIN)
	name = "Courier New";
	size = 10;
#elif defined(Q_OS_LINUX)
	name = "Courier";
	size = 11;
#else
	STATIC_ASSERT(UndefinedOS);
#endif
}

StyleFont::~StyleFont()
{}

StyleFont& StyleFont::operator =( const StyleFont& font )
{
	name = font.name;
	size = font.size;

	return *this;
}

bool StyleFont::operator ==( const StyleFont& font ) const
{
	return name == font.name &&
	       size == font.size;
}

bool StyleFont::operator !=( const StyleFont& font ) const
{
	return !(*this == font);
}

void StyleFont::load(QSettings& settings)
{
	settings >> *this;
}

void StyleFont::save(QSettings& settings) const
{
	settings << *this;
}

StyleFont StyleFont::getDefaultFont()
{
	StyleFont font;
	return font;
}

StyleFont StyleFont::getClassicFont()
{
	StyleFont font;
	font.name = "Fixedsys";

	return font;
}

StyleFont StyleFont::getTracerLogFont()
{
	StyleFont font;
	font.name = "Courier";

	return font;
}

StyleFont StyleFont::getChartViewFont()
{
	StyleFont font;
	font.name = "Tahoma";

	return font;
}

StyleFont StyleFont::getFrameFont()
{
	StyleFont font;

	font.name = "Verdana";
	font.size = 8;

	return font;
}

QSettings& operator<< (QSettings& settings, const StyleFont& font)
{
	settings.setValue("name", QString::fromStdString(font.name));
	settings.setValue("size", font.size);
	return settings;
}

QSettings& operator>> (QSettings& settings, StyleFont& font)
{
	font.name = settings.value("name", QString::fromStdString(font.name)).toString().toStdString();
	font.size = settings.value("size", font.size).toInt();
	return settings;
}

// --------------------------------------------------------------------------------
// -------------------- StyleBase
// --------------------------------------------------------------------------------
StyleBase::StyleBase()
	: font()
{
	defaultColor    = QColor( 0x00, 0x00, 0x00 );
	backgroundColor = QColor( 0xFF, 0xFF, 0xFF );
	defaultStyle = StyleFont::NONE;
}

StyleBase::~StyleBase()
{
}

StyleBase& StyleBase::operator =( const StyleBase& style )
{
	font            = style.font;
	defaultStyle    = style.defaultStyle;
	defaultColor    = style.defaultColor;
	backgroundColor = style.backgroundColor;
	return *this;
}

bool StyleBase::operator ==( const StyleBase& style ) const
{
	return
		font            == style.font &&
		defaultColor    == style.defaultColor &&
		backgroundColor == style.backgroundColor &&
		defaultStyle    == style.defaultStyle;
}

bool StyleBase::operator !=( const StyleBase& style ) const
{
	return !(*this == style);
}

QSettings& operator<< (QSettings& settings, const StyleBase& style)
{
	settings.beginGroup("font");
	settings << style.font;
	settings.endGroup();

	settings.beginGroup("theme");
	settings.setValue("default_color", style.defaultColor.name());
	settings.setValue("background_color", style.backgroundColor.name());
	settings.setValue("default_style", style.defaultStyle);
	settings.endGroup();

	return settings;
}

QSettings& operator>> (QSettings& settings, StyleBase& style)
{
	settings.beginGroup("font");
	settings >> style.font;
	settings.endGroup();

	settings.beginGroup("theme");
	style.defaultColor    = QColor(settings.value("default_color", style.defaultColor.name()).toString());
	style.backgroundColor = QColor(settings.value("background_color", style.backgroundColor.name()).toString());
	style.defaultStyle    = static_cast<StyleFont::style>(settings.value("default_style", style.defaultStyle).toInt());
	settings.endGroup();

	return settings;
}

}}} // namespace rdo::gui::style
