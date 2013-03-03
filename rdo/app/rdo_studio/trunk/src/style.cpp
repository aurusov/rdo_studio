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
// -------------------- StyleTheme
// --------------------------------------------------------------------------------
StyleTheme::StyleTheme()
{
	defaultColor    = QColor( 0x00, 0x00, 0x00 );
	backgroundColor = QColor( 0xFF, 0xFF, 0xFF );

	defaultStyle = StyleFont::NONE;
}

StyleTheme::~StyleTheme()
{}

StyleTheme& StyleTheme::operator =( const StyleTheme& theme )
{
	defaultColor    = theme.defaultColor;
	backgroundColor = theme.backgroundColor;

	defaultStyle = theme.defaultStyle;

	return *this;
}

rbool StyleTheme::operator ==( const StyleTheme& theme ) const
{
	return defaultColor    == theme.defaultColor &&
	       backgroundColor == theme.backgroundColor &&

	       defaultStyle == theme.defaultStyle;
}

rbool StyleTheme::operator !=( const StyleTheme& theme ) const
{
	return !(*this == theme);
}

void StyleTheme::load(QSettings& settings)
{
	settings >> *this;
}

void StyleTheme::save(QSettings& settings) const
{
	settings << *this;
}

QSettings& operator<< (QSettings& settings, const StyleTheme& theme)
{
	settings.setValue("default_color", theme.defaultColor.name());
	settings.setValue("background_color", theme.backgroundColor.name());
	settings.setValue("default_style", theme.defaultStyle);

	return settings;
}

QSettings& operator>> (QSettings& settings, StyleTheme& theme)
{
	theme.defaultColor    = QColor(settings.value("default_color", theme.defaultColor.name()).toString());
	theme.backgroundColor = QColor(settings.value("background_color", theme.backgroundColor.name()).toString());
	theme.defaultStyle    = static_cast<StyleFont::style>(settings.value("default_style", theme.defaultStyle).toInt());

	return settings;
}

// --------------------------------------------------------------------------------
// -------------------- Style
// --------------------------------------------------------------------------------
Style::Style()
	: groupName()
	, font     (NULL)
{
	font = new StyleFont();
}

Style::~Style()
{
	if ( font ) { delete font; font = NULL; };
}

Style& Style::operator =( const Style& style )
{
	if ( font && style.font ) *font = *style.font;

	return *this;
}

rbool Style::operator ==( const Style& style ) const
{
	rbool flag = true;
	if ( font && style.font && flag ) flag &= *font == *style.font;
	return flag;
}

rbool Style::operator !=( const Style& style ) const
{
	return !(*this == style);
}

void Style::init( CREF(QString) _groupName )
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

rbool Style::load()
{
	if (!groupName.isEmpty()) {
		QSettings settings;
		settings.beginGroup(groupName + "font");
		if (font) font->load(settings);
		settings.endGroup();
		return true;
	}
	return false;
}

rbool Style::save() const
{
	if (!groupName.isEmpty()) {
		QSettings settings;
		settings.beginGroup(groupName + "font");
		if (font) font->save(settings);
		settings.endGroup();
		return true;
	}
	return false;
}

// --------------------------------------------------------------------------------
// -------------------- StyleWithTheme
// --------------------------------------------------------------------------------
StyleWithTheme::StyleWithTheme()
	: Style()
	, theme(NULL)
{
	theme = new StyleTheme();
}

StyleWithTheme::~StyleWithTheme()
{
	if ( theme ) { delete theme; theme = NULL; };
}

StyleWithTheme& StyleWithTheme::operator =( const StyleWithTheme& style )
{
	Style::operator=( style );
	if ( theme && style.theme ) *theme = *style.theme;

	return *this;
}

rbool StyleWithTheme::operator ==( const StyleWithTheme& style ) const
{
	rbool flag = Style::operator==( style );
	if ( theme && style.theme && flag ) flag &= *theme == *style.theme;
	return flag;
}

rbool StyleWithTheme::operator !=( const StyleWithTheme& style ) const
{
	return !(*this == style);
}

void StyleWithTheme::init( CREF(QString) _groupName )
{
	Style::init( _groupName );
}

rbool StyleWithTheme::load()
{
	if (Style::load()) {
		QSettings settings;
		settings.beginGroup(groupName + "theme");
		if (theme) theme->load(settings);
		settings.endGroup();
		return true;
	}
	return false;
}

rbool StyleWithTheme::save() const
{
	if (Style::save()) {
		QSettings settings;
		settings.beginGroup(groupName + "theme");
		if (theme) theme->save(settings);
		settings.endGroup();
		return true;
	}
	return false;
}

}}} // namespace rdo::gui::style
