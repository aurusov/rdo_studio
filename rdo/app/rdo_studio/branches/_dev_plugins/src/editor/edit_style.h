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
// -------------------- EditTab
// --------------------------------------------------------------------------------
class EditTab
{
public:
	EditTab();
	virtual ~EditTab();

	EditTab& operator =( const EditTab& tab );
	bool operator ==( const EditTab& tab ) const;
	bool operator !=( const EditTab& tab ) const;

	void load(QSettings& settings);
	void save(QSettings& settings) const;

	int  tabSize;
	int  indentSize;
	bool useTabs;
	bool tabIndents;
	bool backspaceUntabs;
	bool autoIndent;
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
	bool operator ==( const EditWindow& window ) const;
	bool operator !=( const EditWindow& window ) const;

	void load(QSettings& settings);
	void save(QSettings& settings) const;

	bool wordWrap;
	bool showHorzScrollBar;
};

QSettings& operator<< (QSettings& settings, const EditWindow& window);
QSettings& operator>> (QSettings& settings,       EditWindow& window);

// --------------------------------------------------------------------------------
// -------------------- EditStyle
// --------------------------------------------------------------------------------
class EditStyle: public style::StyleBase
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

	EditStyle();
	~EditStyle();

	EditStyle& operator =( const EditStyle& style );
	bool operator ==( const EditStyle& style ) const;
	bool operator !=( const EditStyle& style ) const;

	virtual bool styleDefault( const int styleType ) const;
	virtual bool styleUsing( const int styleType ) const;
	virtual bool styleBold( const int styleType = STYLE_DEFAULT ) const;
	virtual bool styleItalic( const int styleType = STYLE_DEFAULT ) const;
	virtual tstring styleFGColorToHEX( const int styleType = STYLE_DEFAULT ) const;
	virtual tstring styleBGColorToHEX( const int styleType = STYLE_DEFAULT ) const;

	static EditStyle getDefaultStyle();
	static EditStyle getClassicStyle();
	static EditStyle getTwilightStyle();
	static EditStyle getOceanStyle();

	static tstring colorToHEX( const QColor color );

	QColor caretColor;
	QColor selectionBgColor;
	QColor bookmarkFgColor;
	QColor bookmarkBgColor;

	Bookmark bookmarkStyle;

	EditTab    tab;
	EditWindow window;
};

QSettings& operator<< (QSettings& settings, const EditStyle& style);
QSettings& operator>> (QSettings& settings,       EditStyle& style);

}}} // namespace rdo::gui::editor

#endif // _RDO_STUDIO_EDITOR_EDIT_STYLE_H_
