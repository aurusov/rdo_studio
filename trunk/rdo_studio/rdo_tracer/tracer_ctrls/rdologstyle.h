#ifndef RDOLOGSTYLE_H
#define RDOLOGSTYLE_H
#pragma once

#include "../../edit_ctrls/rdobasectrlstyle.h"

namespace rdoTracerLog {

// ----------------------------------------------------------------------------
// ---------- RDOLogStyle
// ----------------------------------------------------------------------------
class RDOLogStyle: public rdoEditCtrl::RDOBaseCtrlStyle
{
public:
	RDOLogStyle();
	virtual ~RDOLogStyle();

	int vertBorder;
	int horzBorder;

	rdoEditCtrl::RDOFontStyle style;

	virtual bool getItemColors( const int index, COLORREF& textColor, COLORREF& backColor ) const;
	virtual bool getItemColors( const std::string& item, COLORREF& textColor, COLORREF& backColor ) const;

	RDOLogStyle& operator =( const RDOLogStyle& style );
	bool operator ==( const RDOLogStyle& style ) const;
	bool operator !=( const RDOLogStyle& style ) const;

	virtual bool load();
	virtual bool save() const;

	static RDOLogStyle getDefaultStyle();
};

}; // namespace rdoTracerLog

#endif // RDOLOGSTYLE_H
