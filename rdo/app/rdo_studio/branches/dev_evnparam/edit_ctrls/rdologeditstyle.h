#ifndef RDOLOGEDITSTYLE_H
#define RDOLOGEDITSTYLE_H
#pragma once

#include "app/rdo_studio_mfc/edit_ctrls/rdobaseeditstyle.h"

namespace rdoEditCtrl {

// --------------------------------------------------------------------------------
// -------------------- RDOLogEditTheme
// --------------------------------------------------------------------------------
class RDOLogEditTheme: public RDOBaseEditTheme
{
public:
	RDOLogEditTheme();
	virtual ~RDOLogEditTheme();

	RDOLogEditTheme& operator =( const RDOLogEditTheme& theme );
	bool operator ==( const RDOLogEditTheme& theme ) const;
	bool operator !=( const RDOLogEditTheme& theme ) const;

	virtual void load( std::string regPath );
	virtual void save( std::string regPath ) const;

	COLORREF selectLineBgColor;

	static RDOLogEditTheme getDefaultTheme();
	static RDOLogEditTheme getClassicTheme();
	static RDOLogEditTheme getTwilightTheme();
	static RDOLogEditTheme getOceanTheme();
};

// --------------------------------------------------------------------------------
// -------------------- RDOLogEditStyle
// --------------------------------------------------------------------------------
class RDOLogEditStyle: public RDOBaseEditStyle
{
protected:
	virtual void initTheme();

public:
	RDOLogEditStyle();
	virtual ~RDOLogEditStyle();

	RDOLogEditStyle& operator =( const RDOLogEditStyle& style );
	bool operator ==( const RDOLogEditStyle& style ) const;
	bool operator !=( const RDOLogEditStyle& style ) const;
};

}; // namespace rdoEditCtrl

#endif // RDOLOGEDITSTYLE_H
