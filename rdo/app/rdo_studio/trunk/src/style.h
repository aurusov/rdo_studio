/*!
  \copyright (c) RDO-Team, 2003-2012
  \file      app/rdo_studio/src/style.h
  \author    Урусов Андрей (rdo@rk9.bmstu.ru)
  \date      27.03.2003
  \brief     
  \indent    4T
*/

#ifndef _RDO_STUDIO_STYLE_H_
#define _RDO_STUDIO_STYLE_H_

// ----------------------------------------------------------------------- INCLUDES
#include <string>
#include <QtGui/qcolor.h>
#include <QtCore/qsettings.h>
// ----------------------------------------------------------------------- SYNOPSIS
#include "utils/smart_ptr/intrusive_ptr.h"
// --------------------------------------------------------------------------------

namespace rdoStyle {

// --------------------------------------------------------------------------------
// -------------------- RDOStyleFont
// --------------------------------------------------------------------------------
class RDOStyleFont
{
public:
	RDOStyleFont();
	virtual ~RDOStyleFont();

	RDOStyleFont& operator =(const RDOStyleFont& font);
	rbool operator ==(const RDOStyleFont& font) const;
	rbool operator !=(const RDOStyleFont& font) const;

	virtual void load(QSettings& settings);
	virtual void save(QSettings& settings) const;

	enum style { NONE = 0x00, BOLD = 0x01, ITALIC = 0x02, UNDERLINE = 0x04 };

	tstring     name;
	int         size;
	int         codepage;
	int         characterSet;

	static RDOStyleFont getDefaultFont();
	static RDOStyleFont getClassicFont();
	static RDOStyleFont getTracerLogFont();
	static RDOStyleFont getChartViewFont();
	static RDOStyleFont getFrameFont();
};

// --------------------------------------------------------------------------------
// -------------------- RDOStyleTheme
// --------------------------------------------------------------------------------
class RDOStyleTheme
{
public:
	RDOStyleTheme();
	virtual ~RDOStyleTheme();

	RDOStyleTheme& operator =(const RDOStyleTheme& theme);
	rbool operator ==(const RDOStyleTheme& theme) const;
	rbool operator !=(const RDOStyleTheme& theme) const;

	virtual void load(QSettings& settings);
	virtual void save(QSettings& settings) const;

	QColor defaultColor;
	QColor backgroundColor;

	rdoStyle::RDOStyleFont::style defaultStyle;
};

QSettings& operator<< (QSettings& settings, const RDOStyleTheme& theme);
QSettings& operator>> (QSettings& settings,       RDOStyleTheme& theme);

// --------------------------------------------------------------------------------
// -------------------- RDOStyle
// --------------------------------------------------------------------------------
OBJECT(RDOStyle)
{
protected:
	QString groupName;

	virtual void initFont();

public:
	RDOStyle();
	virtual ~RDOStyle();

	RDOStyle& operator =(const RDOStyle& style);
	rbool operator ==(const RDOStyle& style) const;
	rbool operator !=(const RDOStyle& style) const;

	virtual void  init(CREF(QString) _groupName = "");
	virtual rbool load();
	virtual rbool save() const;

	RDOStyleFont* font;
};

// --------------------------------------------------------------------------------
// -------------------- RDOStyleWithTheme
// --------------------------------------------------------------------------------
class RDOStyleWithTheme: public RDOStyle
{
protected:
	virtual void initTheme();

public:
	RDOStyleWithTheme();
	virtual ~RDOStyleWithTheme();

	RDOStyleWithTheme& operator =(const RDOStyleWithTheme& style);
	rbool operator ==(const RDOStyleWithTheme& style) const;
	rbool operator !=(const RDOStyleWithTheme& style) const;

	virtual void  init(CREF(QString) _groupName = "");
	virtual rbool load();
	virtual rbool save() const;

	RDOStyleTheme* theme;
};

} // namespace rdoStyle

#endif // _RDO_STUDIO_STYLE_H_
