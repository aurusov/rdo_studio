#pragma once

// ----------------------------------------------------------------------- INCLUDES
#include "utils/src/common/warning_disable.h"
#include <string>
#include <QColor>
#include <QSettings>
#include "utils/src/common/warning_enable.h"
// ----------------------------------------------------------------------- SYNOPSIS
#include "utils/src/smart_ptr/intrusive_ptr/intrusive_ptr.h"
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
	bool operator ==(const StyleFont& font) const;
	bool operator !=(const StyleFont& font) const;

	void load(QSettings& settings);
	void save(QSettings& settings) const;

	enum style { NONE = 0x00, BOLD = 0x01, ITALIC = 0x02, UNDERLINE = 0x04 };

	std::string name;
	int size;

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
PREDECLARE_POINTER(StyleBase);
class StyleBase: public rdo::counter_reference
{
public:
	StyleBase();
	~StyleBase();

	StyleBase& operator =(const StyleBase& style);
	bool operator ==(const StyleBase& style) const;
	bool operator !=(const StyleBase& style) const;

	StyleFont font;
	StyleFont::style defaultStyle;

	QColor defaultColor;
	QColor backgroundColor;
};

QSettings& operator<< (QSettings& settings, const StyleBase& style);
QSettings& operator>> (QSettings& settings,       StyleBase& style);

}}} // namespace rdo::gui::style
