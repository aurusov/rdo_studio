/*!
  \copyright (c) RDO-Team, 2003-2012
  \file      rdobaseeditstyle.h
  \author    Урусов Андрей (rdo@rk9.bmstu.ru)
  \date      28.02.2003
  \brief     
  \indent    4T
*/

#ifndef _RDO_STUDIO_MFC_EDIT_CTRLS_RDOBASEEDITSTYLE_H_
#define _RDO_STUDIO_MFC_EDIT_CTRLS_RDOBASEEDITSTYLE_H_

// ----------------------------------------------------------------------- INCLUDES
// ----------------------------------------------------------------------- SYNOPSIS
#include "app/rdo_studio_mfc/src/style.h"
#include "thirdparty/sci/Scintilla.h"
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
	bool operator ==( const RDOBaseEditTheme& theme ) const;
	bool operator !=( const RDOBaseEditTheme& theme ) const;

	virtual void load( tstring regPath );
	virtual void save( tstring regPath ) const;

	COLORREF caretColor;
	COLORREF selectionBgColor;
	COLORREF bookmarkFgColor;
	COLORREF bookmarkBgColor;

	RDOBookmarkStyle bookmarkStyle;

	virtual bool styleDefault( const int styleType ) const;
	virtual bool styleUsing( const int styleType ) const;
	virtual bool styleBold( const int styleType = STYLE_DEFAULT ) const;
	virtual bool styleItalic( const int styleType = STYLE_DEFAULT ) const;
	virtual tstring styleFGColorToHEX( const int styleType = STYLE_DEFAULT ) const;
	virtual tstring styleBGColorToHEX( const int styleType = STYLE_DEFAULT ) const;

	static RDOBaseEditTheme getDefaultTheme();
	static RDOBaseEditTheme getClassicTheme();
	static RDOBaseEditTheme getTwilightTheme();
	static RDOBaseEditTheme getOceanTheme();

	static tstring colorToHEX( const COLORREF color );
};

// --------------------------------------------------------------------------------
// -------------------- RDOBaseEditTab
// --------------------------------------------------------------------------------
class RDOBaseEditTab
{
public:
	RDOBaseEditTab();
	virtual ~RDOBaseEditTab();

	RDOBaseEditTab& operator =( const RDOBaseEditTab& tab );
	bool operator ==( const RDOBaseEditTab& tab ) const;
	bool operator !=( const RDOBaseEditTab& tab ) const;

	virtual void load( tstring regPath );
	virtual void save( tstring regPath ) const;

	int tabSize;
	int indentSize;
	bool useTabs;
	bool tabIndents;
	bool backspaceUntabs;
	bool autoIndent;
};

// --------------------------------------------------------------------------------
// -------------------- RDOBaseEditWindow
// --------------------------------------------------------------------------------
class RDOBaseEditWindow
{
public:
	RDOBaseEditWindow();
	virtual ~RDOBaseEditWindow();

	RDOBaseEditWindow& operator =( const RDOBaseEditWindow& window );
	bool operator ==( const RDOBaseEditWindow& window ) const;
	bool operator !=( const RDOBaseEditWindow& window ) const;

	virtual void load( tstring regPath );
	virtual void save( tstring regPath ) const;

	bool wordWrap;
	bool showHorzScrollBar;
};

// --------------------------------------------------------------------------------
// -------------------- RDOBaseEditStyle
// --------------------------------------------------------------------------------
class RDOBaseEditStyle: public rdoStyle::RDOStyleWithTheme
{
protected:
	virtual void initTheme();
	virtual void initTab();
	virtual void initWindow();

public:
	RDOBaseEditStyle();
	virtual ~RDOBaseEditStyle();

	RDOBaseEditStyle& operator =( const RDOBaseEditStyle& style );
	bool operator ==( const RDOBaseEditStyle& style ) const;
	bool operator !=( const RDOBaseEditStyle& style ) const;

	virtual void init( CREF(tstring) _regPath = "" );
	virtual bool load();
	virtual bool save() const;

	RDOBaseEditTab*    tab;
	RDOBaseEditWindow* window;
};

}; // namespace rdoEditCtrl

#endif // _RDO_STUDIO_MFC_EDIT_CTRLS_RDOBASEEDITSTYLE_H_
