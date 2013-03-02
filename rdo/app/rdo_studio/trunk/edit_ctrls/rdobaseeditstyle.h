/*!
  \copyright (c) RDO-Team, 2003-2012
  \file      rdobaseeditstyle.h
  \author    Урусов Андрей (rdo@rk9.bmstu.ru)
  \date      28.02.2003
  \brief     
  \indent    4T
*/

#ifndef _RDO_STUDIO_EDIT_CTRLS_RDOBASEEDITSTYLE_H_
#define _RDO_STUDIO_EDIT_CTRLS_RDOBASEEDITSTYLE_H_

// ----------------------------------------------------------------------- INCLUDES
// ----------------------------------------------------------------------- SYNOPSIS
#include "app/rdo_studio/src/style.h"
#include "thirdparty/scintilla/include/Scintilla.h"
// --------------------------------------------------------------------------------

namespace rdoEditCtrl {

// --------------------------------------------------------------------------------
// -------------------- EditBaseTheme
// --------------------------------------------------------------------------------
enum BookmarkStyle {
	RDOBOOKMARKS_NONE = 0,
	RDOBOOKMARKS_CIRCLE,
	RDOBOOKMARKS_RECT,
	RDOBOOKMARKS_ROUNDRECT,
	RDOBOOKMARKS_ARROW
};

class EditBaseTheme: public rdoStyle::RDOStyleTheme
{
public:
	EditBaseTheme();
	virtual ~EditBaseTheme();

	EditBaseTheme& operator =( const EditBaseTheme& theme );
	rbool operator ==( const EditBaseTheme& theme ) const;
	rbool operator !=( const EditBaseTheme& theme ) const;

	virtual void load(QSettings& settings);
	virtual void save(QSettings& settings) const;

	QColor caretColor;
	QColor selectionBgColor;
	QColor bookmarkFgColor;
	QColor bookmarkBgColor;

	BookmarkStyle bookmarkStyle;

	virtual rbool styleDefault( const int styleType ) const;
	virtual rbool styleUsing( const int styleType ) const;
	virtual rbool styleBold( const int styleType = STYLE_DEFAULT ) const;
	virtual rbool styleItalic( const int styleType = STYLE_DEFAULT ) const;
	virtual tstring styleFGColorToHEX( const int styleType = STYLE_DEFAULT ) const;
	virtual tstring styleBGColorToHEX( const int styleType = STYLE_DEFAULT ) const;

	static EditBaseTheme getDefaultTheme();
	static EditBaseTheme getClassicTheme();
	static EditBaseTheme getTwilightTheme();
	static EditBaseTheme getOceanTheme();

	static tstring colorToHEX( const QColor color );
};

QSettings& operator<< (QSettings& settings, const EditBaseTheme& theme);
QSettings& operator>> (QSettings& settings,       EditBaseTheme& theme);

// --------------------------------------------------------------------------------
// -------------------- EditBaseTab
// --------------------------------------------------------------------------------
class EditBaseTab
{
public:
	EditBaseTab();
	virtual ~EditBaseTab();

	EditBaseTab& operator =( const EditBaseTab& tab );
	rbool operator ==( const EditBaseTab& tab ) const;
	rbool operator !=( const EditBaseTab& tab ) const;

	void load(QSettings& settings);
	void save(QSettings& settings) const;

	int tabSize;
	int indentSize;
	rbool useTabs;
	rbool tabIndents;
	rbool backspaceUntabs;
	rbool autoIndent;
};

QSettings& operator<< (QSettings& settings, const EditBaseTab& tab);
QSettings& operator>> (QSettings& settings,       EditBaseTab& tab);

// --------------------------------------------------------------------------------
// -------------------- EditBaseWindow
// --------------------------------------------------------------------------------
class EditBaseWindow
{
public:
	EditBaseWindow();
	virtual ~EditBaseWindow();

	EditBaseWindow& operator =( const EditBaseWindow& window );
	rbool operator ==( const EditBaseWindow& window ) const;
	rbool operator !=( const EditBaseWindow& window ) const;

	void load(QSettings& settings);
	void save(QSettings& settings) const;

	rbool wordWrap;
	rbool showHorzScrollBar;
};

QSettings& operator<< (QSettings& settings, const EditBaseWindow& window);
QSettings& operator>> (QSettings& settings,       EditBaseWindow& window);

// --------------------------------------------------------------------------------
// -------------------- EditBaseStyle
// --------------------------------------------------------------------------------
class EditBaseStyle: public rdoStyle::RDOStyleWithTheme
{
public:
	EditBaseStyle();
	virtual ~EditBaseStyle();

	EditBaseStyle& operator =( const EditBaseStyle& style );
	rbool operator ==( const EditBaseStyle& style ) const;
	rbool operator !=( const EditBaseStyle& style ) const;

	void init( CREF(QString) _groupName = "" );
	rbool load();
	rbool save() const;

	EditBaseTab*    tab;
	EditBaseWindow* window;
};

} // namespace rdoEditCtrl

#endif // _RDO_STUDIO_EDIT_CTRLS_RDOBASEEDITSTYLE_H_
