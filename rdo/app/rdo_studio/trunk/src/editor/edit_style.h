/*!
  \copyright (c) RDO-Team, 2003-2012
  \file      edit_style.h
  \author    Урусов Андрей (rdo@rk9.bmstu.ru)
  \date      28.02.2003
  \brief     
  \indent    4T
*/

#ifndef _RDO_STUDIO_EDITOR_EDIT_STYLE_H_
#define _RDO_STUDIO_EDITOR_EDIT_STYLE_H_

// ----------------------------------------------------------------------- INCLUDES
// ----------------------------------------------------------------------- SYNOPSIS
#include "app/rdo_studio/src/style.h"
#include "thirdparty/scintilla/include/Scintilla.h"
// --------------------------------------------------------------------------------

namespace rdo { namespace gui { namespace editor {

// --------------------------------------------------------------------------------
// -------------------- EditTheme
// --------------------------------------------------------------------------------
class EditTheme: public rdoStyle::RDOStyleTheme
{
public:
	enum Bookmark
	{
		B_NONE = 0,
		B_CIRCLE,
		B_RECT,
		B_ROUNDRECT,
		B_ARROW
	};

	EditTheme();
	virtual ~EditTheme();

	EditTheme& operator =( const EditTheme& theme );
	rbool operator ==( const EditTheme& theme ) const;
	rbool operator !=( const EditTheme& theme ) const;

	virtual void load(QSettings& settings);
	virtual void save(QSettings& settings) const;

	QColor caretColor;
	QColor selectionBgColor;
	QColor bookmarkFgColor;
	QColor bookmarkBgColor;

	Bookmark bookmarkStyle;

	virtual rbool styleDefault( const int styleType ) const;
	virtual rbool styleUsing( const int styleType ) const;
	virtual rbool styleBold( const int styleType = STYLE_DEFAULT ) const;
	virtual rbool styleItalic( const int styleType = STYLE_DEFAULT ) const;
	virtual tstring styleFGColorToHEX( const int styleType = STYLE_DEFAULT ) const;
	virtual tstring styleBGColorToHEX( const int styleType = STYLE_DEFAULT ) const;

	static EditTheme getDefaultTheme();
	static EditTheme getClassicTheme();
	static EditTheme getTwilightTheme();
	static EditTheme getOceanTheme();

	static tstring colorToHEX( const QColor color );
};

QSettings& operator<< (QSettings& settings, const EditTheme& theme);
QSettings& operator>> (QSettings& settings,       EditTheme& theme);

// --------------------------------------------------------------------------------
// -------------------- EditTab
// --------------------------------------------------------------------------------
class EditTab
{
public:
	EditTab();
	virtual ~EditTab();

	EditTab& operator =( const EditTab& tab );
	rbool operator ==( const EditTab& tab ) const;
	rbool operator !=( const EditTab& tab ) const;

	void load(QSettings& settings);
	void save(QSettings& settings) const;

	int tabSize;
	int indentSize;
	rbool useTabs;
	rbool tabIndents;
	rbool backspaceUntabs;
	rbool autoIndent;
};

QSettings& operator<< (QSettings& settings, const EditTab& tab);
QSettings& operator>> (QSettings& settings,       EditTab& tab);

// --------------------------------------------------------------------------------
// -------------------- EditWindow
// --------------------------------------------------------------------------------
class EditWindow
{
public:
	EditWindow();
	virtual ~EditWindow();

	EditWindow& operator =( const EditWindow& window );
	rbool operator ==( const EditWindow& window ) const;
	rbool operator !=( const EditWindow& window ) const;

	void load(QSettings& settings);
	void save(QSettings& settings) const;

	rbool wordWrap;
	rbool showHorzScrollBar;
};

QSettings& operator<< (QSettings& settings, const EditWindow& window);
QSettings& operator>> (QSettings& settings,       EditWindow& window);

// --------------------------------------------------------------------------------
// -------------------- EditStyle
// --------------------------------------------------------------------------------
class EditStyle: public rdoStyle::RDOStyleWithTheme
{
public:
	EditStyle();
	virtual ~EditStyle();

	EditStyle& operator =( const EditStyle& style );
	rbool operator ==( const EditStyle& style ) const;
	rbool operator !=( const EditStyle& style ) const;

	void init( CREF(QString) _groupName = "" );
	rbool load();
	rbool save() const;

	EditTab*    tab;
	EditWindow* window;
};

}}} // namespace rdo::gui::editor

#endif // _RDO_STUDIO_EDITOR_EDIT_STYLE_H_
