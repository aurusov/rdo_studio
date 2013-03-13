/*!
  \copyright (c) RDO-Team, 2003-2012
  \file      app/rdo_studio/src/style.cpp
  \author    Урусов Андрей (rdo@rk9.bmstu.ru)
  \date      27.03.2003
  \brief     
  \indent    4T
*/

// ---------------------------------------------------------------------------- PCH
#include "app/rdo_studio/pch/stdpch.h"
// ----------------------------------------------------------------------- INCLUDES
// ----------------------------------------------------------------------- SYNOPSIS
#include "app/rdo_studio/src/style.h"
#include "thirdparty/scintilla/include/Scintilla.h"
// --------------------------------------------------------------------------------

namespace rdo { namespace gui { namespace style {

// --------------------------------------------------------------------------------
// -------------------- StyleFont
// --------------------------------------------------------------------------------
StyleFont::StyleFont()
{
	name         = "Courier New";
	size         = 10;
	codepage     = 0;
	characterSet = 1;
	if ( PRIMARYLANGID(GetSystemDefaultLangID()) == LANG_RUSSIAN ) {
		characterSet = SC_CHARSET_CYRILLIC;
	}
}

StyleFont::~StyleFont()
{}

StyleFont& StyleFont::operator =( const StyleFont& font )
{
	name         = font.name;
	size         = font.size;
	codepage     = font.codepage;
	characterSet = font.characterSet;

	return *this;
}

rbool StyleFont::operator ==( const StyleFont& font ) const
{
	return name         == font.name &&
	       size         == font.size &&
	       codepage     == font.codepage &&
	       characterSet == font.characterSet;
}

rbool StyleFont::operator !=( const StyleFont& font ) const
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
	settings.setValue("name", QString::fromLocal8Bit(font.name.c_str()));
	settings.setValue("size", font.size);
	settings.setValue("codepage", font.codepage);
	settings.setValue("character_set", font.characterSet);

	return settings;
}

QSettings& operator>> (QSettings& settings, StyleFont& font)
{
	font.name         = settings.value("name", QString::fromLocal8Bit(font.name.c_str())).toString().toLocal8Bit().constData();
	font.size         = settings.value("size", font.size).toInt();
	font.codepage     = settings.value("codepage", font.codepage).toInt();
	font.characterSet = settings.value("character_set", font.characterSet).toInt();
	if (font.characterSet == RUSSIAN_CHARSET)
	{
		font.characterSet = SC_CHARSET_CYRILLIC;
	}
	return settings;
}

// --------------------------------------------------------------------------------
// -------------------- StyleBase
// --------------------------------------------------------------------------------
StyleBase::StyleBase()
	: groupName()
	, font()
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
	font = style.font;
	defaultStyle = style.defaultStyle;

	defaultColor    = style.defaultColor;
	backgroundColor = style.backgroundColor;

	return *this;
}

rbool StyleBase::operator ==( const StyleBase& style ) const
{
	rbool flag = true;
	flag &= font == style.font;

	return flag &&
		defaultColor    == style.defaultColor &&
		backgroundColor == style.backgroundColor &&
		defaultStyle    == style.defaultStyle;
}

rbool StyleBase::operator !=( const StyleBase& style ) const
{
	return !(*this == style);
}

void StyleBase::init( CREF(QString) _groupName )
{
	groupName = _groupName;
	if (!groupName.isEmpty()) 
	{
		groupName.prepend("style/");
		if(groupName.lastIndexOf("/") != groupName.length() - 1)
		{
			groupName.append("/");
		}
	}
}

rbool StyleBase::load()
{
	if (!groupName.isEmpty()) {
		QSettings settings;
		settings.beginGroup(groupName + "font");
		font.load(settings);
		settings.endGroup();
		settings.beginGroup(groupName + "theme");
		loadStyle(settings);
		settings.endGroup();
		return true;
	}
	return false;
}

rbool StyleBase::save() const
{
	if (!groupName.isEmpty()) {
		QSettings settings;
		settings.beginGroup(groupName + "font");
		font.save(settings);
		settings.endGroup();
		settings.beginGroup(groupName + "theme");
		saveStyle(settings);
		settings.endGroup();
		return true;
	}
	return false;
}

void StyleBase::loadStyle(QSettings& settings)
{
	settings >> *this;
}

void StyleBase::saveStyle(QSettings& settings) const
{
	settings << *this;
}

QSettings& operator<< (QSettings& settings, const StyleBase& style)
{
	settings.setValue("default_color", style.defaultColor.name());
	settings.setValue("background_color", style.backgroundColor.name());
	settings.setValue("default_style", style.defaultStyle);

	return settings;
}

QSettings& operator>> (QSettings& settings, StyleBase& style)
{
	style.defaultColor    = QColor(settings.value("default_color", style.defaultColor.name()).toString());
	style.backgroundColor = QColor(settings.value("background_color", style.backgroundColor.name()).toString());
	style.defaultStyle    = static_cast<StyleFont::style>(settings.value("default_style", style.defaultStyle).toInt());

	return settings;
}

}}} // namespace rdo::gui::style
