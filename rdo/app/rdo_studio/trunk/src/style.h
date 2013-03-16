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
#include "utils/warning_disable.h"
#include <string>
#include <QColor>
#include <QSettings>
#include "utils/warning_enable.h"
// ----------------------------------------------------------------------- SYNOPSIS
#include "utils/smart_ptr/intrusive_ptr.h"
// --------------------------------------------------------------------------------

namespace rdo { namespace gui { namespace style {

// --------------------------------------------------------------------------------
// -------------------- StyleFont
// --------------------------------------------------------------------------------
class StyleFont
{
public:
	StyleFont();
	virtual ~StyleFont();

	StyleFont& operator =(const StyleFont& font);
	rbool operator ==(const StyleFont& font) const;
	rbool operator !=(const StyleFont& font) const;

	void load(QSettings& settings);
	void save(QSettings& settings) const;

	enum style { NONE = 0x00, BOLD = 0x01, ITALIC = 0x02, UNDERLINE = 0x04 };

	tstring     name;
	int         size;
	int         codepage;
	int         characterSet;

	static StyleFont getDefaultFont();
	static StyleFont getClassicFont();
	static StyleFont getTracerLogFont();
	static StyleFont getChartViewFont();
	static StyleFont getFrameFont();
};

QSettings& operator<< (QSettings& settings, const StyleFont& font);
QSettings& operator>> (QSettings& settings,       StyleFont& font);

// --------------------------------------------------------------------------------
// -------------------- StyleBase
// --------------------------------------------------------------------------------
OBJECT(StyleBase)
{
public:
	StyleBase();
	~StyleBase();

	StyleBase& operator =(const StyleBase& style);
	rbool operator ==(const StyleBase& style) const;
	rbool operator !=(const StyleBase& style) const;

	StyleFont font;
	StyleFont::style defaultStyle;

	QColor defaultColor;
	QColor backgroundColor;
};

QSettings& operator<< (QSettings& settings, const StyleBase& style);
QSettings& operator>> (QSettings& settings,       StyleBase& style);

}}} // namespace rdo::gui::style

#endif // _RDO_STUDIO_STYLE_H_
