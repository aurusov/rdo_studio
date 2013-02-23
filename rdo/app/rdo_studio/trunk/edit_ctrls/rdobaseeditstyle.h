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
// -------------------- RDOBaseEditTheme
// --------------------------------------------------------------------------------
enum RDOBookmarkStyle {
	RDOBOOKMARKS_NONE = 0,
	RDOBOOKMARKS_CIRCLE,
	RDOBOOKMARKS_RECT,
	RDOBOOKMARKS_ROUNDRECT,
	RDOBOOKMARKS_ARROW
};

class RDOBaseEditTheme: public rdoStyle::RDOStyleTheme
{
public:
	RDOBaseEditTheme();
	virtual ~RDOBaseEditTheme();

	RDOBaseEditTheme& operator =( const RDOBaseEditTheme& theme );
	rbool operator ==( const RDOBaseEditTheme& theme ) const;
	rbool operator !=( const RDOBaseEditTheme& theme ) const;

	virtual void load(QSettings& settings);
	virtual void save(QSettings& settings) const;

	QColor caretColor;
	QColor selectionBgColor;
	QColor bookmarkFgColor;
	QColor bookmarkBgColor;

	RDOBookmarkStyle bookmarkStyle;

	virtual rbool styleDefault( const int styleType ) const;
	virtual rbool styleUsing( const int styleType ) const;
	virtual rbool styleBold( const int styleType = STYLE_DEFAULT ) const;
	virtual rbool styleItalic( const int styleType = STYLE_DEFAULT ) const;
	virtual tstring styleFGColorToHEX( const int styleType = STYLE_DEFAULT ) const;
	virtual tstring styleBGColorToHEX( const int styleType = STYLE_DEFAULT ) const;

	static RDOBaseEditTheme getDefaultTheme();
	static RDOBaseEditTheme getClassicTheme();
	static RDOBaseEditTheme getTwilightTheme();
	static RDOBaseEditTheme getOceanTheme();

	static tstring colorToHEX( const QColor color );
};

QSettings& operator<< (QSettings& settings, const RDOBaseEditTheme& theme);
QSettings& operator>> (QSettings& settings,       RDOBaseEditTheme& theme);

// --------------------------------------------------------------------------------
// -------------------- RDOBaseEditTab
// --------------------------------------------------------------------------------
class RDOBaseEditTab
{
public:
	RDOBaseEditTab();
	virtual ~RDOBaseEditTab();

	RDOBaseEditTab& operator =( const RDOBaseEditTab& tab );
	rbool operator ==( const RDOBaseEditTab& tab ) const;
	rbool operator !=( const RDOBaseEditTab& tab ) const;

	virtual void load(QSettings& settings);
	virtual void save(QSettings& settings) const;

	int tabSize;
	int indentSize;
	rbool useTabs;
	rbool tabIndents;
	rbool backspaceUntabs;
	rbool autoIndent;
};

QSettings& operator<< (QSettings& settings, const RDOBaseEditTab& tab);
QSettings& operator>> (QSettings& settings,       RDOBaseEditTab& tab);

// --------------------------------------------------------------------------------
// -------------------- RDOBaseEditWindow
// --------------------------------------------------------------------------------
class RDOBaseEditWindow
{
public:
	RDOBaseEditWindow();
	virtual ~RDOBaseEditWindow();

	RDOBaseEditWindow& operator =( const RDOBaseEditWindow& window );
	rbool operator ==( const RDOBaseEditWindow& window ) const;
	rbool operator !=( const RDOBaseEditWindow& window ) const;

	virtual void load(QSettings& settings);
	virtual void save(QSettings& settings) const;

	rbool wordWrap;
	rbool showHorzScrollBar;
};

QSettings& operator<< (QSettings& settings, const RDOBaseEditWindow& window);
QSettings& operator>> (QSettings& settings,       RDOBaseEditWindow& window);

// --------------------------------------------------------------------------------
// -------------------- RDOBaseEditStyle
// --------------------------------------------------------------------------------
class RDOBaseEditStyle: public rdoStyle::RDOStyleWithTheme
{
public:
	RDOBaseEditStyle();
	virtual ~RDOBaseEditStyle();

	RDOBaseEditStyle& operator =( const RDOBaseEditStyle& style );
	rbool operator ==( const RDOBaseEditStyle& style ) const;
	rbool operator !=( const RDOBaseEditStyle& style ) const;

	void init( CREF(QString) _groupName = "" );
	virtual rbool load();
	virtual rbool save() const;

	RDOBaseEditTab*    tab;
	RDOBaseEditWindow* window;
};

} // namespace rdoEditCtrl

#endif // _RDO_STUDIO_EDIT_CTRLS_RDOBASEEDITSTYLE_H_
